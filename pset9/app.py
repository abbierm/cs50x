import os, datetime
from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, jsonify
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash


from helpers import apology, login_required, lookup, usd, update_holdings



# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Basic account information from users
    user_id = session["user_id"]
    row = db.execute("SELECT * FROM users WHERE id = ?", user_id)
    try:
        info = row[0]
    except (IndexError):
        # Session needs to be cleared
        redirect("/logout")

    # Queries db for current holdings
    holdings = db.execute("SELECT * FROM holdings WHERE userID = ?", user_id)

    # Updates the holdings rows with the current stock prices
    total_stock_value = 0
    for rows in holdings:
        symbol = rows["symbol"]
        quote = lookup(symbol)
        rows["price"] = quote["price"]
        total_price = quote["price"] * rows["shares"]
        rows["total"] = total_price
        total_stock_value += total_price


    grand_total = total_stock_value + info['cash']

    return render_template("index.html", info=info, holdings=holdings, total_stock_value=total_stock_value, grand_total=grand_total)



@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":

        user_id = session["user_id"]

        # Variables to validate request
        symbol = request.form.get("symbol").upper()
        shares = request.form.get("shares")

        # Ensure user filled in both inputs
        if not symbol or not shares:
            return apology("Both stock symbol and quantity must be inputted", 400)

        # Ensures shares is a valid positive integer
        try:
            quantity = int(shares)
        except (ValueError):
            return apology("Invalid quantity for shares", 400)
        if quantity < 1:
            return apology("You cannot buy a negative value for shares", 400)

        # Ensures stock symbol exists & grabs price
        quote = lookup(symbol)
        if not quote:
            return apology("Must provide a valid stock symbol", 400)

        # Checks how much money you have
        cash_dictionary = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
        funds = cash_dictionary[0]["cash"]
        total_price = quote["price"] * quantity

        if funds < total_price:
            # Too poor :(
            return apology("Insufficient funds in account", 400)

        # Updates users table
        new_total = funds - total_price
        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_total, user_id)

        # Updates transactions
        now = datetime.datetime.now()
        datetime_now = now.strftime("%m/%d/%Y, %H:%M:%S")
        db.execute("INSERT INTO transactions (userID, symbol, shares, total, time, type) VALUES(?, ?, ?, ?, ?, ?)", user_id, symbol, quantity, total_price, datetime_now, "BUY")


        cash_left = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
        cash = cash_left[0]["cash"]

        # Updates holdings table
        company_name = quote["name"]
        update_holdings(user_id, symbol, quantity, company_name, type="buy")
        flash("Bought!")
        return redirect("/")
    else:
        return render_template("buy.html")


# Used to help buyers search for the stock symbol of a company they know the name of
@app.route("/search")
def search():
    q = request.args.get("q")
    if q:
        upper_query = q.upper()
        symbols = db.execute("SELECT symbol, company FROM symbols WHERE company LIKE ? LIMIT 15", upper_query + "%")
    else:
        symbols = []
    return jsonify(symbols)


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    user_id = session["user_id"]
    purchases = db.execute("SELECT * FROM transactions where userID = ? ORDER BY OrderID DESC", user_id)


    return render_template("history.html", purchases=purchases)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 400)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "POST":

        # Ensure there is a symbol
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("must provide stock symbol", 400)

        # Use API and symbol to get quote
        quote = lookup(symbol)

        # Validate that there is a quote (symbol is correct)
        if not quote:
            return apology(f"Could not find {symbol}", 400)

        # Render template of quoted with the valid stock symbol
        else:
            return render_template("quoted.html", quote=quote)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User Posted
    if request.method == "POST":

        # Validate submission
        username = request.form.get("username")
        password = request.form.get("password")

        # Ensure username was submitted
        if not username:
            return apology("must provide username", 400)

        # Ensure both passwords submitted
        elif not password or not request.form.get("confirmation"):
            return apology("must provide password", 400)

        # Ensures both passwords match
        elif password != request.form.get("confirmation"):
            return apology("password and confirmation password must match", 400)

        # Check for duplicate usernames
        count = db.execute("SELECT username FROM users WHERE username = ?", username)
        if len(count) > 0:
            return apology("Username already exists", 400)

        # Generate password hash
        hashed_password = generate_password_hash(password)

        # Add new user to the database
        db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, hashed_password)

        # Returns them to their homepage?
        return redirect("/")

    # User Reached route via "GET"
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    user_id = session["user_id"]

    if request.method == "POST":

        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # Ensures user input a symbol
        if not symbol:
            return apology("Must provide stock symbol", 400)

        # Validates Shares Quantity
        try:
            quantity = int(shares)
        except (ValueError):
            return apology("Invalid quantity for shares", 400)
        if quantity < 1:
            return apology("You must enter a positive whole number for shares", 400)

        # Ensures symbol exists
        quote = lookup(symbol)
        if not quote:
            return apology("Must provide a valid stock symbol", 400)

        # Ensures user has enough stock to tell
        holdings = db.execute("SELECT symbol, shares FROM holdings WHERE userID = ? AND symbol = ?", user_id, symbol)
        try:
            shares_owned = holdings[0]["shares"]
            if quantity > shares_owned:
                return apology(f"You don't own enough {symbol} stock to sell", 400)
        except (ValueError, IndexError):
            return apology(f"You don't own enough {symbol} stock to sell", 400)

        # Updates purchases
        company_name = quote["name"]
        sale_price = quote["price"] * quantity
        db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", sale_price, user_id)
        now = datetime.datetime.now()
        datetime_now = now.strftime("%m/%d/%Y, %H:%M:%S")
        db.execute("INSERT INTO transactions (userID, symbol, shares, total, time, type) VALUES(?, ?, ?, ?, ?, ?)", user_id, symbol, shares, sale_price, datetime_now, "SELL")

        # Updates holdings table
        update_holdings(user_id, symbol, quantity, company_name, type="SELL")
        flash("Sold!")
        return redirect("/")

    else:
        holdings = db.execute("SELECT symbol FROM holdings WHERE userID = ?", user_id)
        return render_template("sell.html", holdings=holdings)
