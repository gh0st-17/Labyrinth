#include "game.hpp"

int main()
{
	double timeElapsed = 0;
	int score;
	while (startGame(timeElapsed, score)) {
		startGame(timeElapsed, score);
	}
	printf("Your time %.3f seconds. Your score %d\n", timeElapsed, score);
	system("pause");
	return 0;
}