"""
readability.py asks users to input text and gives a readability score based off the Coleman-Liau index.
"""

from cs50 import get_string
import re


def main():

    # Gets text from the user
    text = get_string("Text: ")

    # Functions to get the letter, word, and sentences
    l = letter_count(text)
    w = word_count(text)
    s = sentence_count(text)

    average_letters = (l / w) * 100
    average_sentences = (s / w) * 100

    # Computing the Coleman-Liau index
    reading_grade = int(round((.0588 * average_letters) - (.296 * average_sentences) - 15.8))

    #reading_grade = int(reading_index)

    # Uses conditionals to print out reading score.
    if reading_grade < 1:
        print("Before Grade 1")
    elif reading_grade >= 16:
        print("Grade 16+")
    else:
        print(f"Grade {reading_grade}")


def letter_count(words):
    """Gets the average letters per 100 words"""
    # Creating a regex object to search for letters
    letter_regex = re.compile(r"[a-zA-Z]")

    letters = 0

    # Iterates through every word and every character to see if it is a valid letter
    for characters in words:
        letter_search = letter_regex.search(characters)
        if letter_search != None:
            letters += 1

    return letters


def word_count(words):
    """Takes in a string to count the number of words within that string."""

    list_of_words = words.split()
    number_of_words = len(list_of_words)
    return number_of_words


def sentence_count(words):
    """Returns the number of sentences there are in a string."""

    punctuation = (".", "?", "!")
    sentences = 0
    for character in words:
        if character in punctuation:
            sentences += 1

    return sentences


if __name__ == "__main__":
    main()
