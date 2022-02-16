-- Keep a log of any SQL queries you execute as you solve the mystery.
-- CREATE TABLE crime_scene_reports (
--     id INTEGER,
--     year INTEGER,
--     month INTEGER,
--     day INTEGER,
--     street TEXT,
--     description TEXT,
--     PRIMARY KEY(id)
-- );
-- CREATE TABLE interviews (
--     id INTEGER,
--     name TEXT,
--     year INTEGER,
--     month INTEGER,
--     day INTEGER,
--     transcript TEXT,
--     PRIMARY KEY(id)
-- );
-- CREATE TABLE courthouse_security_logs (
--     id INTEGER,
--     year INTEGER,
--     month INTEGER,
--     day INTEGER,
--     hour INTEGER,
--     minute INTEGER,
--     activity TEXT,
--     license_plate TEXT,
--     PRIMARY KEY(id)
-- );
-- CREATE TABLE atm_transactions (
--     id INTEGER,
--     account_number INTEGER,
--     year INTEGER,
--     month INTEGER,
--     day INTEGER,
--     atm_location TEXT,
--     transaction_type TEXT,
--     amount INTEGER,
--     PRIMARY KEY(id)
-- );
-- CREATE TABLE bank_accounts (
--     account_number INTEGER,
--     person_id INTEGER,
--     creation_year INTEGER,
--     FOREIGN KEY(person_id) REFERENCES people(id)
-- );
-- CREATE TABLE airports (
--     id INTEGER,
--     abbreviation TEXT,
--     full_name TEXT,
--     city TEXT,
--     PRIMARY KEY(id)
-- );
-- CREATE TABLE flights (
--     id INTEGER,
--     origin_airport_id INTEGER,
--     destination_airport_id INTEGER,
--     year INTEGER,
--     month INTEGER,
--     day INTEGER,
--     hour INTEGER,
--     minute INTEGER,
--     PRIMARY KEY(id),
--     FOREIGN KEY(origin_airport_id) REFERENCES airports(id),
--     FOREIGN KEY(destination_airport_id) REFERENCES airports(id)
-- );
-- CREATE TABLE passengers (
--     flight_id INTEGER,
--     passport_number INTEGER,
--     seat TEXT,
--     FOREIGN KEY(flight_id) REFERENCES flights(id)
-- );
-- CREATE TABLE phone_calls (
--     id INTEGER,
--     caller TEXT,
--     receiver TEXT,
--     year INTEGER,
--     month INTEGER,
--     day INTEGER,
--     duration INTEGER,
--     PRIMARY KEY(id)
-- );
-- CREATE TABLE people (
--     id INTEGER,
--     name TEXT,
--     phone_number TEXT,
--     passport_number INTEGER,
--     license_plate TEXT,
--     PRIMARY KEY(id)
-- );

-- Will go straight to the scene reports to understand the situation more for the exact date.
SELECT * FROM crime_scene_reports WHERE year = 2020 AND day = 28 AND month = "7";
-- 295 | 2020 | 7 | 28 | Chamberlin Street | Theft of the CS50 duck took place at 10:15am at the Chamberlin Street courthouse. Interviews were conducted today with three witnesses who were present at the time — each of their interview transcripts mentions the courthouse.

-- Will search the interviews for anything that reports courthouse on that day.
SELECT * FROM interviews WHERE transcript like "%courthouse%" AND year = 2020 AND day = 28 AND month = "7";
-- id | name | year | month | day | transcript
-- 161 | Ruth | 2020 | 7 | 28 | Sometime within ten minutes of the theft, I saw the thief get into a car in the courthouse parking lot and drive away. If you have security footage from the courthouse parking lot, you might want to look for cars that left the parking lot in that time frame.
-- 162 | Eugene | 2020 | 7 | 28 | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at the courthouse, I was walking by the ATM on Fifer Street and saw the thief there withdrawing some money.
-- 163 | Raymond | 2020 | 7 | 28 | As the thief was leaving the courthouse, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket.

-- Reports Mention the courthouse parking lot so i will go to the courthouse logs and check that. Theft happened at 10:15 and apparently they left carpark within 10 mins .
SELECT * FROM courthouse_security_logs WHERE year = 2020 AND day = 28 AND month = "7" AND hour = 10 AND minute > 15 AND minute <= 25 AND activity = "exit";
-- id | year | month | day | hour | minute | activity | license_plate
-- 260 | 2020 | 7 | 28 | 10 | 16 | exit | 5P2BI95
-- 261 | 2020 | 7 | 28 | 10 | 18 | exit | 94KL13X
-- 262 | 2020 | 7 | 28 | 10 | 18 | exit | 6P58WS2
-- 263 | 2020 | 7 | 28 | 10 | 19 | exit | 4328GD8
-- 264 | 2020 | 7 | 28 | 10 | 20 | exit | G412CB7
-- 265 | 2020 | 7 | 28 | 10 | 21 | exit | L93JTIZ
-- 266 | 2020 | 7 | 28 | 10 | 23 | exit | 322W7JE
-- 267 | 2020 | 7 | 28 | 10 | 23 | exit | 0NTHK55

