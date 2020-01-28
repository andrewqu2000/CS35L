#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>

int frobcmp (char const *a, char const *b)
{
  while (*a != ' ' && *b != ' ') {
    char ca = (*(a++) ^ 0x2A);
    char cb = (*(b++) ^ 0x2A);
    if (ca > cb)
      return 1;
    if (ca < cb)
      return -1;
  }
  if (*a == ' ' && *b != ' ')
    return -1;
  else if (*a != ' ' && *b == ' ')
    return 1;
  else
    return 0;
}

int ifrobcmp (char const *a, char const *b)
{
  while (*a != ' ' && *b != ' ') {
    char ca = (*(a++) ^ 0x2A);
    char cb = (*(b++) ^ 0x2A);
    // To upper
    if (ca == EOF || (ca >= 0 && ca <= UCHAR_MAX)) {
      ca = toupper(ca);
    }
    if (cb == EOF || (cb >= 0 && cb <= UCHAR_MAX)) {
      cb = toupper(cb);
    }
    if (ca > cb)
      return 1;
    if (ca < cb)
      return -1;
  }
  if (*a == ' ' && *b != ' ')
    return -1;
  else if (*a != ' ' && *b == ' ')
    return 1;
  else
    return 0;
}

int vfrobcmp (const void *left, const void *right) {
  char const* a = *(const char**)left;
  char const* b = *(const char**)right;
  return frobcmp(a, b);
}

int ivfrobcmp (const void *left, const void *right) {
  char const* a = *(const char**)left;
  char const* b = *(const char**)right;
  return ifrobcmp(a, b);
}

int main(int argc, char **argv)
{
  int icase = 0;
  if (argc > 2) {
    fprintf(stderr, "Wrong number of arguments.\n");
    exit(1);
  }
  if (argc == 2) {
    if (strcmp("-f", argv[1]) == 0)
      icase = 1;
    else {
      fprintf(stderr, "No such opition.\n");
      exit(1);
    }
  }
  struct stat fileS;
  // Unable to read information from the file.
  if(fstat(0, &fileS) < 0) {
    fprintf(stderr, "Unable to read info.\n");
    exit(1);
  }
  ssize_t rsta = 1;
  size_t fsize = 0;
  char *fbuf = (char*)malloc(fsize * sizeof(char));
  if (fbuf == NULL) {
    fprintf( stderr, "Memory allocation failed.\n" );
    exit(1);
  }
  // If it is regular file, read everything at once
  if (S_ISREG(fileS.st_mode)) {
    fsize = fileS.st_size;
    if (fsize > 0) {
      char *temp = (char*)realloc(fbuf, fsize * sizeof(char));
      if (temp == NULL) {
	//free(fbuf);
	fprintf( stderr, "Memory allocation failed.\n" );
	exit(1);
      }
      fbuf = temp;
      // Read in
      rsta = read(STDIN_FILENO, fbuf, fsize);
      lseek(0, 0, SEEK_CUR);
      if (rsta < 0) {
	free(fbuf);
	fprintf( stderr, "Input failed.\n" );
	exit(1);
      }
    }
  }
  // Continue to read
  while (rsta > 0) {
    char cur[1];
    lseek(0, 0, SEEK_CUR);
    rsta = read(STDIN_FILENO, cur, 1);
    if (rsta < 0) {
      free(fbuf);
      fprintf( stderr, "Input failed.\n" );
      exit(1);
    }
    if (rsta == 0)
      break;
    char *temp = (char*)realloc(fbuf, (++fsize) * sizeof(char));
    if (temp == NULL) {
      fprintf( stderr, "Memory allocation failed.\n" );
      exit(1);
    }
    fbuf = temp;
    fbuf[fsize - 1] = cur[0];
  }
  // Assign a space if not ending with space.
  if (fsize > 0 && fbuf[fsize - 1] != ' ') {
    char *temp = (char*)realloc(fbuf, (++fsize) * sizeof(char));
    if (temp == NULL) {
      fprintf( stderr, "Memory allocation failed.\n" );
      exit(1);
    }
    fbuf = temp;
    fbuf[fsize - 1] = ' ';
  }
  // Create an array
  size_t arrSize = 0;
  size_t stringSize = 0;
  char **array = (char**)malloc(arrSize * sizeof(char*));
  if (array == NULL) {
    free(fbuf);
    fprintf( stderr, "Memory allocation failed.\n" );
    exit(1);
  }
  // Fill the array with the content read
  size_t ftrack = 0;
  // Set the first one item in the array
  if (fsize > 0) {
    char **temp = (char**)realloc(array, (++arrSize) * sizeof(char*));
    if (temp == NULL) {
      free(fbuf);
      fprintf( stderr, "Memory allocation failed.\n" );
      exit(1);
    }
    array = temp;
    array[0] = fbuf;
  }
  // Set the rest array
  for (ftrack = 0; ftrack < fsize; ftrack++) {
    if (fbuf[ftrack] == ' ' && ftrack + 1 < fsize) {
      char **temp = (char**)realloc(array, (++arrSize) * sizeof(char*));
      if (temp == NULL) {
	free(fbuf);
	fprintf( stderr, "Memory allocation failed.\n" );
	exit(1);
      }
      array = temp;
      array[arrSize - 1] = fbuf + ftrack + 1;
    }
  }

  // Sort
  if (icase == 0)
    qsort(array, arrSize, sizeof(char*), vfrobcmp);
  else
    qsort(array, arrSize, sizeof(char*), ivfrobcmp);
  // Print the sorted array
  int i;
  for (i = 0; i < arrSize; i++) {
    int j = 0;
    while (array[i][j] != ' ') {
      ssize_t wsta = write(STDOUT_FILENO, &array[i][j], 1);
      if (wsta < 0) {
	free(fbuf);
        free(array);
	fprintf( stderr, "Output failed.\n" );
	exit(1);
      }
      j++;
    }
    ssize_t wsta = write(STDOUT_FILENO, &array[i][j], 1);
    if (wsta < 0) {
      free(fbuf);
      free(array);
      fprintf( stderr, "Output failed.\n" );
      exit(1);
    }
  }
  // Free memory
  free(fbuf);
  free(array);
  return 0;
}
