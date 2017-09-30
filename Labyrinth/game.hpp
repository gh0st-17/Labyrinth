#include <windows.h>
#include <iostream>
#include <sstream>
#include "enemy.hpp"

using namespace sf;
using namespace std;

#define windowSize 512
#define spriteSize 64
//#define Debug

int countScore(Player &p, double &timeElapsed);
void createEnemy(vector<Enemy> &enemies, Player &p);
string buildPath(unsigned lvl, unsigned map);
vector <sf::String> readMap(string path);
void mapChanger();
void startGame(double &timeElapsed, int &score);
void restartGame(Player &p, vector<Enemy> &enemies);
void mainCycle(RenderWindow &window,
	Text &scoreText,
	Text &infoText,
	Text &wastedText,
	Sprite &wastedSprite,
	Sprite &s_map,
	Player &p,
	vector<Enemy> &entities,
	vector<Enemy>::iterator &it,
	double &timeElapsed);


void printLvlInfo(unsigned &lvl, unsigned &map, double &timeElapsed, int score){
	printf("LVL: %u MAP: %u. Your time %.3f seconds. Your score %d\n", lvl, map, timeElapsed, score);
	Sleep(350);
}

void createEnemy(vector<Enemy> &enemies, Player &p){
	unsigned id = 0;
	for (int i = 0; i < HEIGHT_MAP; i++)
		for (int j = 0; j < WIDTH_MAP; j++)
		{
			if (mapC[i][j] == '5'){
				enemies.push_back(Enemy(spriteSize * j, spriteSize * i, "images/Enemy.png", &p, 1, id));
				id++;
			}
			else if (mapC[i][j] == '6'){
				enemies.push_back(Enemy(spriteSize * j, spriteSize * i, "images/Enemy.png", &p, 0, id));
				id++;
			}
		}
}

void mapChanger(int action){
	if (action == 1){
		mapNum++;
		if (mapNum > 2) mapNum = 1;
	}
	vector <sf::String> temp = readMap(buildPath(lvl, mapNum));
	MapProcessor().proceedMap(temp);
	mapC = temp;
#ifdef Debug
	printf("mapNum = %d\n", mapNum);
#endif
}

//Данные для инициализации игры
void startGame(double &timeElapsed, int &score){
	//Окно и шрифты
	SetConsoleTitleW(L"Game console output | Ghost-17 | 1.4.0");
	RenderWindow window(sf::VideoMode(windowSize, windowSize), "Labyrinth | Ghost-17 | 1.4.0", Style::Titlebar);
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
	mapChanger(0);

	//Игрок
	Player p(68, 68, "images/Hero.png", &window);
	p.totalMoney = MapProcessor().getCoinsLimit();

	//Массив врагов
	vector<Enemy> enemies;
	//Итератор
	vector<Enemy>::iterator it = enemies.begin();
	createEnemy(enemies, p);

	mainCycle(window, scoreText, infoText, wastedText, wastedSprite, s_map, p, enemies, it, timeElapsed);

	score = countScore(p, timeElapsed);
}

string buildPath(unsigned lvl, unsigned map){
	stringstream ss;
	ss << "levels/level" << lvl
		<< "/map" << map << ".txt";
	return ss.str();
}

vector <sf::String> readMap(string path){
	ifstream ifs(path);
#ifdef Debug
	cout << path << "\n";
#endif
	vector <sf::String> temp;
	while (!ifs.eof()){
		char str[25];
		ifs >> str;
		temp.push_back(str);
	}
	ifs.close();
	return temp;
}

void restartGame(Player &p, vector<Enemy> &enemies){
	enemies.clear();
	p.resetActor();
	p.totalMoney = MapProcessor().getCoinsLimit();
	MapProcessor().proceedMap(mapC);
	mapChanger(0);
	createEnemy(enemies, p);
}

