import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Query for every different quote the user ever bought and add by the same symbol
    share = db.execute(
        'SELECT symbol, name, SUM(shares) AS shares FROM quotes GROUP BY symbol HAVING users_id=?', session['user_id'])
    shares_total = 0

    # For every different symbol check the current price and multiply it by the number of shares
    for quote in share:
        quote_price = lookup(quote['symbol'])
        value = int(quote['shares']) * quote_price['price']
        quote['price'] = quote_price['price']
        quote['total'] = value
        shares_total += value

    # Check the user's current cash
    cash = db.execute('SELECT cash FROM users WHERE id=?', session['user_id'])

    return render_template("index.html", share=share, cash=cash[0]['cash'], grand_total=shares_total+cash[0]['cash'])


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":

        symbol = request.form.get("symbol")
        quote = lookup(symbol)

        # Check if the symbol exits and is valid
        if not symbol:
            return apology("Please enter a Symbol")
        if not quote:
            return apology("Invalid Symbol")

        share_inp = request.form.get('shares')
        # If the input is not an integer
        if not share_inp.isnumeric():
            return apology("Please enter a round number")

        shares = int(request.form.get("shares"))

        # Check if the number of shares is valid
        if shares <= 0:
            return apology("Please enter a number greater than 0")

        quotes_price = quote['price'] * shares
        cash = db.execute("SELECT cash FROM users WHERE id=?", session['user_id'])

        # If the user has enough funds
        if quotes_price <= cash[0]['cash']:
            db.execute("BEGIN TRANSACTION")
            # Witdhraw the amount of money resulting from the cost of each share multilpied by the number of shares from the user's cash
            db.execute("UPDATE users SET cash = (cash - ?) WHERE id = ?", quotes_price, session['user_id'])
            # Add the information from the purchase to a table that stores that info
            db.execute("INSERT INTO quotes(symbol, price, shares, name, users_id) VALUES ( ?,?,?,?,?)",
                       symbol, quote['price'], shares, quote['name'], session['user_id'])
            db.execute("COMMIT")

            # Redirect user to home page
            return redirect("/")
        else:
            return apology("Not enough funds")

        # INSERT INTO quotes(symbol, price, shares, name, users_id) VALUES ( ?,?,?,?,?) ON CONFLICT(users_id, symbol)
        # DO UPDATE SET price = ?, shares = shares + ?, name = ?
        # WHERE users_id = ?",
        # symbol, quote['price'], shares, quote['name'], session['user_id'], quote['price'], shares, quote['name'], session['user_id'])

        # "UPDATE quotes SET symbol = ?, price = ?, shares = ?, name = ? WHERE users_id = ?"
        # , symbol, quote['price'], shares, quote['name'], session['user_id'])

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    share = db.execute('SELECT * FROM quotes WHERE users_id=?', session['user_id'])

    for op in share:
        if int(op['shares']) > 0:
            op['type'] = 'buy'
        else:
            op['type'] = 'sell'

    return render_template("history.html", share=share)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

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
        quoted = lookup(request.form.get("symbol"))

        if not quoted:
            return apology("Please enter a valid symbol")

        return render_template("quote.html", quoted=quoted)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":

        username = request.form.get('username')
        # Store the hash generated from the password
        pass_hash = generate_password_hash(request.form.get("password"))

        # Check if username was provided
        if not username:
            return apology("Must provide username")

        # Check if username already exists
        check_user = []
        for dic in db.execute("SELECT username FROM users"):
            check_user.append(dic['username'])

        if username in check_user:
            return apology("Username already exists, please select another")

        # Check if password was provided
        if not request.form.get('password'):
            return apology("Must provide password")

        # Check if password confirmation was provided and matches the original
        if not request.form.get('confirmation') == request.form.get('password'):
            return apology("Must enter same password")

        # Store the username and the password Hash in finance.db
        id = db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, pass_hash)

        # Log in the user and remember it
        user_id = db.execute("SELECT id FROM users WHERE username = ?", username)

        session["user_id"] = user_id[0]['id']

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # Find the different symbols the user owns stock from
    quotes = db.execute('SELECT DISTINCT symbol FROM quotes WHERE users_id=?', session['user_id'])
    q_symbol = []
    for dic in quotes:
        q_symbol.append(dic['symbol'])

    # If user submitted the sell form
    if request.method == "POST":

        symbol = request.form.get('symbol')
        owned_shares = db.execute('SELECT SUM(shares) as shares FROM quotes GROUP BY symbol HAVING users_id=?', session['user_id'])

        # Check if symbol was provided
        if not symbol:
            return apology('Please select a symbol')

        # Check if the user owns any shares of that symbol
        if symbol not in q_symbol:
            return apology('You do not own any shares of this symbol')

        # Check if number of shares was provided
        if not request.form.get("shares"):
            return apology('Please select number of shares to sell')

        # If the input is not an integer
        if not request.form.get("shares").isnumeric():
            return apology("Please enter a round number")

        shares = int(request.form.get("shares"))

        # Check if the user owns the amount of shares to sell
        if shares > owned_shares[0]['shares']:
            return apology('You do not own that many shares')

        # If input passes the previous validations begin sell
        quote = lookup(symbol)
        quotes_price = shares * quote['price']

        db.execute("BEGIN TRANSACTION")
        # Add the information from the sell to a table that stores that info
        db.execute("INSERT INTO quotes(symbol, price, shares, name, users_id) VALUES ( ?,?,?,?,?)",
                   symbol, quote['price'], -(shares), quote['name'], session['user_id'])
        # Add the amount of money resulting from the price of each share multilpied by the number of shares sold to the user's cash
        db.execute("UPDATE users SET cash = (cash + ?) WHERE id ==?", quotes_price, session['user_id'])
        db.execute("COMMIT")

        return redirect("/")

    # User reached route via GET
    else:
        return render_template("sell.html", quotes=quotes)


@app.route("/pass", methods=["GET", "POST"])
@login_required
def password():
    """Change password"""
    if request.method == 'POST':

        rows = db.execute("SELECT * FROM users WHERE id = ?", session['user_id'])

        if not check_password_hash(rows[0]["hash"], request.form.get('password')):
            return apology('Please enter current password')

        if not request.form.get('new_pass'):
            return apology('Please enter new password')

        if check_password_hash(rows[0]["hash"], request.form.get('new_pass')):
            return apology('Please enter a different password')

        if not request.form.get('confirmation'):
            return apology('Please confirm your password')

        if request.form.get('confirmation') != request.form.get('new_pass'):
            return apology('Please enter the same new password')

        pass_hash = generate_password_hash(request.form.get("new_pass"))
        db.execute('UPDATE users SET hash = ? WHERE id = ?', pass_hash, session['user_id'])

        return redirect('/')

    else:
        return render_template('pass.html')


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
