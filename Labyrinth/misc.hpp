using namespace std;

#pragma warning(disable : 4996)
string getTime() {										//�������� ������ ���� ��� ����� �����
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
	std::vector<String> TileMap;

	std::vector<string> split(string str, char delimiter) {
		std::vector<string> internal;
		stringstream ss(str); // Turn the string into a stream.
		string tok;
		while (getline(ss, tok, delimiter)) {
			internal.push_back(tok);
		}
		return internal;
	}

	bool checkTileMap(std::vector<String> tileMap){
		size_t size = tileMap[0].getSize();
		for (size_t i = 1; i < tileMap.size(); i++) if (tileMap[i].getSize() != size) return 0;
		return 1;
	}
public:
	Map(std::vector<String> tileMap){
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
		std::vector<std::string> init = split(buffer.str(), '\n');
		std::vector<String> tileMap;
		for (size_t i = 0; i < init.size(); i++) tileMap.push_back(String(init[i]));
		assert(tileMap.size() >= 10 && tileMap[0].getSize() >= 10 && checkTileMap(tileMap));
		TileMap = tileMap;
		H = TileMap.size();
		W = TileMap[0].getSize();
	}

	std::vector<String> getTileMap() { return TileMap; }
};*/

/*
std::vector<string> split(string str, char delimiter) {
std::vector<string> internal;
stringstream ss(str); // Turn the string into a stream.
string tok;
while (getline(ss, tok, delimiter)) {
internal.push_back(tok);
}
return internal;
}

bool checkMap(std::vector<String> tileMap){
size_t size = tileMap[0].getSize();
for (size_t i = 1; i < tileMap.size(); i++) if (tileMap[i].getSize() != size) return 0;
return 1;
}

std::vector<String> readMapFromFile(String &mapPath){
ifstream ifs(mapPath.toAnsiString());
stringstream buffer;
buffer << ifs.rdbuf();
ifs.close();
std::vector<std::string> init = split(buffer.str(), '\n');
std::vector<String> tileMap;
for (size_t i = 0; i < init.size(); i++) tileMap.push_back(String(init[i]));
assert(tileMap.size() >= 10 && tileMap[0].getSize() >= 10 && checkMap(tileMap));
return tileMap;
}
*/