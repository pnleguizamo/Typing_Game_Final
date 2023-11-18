//SE 185: Final Project Template//
/////////////////////////
/* 
Team xx (please insert your team number instead of xx)
Team member 1 "Pablo Leguizamo" | "Percentage of Contribution to The Project"
Team member 2 "Nathan Thomas" | "Percentage of Contribution to The Project"
*/

// gcc FinalProjectTemplate.c -o FinalProjectTemplate -lncurses
// ./FinalProjectTemplate wordList.txt

////////////////////
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

//add more here////

//////////////////////
//Struct Definition//
////////////////////
typedef struct word{
	char contents[WORDLENGTH];
	int xCoordinate;
	int yCoordinate;
	int length;
}word; 


/////////////////////////////////////
//User Defined Functions Prototype//
//List prototypes here and define//
//tehm below the main function////
/////////////////////////////////


int read_words(char* WL[MAXWORDS], char* file_name);
void trimws(char* str);

void draw_character(int x, int y, char use);

void initializeArray(struct word wordGrid[][HORIZONTAL_LIMIT]);

void drawOutline(char guessString[]);
void addWordToScreen(int wordcount, char* wordlist[], struct word wordGrid[][HORIZONTAL_LIMIT]);
int shiftWordsDown(struct word wordGrid[][HORIZONTAL_LIMIT]); 
void removeWordFromScreen(struct word wordGrid[][HORIZONTAL_LIMIT], char guess[]);


int debugger = 0;

int main(int argc, char* argv[]){
	srand(time(NULL));
	char* wordlist[MAXWORDS];
	int wordcount = read_words(wordlist, argv[1]);
	int run = 1;
	int difficulty = 2;


	// Print wordlist for debugging reasons
	// for(int i = 0; i<wordcount; i++)
	// 	printf("%d: %s\n", i, wordlist[i]);
	
	word wordGrid[VERTICAL_LIMIT][HORIZONTAL_LIMIT];
	initializeArray(wordGrid);

	// time(null) is current time, t is a time variable
	clock_t t; 
    t = time(NULL); 
	
	
	char *guessString = (char *)malloc(50 * sizeof(char));
	initscr();
	drawOutline(guessString);
	addWordToScreen(wordcount, wordlist, wordGrid);
	shiftWordsDown(wordGrid);
	refresh();

	

	while(run){
		while((double)t<time(NULL)){
			clear();
			addWordToScreen(wordcount, wordlist, wordGrid);
			if(shiftWordsDown(wordGrid)){
				run = 0;
			}
			// Add the amount of seconds in between each new word appearing
			// t + 1 creates a new word every second
			// t + 2 creates a new word every two seconds
			t++;
		}	
		if(run == 0){
			break;
		}
		drawOutline(guessString);
		getstr(guessString);
		removeWordFromScreen(wordGrid, guessString);
	}
	
	
	// getch();
	fflush(stdout);
    endwin();
	
	return 0;
}

///////////////////////////////////////
//User Defined Functions' Definition//
/////////////////////////////////////

void removeWordFromScreen(struct word wordGrid[][HORIZONTAL_LIMIT], char guess[]){
	
	for(int i = 0; i<HORIZONTAL_LIMIT; i++){
		for(int j = VERTICAL_LIMIT; j>0; j--){
			if(strcmp(guess, wordGrid[j][i].contents)==0){
				strcpy(wordGrid[j][i].contents, "");
				// mvprintw(VERTICAL_LIMIT + 12, 10, "%s", guess);
				refresh();
			}
		}
	}	
}

void initializeArray(struct word wordGrid[][HORIZONTAL_LIMIT]){
	for(int i = 0; i<HORIZONTAL_LIMIT; i++){
		for(int j = VERTICAL_LIMIT; j>0; j--){
			strcpy(wordGrid[j][i].contents, "");
		}
	}	
}

int shiftWordsDown(struct word wordGrid[][HORIZONTAL_LIMIT]){
	
	
	for(int i = 0; i<HORIZONTAL_LIMIT; i++){
		for(int j = VERTICAL_LIMIT; j>0; j--){
			if(strcmp(wordGrid[j][i].contents, "")){
				if(j+1>VERTICAL_LIMIT){
					// mvprintw(VERTICAL_LIMIT + 13, 10,"J: %d, i: %d", j, i);
					// mvprintw(VERTICAL_LIMIT + 14, 10,"%s", wordGrid[j][i].contents);
					// refresh();
					// getch();
					return 1;
				}
				wordGrid[j+1][i] = wordGrid[j][i];
				mvprintw(j, i, "%s", wordGrid[j][i].contents);
				strcpy(wordGrid[j][i].contents, "");
				refresh();
			}
		}
	}
	
	return 0;
}

void addWordToScreen(int wordcount, char* wordlist[], struct word wordGrid[][HORIZONTAL_LIMIT]){
	int index = rand()%wordcount;
	int initialX = (rand()%HORIZONTAL_LIMIT)+1;	
	int length = strlen(wordlist[index]);

	while(initialX+length>HORIZONTAL_LIMIT-1)
		initialX = rand()%HORIZONTAL_LIMIT;
	
	strcpy(wordGrid[1][initialX].contents, wordlist[index]);
	wordGrid[1][initialX].length = strlen(wordGrid[1][initialX].contents);
}

void drawOutline(char guessString[]){
	for(int i = 0; i<=HORIZONTAL_LIMIT; i++)
		draw_character(i, 0, '-');
	for(int i = 1; i<VERTICAL_LIMIT; i++)
		draw_character(HORIZONTAL_LIMIT, i, '|');
	for(int i = HORIZONTAL_LIMIT; i>=0; i--)
		draw_character(i, VERTICAL_LIMIT, '-');
	for(int i = VERTICAL_LIMIT-1; i>0; i--)
		draw_character(0, i, '|');
	mvprintw(VERTICAL_LIMIT + 10, 10, "Type here: ");
	refresh();
	
}

int read_words(char* WL[MAXWORDS], char* file_name)
{
	int numread = 0;
	char line[WORDLENGTH];
	char *p;
	FILE* fp = fopen(file_name, "r");
	while (!feof(fp)) {
		p = fgets(line, WORDLENGTH, fp);
		if (p != NULL) 
		{
			trimws(line);
			WL[numread] = (char *)malloc(strlen(line) + 1);
			strcpy(WL[numread], line);
			numread++;
		}
	}
	fclose(fp);
	return numread;
}

void trimws(char* str) {
	int length = strlen(str);
	int x;
	if (length == 0) return;
	x = length - 1;
	while (isspace(str[x]) && (x >= 0)) {
		str[x] = '\0';
		x -= 1;
	}
}

void draw_character(int x, int y, char use)
{
    mvaddch(y,x,use);
    refresh();
}