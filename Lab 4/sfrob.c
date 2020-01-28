#include <stdio.h>
#include <stdlib.h>

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

int vfrobcmp (const void *left, const void *right) {
  char const* a = *(const char**)left;
  char const* b = *(const char**)right;
  return frobcmp(a, b);
}

int main()
{
  // Create an empty array
  size_t arrSize = 0;
  size_t stringSize = 0;
  char **array = (char**)malloc(arrSize * sizeof(char*));
  if (array == NULL) {
    fprintf( stderr, "Memory allocation failed." );
    exit(1);
  }
  char temp = getchar();
  if (ferror(stdin)) {
    fprintf( stderr, "Input failed." );
    exit(1);
  }
  // If this is not an empty file, prepare for the first string
  if (temp != EOF) {
    ungetc(temp, stdin);
    arrSize = 1;
    array = (char**)realloc(array, arrSize * sizeof(char*));
    if (array == NULL) {
      fprintf( stderr, "Memory allocation failed." );
      exit(1);
    }
    array[0] = (char*)malloc(stringSize * sizeof(char));
    if (array[0] == NULL) {
      fprintf( stderr, "Memory allocation failed." );
      exit(1);
    }
  }
  // Deal with the input
  while (temp != EOF) {
    // Read the stream
    char c = getchar();
    if (ferror(stdin)) {
      fprintf( stderr, "Input failed." );
      exit(1);
    }
    // End of file
    if (c == EOF) {
      // If not end with a trailing space and not an empty file, add a space
      if (array[arrSize - 1][stringSize - 1] != ' ') {
	array[arrSize - 1] = (char*)realloc(array[arrSize - 1],
					    (++stringSize) * sizeof(char));
	if (array[arrSize - 1] == NULL) {
	  fprintf( stderr, "Memory allocation failed." );
	  exit(1);
	}
	array[arrSize - 1][stringSize - 1] = ' ';
      }
      break;
    }
    // End of the string
    else if (c == ' ') {
      array[arrSize - 1] = (char*)realloc(array[arrSize - 1],
					  (++stringSize) * sizeof(char));
      if (array[arrSize - 1] == NULL) {
	fprintf( stderr, "Memory allocation failed." );
	exit(1);
      }
      array[arrSize - 1][stringSize - 1] = ' ';
      // Check if reach EOF
      char next = getchar();
      if (ferror(stdin)) {
	fprintf( stderr, "Input failed." );
	exit(1);
      }
      // If reach the end, leave the loop
      if (next == EOF)
	break;
      // Put the character back and prepare for next string
      ungetc(next, stdin);
      array = (char**)realloc(array, (++arrSize) * sizeof(char*));
      if (array == NULL) {
	fprintf( stderr, "Memory allocation failed." );
	exit(1);
      }
      stringSize = 0;
      array[arrSize - 1] = (char*)malloc(stringSize * sizeof(char));
      if (array[arrSize - 1] == NULL) {
	fprintf( stderr, "Memory allocation failed." );
	exit(1);
      }
    }
    // Add a common character to the last string
    else {
      array[arrSize - 1] = (char*)realloc(array[arrSize - 1],
                                          (++stringSize) * sizeof(char));
      if (array[arrSize - 1] == NULL) {
        fprintf( stderr, "Memory allocation failed." );
        exit(1);
      }
      array[arrSize - 1][stringSize - 1] = c;
    }
  }
  // Sort the array
  qsort(array, arrSize, sizeof(char*), vfrobcmp);
  // Print the sorted array
  int i;
  for (i = 0; i < arrSize; i++) {
    int j = 0;
    while (array[i][j] != ' ') {
      putchar(array[i][j]);
      if (ferror(stdout)) {
        fprintf( stderr, "Output failed." );
        exit(1);
      }
      j++;
    }
    putchar(array[i][j]);
    if (ferror(stdout)) {
      fprintf( stderr, "Output failed." );
      exit(1);
    }
  }
  // Free memory
  for (i = 0; i < arrSize; i++) {
    free(array[i]);
  }
  free(array);
  return 0;
}
