#include<stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <math.h>
#include <ctype.h>
#include <time.h>

#define MAXWORDS 1000
#define WORDLENGTH 20
#define HORIZONTAL_LIMIT 100
#define VERTICAL_LIMIT 20


// Placeholder for the draw_character function
void draw_character(int x, int y, char character) {
    mvprintw(y, x, "%c", character);
}

void drawOutline(char guessString[]) {
    // Your existing code for drawing the outline

    // Print the guessString
    mvprintw(VERTICAL_LIMIT + 10, 22, "%s", guessString);

    refresh();
}

int main() {
    initscr();
    cbreak();  // Disable line buffering
    echo();    // Enable echoing of characters
    keypad(stdscr, TRUE);  // Enable special keys

    // Example usage of drawOutline with an empty string
    char guessString[50] = "";  // Replace this with your actual data or input

    drawOutline(guessString);

    // Use getstr() to get user input
    move(VERTICAL_LIMIT + 10, 34);  // Move cursor to the position for user input
    getstr(guessString);

    // Wait for a key press before exiting
    getch();

    endwin();  // End the ncurses library

    printf("You entered: %s\n", guessString);

    return 0;
}




