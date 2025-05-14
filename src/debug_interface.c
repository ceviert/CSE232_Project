#include <stdio.h>
#include <string.h>

#include "cse232editor.h"

int get_argument(char *com, int n, char type, void *ptr, int limit) {
    int arg_length = 10;
    char arg_buffer[10] = {0}; // arg_buffer'ı sıfırladık.

    // Aranan argümanı bul
    while (*com && n) {
        if (*com == ' ') n--;
        com++;
    }

    if (n) return 0; // Eğer n hala 0 değilse, aranan sıraya ulaşılamadı.

    // Argümanı doldur
    if (type == 'c') {
        *(char*)ptr = *com;
        return (limit > 0);
    } else if (type == 's') {
        while (*com && limit) {
            *(char*)ptr++ = *com++;
            limit--;
        }
        *(char*)ptr = '\0'; // Null-terminate string
        return (!(*com));
    } else if (type == 'd') {
        while (*com && arg_length) {
            arg_buffer[10 - arg_length] = *com++;
            arg_length--;
        }
        *(int*)ptr = atoi(arg_buffer);
        return (!(*com));
    } else {
        return 0; // Desteklenmeyen tür
    }
}

void dump_chain(int head, const char *label) {
    printf("\n--- %s Chain Dump ---\n", label);
    printf("%-5s %-5s %-5s %-40s\n", "Idx", "Prev", "Next", "Statement");
    printf("---------------------------------------------------------------\n");

    int current = head;

    while (current != NULL_LINE_TERMINATOR) {
        struct node *n = &textbuffer[current];
        printf("%-5d %-5d %-5d %-40s\n", current, n->prev, n->next, n->statement);
        current = n->next;
    }
}

void dump_list(const char *label) {
    printf("\n--- %s Chain Dump ---\n", label);
    printf("%-5s |%-5s |%-5s |%-40s\n", "Idx", "Prev", "Next", "Statement");
    printf("---------------------------------------------------------------\n");

    for (int i=0; i<TEXT_BUFFER_DEPTH ; i++) {
        struct node *n = &textbuffer[i];
        if (strlen(n->statement))
            printf("%-5d |%-5d |%-5d |%-40s\r", i, n->prev, n->next, n->statement);
        else
            printf("%-5d |%-5d |%-5d |\n", i, n->prev, n->next);
    }
}
