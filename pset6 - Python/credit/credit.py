import cs50
import re

card = cs50.get_string("Number: ")


def main():

    # Regular expressions for the different credit cards
    AMEX = '^3[47]\d{13}$'
    MASTERCARD = '^5[12345]\d{14}$'
    VISA = '^4\d{15}$|^4\d{12}$'

    is_AMEX = re.fullmatch(AMEX, card)
    is_MASTERCARD = re.fullmatch(MASTERCARD, card)
    is_VISA = re.fullmatch(VISA, card)

    # Prints according to the validation of Lunhs Algorithm and the comparison of the regular expressions
    if(lunhAlgorithm(card)):
        if is_AMEX:
            print("AMEX")
        elif is_MASTERCARD:
            print("MASTERCARD")
        elif is_VISA:
            print("VISA")
    else:
        print("INVALID")

# Lunh Algorithm implementation


def lunhAlgorithm(card):
    # reverse the credit card number
    cardR = card[::-1]
    cardLen = len(card)
    lunh_sum = 0

    # iterate over every number
    for i in range(0, cardLen):
        # starting from the second to last (since its already reversed we start from the second digit)
        if i % 2 != 0:
            multiplied = int(cardR[i]) * 2
            if multiplied > 9:
                multiplied = str(multiplied)
                lunh_sum += int(multiplied[0]) + int(multiplied[1])
            else:
                lunh_sum += multiplied
        else:
            lunh_sum += int(cardR[i])

    # if the previous calculations return a number ending in 0 the card passes the check
    return (lunh_sum % 10 == 0)


main()