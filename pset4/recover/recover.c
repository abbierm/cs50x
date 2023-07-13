#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    // Type to store a BYTE
    typedef uint8_t BYTE;

    // Input validation to make sure correct number of command line arguments
    if (argc != 2)
    {
        printf("Invalid command line argument. Expected only 1 argument and received %i\n", argc - 1);
        return 1;
    }

    // Open Memory Card
    // Needs to be in CWD
    FILE *file = fopen(argv[1], "r");

    // Input Validation to make sure argv[1] is an openable image in the CWD
    if (file == NULL)
    {
        printf("Could not open file!\n");
        return 1;
    }

    // Variables to help control program
    int block = 512;
    int counter = 0;
    char *filename = malloc(8);

    // Flag that helps keep track of the programs loop
    int writing = 0;

    // Points to the location of the file we want to store the images
    FILE *output = NULL;

    // Buffer variable to store the blocks of data while checking to see if it meets the criteria
    BYTE buffer[512];

    while (fread(&buffer, sizeof(BYTE), block, file) == 512)
    {
        // Conditional to check if the program has an open file it is writing to
        if (writing == 1)
        {
            // Checks firt 4 bytes for indication of a new image
            if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3]  & 0xf0) == 0xe0)
            {
                // Closes the previous file
                fclose(output);

                // Create a new filename and write to it
                sprintf(filename, "%03i.jpg", counter);
                output = fopen(filename, "w");
                fwrite(&buffer, sizeof(BYTE), block, output);

                //update the counter
                counter++;
            }

            // Else keep writing to the current file
            else
            {
                fwrite(&buffer, sizeof(BYTE), block, output);
            }
        }
        // Haven't found any jpg yet
        else if (writing == 0)
        {
            // Checks first 4 BYTES for an image
            if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3]  & 0xf0) == 0xe0)
            {
                // Create a new filename and write to it
                sprintf(filename, "%03i.jpg", counter);
                output = fopen(filename, "w");
                fwrite(&buffer, sizeof(BYTE), block, output);
                writing = 1;
                counter++;
            }

        }
    }
    // Close the file we are reading
    fclose(file);
    fclose(output);
    free(filename);

}
