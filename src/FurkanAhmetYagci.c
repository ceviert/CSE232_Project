#include <stdio.h>
#include <curses.h>
#include "cse232editor.h"

// display(): textbuffer[]'daki kullanılmakta olan satırları ekrana yazdırır, silinmiş satırları göstermez.
void display(void) {
    int line = inuse_head; // inuse_head'den başla
    int row = 0; // Ekranda yazmaya 4. satırdan başla (main.c'deki cursorY_MINLIMIT ile uyumlu)
    
    // Kullanımdaki tüm satırları sırayla ekrana yazdır
    while (line != NULL_LINE_TERMINATOR) {
        move(row, 0); // İmleci ilgili satıra taşı
        clrtoeol(); // Satırı temizle (önceki içerik varsa silinsin)
        printw("%s", textbuffer[line].statement); // Satırı ekrana yazdır
        line = textbuffer[line].next; // Sonraki satıra geç
        row++; // Ekranda bir alt satıra in
    }
    // Kalan satırları temizle (önceki uzun metinler silinsin diye)
    for (; row < TEXT_BUFFER_DEPTH + 4; row++) {
        move(row, 0);
        clrtoeol();
    }
    refresh(); // Ekranı güncelle
}
