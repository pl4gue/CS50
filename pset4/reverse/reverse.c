#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "wav.h"

int check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 3)
    {
        printf("Usage: ./reverse input.wav output.wav\n");
        return 1;
    }

    // Open input file for reading
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Couldn't open input file.\n");
        return 2;
    }

    // Read header
    WAVHEADER header;
    fread(&header, sizeof(WAVHEADER), 1, input);

    // Use check_format to ensure WAV format
    if (!check_format(header))
    {
        printf("Input is not a WAV file.\n");
        return 1;
    }

    // Open output file for writing
    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        fclose(input);
        printf("Couldn't open output file.\n");
        return 2;
    }

    // Write header to file
    fwrite(&header, sizeof(WAVHEADER), 1, output);

    // Use get_block_size to calculate size of block
    // TODO #7
    int block_size = get_block_size(header);

    // Write reversed audio to file
    BYTE buffer[block_size];

    fseek(input, -block_size, SEEK_END);

    // Moves to the end of the header
    while (ftell(input) >= sizeof(WAVHEADER))
    {
        fread(&buffer, block_size, 1, input);

        fwrite(&buffer, block_size, 1, output);

        fseek(input, block_size * -2, SEEK_CUR);
    }

    //Close the files
    fclose(input);
    fclose(output);
}

int check_format(WAVHEADER header)
{
    return (header.format[0] == 'W' &&
            header.format[1] == 'A' &&
            header.format[2] == 'V' &&
            header.format[3] == 'E');
}

int get_block_size(WAVHEADER header)
{
    //Calculates the block size as the Number of Channels * Bytes per Sample.
    //The header contains bits per sample, so we divide by 8 and have the bytes per sample.
    return header.numChannels * (header.bitsPerSample / 8);
}