// Ertugrul CEVIK

#include <stdio.h>
#include <string.h>
#include "cse232editor.h"

/**
 * Inserts a new statement to the `textbuffer[]` array.
 * 
 * @return 
 * 
 * @param line The position of the line the new statement will be inserted to
 * @param stat The statement to be inserted
 */
int insert(int line, char* stat) {
    // Check if there are any free nodes
    if (free_head == NULL_LINE_TERMINATOR) {
        // perror("ERR: Text buffer is full.\n"); // Display an error message
        return -1; // Terminate the insertion
    }

    int new_node = free_head; // Allocate a new node
    free_head = textbuffer[free_head].next; // Remove the new node from the `text_buffer[]` by advancing the `free_head`

    // Reset the prev of new free_head if it's not NULL_LINE_TERMINATOR
    if (free_head != NULL_LINE_TERMINATOR) {
        textbuffer[free_head].prev = NULL_LINE_TERMINATOR;
    }

    strncpy(textbuffer[new_node].statement, stat, TEXT_BUFFER_STATEMENT_LENGTH - 1); // Copy `stat` to `textbuffer[new_node].statement` up to `TEXT_BUFFER_STATEMENT_LENGTH - 1` characters
    textbuffer[new_node].statement[TEXT_BUFFER_STATEMENT_LENGTH - 1] = '\0'; // Ensure that the statement ends with the null char

    textbuffer[new_node].next = NULL_LINE_TERMINATOR;
    textbuffer[new_node].prev = NULL_LINE_TERMINATOR; // Set the previous and next indices to null

    // Case: `line` is 0 so the user wants to insert at the beginning of the text buffer
    if (line == 0 || inuse_head == NULL_LINE_TERMINATOR) {
        textbuffer[new_node].next = inuse_head; // Set our `new_node` as the first line in `text_buffer[]`
        // The `line` is 0 but the file is not empty
        if (inuse_head != NULL_LINE_TERMINATOR) {
            textbuffer[inuse_head].prev = new_node; // Set our `new_node` as the first line in `text_buffer[]` 
        }
        inuse_head = new_node; // Make our `new_node` the head of the list
        
        return new_node; // Insertion is complete so terminate early
    }

    /** 
     * Case: `line` is other than 0 so we will insert somewhere in the middle
     * But in order to set the prev and next index links properly,
     * first we should get the node just before our insertion point.
     */
    int current = inuse_head; // This `current` will hold that node, so we initialize with `inuse_head` in case there is only one previous node and it's the `inuse_head`
    for (int i = 0; i < line - 1 && textbuffer[current].next != NULL_LINE_TERMINATOR; i++) {
        current = textbuffer[current].next; // Iterate through
    }

    int next_node = textbuffer[current].next; // Get the next node after the previous one (currently `current`)

    // Now we insert the `new_node` in between `current` and `next_node`
    textbuffer[current].next = new_node;
    textbuffer[new_node].prev = current;
    textbuffer[new_node].next = next_node;

    // Lastly, we link the prev of next_node to new_node
    if (next_node != NULL_LINE_TERMINATOR) {
        textbuffer[next_node].prev = new_node;
    }

    return new_node;
}
