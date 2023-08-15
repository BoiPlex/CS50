# c stands for char

import cs50


def main():
    text = cs50.get_string("Text: ")

    # Count these values
    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)

    # Coleman-Liau Index
    index = round(0.0588 * letters / words * 100 - 0.296 * sentences / words * 100 - 15.8)

    # Decide grade level
    if index < 1:
        print("Before Grade 1")
    elif index >= 16:
        print("Grade 16+")
    else:
        print(f"Grade {index}")


def count_letters(text):
    letters = 0

    for c in text:
        uni_c = ord(c.lower())
        # If char is letter
        if uni_c >= 97 and uni_c <= 122:
            letters += 1

    return letters


def count_words(text):
    words = 1

    for c in text:
        uni_c = ord(c)
        # If char is space
        if uni_c == 32:
            words += 1

    return words


def count_sentences(text):
    sentences = 0

    for c in text:
        uni_c = ord(c)
        # If char is period, !, or ?
        if uni_c == 46 or uni_c == 33 or uni_c == 63:
            sentences += 1

    return sentences


if __name__ == "__main__":
    main()