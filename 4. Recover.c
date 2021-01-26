#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


int main(int argc, char *argv[])
{
    // If program is not executed with exactly one command line argument - remind user
    if (argc != 2)
    {
        printf("Usage: ./recover file\n");
        return 1;
    }

    // Open memory card
    FILE *memory_card = fopen(argv[1], "r");

    // If file cannot be opened for reading - inform user
    if (memory_card == NULL)
    {
        printf("Could not open %s\n", argv[1]);
        return 2;
    }

    // Define size of blocks to read
    int block = (sizeof(uint8_t) * 512);

    // Define space to store read data
    uint8_t data[512];

    // Define image filename as string
    char filename[8];

    // Define image as file
    FILE *image = NULL;

    // Define counter to keep track of how many files have been made
    int file_counter = 0;

    // Repeat until EOF:
    // (When the number of items of size 'size' read were less than requested - this is the EOF)
    // Check through blocks one at a time - reading 512 bytes (JPEGs are stored back to back in blocks of 512 byte blocks)
    while ((fread(&data, block, 1, memory_card)) == 1)
    {
        // Look for header at beginning of JPEG - first four bytes:
        if ((data[0] == 0xff) && (data[1] == 0xd8) && (data[2] == 0xff))
        {
            // If new JPEG is found with header
            // And if it is the first JPEG
            if (file_counter == 0)
            {
                // Write first JPEG file

                // Define filename
                sprintf(filename, "%03i.jpg", (file_counter));

                // Open a new JPEG file
                // Define image as file
                image = fopen(filename, "w");

                // If file cannot be opened for reading - inform user
                if (image == NULL)
                {
                    printf("Could not open %s\n", filename);
                    return 4;
                }

                // Write data in 512 byte block from JPEG file to new file
                fwrite(&data, block, 1, image);

                // Update file counter
                file_counter++;

                // Return to loop and check next block
            }

            // Else (if not first JPEG)
            else
            {
                // Close file you have been writing to
                fclose(image);

                // Define filename
                sprintf(filename, "%03i.jpg", (file_counter));

                // Open a new JPEG file
                image = fopen(filename, "w");

                // If file cannot be opened for reading - inform user
                if (image == NULL)
                {
                    printf("Could not open %s\n", filename);
                    return 5;
                }

                // Write data
                fwrite(&data, block, 1, image);

                // Update file counter
                file_counter++;

                // Return to loop and check next block
            }
        }

        // Else (if header is not found)
        else
        {
            // If an image file is already open
            if (file_counter > 0)
            {
                // Keep writing to open file
                fwrite(&data, block, 1, image);

                // Return to loop and check next block
            }
            // Else (if no image file is open yet)
            // Return to loop and check next block
        }
    }

    // When EOF is found
    // Close any remaining files
    fclose(image);
    return 0;
}
