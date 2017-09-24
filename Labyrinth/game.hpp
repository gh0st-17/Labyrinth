#include <windows.h>
#include <iostream>
#include <sstream>
#include "enemy.hpp"

using namespace sf;
using namespace std;

#define windowSize 512
#define spriteSize 64
//#define Debug

vector<string> split(string str, char delimiter);
bool checkMap(vector<String> tileMap);
vector<String> readMapFromFile(String &mapPath);
unsigned countCoins();
unsigned countHearths();
void randomMapGenerate(unsigned currentCoinsCount, unsigned currentHearthsCount);
int countScore(Player &p, double &timeElapsed);
void mainCycle(RenderWindow &window, Text &scoreText, Text &infoText, Text &wastedText, Sprite &wastedSprite, Sprite &s_map, Player &p, vector<Actor*> &entities, vector<Actor*>::iterator &it, double &timeElapsed, bool &restart);
bool startGame(double &timeElapsed, int &score);


bool startGame(double &timeElapsed, int &score){ //Данные для инициализации игры
	//Окно и шрифты
	
	SetConsoleTitleW(L"Game console output | Ghost-17");
	RenderWindow window(sf::VideoMode(windowSize, windowSize), "Labyrinth | Ghost-17", Style::Titlebar);
	printf("Window handle %u\n", window.getSystemHandle());
	view.reset(sf::FloatRect(0, 0, windowSize, windowSize));
	window.setFramerateLimit(60);
	Font font;
	font.loadFromFile("Fipps-Regular.otf");
	Font wastedFont;
	wastedFont.loadFromFile("Pricedown.ttf");
	Text scoreText("", font, 20);
	scoreText.setColor(Color::Black);
	Text infoText("", font, 20);
	infoText.setColor(Color::Black);
	Text wastedText("WASTED", wastedFont, 72);
	wastedText.setColor(Color::Black);

	//Спрайт и текстура карты
	Texture fieldtexture;
	fieldtexture.loadFromFile("levels/level1/map.png");
	Sprite s_map;
	s_map.setTexture(fieldtexture);

	//Спрайт и текстура WASTED
	Sprite wastedSprite;
	Texture wastedTexture;
	wastedTexture.loadFromFile("images/wasted.png");
	wastedSprite.setTexture(wastedTexture);

	//Карта
	randomMapGenerate(countCoins(), countHearths());
#ifdef Debug
	printf("Current coins %u\n", countCoins());
#endif
	//Игрок
	Player p(68, 68, "images/Hero.png", &window);

	//Массив врагов
	vector<Actor*> entities;
	//Итератор
	vector<Actor*>::iterator it = entities.begin();
	//entities.push_back(new Player(34, 34, "images/Hero.png", &window));
	unsigned id = 0;

	for (int i = 0; i < HEIGHT_MAP; i++)
		for (int j = 0; j < WIDTH_MAP; j++)
		{
			if (map1[i][j] == '5'){
				entities.push_back(new Enemy(spriteSize * j, spriteSize * i, "images/Enemy.png", &p, 1, id));
				id++;
			}
			else if (map1[i][j] == '6'){
				entities.push_back(new Enemy(spriteSize * j, spriteSize * i, "images/Enemy.png", &p, 0, id));
				id++;
			}
		}

	bool restart = 0;

	mainCycle(window, scoreText, infoText, wastedText, wastedSprite, s_map, p, entities, it, timeElapsed, restart);

	score = countScore(p, timeElapsed);

	return restart;
}