-- Will now check out atm logs on that day as they were seen withdrawing money earlier here.
SELECT * FROM atm_transactions WHERE year = 2020 AND day = 28 AND month = "7" AND atm_location = "Fifer Street" AND transaction_type = "withdraw";
-- id | year | month | day | hour | minute | activity | license_plate
-- 235 | 2020 | 7 | 28 | 8 | 25 | exit | HOD8639
-- 260 | 2020 | 7 | 28 | 10 | 16 | exit | 5P2BI95
-- 261 | 2020 | 7 | 28 | 10 | 18 | exit | 94KL13X
-- 262 | 2020 | 7 | 28 | 10 | 18 | exit | 6P58WS2
-- 263 | 2020 | 7 | 28 | 10 | 19 | exit | 4328GD8
-- 264 | 2020 | 7 | 28 | 10 | 20 | exit | G412CB7
-- 265 | 2020 | 7 | 28 | 10 | 21 | exit | L93JTIZ
-- 266 | 2020 | 7 | 28 | 10 | 23 | exit | 322W7JE
-- 267 | 2020 | 7 | 28 | 10 | 23 | exit | 0NTHK55
-- 280 | 2020 | 7 | 28 | 14 | 18 | exit | NAW9653
-- 282 | 2020 | 7 | 28 | 15 | 16 | exit | 94MV71O
-- 289 | 2020 | 7 | 28 | 17 | 16 | exit | V47T75I
-- 290 | 2020 | 7 | 28 | 17 | 18 | exit | R3G7486

-- Going to grab the names of all the people that did these transactions
SELECT * FROM people WHERE id IN (SELECT person_id FROM bank_accounts WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE year = 2020 AND day = 28 AND month = "7" AND atm_location = "Fifer Street" AND transaction_type = "withdraw"));
-- id | name | phone_number | passport_number | license_plate
-- 395717 | Bobby | (826) 555-1652 | 9878712108 | 30G67EN
-- 396669 | Elizabeth | (829) 555-5269 | 7049073643 | L93JTIZ
-- 438727 | Victoria | (338) 555-6650 | 9586786673 | 8X428L0
-- 449774 | Madison | (286) 555-6063 | 1988161715 | 1106N58
-- 458378 | Roy | (122) 555-4581 | 4408372428 | QX4YZN3
-- 467400 | Danielle | (389) 555-5198 | 8496433585 | 4328GD8
-- 514354 | Russell | (770) 555-1861 | 3592750733 | 322W7JE
-- 686048 | Ernest | (367) 555-5533 | 5773159633 | 94KL13X

-- As per  one of the interviews they were talking about taking the earliest flight possible, so i will grab all the airports.
SELECT * FROM airports;
-- id | abbreviation | full_name | city
-- 1 | ORD | O'Hare International Airport | Chicago
-- 2 | PEK | Beijing Capital International Airport | Beijing
-- 3 | LAX | Los Angeles International Airport | Los Angeles
-- 4 | LHR | Heathrow Airport | London
-- 5 | DFS | Dallas/Fort Worth International Airport | Dallas
-- 6 | BOS | Logan International Airport | Boston
-- 7 | DXB | Dubai International Airport | Dubai
-- 8 | CSF | Fiftyville Regional Airport | Fiftyville
-- 9 | HND | Tokyo International Airport | Tokyo
-- 10 | CDG | Charles de Gaulle Airport | Paris
-- 11 | SFO | San Francisco International Airport | San Francisco
-- 12 | DEL | Indira Gandhi International Airport | Delhi

-- Will grab the earliest flights from fiftyville on the next day 29th
SELECT * FROM flights WHERE year = 2020 AND day = 29 AND month = 7 ORDER BY hour;
-- id | origin_airport_id | destination_airport_id | year | month | day | hour | minute
-- 36 | 8 | 4 | 2020 | 7 | 29 | 8 | 20
-- 43 | 8 | 1 | 2020 | 7 | 29 | 9 | 30
-- 23 | 8 | 11 | 2020 | 7 | 29 | 12 | 15
-- 53 | 8 | 9 | 2020 | 7 | 29 | 15 | 20
-- 18 | 8 | 6 | 2020 | 7 | 29 | 16 | 0

