-- The CS50 Duck has been stolen! The town of Fiftyville has called upon you to solve the mystery of the stolen duck.
-- Authorities believe that the thief stole the duck and then, shortly afterwards, took a flight out of town with the help of an accomplice.
-- Your goal is to identify:

-- Who the thief is,
-- What city the thief escaped to, and
-- Who the thief’s accomplice is who helped them escape

-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Find the crime scene description where the Duck was reported stolen (the theft took place on July 28, 2020 and that it took place on Chamberlin Street.)
SELECT description FROM crime_scene_reports
WHERE year = 2020 AND month = 7 AND day = 28
AND street LIKE "%Chamberlin%";
-- The previous query returns:
-- Theft of the CS50 duck took place at 10:15am at the Chamberlin Street courthouse. Interviews were conducted today with three witnesses
-- who were present at the time — each of their interview transcripts mentions the courthouse.

-- Find the interviews transcripts with date 28/7/2020, that contains "courthouse" in the transcript.
SELECT name, transcript FROM interviews
WHERE year = 2020 AND month = 7 AND day = 28
AND transcript LIKE "%courthouse%";
-- The previous query returns:
-- name | transcript
-- Ruth | Sometime within ten minutes of the theft, I saw the thief get into a car in the courthouse parking lot and drive away.
--      If you have security footage from the courthouse parking lot, you might want to look for cars that left the parking lot in that time frame.
-- Eugene | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at the courthouse,
--      I was walking by the ATM on Fifer Street and saw the thief there withdrawing some money.
-- Raymond | As the thief was leaving the courthouse, they called someone who talked to them for less than a minute. In the call,
--      I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
--      The thief then asked the person on the other end of the phone to purchase the flight ticket.

-- Find the name of the owners of the cars that left the courthouse parking lot between 10:15 and 10:25
SELECT name FROM people
JOIN courthouse_security_logs ON people.license_plate = courthouse_security_logs.license_plate
WHERE year = 2020 AND month = 7 AND day = 28 AND hour = 10 AND minute > 15 AND minute < 25 AND activity = "exit"
ORDER BY (people.name);
-- The previous query returns:
-- name
-- Amber
-- Danielle
-- Elizabeth
-- Ernest
-- Evelyn
-- Patrick
-- Roger
-- Russell

-- Find the name of the people withdrawing money on the ATM on Fifer Street right before Eugene arrived at the courthouse
SELECT name FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
WHERE year = 2020 AND month = 7 AND day = 28 AND atm_location LIKE "Fifer Street" AND transaction_type LIKE "withdraw"
ORDER BY (people.name);
-- The previous query returns:
-- name
-- Bobby
-- **Danielle
-- **Elizabeth
-- **Ernest
-- Madison
-- Roy
-- **Russell
-- Victoria

-- Query for the people who called someone for les than one minute in the day of the theft
SELECT calling.name, receiving.name FROM phone_calls
JOIN people AS calling ON phone_calls.caller = calling.phone_number
JOIN people AS receiving ON phone_calls.receiver = receiving.phone_number
WHERE year = 2020 AND month = 7 AND day = 28 AND duration < 60
ORDER BY (calling.name);
-- The previous query returns:
-- name | name
-- **Bobby | Doris
-- ***Ernest | Berthold
-- **Evelyn | Larry
-- **Evelyn | Melissa
-- Kimberly | Jacqueline
-- **Madison | James
-- **Roger | Jack
-- ***Russell | Philip
-- **Victoria | Anna

-- People who took flights on the next day of the theft, on the earliest flight, and their destination.
SELECT name, flights.hour, destiny.city FROM people
JOIN passengers ON people.passport_number = passengers.passport_number
JOIN flights ON passengers.flight_id = flights.id
JOIN airports AS originated ON originated.id = flights.origin_airport_id
JOIN airports AS destiny ON destiny.id = flights.destination_airport_id
WHERE year = 2020 AND month = 7 AND day = 29 AND originated.city LIKE "Fiftyville"
ORDER BY (hour), name;
-- The previous query returns:
-- name | hour | city
-- ***Bobby | 8 | London
-- **Danielle | 8 | London
-- Doris | 8 | London
-- Edward | 8 | London
-- ****Ernest | 8 | London
-- ***Evelyn | 8 | London
-- ***Madison | 8 | London
-- ***Roger | 8 | London

-- The person who appeared on all the queries is Ernest, so he must be the theif,
-- and the person to whom he talked the day of the theft less than one minute was Berthold, so he must be his accomplice
-- he traveled to London on the earliest flight.

-- SELECT name FROM people
-- WHERE name IN
-- (SELECT name FROM people
-- JOIN courthouse_security_logs ON people.license_plate = courthouse_security_logs.license_plate
-- WHERE year = 2020 AND month = 7 AND day = 28 AND hour = 10 AND minute > 15 AND minute < 25 AND activity = "exit"
-- ORDER BY (people.name))
-- AND name IN
-- (SELECT name FROM people
-- JOIN bank_accounts ON people.id = bank_accounts.person_id
-- JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
-- WHERE year = 2020 AND month = 7 AND day = 28 AND atm_location LIKE "Fifer Street" AND transaction_type LIKE "withdraw"
-- ORDER BY (people.name))
-- AND name IN
-- (SELECT calling.name FROM phone_calls
-- JOIN people AS calling ON phone_calls.caller = calling.phone_number
-- JOIN people AS receiving ON phone_calls.receiver = receiving.phone_number
-- WHERE year = 2020 AND month = 7 AND day = 28 AND duration < 60
-- ORDER BY (calling.name))
-- AND name IN
-- (SELECT name FROM people
-- JOIN passengers ON people.passport_number = passengers.passport_number
-- JOIN flights ON passengers.flight_id = flights.id
-- JOIN airports AS originated ON originated.id = flights.origin_airport_id
-- JOIN airports AS destiny ON destiny.id = flights.destination_airport_id
-- WHERE year = 2020 AND month = 7 AND day = 29 AND originated.city LIKE "Fiftyville" AND hour = 8
-- ORDER BY (hour), name);
-- The previous query returns:
-- name
-- Ernest