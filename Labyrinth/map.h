#include <SFML\Graphics.hpp>
#include <fstream>
#include <cassert>

const int HEIGHT_MAP = 24;//размер карты высота
const int WIDTH_MAP = 24;//размер карты ширина 


vector<String> TileMap = {
	"111111111111111111111111",
	"100000010000001000010001",
	"101011010111111011110111",
	"101010010000000010000101",
	"101011411110111016111101",
	"111011111110101010010101",
	"105000000010100010110101",
	"111111111010111010010101",
	"160010001010101010110101",
	"101000101010100000000101",
	"101111101000101111010101",
	"101000001111100001010101",
	"101111101000111111010101",
	"101000101010000000000101",
	"101010111111111111110101",
	"101010100050000100010101",
	"101010111111110101010101",
	"101010010001000101010101",
	"101011010100010001000101",
	"101111011111111111110101",
	"160000000000100000000001",
	"101111011111101111111111",
	"100000000005000000000031",
	"111111111111111111111111",
};

class Map{
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
};