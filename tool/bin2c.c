#include <stdio.h>
#include <assert.h>

int main(int argc, char** argv)
{
  assert(argc == 2);
  char* fn = argv[1];
  FILE* f = fopen(fn, "r");
  unsigned long n = 0;
  while(!feof(f))
  {
    unsigned char c;
    if(fread(&c, 1, 1, f) == 0) break;
    printf("0x%.2X,", (int)c);
    ++n;
    if(n % 16 == 0)
      printf("\n");
    else
      printf(" ");
  }
  fclose(f);
}
