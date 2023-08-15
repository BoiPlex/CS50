#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

typedef uint8_t BYTE;

// Number of bytes in a block
const int BLOCK_SIZE = 512;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("Could not open file\n");
        return 1;
    }

    BYTE buffer[BLOCK_SIZE];
    FILE *img;
    int jpeg_num = 0;
    while (fread(buffer, 1, BLOCK_SIZE, file) == BLOCK_SIZE)
    {
        // If JPEG header
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (jpeg_num != 0)
            {
                fclose(img);
            }
            char *file_name = malloc(8);
            sprintf(file_name, "%03i.jpg", jpeg_num);
            img = fopen(file_name, "w");
            fwrite(buffer, 1, BLOCK_SIZE, img);

            free(file_name);
            jpeg_num++;
        }
        else if (jpeg_num != 0)
        {
            fwrite(buffer, 1, BLOCK_SIZE, img);
        }
    }
    fclose(img); // Close final JPEG

}