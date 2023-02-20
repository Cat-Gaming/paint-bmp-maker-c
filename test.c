#include <stdlib.h>
#include <stdio.h>

// License:
/* Copyright (C) 2023  Tyler Wendorff

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
// Description:
// This Program has 2 programs in it main() and othermain()
// I created a bmp image writer which writes down bitmap data
// using a pointer.
// The othermain program does stuff like writing down pixel info
// to stdout and using user input i do stuff.
// idk i just wrote this for fun!
// 

typedef struct Pixel
{
    int isOn;
} Pixel;

static int SCR_WIDTH = 64;
static int SCR_HEIGHT = 16;

Pixel* pixels;

void PrintScreen()
{
    for (int y = 0; y < SCR_HEIGHT; y++)
    {
        for (int x = 0; x < SCR_WIDTH; x++)
        {
            if (pixels[x+y*SCR_WIDTH].isOn)
            {
                printf("#");
            } else
            {
                printf(".");
            }
        }
        printf("\n");
        
    }
    
}

void SaveScreen()
{
    FILE *file_ptr = fopen("file.txt", "w");
    if (!file_ptr) {
    perror("fopen");
    } else {
        for (int y = 0; y < SCR_HEIGHT; y++)
        {
            for (int x = 0; x < SCR_WIDTH; x++)
            {
                if (pixels[x+y*SCR_WIDTH].isOn)
                {
                    fputc('#', file_ptr);
                } else
                {
                    fputc('.', file_ptr);
                }
            }
            fputc('\n', file_ptr);
        }
    }
}

void ClearScreen()
{
    for (int i = 0; i < SCR_WIDTH * SCR_HEIGHT; i++)
    {
        pixels[i].isOn = 0;
    }
    
}

void SetAt(int x, int y, int isOn)
{
    if (isOn)
    {
        pixels[x+y*SCR_WIDTH].isOn = 1;
    } else
    {
        pixels[x+y*SCR_WIDTH].isOn = 0;
    }
}

int othermain()
{
    pixels = malloc(sizeof(Pixel) * (SCR_WIDTH * SCR_HEIGHT));
    for (int i = 0; i < SCR_WIDTH * SCR_HEIGHT; i++)
    {
        pixels[i].isOn = 0;
    }

    char c;
    int x = 5;
    int y = 5;
    int dx = 1;
    int dy = 0;
    SetAt(x, y, 1);
    while ((c = getchar()) != 'q')
    {
        if (c == 'd')
        {
            dx = 1;
        } else if (c == 'a')
        {
            dx = -1;
        }
        if (c == 's')
        {
            dy = 1;
        } else if (c == 'w')
        {
            dy = -1;
        } else if (c == '\n')
        {
            dx = 0;
            dy = 0;
        }
        if (c == 'c')
        {
            ClearScreen();
        }
        x += dx;
        y += dy;
        SetAt(x, y, 1);
        system("clear");
        PrintScreen();
    }
    SaveScreen();
    free(pixels);
    return 0;
}


void write_img(const char* filename, int data[], int dsize, int length, int width, int height)
{
    int size = dsize + 0x36 + 1;
    FILE *f;
    f = fopen(filename, "w");

    if (f == NULL)
    {
        perror("Error");
        exit(-1);
    }

    fputc(0x42, f); // B
    fputc(0x4D, f); // M
    fputc(size, f); // size in 4 bytes idk
    fputc(0x00, f);
    fputc(0x00, f);
    fputc(0x00, f);

    fputc(0x00, f); // Reserved Bytes set to 0
    fputc(0x00, f); // Reserved Bytes set to 0

    fputc(0x00, f); // Reserved Bytes set to 0
    fputc(0x00, f); // Reserved Bytes set to 0

    // starting address of pixel array
    fputc(0x36, f);
    fputc(0x00, f);
    fputc(0x00, f);
    fputc(0x00, f); // end of header

    // DIB
    fputc(0x28, f); // sizeof header (40) bytes
    fputc(0x00, f);
    fputc(0x00, f);
    fputc(0x00, f);
    fputc(width, f); // width in pixels
    fputc(0x00, f);
    fputc(0x00, f);
    fputc(0x00, f);
    fputc(height, f); // height in pixels
    fputc(0x00, f);
    fputc(0x00, f);
    fputc(0x00, f);
    fputc(0x01, f); // color planes
    fputc(0x00, f);
    // bits per pixel
    fputc(32, f);
    fputc(0x00, f);

    fputc(0x00, f); // compression method
    fputc(0x00, f);
    fputc(0x00, f);
    fputc(0x00, f);
    fputc(0x00, f); // dummy 0 can be given for BI_RGB bitmaps
    fputc(0x00, f);
    fputc(0x00, f);
    fputc(0x00, f);
    // horizontal resolution
    fputc(width, f);
    fputc(0x00, f);
    fputc(0x00, f);
    fputc(0x00, f);
    // vertical resolution
    fputc(height, f);
    fputc(0x00, f);
    fputc(0x00, f);
    fputc(0x00, f);

    fputc(0xFF, f); // number of colors
    fputc(0x00, f);
    fputc(0x00, f);
    fputc(0x00, f);

    // 'Important Colors' ignored usually so we're just gonna set this to zero
    fputc(0x00, f);
    fputc(0x00, f);
    fputc(0x00, f);
    fputc(0x00, f);

    // End Of DIB HEADER
    
    // Data Header
    for (int i = 0; i < length * 3; i+=3)
    {
        fputc(data[i], f);
        fputc(data[i+1], f);
        fputc(data[i+2], f);
        fputc(0x00, f);
    }

    // End of Data Header
    fputc(0x00, f);
}

int width = 16;
int height = 16;
int* data;

void set_at(int x, int y, int on)
{
    if (on)
    {
        data[((x+y*width)*3)+0] = 255;
        data[((x+y*width)*3)+1] = 255;
        data[((x+y*width)*3)+2] = 255;
    } else
    {
        data[((x+y*width)*3)+0] = 0;
        data[((x+y*width)*3)+1] = 0;
        data[((x+y*width)*3)+2] = 0;
    }
}

int main()
{
    int pixCount = width * height;
    data = malloc(sizeof(int) * (pixCount * 3));
    for (int i = 0; i < (pixCount * 3); i++)
    {
        data[i] = 255;
    }

    set_at(2, 14, 0);
    set_at(2, 13, 0);
    set_at(2, 12, 0);
    set_at(2, 11, 0);
    set_at(2, 10, 0);
    set_at(2, 9, 0);
    set_at(2, 8, 0);

    set_at(13, 14, 0);
    set_at(13, 13, 0);
    set_at(13, 12, 0);
    set_at(13, 11, 0);
    set_at(13, 10, 0);
    set_at(13, 9, 0);
    set_at(13, 8, 0);

    set_at(1, 4, 0);
    set_at(2, 3, 0);
    set_at(13, 3, 0);
    set_at(14, 4, 0);

    set_at(3, 2, 0);
    set_at(4, 2, 0);
    set_at(5, 2, 0);
    set_at(6, 2, 0);
    set_at(7, 2, 0);
    set_at(8, 2, 0);
    set_at(9, 2, 0);
    set_at(10, 2, 0);
    set_at(11, 2, 0);
    set_at(12, 2, 0);
    
    write_img("img.bmp", data, sizeof(data), pixCount, width, height);

    free(data);
    return 0;
}