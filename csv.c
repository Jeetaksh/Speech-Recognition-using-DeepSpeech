#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

void generateCsv(const char *inputDir, const char *outputCsv);

int main() {
    const char *inputDirectory = "train";
    const char *outputCsv = "train.csv";

    generateCsv(inputDirectory, outputCsv);

    return 0;
}

void generateCsv(const char *inputDir, const char *outputCsv) {
    FILE *csvFile = fopen(outputCsv, "w");

    if (csvFile == NULL) {
        perror("Error opening CSV file for writing");
        exit(EXIT_FAILURE);
    }

    // Write header to CSV
    fprintf(csvFile, "wav_filename,wav_filesize,transcript\n");

    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;

    if ((dir = opendir(inputDir)) == NULL) {
        perror("Error opening input directory");
        fclose(csvFile);
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        char wavFilePath[256];
        snprintf(wavFilePath, sizeof(wavFilePath), "%s/%s", inputDir, entry->d_name);

        if (stat(wavFilePath, &statbuf) == -1) {
            perror("Error getting file information");
            continue;
        }

        // Check if the entry is a regular file and has a .wav extension
        if (S_ISREG(statbuf.st_mode) && strstr(entry->d_name, ".wav") != NULL) {
            // Extract filename without extension
            char filenameWithoutExt[256];
            snprintf(filenameWithoutExt, sizeof(filenameWithoutExt), "%.*s",
                     (int)(strrchr(entry->d_name, '.') - entry->d_name), entry->d_name);

            // Construct the transcript file path
            char transcriptFilePath[256];
            snprintf(transcriptFilePath, sizeof(transcriptFilePath), "%s/%s.txt", inputDir, filenameWithoutExt);


            // Get the size of the WAV file
            long wavFileSize = statbuf.st_size;

            // Write row to CSV
            fprintf(csvFile, "%s,%ld,%s\n", wavFilePath, wavFileSize, transcriptFilePath);
        }
    }

    closedir(dir);
    fclose(csvFile);
}
