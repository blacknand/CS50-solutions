-- Keep a log of any SQL queries you execute as you solve the mystery.


-- I queried this to see a list of all of the tables
.table

-- I queried this to see the schema of the crime_scene_reports table
.schema crime_scene_reports

-- I queried this to see the crime reports for the theft
SELECT description
FROM crime_scene_reports
WHERE year = 2021
AND month = 7
AND day = 28
AND street = 'Humphrey Street';

-- I queried this to see the log of the tables again to find the interviews table
.table

-- I queried this to see the schema of the interviews table
.schema interviews

-- I queried this to see the interviews of the day of the theft
SELECT transcript
FROM interviews
WHERE year = 2021
AND month = 7
AND day = 28;

-- I queried this to see the tables and find the barkey_security_logs table
.table

-- I queried this to see the schema of the bakery_security_logs
.schema bakery_security_logs

-- I queried this to find a list of suspect license plates
SELECT license_plate
FROM bakery_security_logs
WHERE year = 2021
AND day = 28
AND hour = 10
AND minute >= 15
AND minute <= 25;

-- I queried this to see the schema of the people table
.schema people

-- I queried this to try to find out the information of all of the suspect license plates
SELECT *
FROM people
WHERE license_plate = (
    SELECT license_plate
    FROM bakery_security_logs
    WHERE year = 2021
    AND day = 28
    AND hour = 10
    AND minute >= 15
    AND minute <= 25
);

-- I queried this to see the people table again
.schema people

-- I queried this to see which table I could join with people
.table

-- I queried this by accident and got the wrong column
SELECT bakery_security_logs.license_plate
FROM bakery_security_logs
JOIN people
ON bakery_security_logs.license_plate = people.license_plate
WHERE year = 2021
AND day = 28
AND hour = 10
AND minute >= 15
AND minute <= 25;

-- I queried this to try to find out all informatin on the suspects using their license plates
SELECT *
FROM people
JOIN bakery_security_logs
ON people.license_plate = bakery_security_logs.license_plate
WHERE bakery_security_logs.year = 2021
AND bakery_security_logs.day = 28
AND bakery_security_logs.hour = 10
AND bakery_security_logs.minute >= 15
AND bakery_security_logs.minute <= 25;

-- I queried this to then see what tables I should query next
.table

-- I queried this to see the schema of the atm_transactions table
SELECT *
FROM atm_transactions
WHERE year = 2021
AND month = 7
AND day = 28
AND atm_location = 'Leggett Street'
AND transaction_type = 'withdraw';

-- I queried this to then see what tables I should query next
.table

-- I queried this to see the schema of the bank_accounts table
.schema bank_accounts

-- I queried this to see what the person_id column looked like
SELECT person_id
FROM bank_accounts
LIMIT 5;

-- I queried this to try to find the id's of the suspects
SELECT person_id
FROM bank_accounts
WHERE account_number = (
    SELECT account_number
    FROM atm_transactions
    WHERE year = 2021
    AND day = 28
    AND atm_location = 'Leggett Street'
    AND transaction_type = 'withdraw'
);

-- I queried this to finds the id's of all of the suspects
SELECT person_id
FROM bank_accounts
JOIN atm_transactions
ON bank_accounts.account_number = atm_transactions.account_number
WHERE atm_transactions.year = 2021
AND atm_transactions.month = 7
AND atm_transactions.day = 28
AND atm_transactions.atm_location = 'Leggett Street'
AND atm_transactions.transaction_type = 'withdraw';

-- I queried this to try to find all of the info on the suspects
SELECT *
FROM people
WHERE id = (
    SELECT person_id
    FROM bank_accounts
    JOIN atm_transactions
    ON bank_accounts.account_number = atm_transactions.account_number
    WHERE atm_transactions.year = 2021
    AND atm_transactions.month = 7
    AND atm_transactions.day = 28
    AND atm_transactions.atm_location = 'Leggett Street'
    AND atm_transactions.transaction_type = 'withdraw'
);

