// Recovers lost .jpg on .raw files into current directory
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    char *infile = argv[1];

    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Cound not open %s. \n", infile);
        return 1;
    }

    uint8_t buffer[512];
    // Counter for specific files
    int current = 0;
    // Flag helps control closing the open files once
    // the program finds the header of a new .jpg
    int flag = 0;
    FILE *outptr;

    // Create first file
    char *filename = malloc(8);
    if (filename == NULL)
    {
        printf("Not enough memory to store filename\n");
        fclose(inptr);
        return 1;
    }

    while (fread(&buffer, sizeof(buffer), 1, inptr) == 1)
    {
        // Looks for new .jpg header
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            if (flag == 1)
            {
                fclose(outptr);
            }
            sprintf(filename, "%03d.jpg", current);
            outptr = fopen(filename, "a");
            if (outptr == NULL)
            {
                printf("Could not write to %s\n", filename);
                fclose(inptr);
                return 1;
            }
            // Write to file
            fwrite(&buffer, sizeof(buffer), 1, outptr);
            current += 1;
            flag = 1;
        }

        // Program is still writing to the same file
        else if (flag == 1)
        {
            fwrite(&buffer, sizeof(buffer), 1, outptr);
        }
    }

    if (flag == 1)
    {
        fclose(outptr);
    }
    fclose(inptr);
    free(filename);
}
