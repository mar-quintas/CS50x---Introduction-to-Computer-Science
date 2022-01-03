from cs50 import get_int

height = 0

# Option 1
while height not in range(1, 9):
    height = get_int("Height: ")


for i in range(height):
    print(" " * (height - i - 1) + "#" * (i + 1) + "  " + "#" * (i + 1))

# Option 0
# for i in range(height):
#     print(" " * (height - i), end="")
#     print("#" * (i + 1), end="  ")
#     print("#" * (i + 1))

