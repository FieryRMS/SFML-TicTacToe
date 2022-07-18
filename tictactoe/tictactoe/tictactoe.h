#include<vector>
#include <random>
using namespace std;
int RAND(int a, int b)
{
	static random_device rd;
	static mt19937 rng(time(0));
	if (a == b) return a;
	return uniform_int_distribution<int>(a, b)(rng);
}

struct TicTacToe
{
	const int score = 10;
	short now = 1;
	short AI = 1;
	short win[3] = {};
	short board[3][3] = {};
	bool makeMove(short mv)
	{
		if (mv < 0 || mv>8) return 0;
		if (board[mv / 3][mv % 3] != 0) return 0;
		board[mv / 3][mv % 3] = now;
		now *= -1;
		return 1;
	}
	void revMove(short mv)
	{
		board[mv / 3][mv % 3] = 0;
		now *= -1;
	}
	short getMove(short mv)
	{
		return board[mv / 3][mv % 3];
	}
	bool isWin()
	{
		for (int i = 0; i < 3; i++)
		{
			if (board[i][0] == board[i][1] && board[i][0] == board[i][2] && board[i][0] != 0)
			{
				win[0] = i * 3;
				win[1] = i * 3 + 1;
				win[2] = i * 3 + 2;
				return 1;
			}
			if (board[0][i] == board[1][i] && board[0][i] == board[2][i] && board[2][i] != 0)
			{
				win[0] = i;
				win[1] = i + 3;
				win[2] = i + 6;
				return 1;
			}
		}
		if (board[0][0] == board[1][1] && board[0][0] == board[2][2] && board[2][2] != 0)
		{
			win[0] = 0;
			win[1] = 4;
			win[2] = 8;
			return 1;
		}
		if (board[0][2] == board[1][1] && board[0][2] == board[2][0] && board[2][0] != 0)
		{
			win[0] = 2;
			win[1] = 4;
			win[2] = 6;
			return 1;
		}
		return 0;
	}
	bool isDraw()
	{
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				if (board[i][j] == 0)return 0;
		return 1;
	}
	void reset()
	{
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				board[i][j] = 0;
		now = 1;
	}
	short getScore(short depth = 0, short mx = -100, short mn = 100)
	{
		if (isWin()) return (score - depth) * now * AI * -1;
		if (isDraw()) return 0;
		short best;
		if (now == AI)
		{
			best = -100;
			for (int i = 0; i < 9; i++)
			{
				if (makeMove(i))
				{
					short val = getScore(depth + 1, mx, mn);
					revMove(i);
					best = max(best, val);
					mx = max(best, mx);
					if (mn < mx)
						break;
				}
			}
		}
		else
		{
			best = 100;
			for (int i = 0; i < 9; i++)
			{
				if (makeMove(i))
				{
					short val = getScore(depth + 1, mx, mn);
					revMove(i);
					best = min(best, val);
					mn = min(best, mn);
					if (mn < mx)
						break;
				}
			}
		}
		return best;
	}
	short getBestMove()
	{
		AI = now;
		short best_scr = -100, best_mv=0;
		vector<short> lst;
		for (int i = 0; i < 9; i++)
		{
			if (makeMove(i))
			{
				short val = getScore(0, best_scr);
				revMove(i);
				if (best_scr < val)
				{
					best_scr = val;
					lst.clear();
					lst.push_back(i);
				}
				else if (best_scr == val)
					lst.push_back(i);
			}
		}
		return lst[RAND(0, lst.size() - 1)];
	}
	void randMove(short usrmv)
	{
		vector<short> lst;
		for (int i = 0; i < 9; i++)
			if (i!=usrmv && getMove(i) == 0) lst.push_back(i);
		if (lst.size() == 0)makeMove(usrmv);
		else makeMove(lst[RAND(0, lst.size() - 1)]);
	}
};
