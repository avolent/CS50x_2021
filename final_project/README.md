# Sustenance: A meal and shopping list generator.
#### Video Demo: https://youtu.be/YgwbJfOun_M
#### Description:
A flask website that is connected to an SQLite database in which you can store and randomly generate all your meal plans in one place. Fill the database up with recipes and ingredients, then generate a random meal/shopping list based on the count you selected. The shopping list will have the total amount of ingredients required to make all meals. This making it easy to buy/order from the shops with little to no thinking required.

##### Recipe Page
Add, edit and delete the recipes in the database. Once added these recipes will be available to be randomly selected. On the recipe selection page you can see an optional link in the title, ingredient list and the amount of times the meal has been generated. Below the recipe you can add and delete ingredients. All the ingredients are sourced from the inventory which can be edited on the inventory page.

##### Inventory Page
Add, edit and delete items in the inventory. These items can then be used as ingredients in recipes on the recipe page. Unit of measurement is optional and not required when adding.

##### History Page
See your 5 last generations. Important incase you've forgotten your previous meal plans.

#### Technical Stuff

*Database:* Sustenance.db
Connection to database via SQLalchemy - http://flask-sqlalchemy.pocoo.org/

4 Tables

1. Recipe: Stores recipe information like ID, Name, Link and Generation Count
2. Inventory: Stores items in which can be used for recipes. Information used is ID, item (name) and unit (Unit of measurement).
3. Ingredients: Links the recipe and inventory databases together. Tracking what items and quantity are used in the recipes. Information used is ID, recipe id, item id and quantity.
4. History: Stores previously generated meal plans. Information like generation date, shopping list and meal list.

Some interesting design choices I came across/learnt. Using an "api" route in flask for adding and deleting things in the databases. This route being able to handle different types of request whether it being an ingredient, recipe or inventory item. How i managed to do this was by having a hidden element in each form with the type of request (recipe, ingredient, item). This was then checked at the start of the function and sorted out via if statements.


#### Future Improvements

- Block/timeout items from being generated too often.
- Improve ingredient and item edit pages.
- Have a database import and export button.
- Have a stats pages to see info on most generated and most used item etc.