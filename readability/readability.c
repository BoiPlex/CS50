#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    string text = get_string("Text: ");
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    int index = round(0.0588 * (float)letters / (float)words * 100.0 - 0.296 * (float)sentences / (float)words * 100.0 - 15.8);

    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}

// Take string and return # of letters
int count_letters(string text)
{
    int letters = 0;

    for (int i = 0; i < strlen(text); i++)
    {
        // If char is letter
        char c = tolower(text[i]);
        if (c >= 97 && c <= 122)
        {
            ++letters;
        }
    }

    return letters;
}

// Take string and return # of words
int count_words(string text)
{
    int words = 1;

    for (int i = 0; i < strlen(text); i++)
    {
        // If char is space
        if (text[i] == 32)
        {
            ++words;
        }
    }

    return words;
}

// Take string and return # of sentences (# of .?!)
int count_sentences(string text)
{
    int sentences = 0;

    for (int i = 0; i < strlen(text); i++)
    {
        // If char is period, !, or ?
        if (text[i] == 46 || text[i] == 33 || text[i] == 63)
        {
            ++sentences;
        }
    }

    return sentences;
}