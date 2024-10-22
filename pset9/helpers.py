import os
import requests
import urllib.parse

from cs50 import SQL

from flask import redirect, render_template, request, session
from functools import wraps

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

def apology(message, code=400):
    """Render message as an apology to user."""
    def escape(s):
        """
        Escape special characters.

        https://github.com/jacebrowning/memegen#special-characters
        """
        for old, new in [("-", "--"), (" ", "-"), ("_", "__"), ("?", "~q"),
                         ("%", "~p"), ("#", "~h"), ("/", "~s"), ("\"", "''")]:
            s = s.replace(old, new)
        return s
    return render_template("apology.html", top=code, bottom=escape(message)), code


def login_required(f):
    """
    Decorate routes to require login.

    https://flask.palletsprojects.com/en/1.1.x/patterns/viewdecorators/
    """
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if session.get("user_id") is None:
            return redirect("/login")
        return f(*args, **kwargs)
    return decorated_function


def lookup(symbol):
    """Look up quote for symbol."""

    # Contact API
    try:
        api_key = "pk_3f34436e111c4223abb5d10ae750e0a6"
        url = f"https://cloud.iexapis.com/stable/stock/{urllib.parse.quote_plus(symbol)}/quote?token={api_key}"
        response = requests.get(url)
        response.raise_for_status()
    except requests.RequestException:
        return None

    # Parse response
    try:
        quote = response.json()
        return {
            "name": quote["companyName"],
            "price": float(quote["latestPrice"]),
            "symbol": quote["symbol"]
        }
    except (KeyError, TypeError, ValueError):
        return None


def usd(value):
    """Format value as USD."""
    return f"${value:,.2f}"


def update_holdings(user_id, symbol, shares, name, type="BUY"):
    """ Updates the ownership table in SQL. """

    if type.upper() == "SELL":
        shares = -shares

    row = db.execute("SELECT * FROM holdings WHERE userID = ? AND symbol = ?", user_id, symbol)

    # Updates the value if already has shares
    try:
        if row[0]["symbol"]:
            # Update the value if the number exists
            updated_shares = shares + row[0]["shares"]
            if updated_shares != 0:
                db.execute("UPDATE holdings SET shares = ? WHERE userID = ? AND symbol = ? ", updated_shares, user_id, symbol)
            else:
                # Deleting the whole row if the user doesn't have any of the shares
                db.execute("DELETE FROM holdings WHERE userID = ? AND symbol = ?", user_id, symbol)

    # Adding a new row for previously unowned stock
    except (KeyError, IndexError):
        db.execute("INSERT INTO holdings (userID, symbol, company, shares) VALUES (?, ?, ?, ?)", user_id, symbol, name, shares)

    return None



