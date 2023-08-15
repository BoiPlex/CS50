-- Keep a log of any SQL queries you execute as you solve the mystery.
/* Theft Info
- Stolen CS50 Duck
- Date: July 28, 2021
- Location: Humphrey Street
*/

-- Use year, month, day, street, CS50 Duck to get crime report description
SELECT c.description
  FROM crime_scene_reports AS c
 WHERE c.year = 2021 AND c.month = 7 AND c.day = 28 AND c.street = "Humphrey Street" AND c.description LIKE "%CS50 Duck%";
-- Result: Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery.
-- New Info: 10:15am at bakery. 3 witnesses mention bakery in interview transcripts

-- Use year, month, day, bakery in transcript to get witness names and transcripts
SELECT i.name,
       i.transcript
  FROM interviews AS i
 WHERE i.year = 2021 AND i.month = 7 AND i.day = 28 AND i.transcript LIKE "%bakery%";
/* Result:
- Ruth: Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.
- Eugene: I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.
- Raymond: As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket.
*/
/* New Info:
- Check bakery security logs within 10 min of 10:15am
- Check Leggett Street ATM, thief withdrawing money
- Check phone calls, duration < 60s, receiver purchased thief's flight ticket
- Check flights for earliest flight out of Fiftyville (thief)
*/

-- Use year, month, day, time, activity to get license_plates
SELECT b.license_plate
  FROM bakery_security_logs AS b
 WHERE b.year = 2021 AND b.month = 7 AND b.day = 28 AND b.hour = 10 AND b.minute BETWEEN 5 AND 25 AND b.activity = "exit";
-- New Info: License plates (Use for verifying suspects)

-- Use year, month, day, atm location, transaction type to get account numbers and amounts
SELECT a.account_number,
       a.amount
  FROM atm_transactions AS a
 WHERE a.year = 2021 AND a.month = 7 AND a.day = 28 AND a.atm_location = "Leggett Street" AND a.transaction_type = "withdraw";
-- New Info: Account numbers and amounts

-- Use year, month, day, call duration to get callers and receivers
SELECT pc.caller,
       pc.receiver
  FROM phone_calls AS pc
 WHERE pc.year = 2021 AND pc.month = 7 AND pc.day = 28 AND pc.duration < 60;
 -- New Info: Phone numbers of callers and receivers (Use for verifying suspects)

-- Use origin airport, year, month, day, earliest flight time to get destination airport
SELECT a.abbreviation,
       a.full_name,
       a.city
  FROM airports AS a
 WHERE a.id = (SELECT f.id
                 FROM flights AS f
                WHERE f.origin_airport_id = (SELECT a.id
                                               FROM airports AS a
                                              WHERE a.city = "Fiftyville")
                  AND f.year = 2021 AND f.month = 7 AND f.day = 29
              ORDER BY hour ASC, minute ASC
              LIMIT 1);
-- New Info: Thief flew to LaGuardia Airport (LGA) in New York City
-- The thief escaped to New York City

-- Get flight id
SELECT f.id
  FROM flights AS f
 WHERE f.origin_airport_id = (SELECT a.id
                                FROM airports AS a
                               WHERE a.city = "Fiftyville")
                                 AND f.year = 2021 AND f.month = 7 AND f.day = 29
                             ORDER BY hour ASC, minute ASC
                             LIMIT 1;
-- Result: 36
-- Access to passengers in flight

-- Use phone numbers, license plates, bank accounts to get suspect
SELECT p.name,
       p.passport_number
  FROM people AS p
 WHERE p.phone_number IN (SELECT pc.caller
                            FROM phone_calls AS pc
                           WHERE pc.year = 2021 AND pc.month = 7 AND pc.day = 28 AND pc.duration < 60)
   AND p.license_plate IN (SELECT b.license_plate
                             FROM bakery_security_logs AS b
                            WHERE b.year = 2021 AND b.month = 7 AND b.day = 28 AND b.hour = 10 AND b.minute BETWEEN 5 AND 25 AND b.activity = "exit")
   AND p.id IN (SELECT ba.person_id
                  FROM bank_accounts AS ba
                 WHERE ba.account_number IN (SELECT a.account_number
                                               FROM atm_transactions AS a
                                              WHERE a.year = 2021 AND a.month = 7 AND a.day = 28 AND a.atm_location = "Leggett Street" AND a.transaction_type = "withdraw"));
-- Suspect 1: Diana, 3592750733
-- Suspect 2: Bruce, 5773159633

-- Use flight id and suspect passports to check passengers
SELECT p.passport_number,
       p.seat
  FROM passengers AS p
 WHERE p.flight_id = 36 AND p.passport_number IN (3592750733, 5773159633);
-- Suspect 2 was in the flight
-- The thief is Bruce

-- Use Bruce's phone number and crime's phone call info to get accomplice
SELECT p.name
  FROM people AS p
 WHERE p.phone_number = (SELECT pc.receiver
                                FROM phone_calls AS pc
                               WHERE pc.caller = (SELECT p.phone_number
                                                  FROM people AS p
                                                  WHERE p.passport_number = 5773159633)
                                AND pc.year = 2021 AND pc.month = 7 AND pc.day = 28 AND pc.duration < 60);
-- The accomplice is Robin