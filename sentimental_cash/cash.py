from cs50 import get_float

# Get change from user.
dollars = -1
while dollars < 0:
    dollars = get_float("Change owed: ")
cents = round(dollars * 100)

# print(cents)
coins = 0

# Loop through all coins until not able to, add one coin each time.
while cents >= 25:
    cents = cents - 25
    coins += 1

while cents >= 10:
    cents = cents - 10
    coins += 1
    
while cents >= 5:
    cents = cents - 5
    coins += 1
    
while cents >= 1:
    cents = cents - 1
    coins += 1
    
print(coins)