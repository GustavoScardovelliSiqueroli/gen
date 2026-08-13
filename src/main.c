#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

bool file_has_content(char *filename) {
  FILE *p_file;
  p_file = fopen(filename, "rb");
  if (p_file == NULL) {
    return false;
  }
  fseek(p_file, 0, SEEK_END);

  long int file_size = ftell(p_file);
  fclose(p_file);
  return file_size != 0;
}

int main(void) {
  const char DIR_NAME[10] = "anotacoes";

  DIR *p_dir;
  struct dirent *p_entry;
  FILE *p_file;
  time_t current_time;
  char formated_time[20];
  char filename[34];

  p_dir = opendir(DIR_NAME);
  if (p_dir == NULL) {
    fprintf(stderr, "gen: error: %s directory not found\n", DIR_NAME);
    return EXIT_FAILURE;
  }

  while ((p_entry = readdir(p_dir)) != NULL) {
    if (strcmp(p_entry->d_name, ".") == 0 ||
        strcmp(p_entry->d_name, "..") == 0) {
      continue;
    }
    char existing_filename[256] = "";
    strcat(existing_filename, DIR_NAME);
    strcat(existing_filename, "/");
    strcat(existing_filename, p_entry->d_name);

    if (file_has_content(existing_filename) == false) {
      fprintf(
          stderr,
          "gen: error: file %s without content\n\nRun 'gen --clean' for remove "
          "all empty files\nSee 'gen --help'\n",
          existing_filename
      );
      return EXIT_FAILURE;
    };
  }

  time(&current_time);
  strftime(
      formated_time,
      sizeof(formated_time),
      "%Y-%m-%d_%H-%M-%S",
      localtime(&current_time)
  );

  strcpy(filename, DIR_NAME);
  strcat(filename, "/");
  strcat(filename, formated_time);
  strcat(filename, ".txt");

  p_file = fopen(filename, "w");

  if (p_file == NULL) {
    fprintf(stderr, "gen: error: failed to create file '%s'\n", filename);
    return EXIT_FAILURE;
  }

  fclose(p_file);

  printf("gen: generated '%s'\n", filename);
  return EXIT_SUCCESS;
}
