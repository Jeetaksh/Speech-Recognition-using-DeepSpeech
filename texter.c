
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
int count = 0;

void copyFlacFiles(const char *srcDir, const char *destDir);

int main() {
    const char *sourceDirectory = "test-clean/LibriSpeech/test-clean";
    const char *destinationDirectory = "txt_files";

    copyFlacFiles(sourceDirectory, destinationDirectory);

    return 0;
}

void copyFlacFiles(const char *sourceDir, const char *destDir) {
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;

    dir = opendir(sourceDir);

    if (dir == NULL) {
        perror("Error opening source directory");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", sourceDir, entry->d_name);

        if (stat(path, &statbuf) == -1) {
            perror("Error getting file status");
            exit(EXIT_FAILURE);
        }

        if (S_ISDIR(statbuf.st_mode)) {
            // Skip "." and ".." directories
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                copyFlacFiles(path, destDir);
            }
        } else {
            // Check if the file has a .flac extension
            const char *dot = strrchr(entry->d_name, '.');
            if (dot && strcmp(dot + 1, "txt") == 0) {
                // File has .flac extension, copy it to the destination folder
                count++;
                char sourcePath[1024];
                char destPath[1024];
                snprintf(sourcePath, sizeof(sourcePath), "%s/%s", sourceDir, entry->d_name);
                snprintf(destPath, sizeof(destPath), "%s/%s", destDir, entry->d_name);

                FILE *sourceFile = fopen(sourcePath, "rb");
                FILE *destFile = fopen(destPath, "wb");

                if (sourceFile == NULL || destFile == NULL) {
                    perror("Error opening files for copying");
                    exit(EXIT_FAILURE);
                }

                int ch;
                while ((ch = fgetc(sourceFile)) != EOF) {
                    fputc(ch, destFile);
                }

                printf("\nFile: %d ... Successfully copied: %s", count, sourceFile);

                fclose(sourceFile);
                fclose(destFile);
            }
        }
    }

    closedir(dir);
}