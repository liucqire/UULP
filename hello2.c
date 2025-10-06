#include <stdio.h>
#include <curses.h>

int main() {
    int i;
    initscr();
    clear();
    for (int i = 0; i < LINES; i++) {
        move(i, i * 5);
        if (i % 2 == 1)
            standout();
        addstr("Hello, world");
        if (i % 2 == 1)
            standend();
    } 
    refresh();
    getch();
    endwin();
}
