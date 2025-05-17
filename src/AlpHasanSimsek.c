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

void pushUndo(struct stateNode theNode) { // gets called before an insertion, deletion or redo operation being performed
    stateNode* newNode = (stateNode*) malloc(sizeof(struct stateNode));

    if (newNode == NULL) { 
        fprintf(stderr, "Memory allocation failed in pushUndo\n");
        exit(1);
    }

    *newNode = theNode;

    if(undoTop == NULL){
        undoTop = newNode;
        undoTop->next = NULL;
    }else{
        newNode->next = undoTop; 
        undoTop = newNode;       
    }
}

stateNode popUndo(){ // gets called when an operation is being undo { by undo() }
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

void pushRedo(struct stateNode theNode) { // get called after undo() 
    stateNode* newNode = (stateNode*) malloc(sizeof(struct stateNode));

    if (newNode == NULL) { 
        fprintf(stderr, "Memory allocation failed in pushRedo\n");
        exit(1);
    }

    *newNode = theNode;

    if(redoTop == NULL){
        redoTop = newNode;
        redoTop->next = NULL;
    }else{
        newNode->next = redoTop; 
        redoTop = newNode;      
    }
}

stateNode popRedo(){ // Gets called when an operation is being redo { by redo() }
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

void clearRedo(){ // The redo stack will be reset if a new operation is added to the undo stack
    redoTop = NULL;
}

void updateUndoStack(char op, char statement[], int line_num){ 
    stateNode newnode;
    newnode.operation = op;
    newnode.prev_free_head = free_head;
    newnode.prev_inuse_head = inuse_head;
    newnode.line_num = line_num;
    strcpy(newnode.recoveryStatement, statement); 
    pushUndo(newnode);
    clearRedo();
}

void undo(){
    if(undoTop == NULL){
        printf("Nothing to undo\n"); 
        return;
    }
    
    stateNode tmp = popUndo();
    
    if(tmp.operation == 'd'){
        printf("Undoing a delete operation, inserting '%s' at line '%d'.\n", tmp.recoveryStatement, tmp.line_num);
        stateNode redoNode = {'d', free_head, inuse_head, "", tmp.line_num, NULL};
        strcpy(redoNode.recoveryStatement, tmp.recoveryStatement);
        pushRedo(redoNode);
        free_head = tmp.prev_free_head;
        inuse_head = tmp.prev_inuse_head;
        printf("inuse_head: '%d', free_head: '%d' now.\n",inuse_head,free_head);
        //insert(tmp.line_num, tmp.recoveryStatement, true);
    }else{
        printf("Undoing a insert operation, deleting line '%d'\n", tmp.line_num);
        stateNode redoNode = {'i', free_head, inuse_head, "", tmp.line_num, NULL};
        strcpy(redoNode.recoveryStatement, tmp.recoveryStatement);
        pushRedo(redoNode);
        free_head = tmp.prev_free_head;
        inuse_head = tmp.prev_inuse_head;
        printf("inuse_head: '%d', free_head: '%d' now.\n",inuse_head,free_head);
        //delete(tmp.line_num, true);
    }
}

void redo(){
    if(redoTop == NULL){
        printf("Nothing to redo\n"); 
        return;
    }
    
    stateNode tmp = popRedo();
    
    if(tmp.operation == 'd'){
        printf("Redoing a delete operation, deleting line '%d'.\n", tmp.line_num);
        stateNode undoNode = {'d', free_head, inuse_head, "", tmp.line_num, NULL};
        strcpy(undoNode.recoveryStatement, tmp.recoveryStatement);
        pushUndo(undoNode);
        free_head = tmp.prev_free_head;
        inuse_head = tmp.prev_inuse_head;
        printf("inuse_head: '%d', free_head: '%d' now.\n",inuse_head,free_head);
        delete(tmp.line_num, true);
    }else{
        printf("Redoing a insert operation, inserting '%s' at line '%d'\n", tmp.recoveryStatement, tmp.line_num);
        stateNode undoNode = {'i', free_head, inuse_head, "", tmp.line_num, NULL};
        strcpy(undoNode.recoveryStatement, tmp.recoveryStatement);
        pushUndo(undoNode);
        free_head = tmp.prev_free_head;
        inuse_head = tmp.prev_inuse_head;
        printf("inuse_head: '%d', free_head: '%d' now.\n",inuse_head,free_head);
        insert(tmp.line_num, tmp.recoveryStatement, true);
    }
}
