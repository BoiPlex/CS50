#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void print_encryption(string text, string key);
bool non_alphabetic(string key);
bool repeated_letters(string key);

int main(int argc, string argv[])
{
    // Validation checks
    if (argc != 2)
    {
        printf("substitution only accepts 1 command-line argument\n");
        return 1;
    }

    string key = argv[1];
    // Changes key to lowercase
    for (int i = 0, len = strlen(key); i < len; i++)
    {
        key[i] = tolower(key[i]);
    }

    // Continue validation checks
    if (strlen(key) != 26)
    {
        printf("substitution only accepts a 26 character key\n");
        return 1;
    }
    if (non_alphabetic(key))
    {
        printf("substitution doesn't accept a key with non-alphabetic characters\n");
        return 1;
    }
    if (repeated_letters(key))
    {
        printf("substitution deosn't accept a key with repeated letters\n");
        return 1;
    }

    string plaintext = get_string("plaintext: ");

    print_encryption(plaintext, key);

    return 0;
}

// Takes string and key, prints encrpyted substitution text
void print_encryption(string text, string key)
{
    printf("ciphertext: ");

    for (int i = 0, len = strlen(text); i < len; i++)
    {
        int index = tolower(text[i]) - 97;
        char c = text[i];
        char en = key[index];

        // Lowercase char
        if (islower(c))
        {
            printf("%c", tolower(en));
        }
        // Uppercase char
        else if (isupper(c))
        {
            printf("%c", toupper(en));
        }
        // Non-alphabetic char
        else
        {
            printf("%c", c);
        }
    }

    printf("\n");
}

// Takes text, checks if key doesn't only alphabetic characters
bool non_alphabetic(string key)
{
    for (int i = 0, len = strlen(key); i < len; i++)
    {
        if (key[i] < 97 || key[i] > 122)
        {
            return true;
        }
    }
    return false;
}

// Takes key, checks if key has repeated letters
bool repeated_letters(string key)
{
    for (int i = 0, len = strlen(key); i < len; i++)
    {
        for (int j = i + 1; j < len; j++)
        {
            if (key[i] == key[j])
            {
                return true;
            }
        }
    }
    return false;
}