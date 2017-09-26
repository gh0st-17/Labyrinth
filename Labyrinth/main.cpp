#include "game.hpp"

int main()
{
	double timeElapsed = 0;
	int score;
	bool restart = 1;
	while (restart) {
		startGame(timeElapsed, score, restart);
	}
	printf("Your time %.3f seconds. Your score %d\n", timeElapsed, score);
	system("pause");
	return 0;
}