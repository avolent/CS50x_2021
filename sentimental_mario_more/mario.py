from cs50 import get_int

# While loop to confirm correct input.
blocks = 0
while blocks < 1 or blocks > 8:
    blocks = get_int("Height: ")

# Main loop for building the blocks
loop = 0
while loop < blocks:
    space = blocks - loop
    while space > 1:
        print(" ", end="")
        space = space - 1
    
    # While loop for printing front hash
    front_hash = 0
    while front_hash <= loop:
        print("#", end="")
        front_hash = front_hash + 1
    
    # While loop for printing middle gap
    gap = 0
    middle = 2
    while gap < middle:
        print(" ", end="")
        gap = gap + 1
    
    # While loop for printing end hash
    hash = 0
    while hash <= loop:
        print("#", end="")
        hash = hash + 1
    
    print("")
    loop = loop + 1