-- I queried this to find out all the info on the suspects
SELECT *
FROM people
WHERE id IN (
    SELECT person_id
    FROM bank_accounts
    JOIN atm_transactions
    ON bank_accounts.account_number = atm_transactions.account_number
    WHERE atm_transactions.year = 2021
    AND atm_transactions.month = 7
    AND atm_transactions.day = 28
    AND atm_transactions.atm_location = 'Leggett Street'
    AND atm_transactions.transaction_type = 'withdraw'
);

-- I queried this to try to find out which suspects called who
SELECT receiver
FROM phone_calls
WHERE caller = (
    SELECT phone_number
    FROM people
    WHERE id IN (
    SELECT person_id
    FROM bank_accounts
    JOIN atm_transactions
    ON bank_accounts.account_number = atm_transactions.account_number
    WHERE atm_transactions.year = 2021
    AND atm_transactions.month = 7
    AND atm_transactions.day = 28
    AND atm_transactions.atm_location = 'Leggett Street'
    AND atm_transactions.transaction_type = 'withdraw'
));

-- I queried this to try to find out which suspects called who
SELECT *
FROM phone_calls
JOIN people
ON phone_calls.caller = people.phone_number
WHERE people.phone_number = (
    SELECT phone_number
    FROM people
    WHERE id IN (
        SELECT person_id
        FROM bank_accounts
        JOIN atm_transactions
        ON bank_accounts.account_number = atm_transactions.account_number
        WHERE atm_transactions.year = 2021
        AND atm_transactions.month = 7
        AND atm_transactions.day = 28
        AND atm_transactions.atm_location = 'Leggett Street'
        AND atm_transactions.transaction_type = 'withdraw'
));

-- I queried this to find out who called for less than a minute on the day of the theft
SELECT *
FROM phone_calls
JOIN people
ON phone_calls.caller = people.phone_number
WHERE people.phone_number IN (
    SELECT phone_number
    FROM people
    WHERE id IN (
        SELECT person_id
        FROM bank_accounts
        JOIN atm_transactions
        ON bank_accounts.account_number = atm_transactions.account_number
        WHERE atm_transactions.year = 2021
        AND atm_transactions.month = 7
        AND atm_transactions.day = 28
        AND atm_transactions.atm_location = 'Leggett Street'
        AND atm_transactions.transaction_type = 'withdraw'
))
AND phone_calls.year = 2021
AND phone_calls.month = 7
AND phone_calls.day = 28
AND phone_calls.duration < 60;

-- I queried this to find out the passport numbers of the 5 suspects
SELECT *
FROM people
WHERE name IN
('Bruce', 'Taylor', 'Diana', 'Kenny', 'Benista');

-- I queried this to find out the info on the flights that the suspects have taken
SELECT *
FROM passengers
WHERE passport_number IN (
    SELECT passport_number
    FROM people
    WHERE name IN
    ('Bruce', 'Taylor', 'Diana', 'Kenny', 'Benista')
);

-- I queried this to try to find the flights that the suspects had taken
SELECT *
FROM flights
WHERE id = (
    SELECT flight_id
    FROM passengers
    WHERE passport_number IN (
        SELECT passport_number
        FROM people
        WHERE name IN
        ('Bruce', 'Taylor', 'Diana', 'Kenny', 'Benista')
));

-- I queried this to find the flights that the suspects had taken
SELECT *
FROM flights
WHERE id IN (
    SELECT flight_id
    FROM passengers
    WHERE passport_number IN (
        SELECT passport_number
        FROM people
        WHERE name IN
        ('Bruce', 'Taylor', 'Diana', 'Kenny', 'Benista')
));

-- I queried this to find the flights taken the day just after the robbery
SELECT *
FROM flights
WHERE id IN (
    SELECT flight_id
    FROM passengers
    WHERE passport_number IN (
        SELECT passport_number
        FROM people
        WHERE name IN
        ('Bruce', 'Taylor', 'Diana', 'Kenny', 'Benista')
))
AND day = 29;

