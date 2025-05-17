// Muhammet Furkan UZUN

#include <stdio.h>
#include "cse232editor.h"

/**
 * Saves the current text in the text buffer to the specified file.
 * The function opens the file in write mode and writes each line from the text buffer
 * starting from the node pointed to by `inuse_head`. If the file cannot be opened,
 * an error message is printed. Once all lines have been written, the file is closed.
 *
 * @param filename The name of the file to save the text to
 */
void save(char *filename) {
    FILE *fp = fopen(filename, "w");  // Open the file in write mode

    // Check if the file was successfully opened
    if (fp == NULL) {
        perror("There was a problem writing to the file.");  // Print an error message if file opening failed
        return;  // Exit the function if the file can't be opened
    }

    // Loop through the text buffer starting from the line pointed by `inuse_head`
    for (int iter = inuse_head; iter != NULL_LINE_TERMINATOR; iter = textbuffer[iter].next) {
        fprintf(fp, "%s", textbuffer[iter].statement);  // Write each line (statement) to the file
    }

    fclose(fp);  // Close the file after writing
}

