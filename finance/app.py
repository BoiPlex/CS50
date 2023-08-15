import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

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
    user_id = session["user_id"]
    # stocks = db.execute("SELECT * FROM orders WHERE type='buy' AND user_id=? GROUP BY id HAVING SUM(shares) > 0", user_id)
    cash = db.execute("SELECT cash FROM users WHERE id=?", user_id)[0]["cash"]
    total = cash
    cash = usd(cash)
    # found_stocks = {}
    stocks = []

    """
    for stock in stocks:
        id = stock["id"]
        user_id = stock["user_id"]
        type = stock["type"]
        name = stock["name"]
        symbol = stock["symbol"]
        shares = stock["shares"]
        price = stock["price"]
        if symbol in found_stocks.keys():
            found_stocks[symbol]["shares"] += shares
        else:
            found_stocks[symbol] = stock
    """
    symbols = [d["symbol"] for d in db.execute("SELECT DISTINCT symbol FROM orders WHERE type='buy' AND user_id=?", user_id)]
    for symbol in symbols:
        bought_shares = sum([d["shares"] for d in db.execute("SELECT shares FROM orders WHERE type='buy' AND user_id=? AND symbol=?", user_id, symbol)])
        sold_shares = sum([d["shares"] for d in db.execute("SELECT shares FROM orders WHERE type='sell' AND user_id=? AND symbol=?", user_id, symbol)])
        current_shares = bought_shares - sold_shares
        price = lookup(symbol)["price"]
        total += current_shares * price
        # finalize stock
        if current_shares != 0:
            stocks.append({"symbol": symbol, "name": symbol, "shares": current_shares, "price": usd(price), "total": usd(current_shares * price)})
    total = usd(total)
    return render_template("index.html", stocks=stocks, cash=cash, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        input_symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        stock = lookup(input_symbol)
        # check validity
        if not input_symbol or not stock:
            return apology("symbol is blank or doesn't exist")
        if not shares.isdigit():
            return apology("shares is negative or not an integer")
        shares = int(shares)
        if shares == 0:
            return apology("shares is 0")

        # buy shares
        user_id = session["user_id"]
        username = db.execute("SELECT username FROM users WHERE id=?", user_id)[0]["username"]
        stock_price = stock["price"]
        cash = db.execute("SELECT cash FROM users WHERE id=?", user_id)[0]["cash"]
        total = stock_price * shares
        if cash < total:
            return apology(f"you are {usd(total - cash)} short")
        db.execute("UPDATE users SET cash=? WHERE id=?", cash - total, user_id)
        db.execute("INSERT INTO orders (user_id, type, name, symbol, shares, price) VALUES (?, 'buy', ?, ?, ?, ?);", user_id, stock["name"], stock["symbol"], shares, stock_price)
        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session["user_id"]
    stocks = db.execute("SELECT * FROM orders WHERE user_id=? ORDER BY id ASC", user_id)
    for stock in stocks:
        stock["total"] = usd(stock["shares"] * stock["price"])
        stock["price"] = usd(stock["price"])
    return render_template("history.html", stocks=stocks)


@app.route("/account", methods=["GET", "POST"])
@login_required
def account():
    """Account settings"""
    user_id = session["user_id"]
    username = db.execute("SELECT username FROM users WHERE id=?", user_id)[0]["username"]
    if request.method == "POST":
        change = request.form.get("change")
        # change username
        if change == "username":
            new_username = request.form.get("username")
            if not new_username or db.execute("SELECT username FROM users WHERE username=?", new_username):
                return apology("username is blank or already exists")
            db.execute("UPDATE users SET username=? WHERE id=?", new_username, user_id)
            return render_template("account.html", username=new_username)
        # change password
        elif change == "password":
            old_password = request.form.get("old_password")
            new_password = request.form.get("new_password")
            confirmation = request.form.get("confirmation")
            old_password_hash = db.execute("SELECT hash FROM users WHERE id = ?", user_id)[0]["hash"]
            if not old_password or not new_password or not confirmation or not check_password_hash(old_password_hash, old_password) or new_password != confirmation:
                return apology("a password is blank or passwords don't match")
            hash = generate_password_hash(new_password)
            db.execute("UPDATE users SET hash=? WHERE id=?", hash, user_id)
            return render_template("account.html", username=username)
        elif change == "delete":
            username = request.form.get("username")
            password = request.form.get("password")
            user = db.execute("SELECT username, hash FROM users WHERE id=?", user_id)[0]
            if not username or not password or username != user["username"] or not check_password_hash(user["hash"], password):
                return apology("username/password is blank or username/password don't match")
            session.clear() # logout
            db.execute("DELETE FROM users WHERE id=?", user_id)
            db.execute("DELETE FROM orders WHERE user_id=?", user_id)
            return redirect("/")
        else:
            return render_template("account.html", username=username)
    else:
        return render_template("account.html", username=username)


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
        input_symbol = request.form.get("symbol")
        quote = lookup(input_symbol)
        if not quote:
            return apology("invalid stock symbol")
        name = quote["name"]
        price = usd(quote["price"])
        symbol = quote["symbol"]
        return render_template("quoted.html", name=name, price=price, symbol=symbol)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # check validity
        if not username or db.execute("SELECT username FROM users WHERE username=?", username):
            return apology("username is blank or already exists")
        if not password or not confirmation or password != confirmation:
            return apology("a password is blank or passwords don't match")

        hash = generate_password_hash(password)
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?);", username, hash)
        return redirect("/login")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    user_id = session["user_id"]
    if request.method == "POST":
        symbol = request.form.get("symbol")
        stock = db.execute("SELECT * FROM orders WHERE type='buy' AND user_id=? AND symbol=? GROUP BY id HAVING SUM(shares) > 0", user_id, symbol)[0]
        shares = int(request.form.get("shares"))
        # check validity
        symbols = [d["symbol"] for d in db.execute("SELECT DISTINCT symbol FROM orders WHERE type='buy' AND user_id=?", user_id)]
        if not symbol or symbol not in symbols:
            return apology("stock symbol not selected or isn't in your portfolio")
        bought_shares = sum([d["shares"] for d in db.execute("SELECT shares FROM orders WHERE type='buy' AND user_id=? AND symbol=?", user_id, symbol)])
        sold_shares = sum([d["shares"] for d in db.execute("SELECT shares FROM orders WHERE type='sell' AND user_id=? AND symbol=?", user_id, symbol)])
        current_shares = bought_shares - sold_shares
        if shares < 1 or current_shares < shares:
            return apology(f"shares to sell is less than 1 or you don't own enough shares of {symbol} to sell")
        # sell shares
        username = db.execute("SELECT username FROM users WHERE id=?", user_id)[0]["username"]
        stock_price = lookup(symbol)["price"]
        cash = db.execute("SELECT cash FROM users WHERE id=?", user_id)[0]["cash"]
        total = stock_price * shares
        db.execute("UPDATE users SET cash=? WHERE id=?", cash + total, user_id)
        db.execute("INSERT INTO orders (user_id, type, name, symbol, shares, price) VALUES (?, 'sell', ?, ?, ?, ?);", user_id, stock["name"], stock["symbol"], shares, stock_price)
        return redirect("/")
    else:
        stocks = db.execute("SELECT DISTINCT symbol FROM orders WHERE type='buy' AND user_id=? GROUP BY id HAVING SUM(shares) > 0", user_id)
        return render_template("sell.html", stocks=stocks)
