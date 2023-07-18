"""
hello.py is a simple python program that asks user for their name and responds by saying hello.
"""

# CS50 function that gets user input in the form of a string
from cs50 import get_string

name = get_string("What is your name? ")
print(f"hello, {name}")
