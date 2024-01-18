#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    // fread into a buffer - 512 bytes - 1 - card.raw

    // Accept a single command-line argument
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    // Open the memory card
    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        printf("Could not open file\n");
        return 1;
    }

    uint8_t buffer[512];
    int counter = 0;
    FILE *cur_jpeg = NULL;

    // While there's still data left to read from the memory card
    while (fread(buffer, 1, 512, card) == 512)
    {
        // Create JPEGs from the data
        // Get the first 4 bits of the byte and set the remaning bits to 0, so remaning become 0xe0
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // If JPEG file is already open
            if (cur_jpeg != NULL)
            {
                fclose(cur_jpeg);
            }
            char filename[8];
            // Insert formatted JPEG filename into filename string
            sprintf(filename, "%03i.jpg", counter);
            counter++;
            cur_jpeg = fopen(filename, "w");
            fwrite(buffer, 512, 1, cur_jpeg);
        }
        else
        {
            if (cur_jpeg != NULL)
            {
                fwrite(buffer, 512, 1, cur_jpeg);
            }
        }
    }
    fclose(card);
    fclose(cur_jpeg);
}
