#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE rgb;
            BYTE avg_c = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);

            rgb.rgbtRed = avg_c;
            rgb.rgbtGreen = avg_c;
            rgb.rgbtBlue = avg_c;

            image[i][j] = rgb;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int mid = width / 2;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < mid; j++)
        {
            // Swap rgb with opposite pixel
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE blurred[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE rgb;
            float avg_r = 0;
            float avg_g = 0;
            float avg_b = 0;
            float pixel_count = 0.0; // Value to divide by for each color
            // 3x3 grid around pixel (r = pixel row, c = pixel col)
            for (int r = -1; r <= 1; r++)
            {
                // In bounds check (row)
                if (i + r < 0 || i + r >= height)
                {
                    continue;
                }
                for (int c = -1; c <= 1; c++)
                {
                    // In bounds check (col)
                    if (j + c < 0 || j + c >= width)
                    {
                        continue;
                    }
                    avg_r += image[i + r][j + c].rgbtRed;
                    avg_g += image[i + r][j + c].rgbtGreen;
                    avg_b += image[i + r][j + c].rgbtBlue;
                    pixel_count++;
                }
            }
            rgb.rgbtRed = round(avg_r / pixel_count);
            rgb.rgbtGreen = round(avg_g / pixel_count);
            rgb.rgbtBlue = round(avg_b / pixel_count);

            blurred[i][j] = rgb;
        }
    }
    // Copy blurred to image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = blurred[i][j];
        }
    }
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE edged[height][width];

    // x and y kernels
    int gx_grid[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gy_grid[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE rgb;
            float final_r = 0;
            float final_g = 0;
            float final_b = 0;
            // Gx and Gy for rgb
            float gx_r = 0;
            float gx_g = 0;
            float gx_b = 0;
            float gy_r = 0;
            float gy_g = 0;
            float gy_b = 0;

            // 3x3 grid around pixel (r = pixel row, c = pixel col)
            for (int r = -1; r <= 1; r++)
            {
                // In bounds check (row)
                if (i + r < 0 || i + r >= height)
                {
                    continue;
                }
                for (int c = -1; c <= 1; c++)
                {
                    // In bounds check (col)
                    if (j + c < 0 || j + c >= width)
                    {
                        continue;
                    }
                    float gx_num = gx_grid[1 + r][1 + c];
                    float gy_num = gy_grid[1 + r][1 + c];
                    // Gx grid sums
                    gx_r += image[i + r][j + c].rgbtRed * gx_num;
                    gx_g += image[i + r][j + c].rgbtGreen * gx_num;
                    gx_b += image[i + r][j + c].rgbtBlue * gx_num;
                    // Gy grid sums
                    gy_r += image[i + r][j + c].rgbtRed * gy_num;
                    gy_g += image[i + r][j + c].rgbtGreen * gy_num;
                    gy_b += image[i + r][j + c].rgbtBlue * gy_num;
                }
            }
            // Combine Gx and Gy for rgb
            final_r = round(sqrt(pow(gx_r, 2) + pow(gy_r, 2)));
            final_g = round(sqrt(pow(gx_g, 2) + pow(gy_g, 2)));
            final_b = round(sqrt(pow(gx_b, 2) + pow(gy_b, 2)));

            // Cap rgb at 255
            if (final_r > 255)
            {
                final_r = 255;
            }
            if (final_g > 255)
            {
                final_g = 255;
            }
            if (final_b > 255)
            {
                final_b = 255;
            }
            rgb.rgbtRed = final_r;
            rgb.rgbtGreen = final_g;
            rgb.rgbtBlue = final_b;

            edged[i][j] = rgb;
        }
    }
    // Copy edged to image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = edged[i][j];
        }
    }
}
