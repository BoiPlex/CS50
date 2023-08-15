import csv
import sys

# In this program str stands for Short Tandem Repeats in relation to DNA sequences


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        sys.exit()

    # TODO: Read database file into a variable
    dna_data = []
    # list of STRs
    str_ls = []
    with open(sys.argv[1], 'r') as data:
        reader = csv.DictReader(data)
        str_ls = reader.fieldnames[1:]

        for person in reader:
            dna_data.append(person)

    # TODO: Read DNA sequence file into a variable
    with open(sys.argv[2], 'r') as seq:
        dna_seq = seq.readline()

    # TODO: Find longest match of each STR in DNA sequence
    # Dict of each str and its longest occurence
    longest_strs = {}
    for str in str_ls:
        longest_strs[str] = longest_match(dna_seq, str)

    # TODO: Check database for matching profiles
    match = "No Match"
    for person in dna_data:
        is_match = True
        for str, num in longest_strs.items():
            if num != int(person[str]):
                is_match = False
                break
        if is_match:
            match = person["name"]
            break
    print(match)

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


if __name__ == "__main__":
    main()
