#include "game.hpp"
#include <fstream>

string buildPath(unsigned lvl, unsigned map){
	stringstream ss;
	ss << "levels/level" << lvl
		<< "/map" << map << ".txt";
	return ss.str();
}

vector <sf::String> readMap(string path){
	ifstream ifs(path);
	vector <sf::String> temp;
	while (!ifs.eof()){
		char str[25];
		ifs >> str;
		temp.push_back(str);
	}
	ifs.close();
	return temp;
}

void printLvlInfo(unsigned &lvl, unsigned &map, double &timeElapsed, int &score){
	printf("LVL: %u MAP: %u. Your time %.3f seconds. Your score %d\n", lvl, map, timeElapsed, score);
	Sleep(350);
	system("pause");
}

int main()
{
	double timeElapsed = 0;
	int score, restart = 1, pDone;
	unsigned lvl = 1, map = 1; // It's temporary
	MapProcessor ms;
	while (restart) {
		vector <sf::String> temp = readMap(buildPath(lvl, map));
		ms.proceedMap(restart, temp);
		mapC = temp;
		pDone = startGame(timeElapsed, score, restart);
		cout << "pDone = " << pDone << "\n";
		if (pDone == 1) continue;
		else if (pDone == 2){
			printLvlInfo(lvl, map, timeElapsed, score);
			map++;
			if (map > 2) map = 1;
		}
		else{
			printLvlInfo(lvl, map, timeElapsed, score);
			break;
		}
	}
	return 0;
}