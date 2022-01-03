from cs50 import get_string


def main():

    text = get_string("Text:")

    print(coleman_liau(text))


# Determines the grade of the text passed as arguement

def coleman_liau(text):

    letters = 0
    words = 1
    sentences = 0
    text = text.lower()

    # Checks every character and adds it according to the specs
    for i in range(0, len(text)):
        if text[i] >= "a" and text[i] <= "z":
            letters += 1
        elif text[i] == " ":
            words += 1
        elif text[i] == "." or text[i] == "!" or text[i] == "?":
            sentences += 1

    # letters per 100 words
    L = (letters * 100) / words
    # sentences per 100 words
    S = (sentences * 100) / words

    grade = round(0.0588 * L - 0.296 * S - 15.8)

    # Return the grade
    if grade < 1:
        return "Before Grade 1"
    elif grade > 1 and grade < 16:
        return f"Grade {grade}"
    else:
        return "Grade 16+"


main()

