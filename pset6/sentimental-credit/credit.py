# Credit.py authenticates a credit card numbers and prints the CC provider

# Import get_string for input validation
from cs50 import get_string
import re


def main():

    # Gets credit card number from user
    cc = get_string("Number: ")

    # Validate CC by using Luhns algorithim
    if check_luhns(cc) == True:
        # Uses functions to check for each of the different CC companies
        if check_amex(cc) == True:
            print("AMEX")
        elif check_visa(cc) == True:
            print("VISA")
        elif check_mastercard(cc) == True:
            print("MASTERCARD")
        else:
            print("INVALID")
    else:
        print("INVALID")


def check_luhns(number):
    """Returns True if credit card number valid using luhn's algorithim."""
    # Make a list with the numbers
    list_of_numbers = [int(i) for i in number]
    list_of_numbers.reverse()

    # Variable to store sums
    evens = []
    odds = 0

    # Iterates through the list of numbers in reverse order
    for j in range(len(list_of_numbers)):
        number = list_of_numbers[j]
        # Checks every other digit
        # Even number's index modulus won't be 0 if divided by two.
        if j % 2 != 0:
            # Multiplies the digit by two
            product = 2 * number
            # Converts the product to a string and adds to the evens list
            evens.append(str(product))
        else:
            # Adds the number to the odds sum
            odds += number

    # Iterates through the evens list
    even_sum = 0
    for h in evens:
        # Iterates through the numbers digits
        for g in h:
            # Converts each digit to an integer and adds to even_sum
            even_sum += int(g)
    string_sum = str(odds + even_sum)

    if string_sum[-1] == '0':
        return True
    else:
        return False


def check_amex(number):
    """Returns True if credit card number is a valid for American Express."""
    amex_regex = re.compile(r"3(5|7)\d\d\d\d\d\d\d\d\d\d\d\d\d")
    matched_object = amex_regex.search(number)
    if matched_object == None:
        return False
    else:
        return True


def check_visa(number):
    """Returns True is credit card number is a valid for Visa."""
    visa_regex = re.compile(r"4\d\d\d\d\d\d\d\d\d\d\d\d(\d\d\d)?")
    matched_object = visa_regex.search(number)
    if matched_object == None:
        return False
    else:
        return True


def check_mastercard(number):
    """Returns True if credit card number is valid for Mastercard."""
    mastercard_regex = re.compile(r"5(1|2|3|4|5)\d\d\d\d\d\d\d\d\d\d\d\d\d\d")
    matched_object = mastercard_regex.search(number)
    if matched_object == None:
        return False
    else:
        return True


if __name__ == "__main__":
    main()
