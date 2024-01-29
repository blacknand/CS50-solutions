# TODO
import cs50


def main():
    while True:
        try:
            get_cents = cs50.get_float("Changed owed: ")
            if get_cents > 0:
                break
        except ValueError:
            print("Enter non-negative value")

    # Multiply the input to make it easier to divide by 25, 10, 5 and 1
    cents = round(get_cents * 100)

    count = 0

    while cents >= 25:
        cents -= 25
        count += 1
    while cents >= 10:
        cents -= 10
        count += 1
    while cents >= 5:
        cents -= 5
        count += 1
    while cents >= 1:
        cents -= 1
        count += 1

    print(count)


main()

