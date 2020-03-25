/*
  Original code belongs to Incredible Brandan Black
  https://github.com/rubenshibu/encryption-decryption
*/
#include <stdio.h>
#include <stdlib.h>

typedef unsigned char Byte;

void banner()
{
  puts("");
  puts("_______ _____ _______  _____         _______");
  puts("|______   |   |  |  | |_____] |      |______");
  puts("______| __|__ |  |  | |       |_____ |______");
  puts("");
  puts("_______ __   _ _______  ______ __   __  _____  _______  _____   ______");
  puts("|______ | \\  | |       |_____/   \\_/   |_____]    |    |     | |_____/");
  puts("|______ |  \\_| |_____  |    \\_    |    |          |    |_____| |    \\_");
  puts("");
}

int main(int argc, char** argv)
{
	banner();

  if (argc < 3) {
    printf("Use: %s <input_file> <output_file>\n\n", argv[0]);
    exit(-1);
  }

  FILE* in_file = fopen(argv[1], "r");
	FILE* out_file = fopen(argv[2], "w+");

	Byte letter;
	Byte temp;

  while(fscanf(in_file, "%c", &letter) != EOF)
  {
    temp = (letter >> 7) & 1;
    letter = letter << 1;
    letter = letter | temp;
    letter = ~letter;
    fprintf( out_file, "%c", letter );
  }

	fclose(in_file);
	fclose(out_file);
}
