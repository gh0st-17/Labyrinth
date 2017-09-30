#include "game.hpp"
#include <fstream>

int main()
{
	double timeElapsed = 0;
	int score;
	startGame(timeElapsed, score);
	printLvlInfo(lvl, mapNum, timeElapsed, score);
	system("pause");
	return 0;
}