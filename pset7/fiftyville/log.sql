-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Get description from date and location of the crime
SELECT description
FROM crime_scene_reports
WHERE street = 'Humphrey Street'
AND day = 28 AND month = 7;

-- Get the activity and license plates that had activity near the time of the crime
SELECT license_plate || ', ' || activity || ' at ' || hour || ':' || minute
FROM bakery_security_logs
WHERE month = 7 AND day = 28
AND hour BETWEEN 9 AND 11;

/*
SUSPECTS LICENSE PLATES
+----------------------------------------------------------------------+
| license_plate || ', ' || activity || ' at ' || hour || ':' || minute |
+----------------------------------------------------------------------+
| 4328GD8, entrance at 9:14                                            |
| 5P2BI95, entrance at 9:15                                            |
| 6P58WS2, entrance at 9:20                                            |
| G412CB7, entrance at 9:28                                            |
| R3G7486, entrance at 10:8                                            |
| 13FNH73, entrance at 10:14                                           |
| 5P2BI95, exit at 10:16                                               |
| 94KL13X, exit at 10:18                                               |
| 6P58WS2, exit at 10:18                                               |
| 4328GD8, exit at 10:19                                               |
| G412CB7, exit at 10:20                                               |
| L93JTIZ, exit at 10:21                                               |
| 322W7JE, exit at 10:23                                               |
| 0NTHK55, exit at 10:23                                               |
+----------------------------------------------------------------------+ */

-- Find the main suspects info
SELECT *
FROM people
WHERE license_plate IN
(
    SELECT license_plate
    FROM bakery_security_logs
    WHERE month = 7 AND day = 28
);

/*
MAIN SUSPECTS FROM INFO
Vanessa
Barry
Luca
Sofia
*/

-- Get the suspects interview
-- None had interviews
SELECT *
FROM interviews
WHERE name
IN
(
    SELECT name
    FROM people
    WHERE license_plate IN
    (
        SELECT license_plate
        FROM bakery_security_logs
        WHERE month = 7 AND day = 28
        AND hour BETWEEN 9 AND 11
    )
);

-- Get interviews from the day
SELECT *
FROM interviews
WHERE day = 28 AND month = 7;

/*
Ruth said that the suspect left the bakery within 10 minutes,
so before 10:25.
Eugene said he saw the thief at the ATM on Leggett Street, withdrawing money, before arriving at the bakery.
As the thief was leaving, they talked to it's accomplice to buy a flight ticket.
None of them entered the bakery
*/

-- Get the activity and license plates that exited between 10:15 and 10:25
SELECT license_plate || ', ' || activity || ' at ' || hour || ':' || minute
FROM bakery_security_logs
WHERE month = 7 AND day = 28
AND hour = 10 AND
minute BETWEEN 15 AND 25;

-- Get the new info with the new main suspects
SELECT *
FROM people
WHERE license_plate IN
(
    SELECT license_plate
    FROM bakery_security_logs
    WHERE month = 7 AND day = 28
    AND hour = 10 AND
    minute BETWEEN 15 AND 25
    AND activity = 'exit'
);

-- Check the ATM transactions from before Eugene arrived at the bakery
-- Eugene arrived at

SELECT *
FROM atm_transactions
WHERE atm_location = 'Leggett Street'
AND month = 7 AND day = 28
AND transaction_type = 'withdraw';

-- Get info of new suspects from the now known account_number;
SELECT *
FROM people
JOIN bank_accounts bank ON people.id = bank.person_id
WHERE license_plate IN
(
    SELECT license_plate
    FROM bakery_security_logs
    WHERE month = 7 AND day = 28
    AND hour = 10 AND
    minute BETWEEN 15 AND 25
    AND activity = 'exit'
)
AND bank.account_number IN
(
    SELECT account_number
    FROM atm_transactions
    WHERE atm_location = 'Leggett Street'
    AND month = 7 AND day = 28
    AND transaction_type = 'withdraw'
);

/*
Main suspects
Bruce
Diana
Iman
Luca*
*/

-- Get all the phone calls any of the suspects made that are under 60 minutes
SELECT *
FROM people
JOIN phone_calls calls ON calls.caller = people.phone_number
JOIN bank_accounts bank ON people.id = bank.person_id
WHERE calls.month = 7 AND calls.day = 28
AND calls.duration < 60
AND people.license_plate IN
(
    SELECT license_plate
    FROM bakery_security_logs
    WHERE month = 7 AND day = 28
    AND hour = 10 AND
    minute BETWEEN 15 AND 25
    AND activity = 'exit'
)
AND bank.account_number IN
(
    SELECT account_number
    FROM atm_transactions
    WHERE atm_location = 'Leggett Street'
    AND month = 7 AND day = 28
    AND transaction_type = 'withdraw'
);

/*
+-----+----------------+----------------+------+-------+-----+----------+--------+-------+----------------+-----------------+---------------+----------------+-----------+---------------+
| id  |     caller     |    receiver    | year | month | day | duration |   id   | name  |  phone_number  | passport_number | license_plate | account_number | person_id | creation_year |
+-----+----------------+----------------+------+-------+-----+----------+--------+-------+----------------+-----------------+---------------+----------------+-----------+---------------+
| 233 | (367) 555-5533 | (375) 555-8161 | 2021 | 7     | 28  | 45       | 686048 | Bruce | (367) 555-5533 | 5773159633      | 94KL13X       | 49610011       | 686048    | 2010          |
| 255 | (770) 555-1861 | (725) 555-3243 | 2021 | 7     | 28  | 49       | 514354 | Diana | (770) 555-1861 | 3592750733      | 322W7JE       | 26013199       | 514354    | 2012          |
+-----+----------------+----------------+------+-------+-----+----------+--------+-------+----------------+-----------------+---------------+----------------+-----------+---------------+
*/

-- Get first flight parting from Fiftyville

SELECT flights.id, origin.city, destination.city
FROM flights
JOIN airports origin ON (flights.origin_airport_id = origin.id)
JOIN airports destination ON (flights.destination_airport_id = destination.id)
WHERE day = 29 AND month = 7 AND origin.city = 'Fiftyville'
ORDER BY hour, minute
LIMIT 1;

-- get any suspects at this flight

SELECT people.name
FROM people
JOIN passengers ON passengers.passport_number = people.passport_number
WHERE passengers.flight_id = 36
AND people.name IN ('Bruce', 'Diana')

-- Found the suspect as Bruce

-- Get who Bruce was talking to in that phone call with the id 686048

SELECT *
FROM people
WHERE phone_number = '(375) 555-8161';

--Robin is the receiver