#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
int count = 0;

void processTxtFiles(const char *srcDir, const char *destDir);

int main() {
    const char *sourceDirectory = "TRAIN LABELS";
    const char *destinationDirectory = "train extract";

    processTxtFiles(sourceDirectory, destinationDirectory);

    return 0;
}

void processTxtFiles(const char *srcDir, const char *destDir) {
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;

    if ((dir = opendir(srcDir)) == NULL) {
        perror("Error opening source directory");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        char filePath[256];
        snprintf(filePath, sizeof(filePath), "%s/%s", srcDir, entry->d_name);

        if (stat(filePath, &statbuf) == -1) {
            perror("Error getting file information");
            continue;
        }

        if (S_ISREG(statbuf.st_mode)) {
                // Construct the source file path
                char srcFilePath[256];
                snprintf(srcFilePath, sizeof(srcFilePath), "%s/%s", srcDir, entry->d_name);

                // Open and read the contents of the source file
                FILE *srcFile = fopen(srcFilePath, "r");

                if (srcFile == NULL) {
                    perror("Error opening source file");
                    exit(EXIT_FAILURE);
                }

                char line[1024];
                while (fgets(line, sizeof(line), srcFile) != NULL) {
                    // Extract filename and sentence from the line
                    char filename[20];
                    char sentence[1000];
                    if (sscanf(line, "%s %[^\n]", filename, sentence) == 2) {
                        // Construct the destination file path
                        char destFilePath[256];
                        snprintf(destFilePath, sizeof(destFilePath), "%s/%s.txt", destDir, filename);

                        // Open and write the sentence to the destination file
                        FILE *destFile = fopen(destFilePath, "a");
                        count++;

                        if (destFile == NULL) {
                            perror("Error opening destination file");
                            fclose(srcFile);
                            exit(EXIT_FAILURE);
                        }
                        printf("\nProcessed Files: %d", count);

                        fprintf(destFile, "%s\n", sentence);

                        fclose(destFile);
                    }
                }

                fclose(srcFile);
            } 
    }

    closedir(dir);
}
