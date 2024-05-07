import csv
import sys


def main():

    # Check for command-line usage
    if len(sys.argv) != 3:
        print("Usage Error: Must include csv file and DNA sequence file")
        sys.exit()

    # Read database file into a variable
    with open(sys.argv[1]) as csv_file:
        reader = csv.DictReader(csv_file)
        sequences = reader.fieldnames

        # Find longest match of each STR in DNA sequence
        sequence_values = {}
        with open(sys.argv[2]) as dna_file:
            x = dna_file.read()
            for sequence in sequences:
                sequence_values[sequence] = longest_match(x, sequence)

    # Check database for matching profiles
        for row in reader:
            fixed_row = {key: int(value) for key, value in row.items() if key != 'name'}
            goal_count = len(sequence_values)
            for k, v in sequence_values.items():
                if fixed_row.get(k, 0) == v:
                    goal_count -= 1
            if goal_count == 0:
                print(row['name'])
                return
        print('No match')

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
