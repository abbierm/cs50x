"""
cash.py ask's users "how much change is owed?" and will output the smallest number of coins that are needed to create that amount of change
"""

# module for getting float so we don't need to convert the user's response
from cs50 import get_float

# Get's user response for the amount of change owed
change = get_float("Change owed: ")

# While loop that makes sure the user puts a number greater than 0
while change < 0:
    print("Please input a number greater than 0.")
    change = get_float("Change owed: ")

coins = 0

# quarters
while change >= .25:
    change -= .25
    change = round(change, 2)
    coins += 1

# dimes
while change >= .1:
    change -= .1
    change = round(change, 2)
    coins += 1

# nickles
while change >= .05:
    change -= .05
    change = round(change, 2)
    coins += 1

# pennies
while change >= .01:
    change -= .01
    change = round(change, 2)
    coins += 1

print(coins)