-- I queried this to find the id of the flight that the theif took
SELECT *
FROM flights
WHERE id IN (
    SELECT flight_id
    FROM passengers
    WHERE passport_number IN (
        SELECT passport_number
        FROM people
        WHERE name IN
        ('Bruce', 'Taylor', 'Diana', 'Kenny', 'Benista')
))
AND day = 29
AND hour = 8;

-- I queried this to try to find the passport of the theif
SELECT *
FROM passengers
JOIN flights
ON passengers.flight_id = flights.id
WHERE flights.id = 36
AND flights.origin_airport_id = 8
AND destination_airport_id = 4
AND year = 2021
AND month = 7
AND day = 29
AND hour = 8
AND minute = 20;

-- I queried this to find out what city the theif escaped to
SELECT city
FROM airports
WHERE id = 4;

SELECT passport_number
FROM passengers
JOIN flights
ON passengers.flight_id = flights.id
WHERE flights.id = 36
AND flights.origin_airport_id = 8
AND destination_airport_id = 4
AND year = 2021
AND month = 7
AND day = 29
AND hour = 8
AND minute = 20;

-- I queried this to find out the names of all of the people who were on the flight that the theif was also on
SELECT *
FROM people
WHERE passport_number IN (
    SELECT passport_number
    FROM passengers
    JOIN flights
    ON passengers.flight_id = flights.id
    WHERE flights.id = 36
    AND flights.origin_airport_id = 8
    AND destination_airport_id = 4
    AND year = 2021
    AND month = 7
    AND day = 29
    AND hour = 8
    AND minute = 20
)
AND name IN (
    SELECT name
FROM phone_calls
JOIN people
ON phone_calls.caller = people.phone_number
WHERE people.phone_number IN (
    SELECT phone_number
    FROM people
    WHERE id IN (
        SELECT person_id
        FROM bank_accounts
        JOIN atm_transactions
        ON bank_accounts.account_number = atm_transactions.account_number
        WHERE atm_transactions.year = 2021
        AND atm_transactions.month = 7
        AND atm_transactions.day = 28
        AND atm_transactions.atm_location = 'Leggett Street'
        AND atm_transactions.transaction_type = 'withdraw'
))
AND phone_calls.year = 2021
AND phone_calls.month = 7
AND phone_calls.day = 28
AND phone_calls.duration < 60
)
;

-- I queried this to who drove out of the bakery car park on the day of the robbery and hardcoded the license plates into the query because using the previous query would make it too long to read
SELECT *
FROM bakery_security_logs
WHERE license_plate
IN ('30G67EN', '1106N58', '94KL13X');

-- I queried this to who drove out of the bakery car park on the day of the robbery
SELECT *
FROM bakery_security_logs
JOIN people
ON bakery_security_logs.license_plate = people.license_plate
WHERE activity = 'exit'
AND day = 28
AND hour = 10
AND minute >= 15;

-- I queried this to find out who drove and found the theif
SELECT *
FROM bakery_security_logs
JOIN people
ON bakery_security_logs.license_plate = people.license_plate
WHERE activity = 'exit'
AND day = 28
AND hour = 10
AND minute >= 15
AND minute <= 25
AND people.name IN ('Kenny', 'Taylor', 'Bruce');

-- I queried this to see what to do next
.table

-- I queried this to find out what to query within the phone calls table
.schema phone_calls

-- I queried this to find out who the theif called after the robbery
SELECT caller, receiver
FROM phone_calls
WHERE caller = '(367) 555-5533'
AND year = 2021
AND month = 7
AND day = 28
AND duration < 60;

-- I queried this to find the name of the person who the theif called after the theft
SELECT *
FROM people
WHERE phone_number = (
SELECT receiver
FROM phone_calls
WHERE caller = '(367) 555-5533'
AND year = 2021
AND month = 7
AND day = 28
AND duration < 60);
