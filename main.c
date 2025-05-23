// CSE-232 TERM PROJECT
// Text editor using ncurses for TUI (Terminal User Interface) interaction.
// Supports basic editing operations such as edit, insert, delete, undo, redo, save, and display.

#include <stdio.h>
#include <string.h>
#include <curses.h> // For ncurses-based TUI

#include "cse232editor.h" // Contains declarations and macros used in the project
#include "debug_interface.h" // Contains declarations and macros used in the project

// ----------------------------------------------------------------------------
// GLOBAL VARIABLES
// ----------------------------------------------------------------------------

// Text buffer implemented as a doubly linked list using static array
// Each node holds a single line (statement) and has next/prev links
struct node textbuffer[TEXT_BUFFER_DEPTH];

// Head of the free list (unused nodes)
int free_head = NULL_LINE_TERMINATOR;

// Head of the in-use list (lines that are currently in the editor)
int inuse_head = NULL_LINE_TERMINATOR;

// Stores the name of the file being edited
char file_name[MAX_FILE_NAME_LENGTH];

// Cursor position on the Y-axis (row), constrained by buffer size
#define cursorY_MINLIMIT 0
#define cursorY_MAXLIMIT (cursorY_MINLIMIT + TEXT_BUFFER_DEPTH)
int cursorY = cursorY_MINLIMIT;

// ----------------------------------------------------------------------------
// FUNCTION: getStringNcurses
// DESCRIPTION:
//   Allows the user to enter a string using ncurses. Supports character input,
//   backspace handling, and Enter to submit. Accepts only printable characters.
// PARAMETERS:
//   - tostr: output buffer to store input string
//   - maxChar: maximum number of characters allowed in the string
// RETURNS:
//   - number of characters entered (excluding null terminator)
// ----------------------------------------------------------------------------
int getStringNcurses(char *tostr, int maxChar)
{
    int ch, index = 0;
    int y, x;

    getyx(stdscr, y, x); // Get cursor position

    while ((ch = getch()) != '\n' && index < maxChar - 1)
    {
        if (ch == KEY_BACKSPACE || ch == 127)
        {
            if (index > 0)
            {
                index--;
                x--;
                mvdelch(y, x); // Delete character from screen
            }
        }
        else if (ch >= 32 && ch <= 126) // Printable characters
        {
            tostr[index++] = ch;
            mvaddch(y, x++, ch); // Echo character
        }

        move(y, x);
        refresh();
    }

    tostr[index] = '\0'; // Null-terminate the input
    return index;
}

// ----------------------------------------------------------------------------
// FUNCTION: display2
// DESCRIPTION:
//   Displays all the lines currently in use in the text buffer to stdout.
// ----------------------------------------------------------------------------
void display2(void)
{
    for (int iter = inuse_head; iter != NULL_LINE_TERMINATOR; iter = textbuffer[iter].next)
    {
        printf("%s\n", textbuffer[iter].statement);
    }
}

// ----------------------------------------------------------------------------
// FUNCTION: displayMethod
// DESCRIPTION:
//   Updates the TUI screen with the current text buffer and moves cursor to
//   its last known Y-position.
// ----------------------------------------------------------------------------
void displayMethod(void)
{
    display();
    move(cursorY, 0); // Reposition cursor
    refresh();        // Refresh screen
}

// ----------------------------------------------------------------------------
// FUNCTION: main
// DESCRIPTION:
//   Entry point of the program. Based on DEBUG flag, program either runs
//   in ncurses interactive mode or CLI command mode for easier testing.
// ----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    printf("CSE-232 Text Editor - Term Project\n");

    if (argc == 2)
    {
        // File name provided as command-line argument
        char *fileArg = argv[1];
        memcpy(file_name, fileArg, strlen(fileArg));
    }
    else
    {
        // Invalid usage
        printf("DO NOT GIVEN ANY FILE VIA ARGUMENT\n");
        printf("use: ./program file_name\n");
        return -1;
    }

    edit(file_name);

