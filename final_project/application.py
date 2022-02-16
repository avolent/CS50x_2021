import os
import random
from flask import Flask, flash, redirect, render_template, request
from flask_sqlalchemy import SQLAlchemy
from datetime import datetime

# Flask and Database Configuration
app = Flask(__name__)
app.config["SQLALCHEMY_DATABASE_URI"] = "sqlite:///sustenance.db"
db = SQLAlchemy(app)
# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True
app.secret_key = os.urandom(24)

# SQL-Alchemy database table setup.
class Recipe(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.Text, unique=True, nullable=False)
    link = db.Column(db.Text)
    generatedCount = db.Column(db.Integer)
    ingredients = db.relationship("Ingredient", backref="recipe", cascade="all, delete", lazy=True)

    def __repr__(self):
        return "<Recipe %r>" % self.name

class Ingredient(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    quantity = db.Column(db.Integer, nullable=False)
    recipe_id = db.Column(db.Integer, db.ForeignKey("recipe.id"), nullable=False)
    item_id = db.Column(db.Integer, db.ForeignKey("inventory.id"), nullable=False)

    def __repr__(self):
        return "<Ingredient %r>" % self.ingredient

class Inventory(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    item = db.Column(db.Text, unique=True, nullable=False)
    unit = db.Column(db.Text)
    ingredient_id = db.relationship("Ingredient", backref="ingredient", lazy=True)

    def __repr__(self):
        return "<Inventory %r>" % self.item

class History(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    generatedOn = db.Column(db.DateTime, default=datetime.utcnow)
    mealList = db.Column(db.Text, nullable=False)
    ingredientList = db.Column(db.Text, nullable=False)

    def __repr__(self):
        return "<History %r>" % self.generatedOn

db.create_all()

# Homepage (Index) function/route
@app.route("/", methods=["GET", "POST"])
def index():
    # Default return index page on get request. Has generation form on this page.
    # If user submitted a generation of meals via post return meal and shopping list.
    if request.method == "POST":
        # Initial variables. Grab meal count from form, total recipes.
        meal_count = int(request.form.get("meal_count"))
        recipes = Recipe.query.order_by(Recipe.id).all()

        # If meal count less then total recipes in database return to / with error notification.
        if meal_count > len(recipes):
            flash("Failed, meal count greater then total available recipes!", 'alert-danger')
            return render_template("index.html")
        # Randomly select meals from the recipe list based on the meal count from user.
        recipe_selection = random.sample(recipes, meal_count)
        # Setup variables and start looping through selected recipes. Updating generated count in the database.
        shopping_list = {}
        mealList = []
        for recipe in recipe_selection:
            if not recipe.generatedCount:
                recipe.generatedCount = 1
            else:
                recipe.generatedCount += 1
            mealList.append(recipe.name)
            db.session.add(recipe)
            db.session.commit()
            # Grab all ingredients from each recipe and add to the shopping list. If ingredient already there, add to existing quantity.
            for ingredient in recipe.ingredients:
                item = {}
                if ingredient.ingredient.item in shopping_list:
                    shopping_list[ingredient.ingredient.item]["quantity"] += ingredient.quantity
                else:
                    item["quantity"] = ingredient.quantity
                    item["unit"] = ingredient.ingredient.unit
                    shopping_list[ingredient.ingredient.item] = item
        # Save the generated meal list and shopping list to the history table in the database.
        history = History(mealList=str(mealList), ingredientList=str(shopping_list))
        db.session.add(history)
        db.session.commit()
        # Notify user of success and return the meal plan template showing the output of the generation.
        flash("Meal plan and shopping list generated!", "alert-primary")
        return render_template("meal_plan.html", recipe_selection=recipe_selection, shopping_list=shopping_list)
    else:
        return render_template("index.html")

# Recipe page function/route
@app.route("/recipes", methods=["GET", "POST"])
def recipes():
    # Pull current recipes from the database
    recipes = Recipe.query.order_by(Recipe.name).all()
    # Default GET request returns the base recipe page, otherwise if a POST request return template based on form selection.
    # Javascript function auto submits on change of selection, sending a POST request to the /recipes page.
    if request.method == "POST":
        # Grab the selection within the form.
        selection = request.form.get("recipe_selection")
        # If there is no selection, alert user of failure and redirect back to /recipes. Possible when just clicking enter to submit.
        if not selection:
            flash("Failed, please ensure you select something!", "alert-danger")
            return redirect("/recipes")
        # If selection is add, render a template with a form requesting recipe details.
        if selection.lower() == "add":
            return render_template("add_recipe.html", recipes=recipes)
        # If selection is not add, render a edit recipe page for the selection.
        if selection.lower() != "add":
            #Pull selected recipe from database.
            recipe = Recipe.query.filter_by(name=selection).first()
            #Check if users selection is legit.
            if not recipe:
                flash("Failed, please ensure recipe exists!", "alert-danger")
                return redirect("/recipes")
            return render_template("edit.html", recipes=recipes, recipe=recipe)
    # If request is GET render recipe page.
    else:
        return render_template("recipes.html", recipes=recipes)

# Inventory page function/route
@app.route("/inventory", methods=["GET", "POST"])
def inventory():
    # Grab a list of items in the inventory no matter the request type.
    items = Inventory.query.order_by(Inventory.item).all()
    # Default GET request returns the base inventory page, otherwise if a POST request return template based on form selection.
    # Javascript function auto submits on change of selection, sending a POST request to the /inventory page.
    if request.method == "POST":
        # Grab the selection within the form.
        selection = request.form.get("item_selection")
        # Grab form inputs and verify if all data is entered correctly, If not report failed and redirect to page again.
        if not selection:
            flash("Failed, please ensure you select something!", "alert-danger")
            return redirect("/inventory")
        # If selection is add, render template with a form to add items.
        if selection.lower() == "add":
            return render_template("add_item.html", items=items)
        #If option is not add, render the delete item tempalte.
        if selection.lower() != "add":
            return render_template("delete_item.html", items=items, selection=selection)
    # Render inventory page.
    else:
        return render_template("inventory.html", items=items)

# Delete API function. Supports recipe, ingredient and inventory delete requests. Doesnt support GET request.
@app.route("/delete", methods=["POST"])
def delete():
    # Determine what type of delete request user is submitting. A hidden element in the form which depends on the form it is sent from (recipe, ingredient or item).
    type = request.form.get("type")
    print(f"Type: {type}")
    # Type = Recipe
    if type == "recipe":
        # Grab user selection from form and delete it from the database.
        selection = request.form.get("recipe_selection")
        recipe = Recipe.query.filter_by(name=selection).first()
        # Check user selection is legit, if not report failure and return to /recipe.
        if not recipe:
            flash(f"Failed, recipe {selection} doesnt exist!", "alert-danger")
            return redirect("/recipes")
        # Delete the recipe and ingredients attached.
        db.session.delete(recipe)
        db.session.commit()
        # Flash success and return to recipe page.
        flash('Success, ' + selection + ' deleted!', 'alert-primary')
        return redirect("/recipes")
    # Type = Ingredient
    if type == "ingredient":
        # Grab list of recipes for future render.
        recipes = Recipe.query.order_by(Recipe.name).all()
        # Grab recipe name and ingredient selection. Pull these from the database.
        recipe_name = request.form.get("recipe")
        recipe = Recipe.query.filter_by(name=recipe_name).first()
        selection = request.form.get("ingredient_selection")
        item = Inventory.query.filter_by(item=selection).first()
        # Check users selection is legit, if not report failure and return the recipe's edit page.
        if not item:
            flash(f"Failed, ingredient {selection} doesnt exist!", "alert-danger")
            return render_template("edit.html", recipes=recipes, recipe=recipe)
        # Link item and recipe together with the ingredient table, noting quantity. Commit to database.
        ingredient = Ingredient.query.filter_by(item_id=item.id).first()
        db.session.delete(ingredient)
        db.session.commit()
        # Alert user of success and return to recipe edit page.
        flash('Success, ' + selection + ' deleted!', 'alert-primary')
        return render_template("edit.html", recipes=recipes, recipe=recipe)
    # Type = Item
    if type == "item":
        # Grab user's item selection and pull from database
        selection = request.form.get("item_selection")
        item = Inventory.query.filter_by(item=selection).first()
        if not item:
            flash(f"Failed, item {selection} doesnt exist!", "alert-danger")
            return redirect("/inventory")
        # Grab all ingredients that reference this item and delete.
        ingredients = Ingredient.query.filter_by(item_id=item.id).all()
        for ingredient in ingredients:
            db.session.delete(ingredient)
        # Delete the item and commit to database
        db.session.delete(item)
        db.session.commit()
        # Alert user of success and redirect to /inventory.
        flash('Success, ' + selection + ' deleted!', 'alert-primary')
        return redirect("/inventory")

# Recipe edit api, makes the edit section under the recipe dynamic.
@app.route("/edit", methods=["POST"])
def edit():
    # Grab the user's recipe selection, all recipes and ingredient selection (Add/Ingredient)
    selection = request.form.get("recipe")
    recipe = Recipe.query.filter_by(name=selection).first()
    recipes = Recipe.query.order_by(Recipe.name).all()
    ingredient_selection = request.form.get("ingredient_selection")
    # If there is nothing submitted in the ingredient selection, alert user and return the recipe page again.
    if not ingredient_selection:
        flash('Failed, please ensure you select something!', 'alert-danger')
        return render_template("edit.html", recipes=recipes, recipe=recipe)
    # If the ingredient selection is add, render the add ingredient template.
    if ingredient_selection.lower() == "add":
        items = Inventory.query.order_by(Inventory.item).all()
        return render_template("add_ingredient.html", recipes=recipes, recipe=recipe, items=items)
    # If the ingredient selection is not add, render the delete ingredient template.
    else:
        return render_template("delete_ingredient.html", recipes=recipes, recipe=recipe, ingredient_selection=ingredient_selection)

# API for all add request
@app.route("/add", methods=["POST"])
def add():
    # Determine what type of add request the user is submitting. A hidden element in the form depicts the type based on the form it is sent from (recipe, ingredient or item).
    type = request.form.get("type")
    print(f"Type: {type}")
    recipes = Recipe.query.order_by(Recipe.name).all()
    # Type = Recipe
    if type == "recipe":
        # Grab form data from user.
        name = request.form.get("name").strip()
        link = request.form.get("link").strip()
        # Check if recipe already exists, return recipe page if it does
        if Recipe.query.filter_by(name=name).first():
            flash(f'Failed, recipe {name} already exists!', 'alert-danger')
            return redirect("/recipes")
        # Check if recipe name is valid, return recipe if not valid
        if not name:
            flash('Failed, recipe name required!', 'alert-danger')
            return redirect("/recipes")
        recipe = Recipe(name=name, link=link)
        db.session.add(recipe)
        db.session.commit()
        flash('Success, ' + name + ' added!', 'alert-primary')
        return redirect("/recipes")
    # Type = Ingredient
    if type == "ingredient":
        # Grab user input from form.
        recipe_name = request.form.get("recipe")
        recipe = Recipe.query.filter_by(name=recipe_name).first()
        ingredient_selection = request.form.get("item_selection").strip().split()
        # If ingredient has unit details after split the text and only grab the ingredient.
        if len(ingredient_selection) > 1:
            ingredient_selection.pop(len(ingredient_selection) - 1)
            ingredient_selection = " ".join(map(str,ingredient_selection))
        else:
            ingredient_selection = ingredient_selection[0]
        # Pull the items ID from inventory database and grab the quantity the user selected.
        item_id = Inventory.query.filter_by(item=ingredient_selection).first()
        quantity = int(request.form.get("quantity").strip())
        # Confirm item exists in inventory and notify user if not. Also ensure a quantity above 0 is selected.
        if not item_id:
            flash(f'Failed, item does not exist in inventory!', 'alert-danger')
            return render_template("edit.html", recipes=recipes, recipe=recipe)
        if not quantity or quantity <= 0:
            flash(f'Failed, quantity required!', 'alert-danger')
            return render_template("edit.html", recipes=recipes, recipe=recipe)
        # If all is well add the item to the ingredient list linking the selected recipe and item id together with the amount required.
        ingredient = Ingredient(item_id=item_id.id, quantity=quantity, recipe_id=recipe.id)
        db.session.add(ingredient)
        db.session.commit()
        # Notify user of success and rerender the edit page for the recipe.
        flash('Success, ' + ingredient_selection + ' added!', 'alert-primary')
        return render_template("edit.html", recipes=recipes, recipe=recipe)
    # Type = Ingredient
    if type == "item":
        # Pull all current items.
        items = Inventory.query.order_by(Inventory.item).all()
        # Grab user input from form.
        item = request.form.get("item")
        unit = request.form.get("unit")
        # Check if item already exist in database, notify user if it does
        item_db = Inventory.query.filter_by(item=item).first()
        if item_db:
            flash(f'Failed, {item} already exist!', 'alert-danger')
            return render_template("add_item.html", items=items)
        # If form does not have an item, notify user and return the to inventory page. Form should prevent this but if anyone tries manually it will prevent them.
        if not item:
            flash('Failed, item is empty!', 'alert-danger')
            return render_template("add_item.html", items=items)
        # If all is well add item and unit (Optional) to the inventory database
        item_db = Inventory(item=item, unit=unit)
        db.session.add(item_db)
        db.session.commit()
        # Notify user of success and redirect to inventory page.
        flash('Success, ' + item + ' added!', 'alert-primary')
        return redirect("/inventory")

# History route, Last minute and could be definitely improved.
@app.route("/history")
def history():
    # Pull the last 5 generations from the history table in the database.
    history_db = History.query.order_by(History.generatedOn.desc()).limit(5)
    # Create a list and then loop through each history_db entry, pulling the necessary information. Chucking information into a dict and appending to array
    history = []
    for item in history_db:
        y = {}
        y["generatedOn"] = str(item.generatedOn)[:-7]
        y["mealList"] = item.mealList.replace("'", "").replace("[", "").replace("]", "").split(", ")
        y["ingredientList"] = item.ingredientList.replace("': {'quantity': ", ": ").replace(", 'unit': '", " ").replace("'}", "").replace("'", "").replace("{", "").replace("}", "").split(", ")
        history.append(y)
    # Render the History page
    return render_template("history.html", history=history)