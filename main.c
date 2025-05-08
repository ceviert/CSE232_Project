// CSE-232 TERM PROJECT

#include <stdio.h>
#include <string.h>
#include <curses.h>

#include "cse232editor.h"

// A structure to hold the text buffer, with a maximum depth of TEXT_BUFFER_DEPTH (e.g., 25 lines)
struct node textbuffer[TEXT_BUFFER_DEPTH]; // Holds the lines of text in the editor

// Pointers to the first free and used lines in the text buffer
int free_head; // Points to the first free line in the text buffer
int inuse_head; // Points to the first used line in the text buffer

// File name string, holds the name of the current file
char file_name[MAX_FILE_NAME_LENGTH]; // Holds the name of the file being edited

// Cursor position limits (Y axis) in the text buffer
int cursorY_MINLIMIT = 4; // Minimum vertical cursor position
int cursorY_MAXLIMIT = TEXT_BUFFER_DEPTH; // Maximum vertical cursor position
int cursorY = 4; // Current cursor position (starts at 4)
    
/**
 * Reads a string of text from the user using ncurses and stores it in the provided buffer.
 * The user can type up to maxChar characters, with support for backspace and printable characters.
 * The input is terminated when the Enter key ('\n') is pressed.
 *
 * @param tostr The character buffer where the input text will be stored
 * @param maxChar The maximum number of characters that can be input by the user
 * @return The number of characters entered (excluding the null-terminator)
 */
int getStringNcurses(char *tostr, int maxChar) {
    int ch, index = 0;  // Character to hold the input, index to keep track of the buffer position
    int y, x;  // Variables to store current cursor position (Y and X coordinates)
    
    // Get the current position of the cursor
    getyx(stdscr, y, x);  // Get the current cursor position in y (row) and x (column)

    // Loop to get characters until Enter key or maximum character limit is reached
    while ((ch = getch()) != '\n' && index < maxChar - 1) {
        if (ch == KEY_BACKSPACE || ch == 127) {  // Handle backspace key
            if (index > 0) {  // If there are characters to delete
                index--;  // Decrease the index to move backward
                x--;  // Move the cursor left
                mvdelch(y, x);  // Delete the character at the current cursor position
            }
        } else if (ch >= 32 && ch <= 126) {  // Only accept printable characters (ASCII 32 to 126)
            tostr[index++] = ch;  // Store the character in the buffer
            mvaddch(y, x++, ch);  // Print the character at the current position and move the cursor right
        }
        move(y, x);  // Move the cursor to the current position
        refresh();  // Refresh the screen to show the updates
    }

    tostr[index] = '\0';  // Null-terminate the string
    return index;  // Return the number of characters entered
}


/**
 * Displays the main screen with available options and file name.
 * Clears the screen and updates the UI with the following:
 * 1. Project Title
 * 2. A set of available commands (Edit, Insert, Delete, Undo, Redo, etc.)
 * 3. The current file name.
 */
void DisplayMetod(void) {
    clear();  // Clear the screen
    move(0, 0);  // Move the cursor to the top-left corner
    printw(PROJECT_TITLE);  // Print the project title
    printw("\n");
    printw("E (edit), I (insert), D (delete), U (undo), R (redo), P (display), S (save), Q (quit)");
    move(cursorY_MINLIMIT - 1, 0);  // Move to the position before the file name
    printw("File: %s\n", file_name);  // Display the current file name
    // display();  // This line could be uncommented if the display function is to be used
    move(cursorY, 0);  // Move the cursor to the current line position
    refresh();  // Refresh the screen to show the updated content
}

int main(int argc, char *argv[]) {
    printf("Hello, World!\n");

    char *fileArg;
    if (argc == 2) {  // If a file is provided as a command-line argument
        fileArg = argv[1];
    } else {  // If no file is provided, use "TEMP.txt" as the default
        fileArg = "TEMP.txt";
    }

    memcpy(file_name, fileArg, strlen(fileArg));  // Copy the file name to global variable

    // Open file (the corresponding function can be called here)
    if (argc == 2) {
        // edit(file_name);
    }

    initscr();  // Initialize the ncurses screen
    cbreak();   // Disable line buffering (input is available immediately)
    noecho();   // Disable echoing of typed characters
    keypad(stdscr, TRUE);  // Enable special keys (e.g., arrow keys)
    curs_set(1);  // Set the cursor to normal visibility

    DisplayMetod();  // Display the initial screen with instructions

    int ch;
    while ((ch = getch()) != 'Q') {  // Main loop, break when 'Q' is pressed
        switch (ch) {
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
            case 'E':  // Edit file
                clear();  // Clear the screen
                move(0, 0);  // Move cursor to the top
                printw("Open File: ");
                
                getStringNcurses(file_name, MAX_FILE_NAME_LENGTH);  // Get file name input from user

                // edit(file_name);  // Uncomment to open and edit the file
                break;

            case 'I':  // Insert text
                char statement[TEXT_BUFFER_STATEMENT_LENGTH];
                getStringNcurses(statement, TEXT_BUFFER_STATEMENT_LENGTH);  // Get the text to insert

                // insert(cursorY, statement);  // Uncomment to insert the statement
                break;

            case 'D':  // Delete line
                // delete(cursorY);  // Uncomment to delete the current line
                break;

            case 'U':  // Undo last action
                // undo();  // Uncomment to undo the last action
                break;
            
            case 'R':  // Redo last undone action
                // redo();  // Uncomment to redo the last undone action
                break;

            case 'S':  // Save file
                clear();  // Clear the screen
                move(0, 0);  // Move cursor to the top
                printw("Save File: ");
                
                getStringNcurses(file_name, MAX_FILE_NAME_LENGTH);  // Get file name to save

                // save(file_name);  // Uncomment to save the file
                break;

            case 'H':  // Display help
                clear();  // Clear the screen
                printw("Press any key to exit the help menu.\n\n");
                printw("- E (edit): Opens the specified file and reads it into the text buffer\n");
                printw("- I (insert): Inserts a line in the text buffer\n");
                printw("- D (delete): Deletes a line\n");
                printw("- U (undo): Reverts the effect of the last command\n");
                printw("- R (redo): Repeats the last undone command\n");
                printw("- P (display): Displays the current text buffer on the screen\n");
                printw("- S (save): Saves the file\n");
                printw("- Q (quit): Exits the editor\n");
                printw("\n");
                printw("- Use arrow keys (Up and Down) to select a line.\n");
                getch();  // Wait for any key to exit help menu
                break;

            case KEY_UP:  // Arrow key Up
                if (cursorY > cursorY_MINLIMIT) {
                    cursorY--;  // Move the cursor up if it's not at the minimum limit
                } else {
                    beep();  // Make a beep sound if the cursor can't move up
                }
                move(cursorY, 0);  // Move the cursor to the new position
                break;

            case KEY_DOWN:  // Arrow key Down
                if (cursorY < cursorY_MAXLIMIT - 1) {
                    cursorY++;  // Move the cursor down if it's not at the maximum limit
                } else {
                    beep();  // Make a beep sound if the cursor can't move down
                }
                move(cursorY, 0);  // Move the cursor to the new position
                break;
        }

        // Refresh the screen to display the updated content
        switch (ch) {
            case 'E':
            case 'I':
            case 'D':
            case 'U':
            case 'R':
            case 'S':
            case 'H':
                DisplayMetod();  // Display the updated screen after each action
                break;
        }

        refresh();  // Refresh the screen to reflect changes
    }

    endwin();  // End the ncurses session and clean up
    
    return 0;
}
