#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    //check for comand line arguments
    if (argc != 2)
    {
        printf("Usage: ./recover imageToRecover\n");
        return 1;
    }

    // Open file to recover
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // Loop over all the file to recover to find JPEGs and save each on an individual file.
    BYTE header[4];
    BYTE buffer[512];
    long int jpegCounter = 0;

    // Rehusable pointer for images
    FILE *outfile = NULL;

    // Loop over every chunck of 512 bytes
    while (fread(buffer, sizeof(BYTE), 512, input))
    {
        char output[7];

        // Check for jpegs header signature if found start a new file
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {

            if (jpegCounter > 0)
            {
                fclose(outfile);
            }

            sprintf(output, "%03li.jpg", jpegCounter);

            outfile = fopen(output, "w");

            if (outfile == NULL)
            {
                return 1;
            }

            jpegCounter += 1;
        }

        // Write to the new file only when the first header was found
        if (jpegCounter > 0)
        {
            fwrite(buffer, sizeof(BYTE), 512, outfile);
        }
    }
    fclose(outfile);

    // Close file to recover
    fclose(input);

    return 0;
}