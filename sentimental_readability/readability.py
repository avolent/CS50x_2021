from cs50 import get_string
import re

# Grab text from user.
text = get_string("Text: ")

# Split and calculate words, sentences and letters
words = len(text.split(" "))
sentences = len(re.split("\. |\? |\! ", text))
letters = 0
for letter in text:
    if letter.isalpha() is True:
        letters += 1
        print(letter)

# print(words)
# print(sentences)
# print(letters)

# averages values
L = letters / words * 100
S = sentences / words * 100

# Calculate index
index = 0.0588 * L - 0.296 * S - 15.8

# print(L)
# print(S)
# print(index)

# Print grade
if index >= 16:
    print("Grade 16+")

elif index < 1:
    print("Before Grade 1")
    
else:
    grade = round(index)
    print(f"Grade {grade}")