#ifndef DEBUG // ------------ INTERACTIVE NCURSES MODE ------------
    initscr();            // Initialize screen for ncurses
    cbreak();             // Disable line buffering
    noecho();             // Do not echo typed characters
    keypad(stdscr, TRUE); // Enable arrow keys and function keys
    curs_set(1);          // Make cursor visible

    displayMethod(); // Display initial screen

    int ch;
    while ((ch = getch()) != 'Q') // Quit when user presses 'Q'
    {
        switch (ch)
        {
            /*
                - E (edit): Opens the specified file and reads it into the text buffer.
                - I (insert): Inserts a line in the text buffer at the current cursor position.
                - D (delete): Deletes the current line in the text buffer.
                - U (undo): Reverts the effect of the last command.
                - R (redo): Repeats the last undone command.
                - P (display): Displays the current text buffer on the screen.
                - S (save): Saves the current content to the file.
                - Q (quit): Exits the editor.
            */
            case 'I': {
                char statement[TEXT_BUFFER_STATEMENT_LENGTH];
                getStringNcurses(statement, TEXT_BUFFER_STATEMENT_LENGTH);

                if(insert(cursorY, statement) != -1){
                    reserveTheState(cursorY, 'i');
                }

                break;
            }

            case 'D': {

                if(delete(cursorY) != -1){
                    reserveTheState(cursorY, 'd');
                }

                break;
            }

            case 'U':
                undo(); // Undo last operation
                break;

            case 'R':
                redo(); // Redo last undone operation
                break;

            case 'S':
                save(file_name); // Save buffer to file
                break;

            case 'P':
                displayMethod(); // Re-display buffer
                break;

            case KEY_UP:
                if (cursorY > cursorY_MINLIMIT) cursorY--;
                else beep();
                displayMethod();
                break;

            case KEY_DOWN:
                if (cursorY < cursorY_MAXLIMIT - 1) cursorY++;
                else beep();
                displayMethod();
                break;
        }
    }

    endwin(); // Clean up ncurses
#else // ---------------------- DEBUG / CLI MODE ----------------------
    char command[256];
    while (1)
    {
        printf("$> ");
        if (fgets(command, sizeof(command), stdin) == NULL) {
            printf("Unknown Command\n");
            continue;
        }
        
        printf("command: %s\n", command);

        if (!strncmp(command, "edit", 4))
        {
            get_argument(command, 1, 's', file_name, MAX_FILE_NAME_LENGTH);
            edit(file_name);
        }
        else if (!strncmp(command, "insert2", 7))
        {
            int line;
            char statement[TEXT_BUFFER_STATEMENT_LENGTH];
            get_argument(command, 1, 'd', &line, 0);
            get_argument(command, 2, 's', statement, TEXT_BUFFER_STATEMENT_LENGTH);
            statement[strcspn(statement, "\n")] = '\0';
            printf("text inserted: (line: %d, statement: %s)\n", line, statement);

            if(insert(line, statement) != -1){
                reserveTheState(line, 'i');
            }
        }
        else if (!strncmp(command, "insert", 6))
        {
            int line;
            char statement[TEXT_BUFFER_STATEMENT_LENGTH];
            get_argument(command, 1, 'd', &line, 0);
            get_argument(command, 2, 's', statement, TEXT_BUFFER_STATEMENT_LENGTH);
            statement[strcspn(statement, "\n")] = '\0';
            printf("text inserted: (line: %d, statement: %s)\n", line, statement);
            insert(line, statement);
        }
        else if (!strncmp(command, "delete2", 7))
        {
            int line;
            get_argument(command, 1, 'd', &line, 0);

            if(delete(line) != -1){
                reserveTheState(line, 'd');
            }
        }
        else if (!strncmp(command, "delete", 6))
        {
            int line;
            get_argument(command, 1, 'd', &line, 0);
            delete(line);
        }
        else if (!strncmp(command, "undo", 4))
        {
            undo();
        }
        else if (!strncmp(command, "redo", 4))
        {
            redo();
        }
        else if (!strncmp(command, "save", 4))
        {
            save(file_name);
            printf("File saved, file name: %s\n", file_name);
        }
        else if (!strncmp(command, "display", 7))
        {
            display2();
        }
        else if (!strncmp(command, "dump", 4))
        {
            printf("File Name: %s\n", file_name);
            printf("inuse_head: %d\n", inuse_head);
            printf("free_head: %d\n", free_head);
            dump_list("Textbuffer");
        }
        else if (!strncmp(command, "help", 4))
        {
            printf("Help Menu:\n");
            printf("  edit <filename>           : Opens the given file for editing.\n");
            printf("  insert <line> <statement> : Inserts the statement at the given line.\n");
            printf("  delete <line>             : Deletes the line at the specified index.\n");
            printf("  undo                      : Reverts the last change.\n");
            printf("  redo                      : Re-applies the last undone change.\n");
            printf("  save                      : Save the file.\n");
            printf("  display                   : Displays the current content in formatted view.\n");
            printf("  dump                      : Dumps the internal text buffer state.\n");
            printf("  help                      : Shows this help menu.\n");
            printf("  q                         : Quits the editor.\n");
        }
        else if (!strncmp(command, "q", 1))
        {
            printf("quit\n");
            break;
        }
        else
        {
            printf("Unknown Command\n");
        }
    }
#endif

    return 0;
}
