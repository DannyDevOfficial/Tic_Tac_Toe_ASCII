/*
 * TTTutils.h
 *
 *  Created on: Jan 17, 2020
 *      Author: danielgrieco
 */

#ifndef TTTUTILS_H_
#define TTTUTILS_H_

namespace ttt {
	void Start();
	void PlayGame(unsigned short& playerID);
	void PrintIntro();
	char* MakeNewBoard();
	void DestroyBoard(char*& op_board);
	char GetRandomMarker(char previousMarkerUsed);
	void PrintPlayerTurn(unsigned short playerID, char p1Marker, char p2Marker);
	void PrintBoard(const char* const np_board);
	void PlaceMarkerOnBoard(char* const np_board, const unsigned short currentPlayerID,
	                        const char p1Marker, const char p2Marker);
	bool IsPlacementValid(const char* const np_brd, const short r, const short c);
	unsigned short GetPlacementIndex(const unsigned short r, const unsigned short c);
	void NextPlayerTurn(unsigned short& currentPlayerID);
	bool GameOver(const char* const np_board, const char p1m, const char p2m, short& wID);
	void PrintWinner(const short wID);
	bool WantToPlayAgain();
}

#endif /* TTTUTILS_H_ */
