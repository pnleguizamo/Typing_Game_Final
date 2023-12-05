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
#include <unistd.h>
// #include <linux/time.h>
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
	int length;
}word; 


/////////////////////////////////////
//User Defined Functions Prototype//
//List prototypes here and define//
//tehm below the main function////
/////////////////////////////////

void appendWords(char* file_name);
int read_words(char* WL[MAXWORDS], char* file_name);
void trimws(char* str);

void draw_character(int x, int y, char use);

void initializeArray(struct word wordGrid[][HORIZONTAL_LIMIT]);

void drawOutline(char guessString[]);
void addWordToScreen(int wordcount, char* wordlist[], struct word wordGrid[][HORIZONTAL_LIMIT]);
int shiftWordsDown(struct word wordGrid[][HORIZONTAL_LIMIT]); 
void removeWordFromGrid(struct word wordGrid[][HORIZONTAL_LIMIT], char guess[]);

long long findTime(struct timespec *timeStruct);

int debugger = 0;

int main(int argc, char* argv[]){
	srand(time(NULL));
	char* wordlist[MAXWORDS];
	appendWords(argv[1]);
	int wordcount = read_words(wordlist, argv[1]);

	// Print wordlist for debugging reasons
	// for(int i = 0; i<wordcount; i++)
	// 	printf("%d: %s\n", i, wordlist[i]);

	char *guessString = (char *)malloc(50 * sizeof(char));
	
	int run = 1;
	int difficulty = 1000;	

	word wordGrid[VERTICAL_LIMIT][HORIZONTAL_LIMIT];
	initializeArray(wordGrid);

	struct timespec realTime, beginTime; 
	long long t;

	// Set up screen for the first frame
	initscr();
	drawOutline(guessString);
	// Pause the program until user hits a key
	mvprintw(VERTICAL_LIMIT+12, 40, "Hit any key to begin");
	getch();
	

	clear();
	addWordToScreen(wordcount, wordlist, wordGrid);
	shiftWordsDown(wordGrid);
	refresh();
	
	// Save the time that the program began, initialize the realTime timer, and assign a second value to the t variable
	clock_gettime(CLOCK_REALTIME, &beginTime);
	clock_gettime(CLOCK_REALTIME, &realTime);
	t = findTime(&realTime);


	while(run){
		// Each new word created adds to the t variable
		// As long as t < the current real time, more words must be added
		while(t<findTime(&realTime)){
			clear();
			addWordToScreen(wordcount, wordlist, wordGrid);
			if(shiftWordsDown(wordGrid))
				run = 0;
			t+=difficulty;
			// mvprintw(VERTICAL_LIMIT+12, 12, "%d", difficulty);
		}	
		if(run == 0)
			break;
		// difficulty variable is decreased by 5% every time the user guesses
		// By adding less difficulty to the t variable, new words appear more often and the game gets harder
		// difficulty has a hard minimum of 400ms, so the fastest words can appear is 1 every 400ms
		difficulty = difficulty > 400 ? difficulty*0.95 : difficulty;
		
		drawOutline(guessString);
		getstr(guessString);
		removeWordFromGrid(wordGrid, guessString);
	}
	
	
	// getch();
	fflush(stdout);
    endwin();

	// Calculate how much time the game took by subtracting (currentTime - beginningTime)
	long long secondsTaken = (findTime(&realTime)/1000) - beginTime.tv_sec;
	printf("\nThis game took %lld seconds\n", secondsTaken);

	free(guessString);
	return 0;
}

///////////////////////////////////////
//User Defined Functions' Definition//
/////////////////////////////////////

// Ask the user if they want to append to the word text file
void appendWords(char* file_name){
	int edit = 0;
	char newWord[50];

	printf("Type 1 to append to the word list or type 0 to play: ");
	scanf("%d", &edit);
	FILE* fp = fopen(file_name, "a");

	while(edit){
		printf("Enter a new word for the list: ");
		scanf("%s", newWord);
		fputs("\n", fp);
		fputs(newWord, fp);
		printf("Type 1 to append to the word list or type 0 to play: ");
		scanf("%d", &edit);
	}

	fclose(fp);
	
}

// Pass a timespec object pointer to findTime() to refresh the real time and return a long long with a millisecond value
long long findTime(struct timespec *timeStruct){
	clock_gettime(CLOCK_REALTIME, timeStruct);
	return (timeStruct->tv_sec) * 1000 + (timeStruct->tv_nsec) / 1000000;
}

// Pass the 2D array and the guess string to loop through the array and check if the guess string exists in it
// If the string exists, remove it from the array and let shiftWordsDown() handle removing it from the screen
void removeWordFromGrid(struct word wordGrid[][HORIZONTAL_LIMIT], char guess[]){
	for(int i = 0; i<HORIZONTAL_LIMIT; i++){
		for(int j = VERTICAL_LIMIT-1; j>0; j--){
			if(strcmp(guess, wordGrid[j][i].contents)==0){
				strcpy(wordGrid[j][i].contents, "");
				// mvprintw(VERTICAL_LIMIT + 12, 10, "%s", guess);
				refresh();
			}
		}
	}	
}

// Nullify everything in the 2D array because C is dumb and sometimes things exist in there otherwise
void initializeArray(struct word wordGrid[][HORIZONTAL_LIMIT]){
	for(int i = 0; i<HORIZONTAL_LIMIT; i++){
		for(int j = VERTICAL_LIMIT-1; j>0; j--){
			strcpy(wordGrid[j][i].contents, "");
		}
	}	
}

// Loop through the 2D array. If there is a word object at the index, shift that object down the array 1 row, and print it to the screen
// If a word object ever reaches the bottom of the 2D array, return 1 to kill the main while loop
int shiftWordsDown(struct word wordGrid[][HORIZONTAL_LIMIT]){
	for(int i = 0; i<HORIZONTAL_LIMIT; i++){
		for(int j = VERTICAL_LIMIT-1; j>0; j--){
			if(strcmp(wordGrid[j][i].contents, "")){
				if(j+1>=VERTICAL_LIMIT){
					// mvprintw(VERTICAL_LIMIT + 13, 10,"J: %d, i: %d", j, i);
					// mvprintw(VERTICAL_LIMIT + 14, 10,"%s", wordGrid[j][i].contents);
					// refresh();
					// getch();
					return 1;
				}
				if(j+1<VERTICAL_LIMIT)
					wordGrid[j+1][i] = wordGrid[j][i];
				mvprintw(j, i, "%s", wordGrid[j][i].contents);
				strcpy(wordGrid[j][i].contents, "");
				refresh();
			}
		}
	}
	return 0;
}

// Determine a random place at the top of the screen to place a random word from the wordlist
// Take into account the length of the string when placing it, so that it's in bounds
// Copy the word into the 2D array at the appropriate index
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