void mainCycle(RenderWindow &window, Text &scoreText, Text &infoText, Text &wastedText, Sprite &wastedSprite, Sprite &s_map, Player &p, vector<Actor*> &entities, vector<Actor*>::iterator &it, double &timeElapsed, bool &restart){ //Основной цикл игры

	//Звук
	SoundBuffer sb;
	Sound sound;

	//float currentFrame = 0;//хранит текущий кадр(Для анимации, пока нет) )
	Clock clock;
	float lastTime = 0;

	//Счетчик времени в игре
	double t0 = std::clock();
	double t1;

	float cc = 0; //Ограничитель анимации WASTED

	while (window.isOpen())
	{
		if (Keyboard::isKeyPressed(Keyboard::Escape))  { restart = 0; window.close(); }
		if (Keyboard::isKeyPressed(Keyboard::R)) { restart = 1; window.close(); }
		float time;
		time = clock.getElapsedTime().asSeconds() * 1000;
		clock.restart().asSeconds();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		for (size_t i = 0; i < entities.size(); i++){
			if (!entities[i]->life){
#ifdef Debug
				printf("id %u is dead\n", entities[i]->id);
#endif
				if (entities[i]->killedByBullet) sb.loadFromFile("sounds/Headshot.wav");
				else sb.loadFromFile("sounds/Death.wav");
				sound.setBuffer(sb);
				sound.play();
				p.enemies--;
				p.enemiesC++;
				p.incScore(1000);
				if (p.enemies == 0 && p.totalMoney == 0) p.done = 1;
				delete [] entities[i];
				it = entities.erase(entities.begin() + i);
			}
		}

		for (int i = 0; i < HEIGHT_MAP; i++)
			for (int j = 0; j < WIDTH_MAP; j++)
			{
				if (map1[i][j] == '0' || map1[i][j] == '5' || map1[i][j] == '6')
					s_map.setTextureRect(IntRect(0, 0, spriteSize, spriteSize));
				else if (map1[i][j] == '1') s_map.setTextureRect(IntRect(64, 0, spriteSize, spriteSize));
				else if (map1[i][j] == '2') s_map.setTextureRect(IntRect(128, 0, spriteSize, spriteSize));
				else if (map1[i][j] == '3') s_map.setTextureRect(IntRect(192, 0, spriteSize, spriteSize));
				else if (map1[i][j] == '4') s_map.setTextureRect(IntRect(256, 0, spriteSize, spriteSize));
				else if (map1[i][j] == 'h') s_map.setTextureRect(IntRect(320, 0, spriteSize, spriteSize));

				s_map.setPosition(j * spriteSize, i * spriteSize);

				window.draw(s_map);//рисуем квадратики на экран
			}

		float coordinatePlayerX, coordinatePlayerY = 0;
		coordinatePlayerX = p.getRect().left;
		coordinatePlayerY = p.getRect().top;

		//Отображение HUD
		getPlayerView(p.getRect().left + (p.getRect().width / 2), p.getRect().top + (p.getRect().height / 2));//Камера вида
		scoreText.setPosition(view.getCenter().x - spriteSize * 4 + 6, view.getCenter().y - spriteSize * 4 + 12);
		infoText.setPosition(view.getCenter().x - spriteSize * 4 + 6, view.getCenter().y + spriteSize * 2 + 3);

		//Update
		viewmap(time);//функция скроллинга карты, передаем ей время sfml
		p.cacheX = p.getRect().left;
		p.cacheY = p.getRect().top;
		for (unsigned i = 0; i < entities.size(); i++) entities[i]->update(time);
		p.update(time);

		window.setView(view);

		//Draw
		window.draw(s_map);
		window.draw(p.sprite);
		for (unsigned i = 0; i < entities.size(); i++){
			window.draw(entities[i]->sprite);
			entities[i]->drawBars(time, window);
		}
		if (p.getHealth() <= 0) {
			time = 0;
			p.life = 0;
			if (cc == 0){
				sb.loadFromFile("sounds/Enemy.wav");
				sound.setBuffer(sb);
				sound.play();
			}
			wastedSprite.setPosition(view.getCenter().x - windowSize / 2, view.getCenter().y - windowSize / 2);
			window.draw(wastedSprite);
			wastedText.setPosition(view.getCenter().x - spriteSize*1.5, view.getCenter().y - spriteSize);
			window.draw(wastedText);
			view.setCenter(p.getRect().left + (p.getRect().width / 2), p.getRect().top + (p.getRect().height / 2));
			view.rotate(0.125);
			view.zoom(0.990f);
			cc += 1;
		}

		if (cc > 150) {
			view.setRotation(0);
			view.reset(sf::FloatRect(0, 0, windowSize, windowSize));

			p.respawn();
			entities[p.colId]->speed = 0.085;
			cc = 0;
		}
		viewmap(time);
		t1 = std::clock();
		timeElapsed = (double)(t1 - t0) / CLOCKS_PER_SEC;
		stringstream ss;
		ss << "Score: " << countScore(p, timeElapsed) << "\nFPS: " << (unsigned)(1.0f / (time / 1000));
		scoreText.setString(ss.str());
		infoText.setString(p.getInfoString());
		window.draw(scoreText);
		if (Keyboard::isKeyPressed(Keyboard::Tab)) window.draw(infoText);
		window.display();
	}

}

