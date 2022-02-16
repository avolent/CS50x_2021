// Dynamically update html page with number of meals based on sliders position on index page.
function updateTextInput(val) {
  document.getElementById("meal-count").innerHTML = val + " Meals";
}

// Next 3 functions do the same thing for each form. They all submit on change making the website seem more fluid and dynamic.
function recipeList(val) {
  document.getElementById("recipe_form").submit();
}

function itemList(val) {
  document.getElementById("item_form").submit();
}

function ingredientList(val) {
  document.getElementById("ingredient_form").submit();
}
