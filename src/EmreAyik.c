#include <stdio.h>
#include <string.h>
#include "cse232editor.h"

void edit(char *filename) {
    FILE *file = fopen(filename, "r");

    
    for (int i = 0; i < MAX_LINES; i++) {
        textbuffer[i].prev = i - 1;
        textbuffer[i].next = i + 1;
        textbuffer[i].statement[0] = '\0';
    }
    textbuffer[0].prev = -1;
    textbuffer[MAX_LINES - 1].next = -1;

    free_head = 0;
    inuse_head = -1;

    if (file == NULL) {
        printf("File not found. A new file will be created: %s\n", filename);
        return;
    }

    char line[100];
    int last_index = -1;
    int current_index = free_head;

    while (fgets(line, sizeof(line), file)) {
        if (current_index == -1) {
            printf("Text buffer is full, no more lines can be loaded.\n");
            break;
        }

        
        line[strcspn(line, "\n")] = '\0';

        strncpy(textbuffer[current_index].statement, line, 39);
        textbuffer[current_index].statement[39] = '\0';

       
        textbuffer[current_index].prev = last_index;
        textbuffer[current_index].next = -1;

        if (last_index != -1) {
            textbuffer[last_index].next = current_index;
        } else {
            inuse_head = current_index;  
        }

        last_index = current_index;
        current_index = textbuffer[current_index].next;
    }

    
    free_head = current_index;

    fclose(file);
}