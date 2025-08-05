// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Board.h"
#include "Random.h"

BoardState::BoardState()
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			mBoard[i][j] = Empty;
		}
	}
}

std::vector<BoardState*> BoardState::GetPossibleMoves(SquareState player) const
{
	std::vector<BoardState*> retVal;

	// For each column, find if a move is possible
	for (int col = 0; col < 7; col++)
	{
		for (int row = 5; row >= 0; row--)
		{
			if (mBoard[row][col] == BoardState::Empty)
			{
				retVal.emplace_back(new BoardState(*this));
				retVal.back()->mBoard[row][col] = player;
				break;
			}
		}
	}

	return retVal;
}

bool BoardState::IsTerminal() const
{
	// Is the board full?
	if (IsFull())
	{
		return true;
	}

	// Is there a four-in-a-row?
	int fourInRow = GetFourInARow();
	if (fourInRow != 0)
	{
		return true;
	}

	return false;
}

float BoardState::GetScore() const
{
	// If the board is full, the score is 0
	if (IsFull())
	{
		return 0.0f;
	}

	// Is there a four-in-a-row?
	int fourInRow = GetFourInARow();
	if (fourInRow != 0)
	{
		return static_cast<float>(fourInRow);
	}

	return CalculateHeuristic();
}

bool BoardState::IsFull() const
{
	bool isFull = true;
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if (mBoard[i][j] == Empty)
			{
				isFull = false;
			}
		}
	}

	return isFull;
}

int BoardState::GetFourInARow() const
{
	// Returns -1 if yellow wins, 1 if red wins, 0 otherwise

	// Check if there's a row with four in a row
	for (int row = 0; row < 6; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			if (mBoard[row][col] == mBoard[row][col + 1] &&
				mBoard[row][col] == mBoard[row][col + 2] &&
				mBoard[row][col] == mBoard[row][col + 3])
			{
				if (mBoard[row][col] == BoardState::Yellow)
				{
					return -1;
				}
				else if (mBoard[row][col] == BoardState::Red)
				{
					return 1;
				}
			}
		}
	}

	// Check if there's a column with four in a row
	for (int col = 0; col < 7; col++)
	{
		for (int row = 0; row < 3; row++)
		{
			if (mBoard[row][col] == mBoard[row + 1][col] &&
				mBoard[row][col] == mBoard[row + 2][col] &&
				mBoard[row][col] == mBoard[row + 3][col])
			{
				if (mBoard[row][col] == BoardState::Yellow)
				{
					return -1;
				}
				else if (mBoard[row][col] == BoardState::Red)
				{
					return 1;
				}
			}
		}
	}

	// Check if there's a right diagonal four in a row
	for (int col = 0; col < 4; col++)
	{
		for (int row = 0; row < 3; row++)
		{
			if (mBoard[row][col] == mBoard[row + 1][col + 1] &&
				mBoard[row][col] == mBoard[row + 2][col + 2] &&
				mBoard[row][col] == mBoard[row + 3][col + 3])
			{
				if (mBoard[row][col] == BoardState::Yellow)
				{
					return -1;
				}
				else if (mBoard[row][col] == BoardState::Red)
				{
					return 1;
				}
			}
		}
	}

	// Check if there's a left diagonal for in a row
	for (int col = 0; col < 4; col++)
	{
		for (int row = 3; row < 6; row++)
		{
			if (mBoard[row][col] == mBoard[row - 1][col + 1] &&
				mBoard[row][col] == mBoard[row - 2][col + 2] &&
				mBoard[row][col] == mBoard[row - 3][col + 3])
			{
				if (mBoard[row][col] == BoardState::Yellow)
				{
					return -1;
				}
				else if (mBoard[row][col] == BoardState::Red)
				{
					return 1;
				}
			}
		}
	}
	return 0;
}

float BoardState::CalculateHeuristic() const
{
	// TODO: You could change this to calculate an actual heuristic
	float score = 0.0f;

	for (int row = 0; row < 6; ++row)
	{
		for (int col = 0; col < 7; ++col)
		{
			if (mBoard[row][col] == Red)
			{
				score += 3.f;
			}
			else if (mBoard[row][col] == Yellow)
			{
				score -= 1.f;
			}
		}
	}

	return score;
}

bool TryPlayerMove(BoardState* state, int column)
{
	// Find the first row in that column that's available
	// (if any)
	for (int row = 5; row >= 0; row--)
	{
		if (state->mBoard[row][column] == BoardState::Empty)
		{
			state->mBoard[row][column] = BoardState::Yellow;
			return true;
		}
	}

	return false;
}

float evaluateAlphaBetaMin(BoardState* pState, const int depth, const float alpha, const float beta)
{
	if (depth == 0 || pState->IsTerminal())
	{
		return pState->GetScore();
	}

	float minValue = std::numeric_limits<float>::infinity();

	std::vector<BoardState*> moves = pState->GetPossibleMoves(BoardState::SquareState::Yellow);

	float currBeta = beta;
	for (BoardState* const p : moves)
	{
		minValue = std::min(minValue, evaluateAlphaBetaMax(p, depth - 1, alpha, beta));
		if (minValue <= alpha)
		{
			break;
		}

		currBeta = std::min(minValue, currBeta);
	}

	for (BoardState* p : moves)
	{
		delete p;
	}

	return minValue;
}

float evaluateAlphaBetaMax(BoardState* pState, const int depth, const float alpha, const float beta)
{
	if (depth == 0 || pState->IsTerminal())
	{
		return pState->GetScore();
	}

	float maxValue = -std::numeric_limits<float>::infinity();

	std::vector<BoardState*> moves = pState->GetPossibleMoves(BoardState::SquareState::Red);

	float currAlpha = alpha;
	for (BoardState* const p : moves)
	{
		maxValue = std::max(maxValue, evaluateAlphaBetaMin(p, depth - 1, alpha, beta));
		if (maxValue >= beta)
		{
			break;
		}

		currAlpha = std::max(maxValue, currAlpha);
	}

	for (BoardState* p : moves)
	{
		delete p;
	}

	return maxValue;
}

void decideMoveAlphaBeta(BoardState* pState, const int maxDepth, BoardState& outState)
{
	BoardState* pChoice = nullptr;

	float max = -std::numeric_limits<float>::infinity();
	float beta = std::numeric_limits<float>::infinity();

	std::vector<BoardState*> moves = pState->GetPossibleMoves(BoardState::SquareState::Red);

	for (BoardState* const p : moves)
	{
		float v = evaluateAlphaBetaMin(p, maxDepth - 1, max, beta);
		if (v > max)
		{
			max = v;
			pChoice = p;
		}
	}

	outState = *pChoice;

	for (BoardState* p : moves)
	{
		delete p;
	}
}

void CPUMove(BoardState* state)
{
	// For now, this just randomly picks one of the possible moves

	decideMoveAlphaBeta(state, 6, *state);
}
