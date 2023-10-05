from cs50 import get_string

def main():
    text = get_string("Text: ")
    L = (count_letters(text) / count_words(text)) * 100;
    S = (count_sentences(text) / count_words(text)) * 100;

    result = round((0.0588 * L) - (0.296 * S) - 15.8)

    if result < 1:
        print("Before Grade 1")
        return

    if result > 16:
        print("Grade 16+")
        return

    print(f"Grade {result}")

def count_letters(text):
    count = 0
    for c in text:
        if c.isalpha():
            count += 1

    return count

def count_words(text):
    return len(text.split())

def count_sentences(text):
    sum = 0
    for c in text:
        if c in [".", "!", "?"]:
            sum += 1

    return sum

main()