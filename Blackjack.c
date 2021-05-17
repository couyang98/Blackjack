//Chongjie Ouyang

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void beginning(int gameRecord[]);
int playing(int gameRecord[]);
int dealing();
int checkBlackjack(int playerHand[], int dealerHand[], int gameRecord[], int bet);
int continueGame(int gameRecord[]);
int displayPlayerBust(int playerHand[], int numCards);
void displayDealer(int dealerHand[], int numDealerCards, int hole);
int dealerTurn(int dealerHand[], int numDealerCards);
void checkHands(int playerHand[], int dealerHand[], int gameRecord[], int bet, int numPlayerCards, int numDealerCards);
void ending(int gameRecord[]);
//The lowest possible bet by the player is a constant 10
#define MINIMUM_BET 10

//main function of the program
int main(void){
	//Seeding random number generator
	srand((unsigned int)(time(NULL)));
	
	//declare an array to store the information of the game(money, win, lose, blackjack, bust)
	int gameRecord[5] = {0};
	
	//Check to set up new game or continue existing game
	beginning(gameRecord);
	
	//While playing returns 1, continue to play the game
	while(playing(gameRecord) == 1){
	}

	//Finalizing the game
	ending(gameRecord);

	//Ending the program
	return 0;
}

//Initializes the game
void beginning(int gameRecord[]){
	//Initializing variables for game to begin
	int newGame = 1, bankRoll = 0, i, gameRecord1, gameRecord2, gameRecord3, gameRecord4;

	//Open already existing game if there is one
	FILE *previousGame = fopen("previousGame.txt", "r");

	//If "previousGame.txt" doesnt exist then make new game
	if(previousGame == '\0'){
		printf("New Game\n");
		
		//Set all values of 'gameRecord' to 0 for new game
		i = 0;
		while(i <= 4){
			gameRecord[i] = 0;
			i++;
		}
		//start the current bankroll which is in 'gameRecord[0] to 1000
		gameRecord[0] = 1000;
		//Finish initialzing the game
		return;
	}
	else {
		//If game already exist then scan "previousGame.txt" for values of bankroll, # wins, # loses, # blackjacks, and # busts
		//Set values found in "previousGame.txt" to its holders
		fscanf(previousGame, "%d %d %d %d %d", &bankRoll, &gameRecord1, &gameRecord2, &gameRecord3, &gameRecord4);
		
		//Closing "previousGame.txt"
		fclose(previousGame);
		
		//Set the values of gameRecord array to respective holders
		gameRecord[1] = gameRecord1;
		gameRecord[2] = gameRecord2;
		gameRecord[3] = gameRecord3;
		gameRecord[4] = gameRecord4;
		
		//Check if data to see if new game, if it is not a new game, then set newGame to 0 (false)
		i = 1;
		while(i <= 4){
			if(gameRecord[i] != 0){
				newGame = 0;
			}
			i++;
		}

		//Check if bankroll is below the minimum possible bet or if it is already a new game
		if((bankRoll < MINIMUM_BET) || ((bankRoll == 1000) && (newGame == 1))){
			printf("New Game\n");
			
			//Set all values of 'gameRecord' back to 0
			i = 0;
			while(i <= 4){
				gameRecord[i] = 0;
				i++;
			}

			//Set the bankroll back to 1000 for new game
			gameRecord[0] = 1000;
			//Finish initializing the game
			return;
		}
		else {
			//If continual game, play game using values found in "previousGame.txt"
			//Display "Continual Game" to remind user this is an already existing game
			printf("Continual Game\n");
			gameRecord[0] = bankRoll;
			//Finish initializing the existing game
			return;
		}
	}
}

