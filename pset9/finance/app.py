import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    inventory = db.execute(
        "SELECT * FROM inventory WHERE user_id = ? AND amount > 0;", session["user_id"]
    )
    for i in inventory:
        i["price"] = float(lookup(i["symbol"])["price"])
        i["total"] = i["price"] * i["amount"]

    cash = float(
        db.execute("SELECT cash FROM users WHERE id = ?;", session["user_id"])[0][
            "cash"
        ]
    )

    sum = 0
    for i in inventory:
        sum += i["total"]

    total = cash + sum

    return render_template("index.html", inventory=inventory, cash=cash, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")

    symbol = request.form.get("symbol")

    shares = 0
    try:
        shares = int(request.form.get("shares"))
    except:
        return apology("Invalid shares")

    if symbol.strip() == "":
        return apology("Invalid symbol")

    if shares < 0:
        return apology("Invalid shares")

    stock = lookup(symbol)
    if stock == None:
        return apology("Invalid symbol")

    price = stock["price"] * shares

    user = db.execute("SELECT cash FROM users WHERE id = ?;", session["user_id"])
    user_stock = db.execute(
        "SELECT * FROM inventory WHERE id = ? AND symbol = ?;",
        session["user_id"],
        symbol,
    )

    if user[0]["cash"] < price:
        return apology("Not enough money to buy this stock.")

    try:
        db.execute("BEGIN TRANSACTION;")
        db.execute(
            "UPDATE users SET cash = cash - ? WHERE id = ?;", price, session["user_id"]
        )

        if len(user_stock) == 0:
            db.execute(
                "INSERT INTO inventory (user_id, symbol, amount) VALUES (?, ?, ?)",
                session["user_id"],
                symbol,
                shares,
            )
        else:
            db.execute(
                "UPDATE inventory SET amount = amount + ? WHERE user_id = ? AND symbol = ?;",
                shares,
                session["user_id"],
                symbol,
            )

        db.execute(
            "INSERT INTO logs (user_id, symbol, amount, price, transaction_type, time) VALUES (?, ?, ?, ?, 'buy', datetime('now'));",
            session["user_id"],
            symbol,
            shares,
            stock["price"],
        )

        db.execute("COMMIT;")
    except:
        db.execute("ROLLBACK;")

    return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    logs = db.execute(
        "SELECT * FROM logs WHERE user_id = ? ORDER BY time DESC;", session["user_id"]
    )

    for i in logs:
        if i["transaction_type"] == "sell":
            i["amount"] *= -1

    return render_template("history.html", logs=logs)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")

    symbol = request.form.get("symbol")
    quote = lookup(symbol)

    if quote == None:
        return apology("Invalid symbol")

    return render_template("quoted.html", quote=quote)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "GET":
        return render_template("register.html")

    username = request.form.get("username")
    usernameQuery = db.execute(
        "SELECT username FROM users WHERE username = ?;", username
    )

    if username.strip() == "":
        return apology("Invalid username.")

    if len(usernameQuery) > 0:
        return apology("Username already exists.")

    password = request.form.get("password")
    confirmation = request.form.get("confirmation")

    if password.strip() == "" or password != confirmation:
        return apology("Invalid passoword.")

    db.execute(
        "INSERT INTO users (username, hash) VALUES (?, ?);",
        username,
        generate_password_hash(password),
    )

    return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "GET":
        options = db.execute(
            "SELECT symbol FROM inventory WHERE user_id = ? AND amount > 0;",
            session["user_id"],
        )

        return render_template("sell.html", options=options)

    symbol = request.form.get("symbol")
    shares = int(request.form.get("shares"))

    if symbol.strip() == "":
        return apology("Invalid symbol")

    if shares <= 0:
        return apology("Shares must be positive")

    user_amount = db.execute(
        "SELECT amount FROM inventory WHERE user_id = ? AND symbol = ?;",
        session["user_id"],
        symbol,
    )

    if shares > user_amount[0]["amount"]:
        return apology("Can't sell more shares than you have")

    stock = lookup(symbol)
    price = stock["price"] * shares

    try:
        db.execute("BEGIN TRANSACTION;")
        db.execute(
            "UPDATE users SET cash = cash + ? WHERE id = ?;", price, session["user_id"]
        )

        db.execute(
            "UPDATE inventory SET amount = amount - ? WHERE user_id = ? AND symbol = ?;",
            shares,
            session["user_id"],
            symbol,
        )

        db.execute(
            "INSERT INTO logs (user_id, symbol, amount, price, transaction_type, time) VALUES (?, ?, ?, ?, 'sell', datetime('now'));",
            session["user_id"],
            symbol,
            shares,
            stock["price"],
        )

        db.execute("COMMIT;")
    except:
        db.execute("ROLLBACK;")

    return redirect("/")
