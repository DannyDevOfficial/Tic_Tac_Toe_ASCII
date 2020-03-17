/*
 * TTTutils.cpp
 *
 *  Created on: Jan 17, 2020
 *      Author: danielgrieco
 */

#include "TTTutils.h"

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cctype>

using namespace std;

namespace ttt {
	// Initiates the Tic Tac Toe game.
	void Start() {
		// Keep track of what player is currently placing a marker.
		// It can only be one or two.
		unsigned short currentPlayerID = 1;

		// Keep playing until the user wants to
		do {
			PlayGame(currentPlayerID);
		} while (WantToPlayAgain());
	}

	// Plays the game.
	void PlayGame(unsigned short& playerID) {
		PrintIntro();

		// Store the players' markers.
		char player1Marker = GetRandomMarker(' ');
		char player2Marker = GetRandomMarker(player1Marker);

		// Store the winner.
		short winnerID = -1;

		char* np_theBoard = MakeNewBoard();

		// Keep the game up until it isn't over.
		do {
			PrintPlayerTurn(playerID, player1Marker, player2Marker);
			PrintBoard(np_theBoard);
			PlaceMarkerOnBoard(np_theBoard, playerID, player1Marker, player2Marker);
			NextPlayerTurn(playerID);
		} while (!GameOver(np_theBoard, player1Marker, player2Marker, winnerID));

		// Display the final board, so it's clear who won the game.
		PrintBoard(np_theBoard);

		// Print the winner, if any.
		PrintWinner(winnerID);

		DestroyBoard(np_theBoard);
	}

	// Prints the introduction to Tic Tac Toe.
	void PrintIntro() {
		// All this does is printing info about the game.
		cout << "                                    +-----------------------+                                    " << endl;
		cout << "*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#|Welcome to Tic Tac Toe!|*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*" << endl;
		cout << "                                    +-----------------------+                                    " << endl;
		cout << "+----------------------------------------------------------------------------------------------+" << endl;
		cout << "| It's a game played by 2 players.                                                             |" << endl;
		cout << "| One player places O markers and the other player places X markers.                           |" << endl;
		cout << "| Each player takes turns placing their marker                                                 |" << endl;
		cout << "| in order to get 3 of their markers to line up on the board.                                  |" << endl;
		cout << "| A player wins when 3 or their markers line up either horizontally, vertically or diagonally. |" << endl;
		cout << "| If no player has their markers lined up in that fashion,                                     |" << endl;
		cout << "| and the board is full, then the game is called a \"Cat Game\".                                 |" << endl;
		cout << "+----------------------------------------------------------------------------------------------+" << endl;
		cout << endl;
	}

	// Allocates a char array on the heap and returns a pointer to it.
	char* MakeNewBoard() {
		const int BOARD_SIZE = 9;
		char* np_board = new char[BOARD_SIZE];

		// Initialize all the elements.
		for (int i = 0; i < BOARD_SIZE; i++)
			np_board[i] = ' ';

		return np_board;
	}

	// Deallocate the board.
	void DestroyBoard(char*& op_board) {
		delete[] op_board;
		op_board = nullptr;
	}

	// Gets a random marker based on the previous marker used.
	// e.g. previousMarkerUsed == 'O' return 'X' and vice versa,
	// make it random otherwise.
	char GetRandomMarker(char previousMarkerUsed) {
		char rMarker = ' ';

		// The marker is only random if no marker has been used yet.
		switch (previousMarkerUsed) {
			case 'O':
				rMarker = 'X';
				break;
			case 'X':
				rMarker = 'O';
				break;
			default: {
				// Random number to use to pick random marker.
				srand(time(nullptr));
				int rNum = rand() % 10 + 1; // Range is 1 - 10

				// 50% chance to get any of the two markers.
				rMarker = (rNum <= 5) ? 'O' : 'X';
			}
			break;
		}

		return rMarker;
	}

	// Prints the next player turn e.g. "Player 'ID' 'Marker', it's your turn now!".
	void PrintPlayerTurn(unsigned short playerID, char p1Marker, char p2Marker) {
		// Set the marker to print based on the player ID.
		char marker = ' ';

		switch (playerID) {
			case 1:
				marker = p1Marker;
				break;
			case 2:
				marker = p2Marker;
				break;
		}

		cout << "Player " << playerID << " \'" << marker << "\'" << ", it's your turn now!" << endl;
		cout << endl;
	}

