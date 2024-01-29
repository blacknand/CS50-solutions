# TODO

while True:
    try:
        size = int(input("Size: "))
        if size <= 8 and size >= 1:
            break
    except ValueError:
        print("Not an int")

n = len(range(size))

for i in range(size):
    for j in range(size):
        if i + j < n - 1:
            print(" ", end='')
        else:
            print("#", end='')
    print()

