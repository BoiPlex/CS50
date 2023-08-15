import cs50


def main():
    num = cs50.get_int("Number: ")

    num_len = len(str(num))
    first_2_digits = int(str(num)[:2])

    if not valid_card_num(num):
        print("INVALID")
        return 1

    # American Express
    if num_len == 15 and (first_2_digits == 34 or first_2_digits == 37):
        print("AMEX")
    # Mastercard
    elif num_len == 16 and first_2_digits >= 51 and first_2_digits <= 55:
        print("MASTERCARD")
    # VISA
    elif (num_len == 13 or num_len == 16) and int(first_2_digits / 10) == 4:
        print("VISA")
    else:
        print("INVALID")


def valid_card_num(num):
    sum = 0
    # num as a string
    n = str(num)
    # Tracks every first and secnod digit
    tracker = 0

    # Iterate through every digit
    for i in reversed(n):
        digit = int(i)
        if tracker % 2 != 0:
            product = digit * 2
            # If product is 2 digits or not
            sum += product % 10 + int(product / 10)
        else:
            sum += digit

        tracker += 1

    return sum % 10 == 0


if __name__ == "__main__":
    main()