-- Grab all the peoples details which were on the earliest flight (36)
SELECT * FROM people WHERE passport_number IN (SELECT passport_number FROM passengers WHERE flight_id = 36);
-- id | name | phone_number | passport_number | license_plate
-- 395717 | Bobby | (826) 555-1652 | 9878712108 | 30G67EN
-- 398010 | Roger | (130) 555-0289 | 1695452385 | G412CB7
-- 449774 | Madison | (286) 555-6063 | 1988161715 | 1106N58
-- 467400 | Danielle | (389) 555-5198 | 8496433585 | 4328GD8
-- 560886 | Evelyn | (499) 555-9472 | 8294398571 | 0NTHK55
-- 651714 | Edward | (328) 555-1152 | 1540955065 | 130LD9Z
-- 686048 | Ernest | (367) 555-5533 | 5773159633 | 94KL13X
-- 953679 | Doris | (066) 555-9701 | 7214083635 | M51FA04

-- Find all the callers on that day which were less then a minute
SELECT * FROM phone_calls WHERE year = 2020 AND day = 28 AND month = 7 AND duration < 60;
-- id | caller | receiver | year | month | day | duration
-- 221 | (130) 555-0289 | (996) 555-8899 | 2020 | 7 | 28 | 51
-- 224 | (499) 555-9472 | (892) 555-8872 | 2020 | 7 | 28 | 36
-- 233 | (367) 555-5533 | (375) 555-8161 | 2020 | 7 | 28 | 45
-- 251 | (499) 555-9472 | (717) 555-1342 | 2020 | 7 | 28 | 50
-- 254 | (286) 555-6063 | (676) 555-6554 | 2020 | 7 | 28 | 43
-- 255 | (770) 555-1861 | (725) 555-3243 | 2020 | 7 | 28 | 49
-- 261 | (031) 555-6622 | (910) 555-3251 | 2020 | 7 | 28 | 38
-- 279 | (826) 555-1652 | (066) 555-9701 | 2020 | 7 | 28 | 55
-- 281 | (338) 555-6650 | (704) 555-2131 | 2020 | 7 | 28 | 54


SELECT * FROM people WHERE phone_number in (SELECT caller FROM phone_calls WHERE year = 2020 AND day = 28 AND month = 7 AND duration < 60);
-- Caller
-- id | name | phone_number | passport_number | license_plate
-- 395717 | Bobby | (826) 555-1652 | 9878712108 | 30G67EN
-- 398010 | Roger | (130) 555-0289 | 1695452385 | G412CB7
-- 438727 | Victoria | (338) 555-6650 | 9586786673 | 8X428L0
-- 449774 | Madison | (286) 555-6063 | 1988161715 | 1106N58
-- 514354 | Russell | (770) 555-1861 | 3592750733 | 322W7JE
-- 560886 | Evelyn | (499) 555-9472 | 8294398571 | 0NTHK55
-- 686048 | Ernest | (367) 555-5533 | 5773159633 | 94KL13X
-- 907148 | Kimberly | (031) 555-6622 | 9628244268 | Q12B3Z3


SELECT * FROM people WHERE phone_number in (SELECT receiver FROM phone_calls WHERE year = 2020 AND day = 28 AND month = 7 AND duration < 60); 
-- Receiver
-- id | name | phone_number | passport_number | license_plate
-- 250277 | James | (676) 555-6554 | 2438825627 | Q13SVG6
-- 251693 | Larry | (892) 555-8872 | 2312901747 | O268ZZ0
-- 484375 | Anna | (704) 555-2131 |  | 
-- 567218 | Jack | (996) 555-8899 | 9029462229 | 52R0Y8U
-- 626361 | Melissa | (717) 555-1342 | 7834357192 | 
-- 712712 | Jacqueline | (910) 555-3251 |  | 43V0R5D
-- 847116 | Philip | (725) 555-3243 | 3391710505 | GW362R6
-- 864400 | Berthold | (375) 555-8161 |  | 4V16VO0
-- 953679 | Doris | (066) 555-9701 | 7214083635 | M51FA04


-- All the people with where their license plate left in the hour of the event.
SELECT * FROM people WHERE license_plate in (SELECT * FROM courthouse_security_logs WHERE year = 2020 AND day = 28 AND month = "7" AND hour = 10 AND minute > 15 AND minute <= 25 AND activity = "exit");
-- id | name | phone_number | passport_number | license_plate
-- 221103 | Patrick | (725) 555-4692 | 2963008352 | 5P2BI95
-- 243696 | Amber | (301) 555-4174 | 7526138472 | 6P58WS2
-- 396669 | Elizabeth | (829) 555-5269 | 7049073643 | L93JTIZ
-- 398010 | Roger | (130) 555-0289 | 1695452385 | G412CB7
-- 467400 | Danielle | (389) 555-5198 | 8496433585 | 4328GD8
-- 514354 | Russell | (770) 555-1861 | 3592750733 | 322W7JE
-- 560886 | Evelyn | (499) 555-9472 | 8294398571 | 0NTHK55
-- 686048 | Ernest | (367) 555-5533 | 5773159633 | 94KL13X




-- Thief = Ernest
-- Berthold as they were on the other end of the phone.

-- To London

