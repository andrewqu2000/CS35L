#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
int main(int argc, const char* argv[])
{
  // Wrong number of arguments
  if (argc != 3) {
    fprintf(stderr, "Wrong number of arguments.\n");
    exit(1);
  }
  // Compare the length of argv[1] and to
  if (strlen(argv[1]) != strlen(argv[2])) {
    fprintf(stderr, "Lengths of arguments are not the same.\n");
    exit(1);
  }
  // Check duplicates in argv[1]
  int i = 0;
  int j = 0;
  for (i = 0; argv[1][i] != '\0'; i++) {
    for (j = i + 1; argv[1][j] != '\0'; j++) {
      if (argv[1][i] == argv[1][j]) {
        fprintf(stderr, "Duplicates in the first argument.\n");
        exit(1);
      }
    }
  }

  // Get first character
  char buf[1];
  ssize_t status = read(STDIN_FILENO, buf, 1);
  if (status < 0) {
    fprintf( stderr, "Input failed." );
    exit(1);
  }
  while (status > 0) {
    int f = 0;
    for (f = 0; argv[1][f] != '\0'; f++) {
      // If found, prepare argv[2] for translation
      if (argv[1][f] == buf[0]) {
	buf[0] = argv[2][f];
	break;
      }
    }
    // Write to stdout
    ssize_t wstatus = write(STDOUT_FILENO, buf, 1);
    if (wstatus < 1) {
      fprintf( stderr, "Output failed." );
      exit(1);
    }
    // Read next character
    status = read(STDIN_FILENO, buf, 1);
  }
  if (status < 0) {
    fprintf( stderr, "Input failed." );
    exit(1);
  }
  return 0;
}
