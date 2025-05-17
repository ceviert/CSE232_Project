#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include "cse232editor.h"

typedef struct stateNode {
    char operation; // 'i' for insertion or 'd' for deletion operation
    int prev_free_head; // free_head before the operation
    int prev_inuse_head; // inuse_head before the operation
    char recoveryStatement[TEXT_BUFFER_STATEMENT_LENGTH];
    int line_num;
    struct stateNode* next;
} stateNode;

struct stateNode* undoTop = NULL;
struct stateNode* redoTop = NULL;

void pushUndo(struct stateNode theNode) {
    stateNode* newNode = (stateNode*) malloc(sizeof(struct stateNode));

    if (newNode == NULL) { 
        fprintf(stderr, "Memory allocation failed in pushUndo\n");
        exit(1);
    }

    *newNode = theNode;

    if (undoTop == NULL) {
        undoTop = newNode;
        undoTop->next = NULL;
    } else {
        newNode->next = undoTop; 
        undoTop = newNode;       
    }
}

stateNode popUndo() {
    if (undoTop == NULL) {
        stateNode empty = {'\0', 0, 0, "", 0, NULL};
        return empty;
    }

    stateNode* temp = undoTop;
    stateNode recoveryNode = *undoTop;
    undoTop = undoTop->next;
    free(temp);
    
    return recoveryNode;
}

void pushRedo(struct stateNode theNode) {
    stateNode* newNode = (stateNode*) malloc(sizeof(struct stateNode));

    if (newNode == NULL) { 
        fprintf(stderr, "Memory allocation failed in pushRedo\n");
        exit(1);
    }

    *newNode = theNode;

    if (redoTop == NULL) {
        redoTop = newNode;
        redoTop->next = NULL;
    } else {
        newNode->next = redoTop; 
        redoTop = newNode;      
    }
}

stateNode popRedo() {
    if (redoTop == NULL) {
        stateNode empty = {'\0', 0, 0, "", 0, NULL};
        return empty;
    }

    stateNode* temp = redoTop;
    stateNode recoveryNode = *redoTop;
    redoTop = redoTop->next;
    free(temp);
    
    return recoveryNode;
}

void clearRedo() {
    redoTop = NULL;
}

void updateUndoStack(char op, char statement[], int line_num) {
    stateNode newnode;
    newnode.operation = op;
    newnode.prev_free_head = free_head;
    newnode.prev_inuse_head = inuse_head;
    newnode.line_num = line_num;
    strcpy(newnode.recoveryStatement, statement); 
    pushUndo(newnode);
    clearRedo();
}

void undo() {
    if (undoTop == NULL) {
        DEBUG_PRINT("Nothing to undo\n"); 
        return;
    }
    
    stateNode tmp = popUndo();
    
    if (tmp.operation == 'd') {
        DEBUG_PRINT("Undoing a delete operation, inserting '%s' at line '%d'.\n", tmp.recoveryStatement, tmp.line_num);
        stateNode redoNode = {'d', free_head, inuse_head, "", tmp.line_num, NULL};
        strcpy(redoNode.recoveryStatement, tmp.recoveryStatement);
        pushRedo(redoNode);
        free_head = tmp.prev_free_head;
        inuse_head = tmp.prev_inuse_head;
        DEBUG_PRINT("inuse_head: '%d', free_head: '%d' now.\n", inuse_head, free_head);
        insert(tmp.line_num, tmp.recoveryStatement, true);
    } else {
        DEBUG_PRINT("Undoing an insert operation, deleting line '%d'\n", tmp.line_num);
        stateNode redoNode = {'i', free_head, inuse_head, "", tmp.line_num, NULL};
        strcpy(redoNode.recoveryStatement, tmp.recoveryStatement);
        pushRedo(redoNode);
        free_head = tmp.prev_free_head;
        inuse_head = tmp.prev_inuse_head;
        DEBUG_PRINT("inuse_head: '%d', free_head: '%d' now.\n", inuse_head, free_head);
        delete(tmp.line_num, true);
    }
}

void redo() {
    if (redoTop == NULL) {
        DEBUG_PRINT("Nothing to redo\n"); 
        return;
    }
    
    stateNode tmp = popRedo();
    
    if (tmp.operation == 'd') {
        DEBUG_PRINT("Redoing a delete operation, deleting line '%d'.\n", tmp.line_num);
        stateNode undoNode = {'d', free_head, inuse_head, "", tmp.line_num, NULL};
        strcpy(undoNode.recoveryStatement, tmp.recoveryStatement);
        pushUndo(undoNode);
        free_head = tmp.prev_free_head;
        inuse_head = tmp.prev_inuse_head;
        DEBUG_PRINT("inuse_head: '%d', free_head: '%d' now.\n", inuse_head, free_head);
        delete(tmp.line_num, true);
    } else {
        DEBUG_PRINT("Redoing an insert operation, inserting '%s' at line '%d'\n", tmp.recoveryStatement, tmp.line_num);
        stateNode undoNode = {'i', free_head, inuse_head, "", tmp.line_num, NULL};
        strcpy(undoNode.recoveryStatement, tmp.recoveryStatement);
        pushUndo(undoNode);
        free_head = tmp.prev_free_head;
        inuse_head = tmp.prev_inuse_head;
        DEBUG_PRINT("inuse_head: '%d', free_head: '%d' now.\n", inuse_head, free_head);
        insert(tmp.line_num, tmp.recoveryStatement, true);
    }
}