//Playing the game
int playing(int gameRecord[]){
	//Initializing variables that will be used in game
	int playerHand[11] = {0}, dealerHand[11] = {0}, hole = 1, numPlayerCards = 0, numDealerCards = 0, bet, turn = 0, i, bust;
	char input;

	//Prompt and scan for player's bet - check if bet is above what is in bankRoll or below the minimum allowed bet
	printf("Start Game!\nCurrent Bankroll: %d\nPlace a bet! Maximum is %d, Minimum is %d\n", gameRecord[0], gameRecord[0], MINIMUM_BET);
	scanf(" %d", &bet);
	//If the input is above bankRoll and under minimum allowed bet - End the game
	if((bet > gameRecord[0]) || (bet < MINIMUM_BET)){
		printf("Invalid input! Game ending.\n");
		return 0;
	}
	printf("-Opening Deal-\n");

	//Deal first two cards
	for(i = 0; i < 2; i++){
		playerHand[i] = dealing();
		dealerHand[i] = dealing();
		numPlayerCards++;
		numDealerCards++;
	}

	//Display the player's hand and checks if player busted - if bust then incriment bust and subtracts the bet from bankRoll
	//Ask if the user wants to continue game and prints stats from gameRecord
	bust = displayPlayerBust(playerHand, numPlayerCards); //////////////////////////////////////////////////////////////////////
	if(bust == 2){
		return 0;}
	if(bust == 1){
		gameRecord[4] += 1;
		gameRecord[0] -= bet;
		printf("\nBankroll: %d\nTotal Wins: %d\nTotal Loses: %d\nTotal Blackjacks: %d\nTotal Busts: %d\n", gameRecord[0], gameRecord[1], gameRecord[2], gameRecord[3], gameRecord[4]);
		//If player chooses to play another round - return 1
		if(continueGame(gameRecord) == 1){
			return 1;
		}
		//Else end the game - return 0
		else {
			return 0;
		}
	}
	
	//Display dealer hand
	displayDealer(dealerHand, numDealerCards, hole);

	//Checks if player gets blackjack
	//Ask to continue game if true (1) is returned
	//Display game stats
	if(checkBlackjack(playerHand, dealerHand, gameRecord, bet) == 1){
		printf("\nBankroll: %d\nTotal Wins: %d\nTotal Loses: %d\nTotal Blackjacks: %d\nTotal Busts: %d\n", gameRecord[0], gameRecord[1], gameRecord[2], gameRecord[3], gameRecord[4]);
		if(continueGame(gameRecord) == 1){
			return 1;
		} else {
			return 0;
		}
	}

	//Initialize i = 2 because two cards are already dealt to player - program will iterate depending on how many cards player gets
	i = 2;
	
	//While it is player's turn, check if player wants a hit or stand
	while(turn == 0){
		//Prompt and scan if player wants to hit('y' or 'Y') or stand('n' or 'N')
		printf("\nHit? <Y, N>\n");
		scanf(" %c", &input);

		//Switch statement for input of hit or stand
		switch(input){
			//If input is to hit, deal another card to player hand, display player hand and check if bust
			case 'Y':
			case 'y':
				playerHand[i] = dealing(); 
				numPlayerCards++;
				bust = displayPlayerBust(playerHand, numPlayerCards);
				if(bust == 2){
					return 0;
				}
				if(bust == 1){
					//Increment busts and subtracts bet from bankroll
					gameRecord[4] += 1;
					gameRecord[0] -= bet;

					//Prints current stats and prompts if player wants to continue game
					printf("\nBankroll: %d\nTotal Wins: %d\nTotal Loses: %d\nTotal Blackjacks: %d\nTotal Busts: %d\n", gameRecord[0], gameRecord[1], gameRecord[2], gameRecord[3], gameRecord[4]);
					if(continueGame(gameRecord) == 1){
						return 1;
					} else {
						return 0;
					}
				}
				break;
			//If input is to stand, then its is dealer's turn
			case 'N':
			case 'n':
				//Hole is set to false to display dealer's hole card
				hole = 0;

				//End player's turn by switching turn
				turn = 1;

				//If dealer busts, dealerTurn returns 1 - increment win and add the bet to bankRoll
				if(dealerTurn(dealerHand, numDealerCards) == 1){
					gameRecord[1]++;
					gameRecord[0] += bet;
					

					//Prints current stats and prompt if player wants to continue game
					printf("\nBankroll: %d\nTotal Wins: %d\nTotal Loses: %d\nTotal Blackjacks: %d\nTotal Busts: %d\n", gameRecord[0], gameRecord[1], gameRecord[2], gameRecord[3], gameRecord[4]);
					if(continueGame(gameRecord) == 1){
						return 1;
					} else {
						return 0;
					}
				}
				break;
			//If input is not yes or no, then inform the player input is invalid and end game
			default:
				printf("Invalid answer - game ending\n");
				return 0;
		}
		//Increments i for each card in player hand
		i++;
	}


	//If neither players have busted, after the dealer ends turn - check the two hands to determine winner
	checkHands(playerHand, dealerHand, gameRecord, bet, numPlayerCards, numDealerCards);
	
	//Prints current stats after each game
	printf("\nBankroll: %d\nTotal Wins: %d\nTotal Loses: %d\nTotal Blackjacks: %d\nTotal Busts: %d\n", gameRecord[0], gameRecord[1], gameRecord[2], gameRecord[3], gameRecord[4]);

	//Prompts the player if they want to continue game
	if(continueGame(gameRecord) == 1){
		return 1;
	} else {
		return 0;
	}
}

