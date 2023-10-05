from cs50 import get_int

while True:
    height = get_int("Height: ")
    if height > 0 and height < 9:
        break

for i in range(height):
    # print first part
    print(" " * (height - (i + 1)), end='')
    print("#" * (i + 1) + "  ", end='')
    # print second part
    print("#" * (i + 1))