int countScore(Player &p, double &timeElapsed){
	if (timeElapsed != 0 && p.enemiesC != 0) return (p.score * (p.enemiesC + 1)) / (((unsigned)timeElapsed / (p.enemiesC + 1)) * (p.dieCounter + 1));
	else return 0;
}

unsigned countCoins(){
	unsigned temp = 0;
	for (unsigned i = 1; i < WIDTH_MAP - 1; i++)
		for (unsigned j = 1; j < HEIGHT_MAP - 1; j++)
			if (map1[i][j] == '2') temp++;
	return temp;
}

unsigned countHearths(){
	unsigned temp = 0;
	for (unsigned i = 1; i < WIDTH_MAP - 1; i++)
		for (unsigned j = 1; j < HEIGHT_MAP - 1; j++)
			if (map1[i][j] == 'h') temp++;
	return temp;
}

void randomMapGenerate(unsigned currentCoinsCount, unsigned currentHearthsCount){ //Генерация
	int randomElementX = 0;
	int randomElementY = 0;
	srand(time(0));
	unsigned limitCoins = 50 - currentCoinsCount;
	unsigned limitHearths = 2 - currentHearthsCount;

	while (limitCoins > 0){
		randomElementX = 1 + rand() % (WIDTH_MAP - 1);
		randomElementY = 1 + rand() % (HEIGHT_MAP - 1);

		if (map1[randomElementY][randomElementX] == '0') {
			map1[randomElementY][randomElementX] = '2';
			limitCoins--;
		}
	}

	while (limitHearths > 0){
		randomElementX = 1 + rand() % (WIDTH_MAP - 1);
		randomElementY = 1 + rand() % (HEIGHT_MAP - 1);

		if (map1[randomElementY][randomElementX] == '0') {
			map1[randomElementY][randomElementX] = 'h';
			limitHearths--;
		}
	}
}

vector<string> split(string str, char delimiter) {
	vector<string> internal;
	stringstream ss(str); // Turn the string into a stream.
	string tok;
	while (getline(ss, tok, delimiter)) {
		internal.push_back(tok);
	}
	return internal;
}

bool checkMap(vector<String> tileMap){
	size_t size = tileMap[0].getSize();
	for (size_t i = 1; i < tileMap.size(); i++) if (tileMap[i].getSize() != size) return 0;
	return 1;
}

vector<String> readMapFromFile(String &mapPath){
	ifstream ifs(mapPath.toAnsiString());
	stringstream buffer;
	buffer << ifs.rdbuf();
	ifs.close();
	vector<std::string> init = split(buffer.str(), '\n');
	vector<String> tileMap;
	for (size_t i = 0; i < init.size(); i++) tileMap.push_back(String(init[i]));
	assert(tileMap.size() >= 10 && tileMap[0].getSize() >= 10 && checkMap(tileMap));
	return tileMap;
}