from cs50 import get_int


def get_creditcard_number() -> int:
    num = get_int('Number: ')
    return num


def checksum_check(x: int) -> bool:
    """Returns True if x passes luhn's algo."""
    assert isinstance(x, int), 'x should be an integer.'
    x = str(x)
    one, two = 0, 0
    while len(x) > 0:
        if len(x) == 1:
            one += int(x)
            break
        even, odd = int(x[-2]) * 2, int(x[-1])
        one += odd
        even = str(even)
        while len(even) > 0:
            two += int(even[-1])
            even = even[:-1]
        x = x[:-2]
    total_sum = one + two
    if total_sum % 10 == 0:
        return True
    else:
        return False


def check_visa(x: int) -> bool:
    # Checks the length
    if len(str(x)) != 13 and len(str(x)) != 16:
        return False
    first = int(str(x)[0])
    if first == 4:
        return True
    else:
        return False


def check_mastercard(x: int) -> bool:
    if len(str(x)) != 16:
        return False
    first_two = int(str(x)[:2])
    if first_two >= 51 and first_two < 56:
        return True
    return False


def check_amex(x: int) -> bool:
    if len(str(x)) != 15:
        return False
    first_two = int(str(x)[:2])
    if first_two != 34 and first_two != 37:
        return False
    return True


if __name__ == "__main__":
    cc = get_creditcard_number()
    if checksum_check(cc) == False:
        print("INVALID\n")
    else:
        if check_visa(cc) == True:
            print("VISA\n")
        elif check_mastercard(cc) == True:
            print("MASTERCARD\n")
        elif check_amex(cc) == True:
            print("AMEX\n")
        else:
            print("INVALID\n")