	// Prints the board to the console.
	void PrintBoard(const char* const np_board) {
		const int TOT_NUM_ROWS = 4;

		for (int i = 0; i < TOT_NUM_ROWS; i++) {
			cout << "+---+---+---+" << endl;

			// Don't want to print an extra row of this.
			if (i < TOT_NUM_ROWS - 1)
				cout << "| " << np_board[i * 3] << " | " <<
				np_board[(i * 3) + 1] << " | " <<
				np_board[(i * 3) + 2] << " |" << endl;
		}

		cout << endl;
	}

	// Given a board, the player current ID and players' markers, places the marker related to the player ID onto the board.
	void PlaceMarkerOnBoard(char* const np_board, const unsigned short currentPlayerID,
	                        const char p1Marker, const char p2Marker) {
		// Used for input checking.
		const int CHARS_TO_IGNORE = 256;
		bool failure = false;

		// User input for row and column of their choosing.
		short row = -1;
		short col = -1;

		// Keep asking for input until it's valid.
		do {
			// Reset the failure flag.
			failure = false;

			// Ask for valid input.
			cout << "Input your desired marker position (range=1-3, format=row column): ";
			cin >> row >> col;

			// Check for correct input.
			if (cin.fail()) {
				// Clear the console from the bad input and then ignore it.
				cin.clear();
				cin.ignore(CHARS_TO_IGNORE, '\n');

				// Tell the user that the input was bad.
				cout << "This is an invalid input! Please try again." << endl;

				failure = true;

				// Spacing for nicer-looking output.
				cout << endl;
			} else if (!IsPlacementValid(np_board, row, col)) {
				// Ignore the wrong input.
				cin.ignore(CHARS_TO_IGNORE, '\n');

				// Tell the user that the input was wrong, hence unacceptable.
				cout << "The input wasn't in range or the spot you chose already has a marker! Please try again." << endl;

				failure = true;

				// Spacing for nicer-looking output.
				cout << endl;
			} else {
				// The marker to use will be based on the player ID, if it's player 1,
				// the p1marker will be used, otherwise, it'll be p2marker.
				const unsigned short INDEX = GetPlacementIndex(row, col);

				switch (currentPlayerID) {
					case 1:
						np_board[INDEX] = p1Marker;
						break;
					case 2:
						np_board[INDEX] = p2Marker;
						break;
				}

				failure = false;

				// Spacing for nicer-looking output.
				cout << endl << endl;
			}
		} while (failure);
	}

	// Returns true when the input is both in range and placed in an unoccupied spot on the board,
	// false otherwise.
	bool IsPlacementValid(const char* const np_brd, const short r, const short c) {
		// Row or column or both aren't in the board's range.
		if ((r < 1 || r > 3) || (c < 1 || c > 3))
			return false;

		const unsigned short INDEX = GetPlacementIndex(r, c);

		// Row and column are in range but there's already a marker on the chosen spot.
		if ((np_brd[INDEX] == 'O') || (np_brd[INDEX] == 'X'))
			return false;

		// If we get here, all the tests are passed, hence the placement is valid.
		return true;
	}

	// Maps the row and column to an index in the linear array.
	// Row and column are assumed to be in range from 1 to 3.
	unsigned short GetPlacementIndex(const unsigned short r, const unsigned short c) {
		// Deducting one because we need indices for the array.
		const short R = r - 1;
		const short C = c - 1;

		// Map row and column to the correct index to access our linear array.
		return (R * 3) + C;
	}

	// Updates the player ID to be the ID of the next player.
	void NextPlayerTurn(unsigned short& currentPlayerID) {
		switch (currentPlayerID) {
			case 1:
				currentPlayerID = 2;
				break;
			case 2:
				currentPlayerID = 1;
				break;
		}
	}