//Returns a random card with points from [1, 10]
int dealing(){
	//Generates random card value between [1, 13]
	long int cardValue = ((rand() % 13) + 1);

	//Card value 11, 12, 13 are considered 10 points
	//All other card value have face value points
	if(cardValue >= 11 && cardValue <= 13){
		return 10;
	} else {
		return cardValue;
	}
}

//Checks if either player or dealer has a blackjack and see if there is a winner
int checkBlackjack(int playerHand[], int dealerHand[], int gameRecord[], int bet){
	//Initializing variables
	int playerPoint = 0, dealerPoint = 0, i;

	//Calculating the points of the two hands separately
	for(i = 0; i <= 10; i++){
		playerPoint += playerHand[i];
		dealerPoint += dealerHand[i];
	}

	//If both players have blackjack - then the outcome is a draw
	if(playerPoint == 21 && dealerPoint == 21){
		printf("Both have Blackjack! Draw!\n");
		return 1;
		
	//If player has blackjack but dealer does not, increment blackjack, and add (1.5 * bet) to bankroll
	} else if(playerPoint == 21){
		printf("Player has blackjack! Player Wins!\n");
		gameRecord[3]++;
		gameRecord[0] += (bet * 1.5);
		return 1;

	//If both player does not have blackjack, return false
	} else {
		return 0;
	}
}

//Ask if player want to continue the game
//Returns true or false based on player input
int continueGame(int gameRecord[]){
	//Initialize variable
	char input;
	
	//Check if bankRoll is below minimum bet
	//End game if bankRoll is below minimum
	if(gameRecord[0] < 10){
		printf("You ran out of money! Game ending.\n");
		return 0;
	}

	//Prompt if player wants to continue game and scan for input
	printf("Continue Game? <Y, N>\n");
	scanf(" %c", &input);

	//Switch statement to return true or false accordingly
	switch(input){
		//If input Y, then true, which continues game
		case 'Y':
		case 'y':
			return 1;
			break;

		//If input N, then false, which ends game
		case 'N':
		case 'n':
			return 0;
			break;

		//If input is unrecognizible then end game
		default:
			printf("Invalid input! Game ending.\n");
			return 0;
			break;
	}
}


//Display player's hand and check if they bust
int displayPlayerBust(int playerHand[], int numCards){
	//Initializing variables
	int points = 0, i, tempPoint = 0;
	printf("\nPlayer Hand: ");

	//Display the cards dealt to player
	for(i = 0; i <= (numCards - 1); i++){		
		printf("%d ", playerHand[i]);
	}

	//Loops through player hand and checks for aces, player chooses if value is 1 or 11
	//Allows player to choose value of their aces
	for(i = 0; i <= (numCards - 1); i++){
		//Ask how player wants to define their ace cards
		if(playerHand[i] == 1 || playerHand[i] == 11){
			//Prompts player on what point they want to give their Ace card
			printf("\nAce Card for card %d - Set point 1 or 11? \n", (i + 1));
			scanf(" %d", &tempPoint);
			if(tempPoint == 1 || tempPoint == 11){
				playerHand[i] = tempPoint;
			//If player gives input other than 1 or 11 - end game
			} else {
				printf("Invalid input! Game ending.");
				return 2;
			}
		}
		//Calculates points with updated ace points
		points += playerHand[i];
	}

	printf("\nTotal: %d\n", points);
	
	//If points of player hand is greater than 21, then player is busted! - Return true for bust
	if(points > 21){
		printf("\nTotal greater than 21! Bust!\n");
		return 1;
	} else {
		return 0;
	}
}

