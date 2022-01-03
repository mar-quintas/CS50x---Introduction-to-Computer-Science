import sys
import csv
import re

# Check usage
if len(sys.argv) != 3:
    sys.exit("Usage: python dna.py data.csv sequence.txt")

database = []

# Open csv and makes a list of dictionaries from it, also takes the headers in another file
with open(sys.argv[1], "r") as file:
    reader = csv.DictReader(file)
    sequences = reader.fieldnames

    for person in reader:
        database.append(person)

dna_str = {}

# Creates from the headers a new dictionary
for seq in sequences:
    if seq == "name":
        continue
    dna_str[seq] = {
        "count": 0,
        "lenght": len(seq),
        "reg": re.compile(f"(({seq})+)")
    }

# Open txt with dna sequence to identify
with open(sys.argv[2], "r") as file2:
    dna = file2.read()

# Finds the sequences present in the header from de csv file in the dna sequence to identify
# Counts the highest number of sequential appereances and saves it
for seq in dna_str:

    reg_search = dna_str[seq]["reg"].findall(dna)

    for j in range(len(reg_search)):
        STR_count = round(len(reg_search[j][0]) / dna_str[seq]["lenght"])

        if STR_count > dna_str[seq]["count"]:
            dna_str[seq]["count"] = STR_count

result = "No match"

# Checks every person in the database from de csv file against the info gathered from the dna sequence in the txt
for person in database:
    match = True

    for key in person:
        if key == "name":
            continue
        if int(person[key]) != dna_str[key]["count"]:
            match = False
            break
    if match:
        result = person["name"]
        break

# prints no match when the sequence doesnt belong to any person in te csv or the name of the person if found
print(result)