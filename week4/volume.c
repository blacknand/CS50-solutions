// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // TODO: Copy header from input file to output file
    uint8_t buffer[HEADER_SIZE];
    fread(buffer, HEADER_SIZE, 1, input);   // 1st arg needs to be a pointer to a location in memory where you store the location
                                            // fread reads data from a file into a buffer,
    fwrite(buffer, HEADER_SIZE, 1, output); // fwrite writes data from a buffer into a file

    // TODO: Read samples from input file and write updated data to output file
    int16_t mem;
    while (fread(&mem, sizeof(mem), 1, input) != 0)
    {
        mem *= factor;
        fwrite(&mem, sizeof(mem), 1, output);
    }

    // Close files
    fclose(input);
    fclose(output);
}
