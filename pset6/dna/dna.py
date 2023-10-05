import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        exit("Usage: python dna.py database.csv dna.txt")

    # TODO: Read database file into a variable
    database = []
    subsequences = []
    with open(sys.argv[1], "r") as file:
        for f in csv.DictReader(file):
            person = {}

            for key in f:
                person[key] = f[key]

                if key != "name":
                    person[key] = int(f[key])

                    if key not in subsequences:
                        subsequences.append(key)

            database.append(person)

    # TODO: Read DNA sequence file into a variable
    sequence = ""
    with open(sys.argv[2], "r") as file:
        sequence = file.read()

    # TODO: Find longest match of each STR in DNA sequence
    match = {}
    for subsequence in subsequences:
        current_longest_match = longest_match(sequence, subsequence)
        try:
            if current_longest_match > match[subsequence]:
                match[subsequence] = current_longest_match
        except KeyError:
                match[subsequence] = int(current_longest_match)

    # TODO: Check database for matching profiles
    result = "No match"
    for person in database:
        matched = False

        for subsequence in match:
            if person[subsequence] != match[subsequence]:
                matched = False
                break
            else:
                matched = True

        if matched:
            result = person["name"]

    print(result)
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
