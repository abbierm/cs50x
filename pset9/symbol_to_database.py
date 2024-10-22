# symbol_to_database.py

"""
A simple python script to add a table with valid stock symbols and the company name to the finance.db.

Ideally this will allow people to search for a stock symbol and have a list of valid stock symbols and symbols populate underneath the search bar.

 want to create a table inside of finance.db that allows people to search for stock symbols and have a list of the symbols populate underneath the search box with valid symbols and the company name.

Original source of stock symbols provided by user rreichel3 on github at https://github.com/rreichel3/US-Stock-Symbols

"""

from cs50 import SQL
import os
from pathlib import Path
from helpers import lookup


db = SQL("sqlite:///finance.db")

this_folder = os.path.dirname(__file__)
symbols_filepath = Path(this_folder , "all_tickers.txt")

symbols = open(symbols_filepath).readlines()
symbol_list = [symbol.rstrip("\n") for symbol in symbols]



for symbol in symbol_list:
    quote = lookup(symbol)
    if quote == None:
        print(f"{symbol} not added to database")
        continue
    else:
        name = quote['name']
        db.execute("INSERT INTO symbols (symbol, company) VALUES (?, ?)",  symbol, name)
        print(f"{symbol} added to database")

