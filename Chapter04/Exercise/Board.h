// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <vector>

class BoardState
{
public:
	enum SquareState { Empty, Red, Yellow };
	BoardState();
	std::vector<BoardState*> GetPossibleMoves(SquareState player) const;
	bool IsTerminal() const;
	float GetScore() const;

	SquareState mBoard[6][7];
protected:
	bool IsFull() const;
	int GetFourInARow() const;
	float CalculateHeuristic() const;
};

// Try to place the player's piece
bool TryPlayerMove(class BoardState* state, int column);

float evaluateAlphaBetaMin(BoardState* pState, const int depth, const float alpha, const float beta);
float evaluateAlphaBetaMax(BoardState* pState, const int depth, const float alpha, const float beta);
void decideMoveAlphaBeta(BoardState* pState, const int maxDepth, BoardState& outState);

// Make the next CPU move
void CPUMove(class BoardState* state);