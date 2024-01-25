import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

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

    stocks = db.execute("SELECT stock, SUM(shares) as shares FROM transactions WHERE user_id = ? GROUP BY stock", session["user_id"])
    user_cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
    portfolio = db.execute("SELECT stock, SUM(shares) as shares FROM portfolio WHERE user_id = ? GROUP BY stock", session["user_id"])

    stocks_info = []
    total = 0

    for stock in portfolio:
        api_call = round(lookup(stock["stock"])["price"], 2)
        stock_info = {
            "stocks_owned": stock["stock"],
            "shares_owned": stock["shares"],
            "company": api_call,
            "total_value": round(stock["shares"] * float(api_call), 2)
        }
        stocks_info.append(stock_info)
        total += round(api_call * stock["shares"], 2)

    cash_balance = round(user_cash[0]["cash"], 2)
    grand_total = round(user_cash[0]["cash"] + total, 2)

    return render_template("index.html", stocks_info=stocks_info, cash_balance=cash_balance, grand_total=grand_total)

@app.route("/add", methods=["GET", "POST"])
@login_required
def add():
    if request.method == "POST":
        cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        cash = cash[0]["cash"]
        cash += float(request.form.get("cash"))
        db.execute("UPDATE users SET cash = ? WHERE id = ?", cash, session["user_id"])
        return redirect(url_for('index'))

    return render_template("cash.html")

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        shares = request.form.get("shares")

        try:
            shares = int(shares)
        except ValueError:
            return apology("must provide int", 400)

        if not request.form.get("symbol"):
            return apology("must provide symbol", 400)

        elif request.form.get("symbol") == "":
            return apology("must provide stock", 400)

        elif not shares > 0:
            return apology("must provide positive shares", 400)

        elif lookup(request.form.get("symbol")) == None:
            return apology("invalid stock", 400)

        price = lookup(request.form.get("symbol"))["price"]
        symbol = lookup(request.form.get("symbol"))["symbol"]
        cash = db.execute("SELECT cash FROM users WHERE id = (?)", session["user_id"])
        cash = cash[0]["cash"]
        total_cost = float(price) * int(request.form.get("shares"))

        if total_cost > cash:
            return apology("not enough money", 403)

        cash -= total_cost
        current_datetime = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

        string = "purchase"

        db.execute("UPDATE users SET cash = ? WHERE id = ?", cash, session["user_id"])
        db.execute("INSERT INTO transactions (user_id, stock, price, datetime, shares, type) VALUES (?, ?, ?, ?, ?, ?)",
        session["user_id"], symbol, float(price), current_datetime, int(request.form.get("shares")), string)
        bought_stock = db.execute("SELECT stock, shares FROM portfolio WHERE user_id = ? AND stock = ?", session["user_id"], symbol)
        if bought_stock:
            current_shares = int(bought_stock[0]["shares"])
            update_shares = current_shares + int(request.form.get("shares"))
            db.execute("UPDATE portfolio SET shares = ? WHERE user_id = ? AND stock = ?", update_shares, session["user_id"], symbol)
        elif not bought_stock:
            db.execute("INSERT INTO portfolio (user_id, stock, shares) VALUES (?, ?, ?)", session["user_id"], symbol, int(request.form.get("shares")))

        return redirect(url_for("index"))

    return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    stocks = db.execute("SELECT * FROM transactions WHERE user_id = ?", session["user_id"])

    stock_history = []

    for stock in stocks:
        history = {
            "transaction_type": stock["type"],
            "stock": stock["stock"],
            "price": stock["price"],
            "datetime": stock["datetime"],
            "shares": stock["shares"]
        }
        stock_history.append(history)

    return render_template("history.html", stock_history=stock_history)


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
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
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
        symbol = request.form.get("symbol")

        if symbol == "":
            return apology("must provide stock", 400)

        stock = lookup(symbol)

        if stock == None:
            return apology("invalid stock", 400)

        price = round(stock["price"], 2)

        return render_template("quoted.html", result=stock, price=price)

    return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":

        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        if not request.form.get("username"):
            return apology("must provide username", 400)

        elif len(rows) != 0:
            return apology("username is already in use", 400)

        elif not request.form.get("password"):
            return apology("must provide password", 400)

        elif not request.form.get("confirmation"):
            return apology("must confirm password", 400)

        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords must match", 400)

        else:

            username = request.form.get("username")
            password = request.form.get("password")
            hashed_password = generate_password_hash(password)

            db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, hashed_password)

            return redirect(url_for("login"))

    return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    stock_parse = db.execute("SELECT stock FROM portfolio WHERE user_id = ? GROUP BY stock", session["user_id"])
    if request.method == "POST":
        shares = request.form.get("shares")

        try:
            shares = int(shares)
        except ValueError:
            return apology("must provide int", 400)

        stocks = db.execute("SELECT stock, shares FROM portfolio WHERE user_id = ? AND stock = ?", session["user_id"], request.form.get("symbol"))

        if request.form.get("symbol") == "":
            return apology("must provide stock", 400)

        elif lookup(request.form.get("symbol")) == None:
            return apology("invalid stock", 400)

        elif len(stocks) == 0:
            return apology("you do not own this stock", 400)

        elif not shares > 0:
            return apology("must provide positive shares", 400)

        elif not shares <= int(stocks[0]["shares"]):
            print(int(stocks[0]["shares"]))
            return apology("you do not own enough shares to sell", 400)

        transac_query = db.execute("SELECT * FROM transactions WHERE user_id = ?", session["user_id"])
        add = transac_query[0]

        new_shares = round(stocks[0]["shares"] - int(request.form.get("shares")), 2)
        new_money =  round(lookup(stocks[0]["stock"])["price"] * int(request.form.get("shares")), 2)
        current_cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]
        current_cash = round(current_cash["cash"], 2)
        added_cash = round(current_cash + new_money, 2)
        current_date = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        string = "sell"
        db.execute("UPDATE portfolio SET shares = ? WHERE stock = ? AND user_id = ?", new_shares, stock_parse[0]["stock"], session["user_id"])
        db.execute("INSERT INTO transactions (user_id, stock, price, datetime, shares, type) VAlUES (?, ?, ?, ?, ?, ?)", session["user_id"], add["stock"], add["price"], add["datetime"], shares, "sell")
        db.execute("UPDATE users SET cash = ? WHERE id = ?", added_cash, session["user_id"])

        return redirect(url_for("index"))

    return render_template("sell.html", stock_parse=stock_parse)
