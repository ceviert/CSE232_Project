//Poyraz Erdem Koyuncu

#include <stdio.h>
#include "cse232editor.h"

/*
textbuffer[]: represents the entire memory of the editor (25 lines max).

Each node has a prev and next, creating a doubly linked list.

inuse_head: index of the first node in the currently used list.

free_head: index of the first available free node.
*/

/** 

 * Traverse the in-use list until you reach the node at line.

 * Re-link its neighbors to skip over this node.

 * Add the node back to the front of the free_head list.

 * Update inuse_head if you're deleting the first node.

 * @param line the line to be deleted

 */
int delete(int line){
    if (inuse_head == NULL_LINE_TERMINATOR){ // to check if the file is empty
        perror("ERROR: Nothing to delete");
        return -1;
    }
    
    int current_head = inuse_head;
    int count = 0;

    while (current_head != NULL_LINE_TERMINATOR && count < line) { // a loop for to reach to be deleted line
        current_head = textbuffer[current_head].next;
        count++;
    }

    if (current_head == NULL_LINE_TERMINATOR){ // if the given line number is out of reach 
        perror("ERROR: Out of range");
        return -1;
    }

    int prev = textbuffer[current_head].prev;
    int next = textbuffer[current_head].next;

    if(prev != NULL_LINE_TERMINATOR){ // if the selected node is not the beggining node make the previous node's next the next node's preivous
        textbuffer[prev].next = next;
    }
    else{
        inuse_head = next; // if the line is the beggining node of the list
    }

    if(next != NULL_LINE_TERMINATOR){ // to link up the previous's next to the next's previous
        textbuffer[next].prev = prev;
    }
    // no need for a else case since if the selected node's next is already the end of the list the next after deletion will still be -1
    textbuffer[current_head].next = free_head;
    textbuffer[current_head].prev = NULL_LINE_TERMINATOR;
    free_head = current_head;
    /*
    the code from 62-64 explains that the deleted node will be new free_head by making its next the current free_head and
    previous -1 then labeling it as free_head therefore making it the new free_head
    */

    return 0;
}   