//Display cards on dealer's hand with the hole card
void displayDealer(int dealerHand[], int numDealerCards, int hole){
	//Initializing variables
	int i, points = 0;
	printf("\nDealer Hand: ");

	//Display dealer's hand and their hole card
	for(i = 0; i <= (numDealerCards - 1); i++){
		if(i == 1 && hole == 1){
			printf("hole ");
		} else {
			printf("%d ", dealerHand[i]);
		}
		points += dealerHand[i];
	}

	//If dealer still has a hole card display "??"
	if(hole == 1){
		printf("\nTotal: ??\n");
	} else {
		printf("\nTotal: %d\n", points);
	}
}

//Dealer's turn
int dealerTurn(int dealerHand[], int numDealerCards){
	//Initialize variable
	//j is how many cards already in dealer's hand
	int i, j = 2;
	
	//Dealer will keep hitting until they have at least 17 points in their hand
	while(1){
		//Initialize dealerPoint back to 0 for beginning of a new round
		int dealerPoint = 0;

		//Sum up all the card in dealer's hand
		for(i = 0; i <= 10; i++){
			dealerPoint += dealerHand[i];
		}

		//If dealer has an Ace card
		//Ace card is 11 if dealer total point without Ace is less than 10
		for(i = 0; i <= 10; i++){
			if(dealerHand[i] == 1 && ((dealerPoint - 10) >= 0)){
				dealerHand[i] = 11;
				dealerPoint += 10;
			}
		}

		//Display dealer hand
		displayDealer(dealerHand, numDealerCards, 0);

		//Check if dealer shuld stand, hit, or is busted
		if(dealerPoint >= 17 && dealerPoint <= 21){ 
			return 0;
		} else if(dealerPoint< 17){
			dealerHand[j] += dealing();
			numDealerCards++; 
		} else {
			printf("\nDealer is Busted! Player Wins!\n");
			return 1;
		}

		//Iterating value of j
		j++;
	}
}

//If neither players are busted - check hand for winner
void checkHands(int playerHand[], int dealerHand[], int gameRecord[], int bet, int numPlayerCards, int numDealerCards){
	//Initializing variables
	int playerPoint = 0, dealerPoint = 0, i;

	//Iterating through arrays to find total points of all cards
	//Find the point of player's cards and dealer's cards - compare total card point
	for(i = 0; i <= 10; i++){
		playerPoint += playerHand[i];
		dealerPoint += dealerHand[i];
	}

	//Compare player's total point with dealer's total point to find winner
	//If both have same points then it is draw
	if(playerPoint == dealerPoint){
		if(playerPoint == 21){
			//If dealer wins while both have 21 then increment a loss and substract bet from bankRoll
			if(numPlayerCards > numDealerCards){	
				printf("\nTie at 21, Dealer wins with less cards!\n");
				gameRecord[2]++;
				gameRecord[0] -= bet;
			}//If player wins while both have 21 then increment a win and add bet to bankRoll
			else if (numDealerCards > numDealerCards){
				printf("\nTie at 21, Player wins with less cards!\n");
				gameRecord[1]++;
				gameRecord[0] += bet;
			}
			//If both have 21 with same amount of card then it is a draw
			else {
				printf("\nHands are equal! Draw!\n");
			}
		} 
		//If both side have same point that this not 21 then it is a draw
		else {
			printf("\nHands are equal! Draw!\n");
		}
	} 
	//If dealer has higher points - subtract bet from bankRoll and increment a loss
	else if(dealerPoint > playerPoint){
		printf("\nDealer hand stronger! Dealer Wins!\n");
		gameRecord[0] -= bet;
		gameRecord[2]++;
	}
	//If player has higher points - add bet to bankRoll and increment win
	else if(dealerPoint < playerPoint) {
		printf("\nPlayer hand stronger! Player Wins!\n");
		gameRecord[0] += bet;
		gameRecord[1]++;
	}
}

//When game ends and player still has money put game data in "previousGame.txt"
void ending(int gameRecord[]){
	//Opens previousGame.txt overwrites previous data with current game data
	FILE *previousGame = fopen("previousGame.txt", "w");

	//Display the final game stats and saves them in "previousGame.txt"
	printf("\nFinal Bankroll: %d\nTotal Wins: %d\nTotal Loses: %d\nTotal Blackjacks: %d\nTotal Busts: %d\n", gameRecord[0], gameRecord[1], gameRecord[2], gameRecord[3], gameRecord[4]);
	fprintf(previousGame, "%d %d %d %d %d", gameRecord[0], gameRecord[1], gameRecord[2], gameRecord[3], gameRecord[4]);

	//Closes and saves changes to the file
	fclose(previousGame);
}