	// Returns true when the game is over and false otherwise.
	bool GameOver(const char* const np_board, const char p1m, const char p2m, short& wID) {
		// Check for horizontal win.
		for (int i = 0; i <= 6; i += 3) {
			// If one of the columns has a blank space, we won't need to check for a win.
			// We'll move onto the next row.
			if (np_board[i] == ' ')	continue;

			// Check all the columns on a row are equal.
			if ((np_board[i] == np_board[i + 1]) && (np_board[i + 1] == np_board[i + 2])) {
				// Set winner ID based on what marker matched up with the board.
				if (np_board[i] == p1m)			wID = 1;
				else if (np_board[i] == p2m)	wID = 2;

				return true;
			}
		}

		// Check for vertical win.
		for (int i = 0; i < 3; i++) {
			// If one of the rows has a blank space, we won't need to check for a win.
			// We'll move onto the next column.
			if (np_board[i] == ' ')	continue;

			// Check all the rows on a column are equal.
			if ((np_board[i] == np_board[i + 3]) && (np_board[i + 3] == np_board[i + 6])) {
				// Set winner ID based on what marker matched up with the board.
				if (np_board[i] == p1m)			wID = 1;
				else if (np_board[i] == p2m)	wID = 2;

				return true;
			}
		}

		// This will be used to decide the last column of the diagonals.
		unsigned short increment = 8;

		// Check for diagonal win.
		for (int i = 0; i <= 2; i += 2) {
			// If one of the diagonal has a blank space, we won't need to check for a win.
			// We'll move onto the next diagonal.
			if (np_board[i] == ' ')	continue;

			// Change the increment value whenever the index equals 2.
			if (i == 2)	increment = 4;

			// Check all the diagonals are equal.
			if ((np_board[i] == np_board[4]) && (np_board[4] == np_board[i + increment])) {
				// Set winner ID based on what marker matched up with the board.
				if (np_board[i] == p1m)			wID = 1;
				else if (np_board[i] == p2m)	wID = 2;

				return true;
			}
		}

		// Check for a tie.
		for (int i = 0; i < 9; i++) {
			// If there's a blank spot, the game isn't over yet.
			if (np_board[i] == ' ')	break;

			// If the current spot isn't blank and we got to the last index,
			// then it must mean all the spots are filled and nobody won.
			if ((np_board[i] != ' ') && (i == 8)) {
				// Set the winner ID to nothing, there was no winner.
				wID = 0;
				return true;
			}
		}

		// No win nor tie happened if we get here.
		return false;
	}

	// Prints the winner or "Cat game" if it's a tie.
	void PrintWinner(const short wID) {
		if ((wID == 1) || (wID == 2))
			cout << "Player " << wID << ", you won the game!" << endl;
		else
			cout << "It's a \"Cat Game\"" << endl;

		cout << endl;
	}

	// Returns true if the user wants to play another round,
	// false otherwise.
	bool WantToPlayAgain() {
		// Used for input checking.
		const int CHARS_TO_IGNORE = 256;
		bool failure = false;

		char answer = ' ';

		// Keep asking for input until it's valid.
		do {
			// Reset the failure flag.
			failure = false;

			// Ask for valid input.
			cout << "Do you want to play again? (y/n): ";
			cin >> answer;

			// Check for correct input.
			if (cin.fail()) {
				// Clear the console from the bad input and then ignore it.
				cin.clear();
				cin.ignore(CHARS_TO_IGNORE, '\n');

				// Tell the user that the input was bad.
				cout << "This is an invalid input! Please try again." << endl;

				failure = true;

				// Spacing for nicer-looking output.
				cout << endl;
			} else {
				// Convert answer to lowercase.
				answer = tolower(answer);

				// Answer is correct, we can move on.
				if (answer == 'y' || answer == 'n') {
					failure = false;
				} else {
					// Ignore the wrong input.
					cin.ignore(CHARS_TO_IGNORE, '\n');

					// Tell the user that the input was wrong, hence unacceptable.
					cout << "The input has to be either \'y\' or \'n\' (uppercase or lowercase)! Please try again." << endl;

					failure = true;

					// Spacing for nicer-looking output.
					cout << endl;
				}
			}
		} while (failure);

		// Spacing for nicer-looking output.
		cout << endl << endl;

		// The answer can only be 'y' or 'n', which means that if it's equal to 'y'
		// then we can return true, false otherwise.
		return (answer == 'y');
	}

}
