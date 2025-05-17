#ifndef __CSE232EDITOR_H__
#define __CSE232EDITOR_H__

/*
    For the Linked List structure, it is used for the line that represents the last element of the list.
    LinkList yapısı için listenin son elemanın gösterdiği satır için kullanılır.
*/
#define NULL_LINE_TERMINATOR                    -1

#define TEXT_BUFFER_DEPTH                       25
#define TEXT_BUFFER_STATEMENT_LENGTH            40

#define MAX_FILE_NAME_LENGTH                    256
#define PROJECT_TITLE                           "CSE-232 EDITOR (press H for help)"

struct node
{  
    int prev;   // points to the textbuffer[] index of the previous statement   
    char statement[TEXT_BUFFER_STATEMENT_LENGTH];  // max. 40 characters  
    int next;   // points to the textbuffer[] index of the next statement   
};

extern struct node textbuffer[TEXT_BUFFER_DEPTH]; // max. 25 lines of text

extern int free_head; // head node that points to the first free line in textbuffer[]  
extern int inuse_head; // head node that points to the first used line in textbuffer[] 

extern char file_name [MAX_FILE_NAME_LENGTH];

// Functions
void edit(char *filename);
void insert(int line, char *stat);
void delete(int line);
void undo( void );
void redo( void ); 
void display( void );
void save(char *filename);

#endif //__CSE232EDITOR_H__
