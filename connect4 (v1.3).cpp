/*****************************************************************
By Fletcher van Ameringen on a Friday during the Coronavirus lockdown!

This is a basic CMD terminal game that functions fundamentally 
in a similar manner to the game 'Connect Four':
1. A general game grid is displayed 
2. There are two players
3. Each player has a different token (player 1 has token 'O', while player 2 has token 'X')
4. Player 1 has their turn, player 2 has their turn, player 1 has their turn etc., until one player wins 
5. A turn is taken by placing a player's token in a 'slot' on the game grid
6. The first player to have four tokens in-a-row horizontally, vertically, or diagonally, wins.

Game input:
- Game grid slots are selected by typing a number from the grid and then <enter>
- A game is restarted by typing lowercase 'y' to restart, or 'n' to exit when prompted

Limitations:
- Basic data validation has been employed 
(will tolerate if a wrong character(s) are entered up to a limit of 10 characters)
- A grid larger than 30 x 30 is not recommended
I recognize that this program does not utilize dynamic memory or a linked list for the game grid. 
I chose not to do this because I think it would be computationally ineffecient to do so, as several 
list traversals would need to be performed after each turn. 
The most vital aspect of this game grid is quick insertion and access, which a 2d array provides.

Additional functionality:
- The game will 'store' the past 'MAX_SAVES' played games (the program will then begin to overwrite 'saves')
- In principal, the 'saves' function uses

Enjoy!
******************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEIGHT 6
#define WIDTH 7
#define MAX_SAVES 5 // should not be less than 2

class plot {
	private:
		char graph[HEIGHT][WIDTH];
		int columnHeight[WIDTH];
	public:
		void makeGuess(int user);
		void printPlot(); // print out the row
		void addGuess(int guessLocation, int user); // add selection at the selected 'place'
		bool checkIfWon(int user); // checks entire graph to see if there is a combination of four in a row (either user)
		bool diagonalCheck(int user);
		bool verticalCheck(int user);
		bool horizontalCheck(int user);
		void resetPlot();
		plot();
		~plot() {}
}; 

plot graphs[MAX_SAVES]; // This program will store the record of MAX_SAVES games (for the duration of the executable)
int wins[MAX_SAVES]; // Which user won each game
int GC = 0; // Game counter (index of last 'save')
int startingGC = 0; // The start index of the circular array
int counter = 1; // Actual number of games played 

plot::plot() {
	int i, j;
	
	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			graph[i][j] = ' ';
		}
	}
	for (i = 0; i < WIDTH; i++) {
		columnHeight[i] = 0;
	}		
	for (i = 0; i < MAX_SAVES; i++) {
		wins[i] = 0;
	}
}
void plot::resetPlot() {
	int i, j;
	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			graph[i][j] = ' ';
		}
	}
	for (i = 0; i < WIDTH; i++) {
		columnHeight[i] = 0;
	}	
}
void plot::printPlot() {
	// Print the game grid
	printf("\n\n");
	int i, j;
	for (i = HEIGHT-1; i >= 0; i--) {
		printf("\t");
		for (j = 0; j < WIDTH; j++) {
			if (j >= 9) {
				printf("| %c", graph[i][j]);
			} else {
				printf("|%c", graph[i][j]);				
			}
			if (j == WIDTH-1) {
				printf("|\n");
			}
		}
	}
	// Print additional features
	printf("\t");
	for (i = 0; i < WIDTH; i++) {
		if (i >= 9) {
			printf("-**");
		} else {
			printf("-*");
		}
		if (i == WIDTH-1) {
			printf("-\n");
		}
	}
	printf("\t");
	for (i = 0; i < WIDTH; i++) {
		printf("-%d", i+1);
		if (i == WIDTH-1) {
			printf("-\n\n");
		}
	}
}
void plot::makeGuess(int user) {
	char choice_str[10], sym = 'X';
	int choice;
	
	if (user == 1) {
		sym = 'O';
	} else {
		sym = 'X';
	}
	printf(">>Player %d's guess (%c): ", user, sym);
	fgets(choice_str, 10, stdin);
	choice_str[strlen(choice_str)-1] = '\0';
	sscanf(choice_str, "%d", &choice);		
	// Data validation
	if  ((choice >= 1) && (choice <= WIDTH)) {
		graphs[GC].addGuess(choice, user);
	} else {
		while ((choice < 1) || (choice > WIDTH)) {
			printf("Guess not valid: ");
			fgets(choice_str, 10, stdin);
			choice_str[strlen(choice_str)-1] = '\0';
			sscanf(choice_str, "%d", &choice);
		}
		graphs[GC].addGuess(choice, user);
	}
}
void plot::addGuess(int guessLocation, int user) {
	// Add selection/guess to game grid
	guessLocation -= 1; 
	int height = columnHeight[guessLocation];
	if (height == HEIGHT) {
		printf("Column is full, please try another.\n");
		makeGuess(user);
	}
	if (user == 1) {
		graph[height][guessLocation] = 'O';
		columnHeight[guessLocation] += 1;
		
	} else if (user == 2) {
		graph[height][guessLocation] = 'X';
		columnHeight[guessLocation] += 1;
	}
}
bool plot::checkIfWon(int user) { // Run four-in-a-row checks on game grid
	if ((diagonalCheck(user)) || (horizontalCheck(user)) || (verticalCheck(user))) {
		return true;
	}
	return false;
}
bool plot::horizontalCheck(int user) { // Check horizontally for four-in-a-row
	char target = ' ';
	int i, j, count;
	if (user == 1) {
		target = 'O';
	} else if (user == 2) {
		target = 'X';
	}
	for (i = 0; i < HEIGHT; i++) { // each row
		count = 0;
		for (j = 0; j < WIDTH; j++) { //each column
			if (graph[i][j] == target) {
				count++;
			} else {
				count = 0;
			}
			if (count == 4) {
				return true;
			}
		}
	}
	return false;
}
bool plot::verticalCheck(int user) { // Check vertically for four-in-a-row
	char target = ' ';
	int col, row, count;
	if (user == 1) {
		target = 'O';
	} else if (user == 2) {
		target = 'X';
	}
	
	for (col = 0; col < WIDTH; col++) { // each columns
		count = 0;
		for (row = 0; row < HEIGHT; row++) { // each row
			if (graph[row][col] == target) {
				count++;
			} else {
				count = 0;
			}
			if (count == 4) {
				return true;
			}
		}
	}
	return false;
}
bool plot::diagonalCheck(int user) { // Check diagonally for four-in-a-row
	char target = ' ';
	int col, row, count;

	if (user == 1) {
		target = 'O';
	} else if (user == 2) {
		target = 'X';
	}
	
	// there are two directions that a diagonal four-in-a-row could go: 'upwards' or 'downwards'
	// 1. checking for downwards direction: 	
	int sCol, sRow;
	
	for (col = 0; col < WIDTH; col++) { // each column
		for (row = 0; (row < HEIGHT); row++) { // each row
			count = 0;
			if (graph[row][col] == target) {
				sCol = col;
				for (sRow = row; sRow < HEIGHT; sRow++) {
					if (graph[sRow][sCol] == target) {
						count++;
						sCol++; // move onto the next column
					} 
					if (count == 4) {
						return true;
					}
				}
			}
		}
	}
	
	// 2. checking for upwards direction: 	
	for (col = 0; col < WIDTH; col++) { // each column
		for (row = 0; (row < HEIGHT); row++) { // each row
			count = 0;
			if (graph[row][col] == target) {
				sCol = col;	
				for (sRow = row; (sRow < HEIGHT) && (sCol >= 0); sRow++) {
					if (graph[sRow][sCol] == target) {
						count++;
						sCol--; // move onto the next column
					} 
					if (count == 4) {
						return true;
					}
				}	
			}
		}
	}	
	return false;
}

void printVictories() { // Final printout of game 'saves'
	int i, temp = startingGC;
	for (i = 0; i < MAX_SAVES; i++) {
	    if (temp != GC) {
	        if (temp == MAX_SAVES) {
	            temp = 0;
	        }
    		graphs[temp].printPlot();
    		printf("Game %d: ", i+1);
    		if (wins[temp] == 1) {
    			printf("(Player 1 won this game)\n\n");
    		} else if (wins[temp] == 2) {
    			printf("(Player 2 won this game)\n\n");				
    		}
		    temp++; 
	    } else {
			// print out final game
	     	graphs[GC].printPlot();
    		printf("Game %d: ", i+1);
    		if (wins[GC] == 1) {
    			printf("(Player 1 won this game)\n\n");
    		} else if (wins[GC] == 2) {
    			printf("(Player 2 won this game)\n\n");				
    		}
	        break;
	    }
	}
}

int main() {
	char exitOrNot[10] = "y", seeVic[10];
	bool GameOver, won1, won2;
	bool movingStart = false;
	
	printf("---------------------------------\n");
	printf("------Welcome to Connect 4!------\n");
	printf("---------------------------------\n");
	printf("This is simply a game of PvP connect-4!\n\n");
	
	while (strcmp(exitOrNot, "y") == 0) {
		graphs[GC].resetPlot();
		GameOver = false;
		won1 = false;
		won2 = false;
		printf("Game %d\n", counter);

		while (!GameOver) {
			// Print out graph first time
			graphs[GC].printPlot();
			
			// First user guesses
			graphs[GC].makeGuess(1);
			// Check each turn to see if user one has won
			won1 = graphs[GC].checkIfWon(1);
			if (won1) {
				printf("Congratulations player 1! You win!\n");
				graphs[GC].printPlot();
				wins[GC] = 1;
				GameOver = true;
				continue;
			}			
			// Second user guesses
			graphs[GC].printPlot();
			graphs[GC].makeGuess(2);
			// Check each turn to see if user two has won
			won2 = graphs[GC].checkIfWon(2);
			if (won2) {
				printf("Congratulations player 2! You win!\n");
				graphs[GC].printPlot();
				wins[GC] = 2;
				GameOver = true;
				continue;
			}	
		}	
		
		printf("Would you like to play again (n\\y): ");
		fgets(exitOrNot, 10, stdin);
		exitOrNot[strlen(exitOrNot)-1] = '\0';
		// Validate input
    	while ((strcmp(exitOrNot, "y") != 0) && (strcmp(exitOrNot, "n") != 0)) {
    		printf("Input not valid: ");
    		fgets(exitOrNot, 10, stdin);
    		exitOrNot[strlen(exitOrNot)-1] = '\0';	
    	}

		if (strcmp(exitOrNot, "y") == 0) {
			// User has chosen to play again
			GC++;
			counter++; 
			if (GC == MAX_SAVES) {
				GC = 0;
				movingStart = true;
			}
			if (movingStart) {
			  	startingGC++;
			}
			if (startingGC == MAX_SAVES) {
				startingGC = 0;
			}
		}
	}
	
	if (counter > 1 ) {
    	printf("Would you see the games you have played before you leave (n\\y): ");
    	fgets(seeVic, 10, stdin);
    	seeVic[strlen(seeVic)-1] = '\0';	
    	while ((strcmp(seeVic, "y") != 0) && (strcmp(seeVic, "n") != 0)) {
    		printf("Input not valid: ");
    		fgets(seeVic, 10, stdin);
    		seeVic[strlen(seeVic)-1] = '\0';	
    	}
    	if  (strcmp(seeVic, "y") == 0) {
    		printVictories();
    	}
	}
	printf("Thank you for playing!\n");
	exit(0);
}