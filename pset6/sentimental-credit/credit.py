from cs50 import get_string

while True:
    card = get_string("Number: ")
    try:
        int(card)
    except ValueError:
        continue

    break

sum = 0;

#multiply every other number
for i in range(2, len(card) + 1, 2):
    num = int(card[-i]) * 2
    if num >= 10:
        sum += 1
    sum += num % 10

for i in range(1, len(card) + 1, 2):
    sum += int(card[-i])

if sum % 10 != 0:
    exit(f"INVALID")

firstDigit = card[0]
secondDigit = card[1]

if (len(card) == 13 or len(card) == 16) and firstDigit == "4":
    exit("VISA")
elif len(card) == 16 and int(firstDigit + secondDigit) in range(51,56,1):
    exit("MASTERCARD")
elif len(card) == 15 and int(firstDigit + secondDigit) in [34, 37]:
    exit("AMEX")
else:
    exit("INVALID")