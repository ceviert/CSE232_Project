#include <stdio.h>
#include <string.h>
#include "cse232editor.h"

void edit(char *filename) {
    FILE *file = fopen(filename, "r");

    
    for (int i = 0; i < TEXT_BUFFER_DEPTH; i++) {
        textbuffer[i].prev = i - 1;
        textbuffer[i].next = i + 1;
        textbuffer[i].statement[0] = '\0';
    }
    textbuffer[0].prev = NULL_LINE_TERMINATOR;
    textbuffer[TEXT_BUFFER_DEPTH - 1].next = NULL_LINE_TERMINATOR;

    free_head = 0;
    inuse_head = NULL_LINE_TERMINATOR;

    if (file == NULL) {
        printf("File not found. A new file will be created: %s\n", filename);
        return;
    }

    char line[100];
    int last_index = NULL_LINE_TERMINATOR;
    int current_index = free_head;

    while (fgets(line, sizeof(line), file)) {
        if (current_index == NULL_LINE_TERMINATOR) {
            printf("Text buffer is full, no more lines can be loaded.\n");
            break;
        }

        
        line[strcspn(line, "\n")] = '\0';

        strncpy(textbuffer[current_index].statement, line, 39);
        textbuffer[current_index].statement[39] = '\0';

        int temp = textbuffer[current_index].next;

        textbuffer[current_index].prev = last_index;
        textbuffer[current_index].next = NULL_LINE_TERMINATOR;

        if (last_index != NULL_LINE_TERMINATOR) {
            textbuffer[last_index].next = current_index;
        } else {
            inuse_head = current_index;  
        }

        last_index = current_index;
        current_index = temp;
    }

    
    free_head = current_index;

    fclose(file);
}