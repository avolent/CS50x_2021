import sys
import csv


def main():

    # Ensure correct usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py database.csv sequence.txt")
    database = sys.argv[1]
    sequence = sys.argv[2]
    people = []
    
    # Read database file and load into memory
    with open(database) as file:
        reader = csv.DictReader(file)
        for person in reader:
            people.append(person)
    # print(people)
    # Get list of searchable STRs
    STR = list(people[0].keys())[1:]
    # print(STR)
    # Read sequence file and load into memory
    with open(sequence) as file:
        string = file.read()
        # print(string)
        
        # For each STR check the longest repetition in string.
        selection = 0
        counts = []
        while selection < len(STR):
            match = str(STR[selection])
            counts.append(0)
            # print(f"Searching for: {match}")
            # Add another STR to the search until it cant see anymore and count.
            while True:
                if match in string:
                    # print(f"Found: {match}")
                    match = match + str(STR[selection])
                    counts[selection] += 1
                else:
                    break
            # print(f"Count: {counts[selection]}")
            selection += 1
        str_dict = dict(zip(STR, counts))
        # print(f"Looking for: {str_dict}")
        match = "No match"
        
        # Loop through people until you find a perfect match
        while True:
            for person in people:
                selection = 0
                # print(person)
                # Loop through persons STRs until they find a match
                while selection < len(STR):
                    if str_dict[STR[selection]] == int(person[STR[selection]]):
                        # print(f"{STR[selection]} count matches {person['name']}")
                        selection += 1   
                        continue
                    else:
                        # print(f"{STR[selection]} count does not match {person['name']}")
                        break
                # If all STR counts are a perfect match, break from loop.
                if selection == len(STR):
                    match = person['name']
                    break
            break   
        print(match)


main()
