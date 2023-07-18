-- Keep a log of any SQL queries you execute as you solve the mystery.

-- General information from the crime_scene_report based on information given to us in the fiftyville description
SELECT id, street, description
    FROM crime_scene_reports
    WHERE day = 28
    AND month = 7
    AND year = 2021
    AND street = "Humphrey Street";

-- Got the description of the incident that states:
-- " Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery. "

-- Time was 10:15 a.m on 28/07/2021 Humphrey Street at a Bakery

-- Look up witness interviews from the crime
SELECT id, name, transcript
    FROM interviews
    WHERE year = 2021
    AND day = 28
    AND month = 7;

-- Ruth ~ 10 min from the theft, suspect leaves bakery in a car

--- Eugene recognizes thief, saw them earlier on Leggett Street withdrawing money from at ATM.

-- Raymond -- After leaving bakery, made a call that lasted less than a minute. Thief was going to leave fiftyville on earliest flight on 29/07/2021.  The thief asks person on the phone to purchase the flight ticket.

-- Looking up security footage from bakery @10 a.m - 11 a.m
SELECT id, minute, activity, license_plate
    FROM bakery_security_logs
    WHERE minute BETWEEN 15 AND 30
    AND year = 2021
    AND month = 7
    AND day = 28
    AND hour = 10;

-- Using information from above to broaden the search and get the name of all people who left the bakery between 10:15 and 10:30 along with their phone numbers and license plates.
SELECT name, phone_number, bakery_security_logs.license_plate
    FROM bakery_security_logs, people
    WHERE minute BETWEEN 15 AND 30
    AND year = 2021
    AND month = 7
    AND day = 28
    AND hour = 10
    AND bakery_security_logs.license_plate = people.license_plate;

-- Now looking at the people who left the bakery between 10:15 and 10:30 and used the atm on Leggett Street on the same day. The people on both lists will be the possible suspects.
SELECT name, people.id, people.license_plate, people.phone_number
    FROM bakery_security_logs, people, atm_transactions, bank_accounts
    WHERE minute BETWEEN 15 AND 30
    AND bakery_security_logs.year = 2021
    AND bakery_security_logs.month = 7
    AND bakery_security_logs.day = 28
    AND bakery_security_logs.hour = 10
    AND bakery_security_logs.activity = "exit"
    AND bakery_security_logs.license_plate = people.license_plate
    AND people.id = bank_accounts.person_id
    AND bank_accounts.account_number = atm_transactions.account_number
    AND atm_transactions.atm_location = "Leggett Street"
    AND atm_transactions.year = 2021
    AND atm_transactions.month = 7
    AND atm_transactions.day = 28
    AND atm_transactions.transaction_type = "withdraw";


-- With the last querry, we had 4 possible suspects:
-- Bruce, Luca, Iman, and Diana.

-- Now we need to check the information from phone records.
-- Thief made a phone call that lasted less than a minute

SELECT DISTINCT phone_calls.id, name, caller, receiver, duration
FROM phone_calls, people
    WHERE year = 2021
    AND month = 7
    AND day = 28
    AND duration BETWEEN 1 AND 61
    AND phone_calls.caller = people.phone_number
    AND name IN ("Bruce", "Luca", "Iman", "Diana");

-- From this querry, Bruce and Diana were the only two people who made a phonecall that day that lasted less than a minute.

-- The two suspects who were accomplices to the thief had the phone numbers:
-- Bruce called (375) 555-8161 and Diana called (725) 555-3242


-- I wrote a querry to see how fiftyville was represented in the airports table
SELECT * FROM airports
-- | 8  | CSF | Fiftyville Regional Airport | Fiftyville    |


-- Now we can look and see if either Diana or Bruce went on a trip the very next day by searching for their names in the passengers list for flights that occured on 29/7/2021
SELECT name, city, hour
FROM people, passengers, flights, airports
    WHERE name IN ("Bruce", "Diana")
    AND people.passport_number = passengers.passport_number
    AND passengers.flight_id = flights.id
    AND flights.year = 2021
    AND flights.month = 7
    AND flights.day = 29
    AND flights.hour BETWEEN 0 AND 12
    AND flights.origin_airport_id = 8
    AND flights.destination_airport_id = airports.id;

-- From this querry we can see that Bruce went to New York City and we can search for the receiving number in people to determine the accomplice.
SELECT name FROM people
WHERE phone_number = "(375) 555-8161";

-- Which gives us the name Robin as the person Bruce called.
