"""
mario.py takes a user's integer input and creates a half pyramid of that size
"""

# CS50 module to get an integer from the user.
from cs50 import get_int

# Gets user height integer of pyramid.
height = get_int('Height: ')

# Checks user's input for valid integer.
while height < 1 or height > 8:
    print('Please choose a height between 1 and 8')
    height = get_int('Height: ')

# For loop that prints the pyramid
for i in range(height):
    spaces = height - (i + 1)
    row = " " * spaces + "#" * (i + 1)
    print(row)