void mainCycle(RenderWindow &window,
	Text &scoreText,
	Text &infoText,
	Text &wastedText,
	Sprite &wastedSprite,
	Sprite &s_map,
	Player &p,
	vector<Enemy> &enemies,
	vector<Enemy>::iterator &it,
	double &timeElapsed){ //Основной цикл игры

	//Звук
	SoundBuffer sb;
	Sound sound;

	//Жизни
	Texture heartsTexture;
	heartsTexture.loadFromFile("images/Life.png");
	Sprite heartsSprite;
	heartsSprite.setTexture(heartsTexture);
	heartsSprite.setTextureRect(IntRect(0, 0, 157, 48));

	float time;
	Clock clock;
	float lastTime = 0;

	//Счетчик времени в игре
	double t0 = std::clock();
	double t1;
	float cc = 0; //Ограничитель анимации смерти

	while (window.isOpen())
	{
		if (Keyboard::isKeyPressed(Keyboard::Escape)) window.close();
		if (Keyboard::isKeyPressed(Keyboard::R)){ restartGame(p, enemies); }
		if (p.nextLvl) {
			p.nextLvl = 0;
			printLvlInfo(lvl, mapNum, timeElapsed, countScore(p, timeElapsed));
			t0 = std::clock();
			mapChanger(1);
			restartGame(p, enemies);
		}
		time = clock.getElapsedTime().asMicroseconds();
		time = time / 800;
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		for (size_t i = 0; i < enemies.size(); i++){
			if (!enemies[i].life){
#ifdef Debug
				printf("id %u is dead\n", entities[i]->id);
#endif
				if (enemies[i].killedByBullet) sb.loadFromFile("sounds/Headshot.wav");
				else sb.loadFromFile("sounds/Death.wav");
				sound.setBuffer(sb);
				sound.play();
				p.enemies--;
				p.enemiesC++;
				p.incScore(1000);
				if (p.enemies == 0 && p.totalMoney == 0) p.done = 1;
				it = enemies.erase(enemies.begin() + i);
			}
		}

		for (int i = 0; i < HEIGHT_MAP; i++)
			for (int j = 0; j < WIDTH_MAP; j++)
			{
				if (mapC[i][j] == '0' || mapC[i][j] == '5' || mapC[i][j] == '6')
					s_map.setTextureRect(IntRect(0, 0, spriteSize, spriteSize));
				else if (mapC[i][j] == '1') s_map.setTextureRect(IntRect(64, 0, spriteSize, spriteSize));
				else if (mapC[i][j] == '2') s_map.setTextureRect(IntRect(128, 0, spriteSize, spriteSize));
				else if (mapC[i][j] == '3') s_map.setTextureRect(IntRect(192, 0, spriteSize, spriteSize));
				else if (mapC[i][j] == '4') s_map.setTextureRect(IntRect(256, 0, spriteSize, spriteSize));
				else if (mapC[i][j] == 'h') s_map.setTextureRect(IntRect(320, 0, spriteSize, spriteSize));

				s_map.setPosition(j * spriteSize, i * spriteSize);

				window.draw(s_map);//рисуем квадратики на экран
			}

		float coordinatePlayerX, coordinatePlayerY = 0;
		coordinatePlayerX = p.getFloatRect().left;
		coordinatePlayerY = p.getFloatRect().top;

		//Отображение HUD
		getPlayerView(p.getFloatRect().left + (p.getFloatRect().width / 2.0f), p.getFloatRect().top + (p.getFloatRect().height / 2.0f));//Камера вида
		heartsSprite.setPosition(view.getCenter().x + (spriteSize * 1.4) + (52 * (abs((int)p.lifeCount - 3))), view.getCenter().y - spriteSize * 4 + 12);
		heartsSprite.setTextureRect(IntRect(0, 0, 52 * p.lifeCount, 48));
		scoreText.setPosition(view.getCenter().x - spriteSize * 4 + 6, view.getCenter().y - spriteSize * 4 + 12);
		infoText.setPosition(view.getCenter().x - spriteSize * 4 + 6, view.getCenter().y + spriteSize * 2 + 3);

		//Update
		viewmap(time);//функция скроллинга карты, передаем ей время sfml
		p.cacheX = p.getFloatRect().left;
		p.cacheY = p.getFloatRect().top;
		for (unsigned i = 0; i < enemies.size(); i++) enemies[i].update(time);
		p.update(time);

		window.setView(view);

		//Draw
		window.draw(p.sprite);
		for (unsigned i = 0; i < enemies.size(); i++){
			window.draw(enemies[i].sprite);
			enemies[i].drawBars(time, window);
		}
		if (p.getHealth() <= 0) {
			time = 0;
			p.life = 0;
			if (cc == 0){
				sb.loadFromFile("sounds/Enemy.wav");
				sound.setBuffer(sb);
				sound.play();
				p.lifeCount--;
			}
			wastedSprite.setPosition(view.getCenter().x - windowSize / 2.0f, view.getCenter().y - windowSize / 2.0f);
			window.draw(wastedSprite);
			if (p.lifeCount == 0){
				wastedText.setString("GAME OVER");
				wastedText.setPosition(view.getCenter().x - spriteSize * 2.0f, view.getCenter().y - spriteSize);
			}
			wastedText.setPosition(view.getCenter().x - spriteSize * 1.5f, view.getCenter().y - spriteSize);
			window.draw(wastedText);
			view.setCenter(p.getFloatRect().left + (p.getFloatRect().width / 2.0f), p.getFloatRect().top + (p.getFloatRect().height / 2.0f));
			view.rotate(0.125);
			view.zoom(0.990f);
			cc++;
		}

		if (cc > 150) {
			if (p.lifeCount == 0) window.close();
			view.setRotation(0);
			view.reset(sf::FloatRect(0, 0, windowSize, windowSize));
			p.respawn();
			enemies[p.colId].speed = 0.085f;
			cc = 0;
		}
		viewmap(time);
		t1 = std::clock();
		timeElapsed = (double)(t1 - t0) / CLOCKS_PER_SEC;
		time = clock.getElapsedTime().asSeconds();
		clock.restart().asSeconds();
		stringstream ss;
		ss << "Score: " << countScore(p, timeElapsed) << "\nFPS: " << (unsigned)(1.0f / time);
		scoreText.setString(ss.str());
		infoText.setString(p.getInfoString());
		window.draw(heartsSprite);
		window.draw(scoreText);
		if (Keyboard::isKeyPressed(Keyboard::Tab)) window.draw(infoText);
		window.display();
	}
}

int countScore(Player &p, double &timeElapsed){
	if (timeElapsed != 0 && p.enemiesC != 0) return (p.score * (p.enemiesC + 1) * ((p.lifeCount + 1) / 2)) / (((unsigned)timeElapsed / (p.enemiesC + 1)) * (p.dieCounter + 1));
	else return 0;
}
