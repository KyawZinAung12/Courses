#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>

#define SIZE 512

int main(int argc, char *argv[])
{
    if (argv[1] == NULL)
    {
        printf("Usage: ./recover image");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");

  unsigned char buffer[SIZE];

  int filecount = 0;
    
    FILE* picture = NULL; 
    
    // check if we've found a jpeg yet or not
    int jpg_found = 0; //false
    
    // go through cardfile until there aren't any blocks left
    while (fread(buffer, SIZE, 1, file) == 1)
    {
        // read first 4 bytes of buffer and see if jpg signature using bitwise on last byte
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xe0) == 0xe0)
        {
            if (jpg_found == 1)
            {
                // We found the start of a new pic so close out current picture
                fclose(picture);
            }
            else
            {
                // jpg discovered and now we have the green light to write
                jpg_found = 1;
            }
            
            char filename[8];
            sprintf(filename, "%03d.jpg", filecount);
            picture = fopen(filename, "a");
            filecount++;
        }
        
        if (jpg_found == 1)
        {
            // write 512 bytes to file once we start finding jpgs
            fwrite(&buffer, SIZE, 1, picture);
        }
        
    }

    // close files
    fclose(file);
    fclose(picture);

    return 0;
}
