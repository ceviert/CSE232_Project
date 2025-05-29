#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include "cse232editor.h"

struct stateNode* undoTop = NULL;
struct stateNode* redoTop = NULL;

void reserveTheState(int buffer_index, char operation_type);
void updateUndoStack(char op, char statement[], int line_num);
void pushUndo(struct stateNode theNode);
stateNode popUndo();
void pushRedo(struct stateNode theNode);
stateNode popRedo();
void clearRedo();
void undo(void);
void redo(void);
void cancel_recovery( void );

void updateUndoStack(char op, char statement[], int line_num) {
    stateNode newnode;
    newnode.operation = op;
    newnode.line_num = line_num;
    strcpy(newnode.recoveryStatement, statement); 
    pushUndo(newnode);
    clearRedo();
}

void reserveTheState(int buffer_index, char operation_type){
    int tmp = 0;
    int op_line = 0;
    while(tmp != buffer_index && tmp < TEXT_BUFFER_DEPTH){
        tmp = textbuffer[tmp].next;
        op_line++;
    }

    char *statement = textbuffer[buffer_index].statement;

    updateUndoStack(operation_type, statement, op_line);

    DEBUG_PRINT("reserveTheState:: line: %d, type: %c, saved statement: %s\n", op_line, operation_type, statement);
}

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

stateNode popUndo(void) {
    if (undoTop == NULL) {
        stateNode empty;
        empty.line_num = 0;
        empty.next = 0;
        empty.operation = '?';
        empty.recoveryStatement[0] = '\0';
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

stateNode popRedo(void) {
    if (redoTop == NULL) {
        stateNode empty;
        empty.line_num = 0;
        empty.next = 0;
        empty.operation = '?';
        empty.recoveryStatement[0] = '\0';
        return empty;
    }

    stateNode* temp = redoTop;
    stateNode recoveryNode = *redoTop;
    redoTop = redoTop->next;
    free(temp);
    
    return recoveryNode;
}

void clearRedo(void) {
    redoTop = NULL;
}

void undo(void) {
    if (undoTop == NULL) {
        DEBUG_PRINT("Nothing to undo\n"); 
        return;
    }
    
    DEBUG_PRINT("Doing undo operation\n"); 
    stateNode tmp = popUndo();
    
    if (tmp.operation == 'd') {
        DEBUG_PRINT("Undoing a delete operation, inserting '%s' at line '%d'.\n", tmp.recoveryStatement, tmp.line_num);
        stateNode redoNode;
        redoNode.line_num = tmp.line_num;
        redoNode.next = NULL;
        redoNode.operation = 'd';
        strcpy(redoNode.recoveryStatement, tmp.recoveryStatement);
        pushRedo(redoNode);
        insert(tmp.line_num, tmp.recoveryStatement);
        DEBUG_PRINT("recoveryStatment: %s, what it should be: %s\n",redoNode.recoveryStatement, tmp.recoveryStatement); 
    } else {
        DEBUG_PRINT("Undoing an insert operation, deleting line '%d'\n", tmp.line_num);
        stateNode redoNode;
        redoNode.line_num = tmp.line_num;
        redoNode.next = NULL;
        redoNode.operation = 'i';
        strcpy(redoNode.recoveryStatement, tmp.recoveryStatement);
        pushRedo(redoNode);
        delete(tmp.line_num);
        DEBUG_PRINT("recoveryStatment: %s, what it should be: %s\n",redoNode.recoveryStatement, tmp.recoveryStatement); 
    }
}

void redo(void) {
    if (redoTop == NULL) {
        DEBUG_PRINT("Nothing to redo\n"); 
        return;
    }
    
    DEBUG_PRINT("Doing redo operation\n"); 
    stateNode tmp = popRedo();
    
    if (tmp.operation == 'd') {
        DEBUG_PRINT("Redoing a delete operation, deleting line '%d'.\n", tmp.line_num);
        stateNode undoNode;
        undoNode.line_num = tmp.line_num;
        undoNode.next = NULL;
        undoNode.operation = 'd';
        strcpy(undoNode.recoveryStatement, tmp.recoveryStatement);
        pushUndo(undoNode);
        delete(tmp.line_num);
        DEBUG_PRINT("recoveryStatment: %s, what it should be: %s\n",undoNode.recoveryStatement, tmp.recoveryStatement); 
    } else {
        DEBUG_PRINT("Redoing an insert operation, inserting '%s' at line '%d'\n", tmp.recoveryStatement, tmp.line_num);
        stateNode undoNode;
        undoNode.line_num = tmp.line_num;
        undoNode.next = NULL;
        undoNode.operation = 'i';
        strcpy(undoNode.recoveryStatement, tmp.recoveryStatement);
        pushUndo(undoNode);
        insert(tmp.line_num, tmp.recoveryStatement);
        DEBUG_PRINT("recoveryStatment: %s, what it should be: %s\n",undoNode.recoveryStatement, tmp.recoveryStatement); 
    }
}

void cancel_recovery( void ) {
    DEBUG_PRINT("cancel_recovery\n");
    popUndo();
}
