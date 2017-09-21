using namespace std;

#pragma warning(disable : 4996)
string getTime() {										//Получаем полную дату для имени файла
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buffer, sizeof(buffer), "[%H:%M:%S]", timeinfo);
	string str(buffer);
	return str;
}

/*class Map{
private:
	unsigned H, W;
	vector<String> TileMap;

	vector<string> split(string str, char delimiter) {
		vector<string> internal;
		stringstream ss(str); // Turn the string into a stream.
		string tok;
		while (getline(ss, tok, delimiter)) {
			internal.push_back(tok);
		}
		return internal;
	}

	bool checkTileMap(vector<String> tileMap){
		size_t size = tileMap[0].getSize();
		for (size_t i = 1; i < tileMap.size(); i++) if (tileMap[i].getSize() != size) return 0;
		return 1;
	}
public:
	Map(vector<String> tileMap){
		assert(tileMap.size() >= 10 && tileMap[0].getSize() >= 10 && checkTileMap(tileMap));
		TileMap = tileMap;
		H = TileMap.size();
		W = TileMap[0].getSize();
	}

	Map(String mapPath){
		ifstream ifs(mapPath.toAnsiString());
		stringstream buffer;
		buffer << ifs.rdbuf();
		ifs.close();
		vector<std::string> init = split(buffer.str(), '\n');
		vector<String> tileMap;
		for (size_t i = 0; i < init.size(); i++) tileMap.push_back(String(init[i]));
		assert(tileMap.size() >= 10 && tileMap[0].getSize() >= 10 && checkTileMap(tileMap));
		TileMap = tileMap;
		H = TileMap.size();
		W = TileMap[0].getSize();
	}

	vector<String> getTileMap() { return TileMap; }
};*/