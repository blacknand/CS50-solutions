# TODO
import cs50


def main():
    text = cs50.get_string("Text: ")
    L = float(count_letters(text) / count_words(text)) * 100
    S = float(count_sentences(text) / count_words(text)) * 100

    index = round(0.0588 * L - 0.296 * S - 15.8)

    if index >= 16:
        print("Grade 16+")
    elif index < 1:
        print("Before Grade 1")
    else:
        print(f"Grade {index}")


def count_letters(text):
    count = 0
    for i in range(len(text)):
        if text[i].isalpha():
            count += 1
    return count


def count_words(text):
    count = 0
    for i in range(len(text)):
        if text[i] == " ":
            count += 1
    return count + 1


def count_sentences(text):
    count = 0
    for i in range(len(text)):
        if text[i] == "." or text[i] == "?" or text[i] == "!":
            count += 1
    return count


main()

