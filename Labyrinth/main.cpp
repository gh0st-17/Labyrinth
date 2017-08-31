#include <windows.h>
#include <iostream>
#include <sstream>
#include "entity.hpp"
#include "misc.hpp"
#include "view.hpp"

using namespace sf;
using namespace std;

#define windowSize 512
#define spriteSize 32

vector<string> split(string str, char delimiter);
bool checkMap(vector<String> tileMap);
vector<String> readMapFromFile(String &mapPath);
void randomMapGenerate();

int countScore(Player &p, unsigned timeElapsed){
	if (timeElapsed != 0) return (p.score * (p.enemiesC + 1)) / (timeElapsed * (p.dieCounter + 1));
	else return 0;
}

int main()
{
	//Окно и шрифты
	SetConsoleTitleW(L"Game console output | Ghost-17");
	RenderWindow window(sf::VideoMode(windowSize, windowSize), "Labyrinth | Ghost-17", Style::Titlebar);
	view.reset(sf::FloatRect(0, 0, windowSize/2, windowSize/2));
	window.setFramerateLimit(60);
	Font font;
	font.loadFromFile("Copyright_House.ttf");
	Font wastedFont;
	wastedFont.loadFromFile("Pricedown.ttf");
	Text scoreText("", font, 20);
	scoreText.setFillColor(Color::Black);
	scoreText.setStyle(Text::Bold);
	Text infoText("", font, 20);
	infoText.setFillColor(Color::Black);
	infoText.setStyle(Text::Bold);
	Text wastedText("WASTED", wastedFont, 36);
	wastedText.setFillColor(Color::Black);

	//float CurrentFrame = 0;//хранит текущий кадр
	Clock clock;

	//Спрайт и текстура карты
	Texture fieldtexture;
	fieldtexture.loadFromFile("images/field.png");
	Sprite s_map;
	s_map.setTexture(fieldtexture);

	//Спрайт и текстура WASTED
	Sprite wastedSprite;
	Texture wastedTexture;
	wastedTexture.loadFromFile("images/wasted.png");
	wastedSprite.setTexture(wastedTexture);
	wastedSprite.setPosition(0, 0);

	//Карта
	randomMapGenerate();

	//Игрок
	Player p(34, 34, "images/Hero.png", &window);

	//Массив врагов
	vector<Enemy> enemies;
	for (int i = 0; i < HEIGHT_MAP; i++)
	for (int j = 0; j < WIDTH_MAP; j++)
	{
		if (map1[i][j] == '5') enemies.push_back(Enemy(spriteSize * j, spriteSize * i, "images/Enemy.png", 1));
		else if (map1[i][j] == '6') enemies.push_back(Enemy(spriteSize * j, spriteSize * i, "images/Enemy.png", 0));
	}
	p.ptrCollidingEnemy = &enemies;

	//Счетчик времени в игре
	double t0 = std::clock();
	double t1, timeElapsed;

	float cc = 0; //Ограничитель анимации WASTED

	while (window.isOpen())
	{
		if (Keyboard::isKeyPressed(Keyboard::Escape)) break;
		float time;
		if (!p.life) time = 0;// clock.getElapsedTime().asSeconds();
		else time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 1000;
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		for (int i = 0; i < HEIGHT_MAP; i++)
		for (int j = 0; j < WIDTH_MAP; j++)
		{
			if (map1[i][j] == '0' || map1[i][j] == '5' || 
				map1[i][j] == '6')  s_map.setTextureRect(IntRect(0, 0, spriteSize, spriteSize));
			else if (map1[i][j] == '1') s_map.setTextureRect(IntRect(32, 0, spriteSize, spriteSize));
			else if (map1[i][j] == '2') s_map.setTextureRect(IntRect(64, 0, spriteSize, spriteSize));
			else if (map1[i][j] == '3') s_map.setTextureRect(IntRect(96, 0, spriteSize, spriteSize));
			else if (map1[i][j] == '4') s_map.setTextureRect(IntRect(128, 0, spriteSize, spriteSize));

			s_map.setPosition(j * spriteSize, i * spriteSize);

			window.draw(s_map);//рисуем квадратики на экран
		}

		float coordinatePlayerX, coordinatePlayerY = 0;
		coordinatePlayerX = p.x;
		coordinatePlayerY = p.y;

		//Управление персонажем и камерой вида
		getPlayerView(p.x + (p.w / 2), p.y + (p.h / 2));
		scoreText.setPosition(view.getCenter().x - spriteSize * 4 + 6, view.getCenter().y - spriteSize * 4);
		infoText.setPosition(view.getCenter().x - spriteSize * 4 + 6, view.getCenter().y + spriteSize);
		if ((Keyboard::isKeyPressed(Keyboard::Left) || (Keyboard::isKeyPressed(Keyboard::A)))){
			p.dir = 1; p.speed = 0.125;//dir =1 - направление вверх, speed =0.125 - скорость движения.
			p.sprite.setTextureRect(IntRect(28, 0, p.w, p.h));
		}
		else if ((Keyboard::isKeyPressed(Keyboard::Right) || (Keyboard::isKeyPressed(Keyboard::D)))){
			p.dir = 0; p.speed = 0.125;//направление вправо, см выше
			p.sprite.setTextureRect(IntRect(0, 0, p.w, p.h));
		}
		else if ((Keyboard::isKeyPressed(Keyboard::Up) || (Keyboard::isKeyPressed(Keyboard::W)))){
			p.dir = 3; p.speed = 0.125;//направление вниз, см выше
		}
		else if ((Keyboard::isKeyPressed(Keyboard::Down) || (Keyboard::isKeyPressed(Keyboard::S)))) {
			p.dir = 2; p.speed = 0.125;//направление вверх, см выше
		}

		//Update
		viewmap(time);//функция скроллинга карты, передаем ей время sfml
		p.update(time);
		for (unsigned i = 0; i < enemies.size(); i++) enemies[i].update(time);

		window.setView(view);

		//Draw
		window.draw(s_map);
		window.draw(p.sprite);
		for (unsigned i = 0; i < enemies.size(); i++) window.draw(enemies[i].sprite);
		if (!p.life) {
			window.draw(wastedSprite);
			wastedText.setPosition(view.getCenter().x - spriteSize*1.5, view.getCenter().y - spriteSize);
			window.draw(wastedText);
			time = 0;
			view.setCenter(p.x + (p.w / 2), p.y + (p.h / 2));
			view.rotate(0.125);
			view.zoom(0.990f);
			cc += 1;
		}
		if (cc > 150) {
			view.setRotation(0);
			view.reset(sf::FloatRect(0, 0, windowSize / 2, windowSize / 2));
			p.respawn();
			cc = 0;
		}
		viewmap(time);
		t1 = std::clock();
		timeElapsed = (double)(t1 - t0) / CLOCKS_PER_SEC;
		stringstream ss;
		ss << p.getScoreString() << "\nTotal Score: " << countScore(p, timeElapsed);
		scoreText.setString(ss.str());
		infoText.setString(p.getInfoString());
		window.draw(scoreText);
		if(Keyboard::isKeyPressed(Keyboard::I)) window.draw(infoText);
		window.display();
	}


	if (p.totalMoney == 0) {
		printf("Your time %.2f seconds. Your score %u\n", timeElapsed, countScore(p, (unsigned)timeElapsed));
		Sleep(1000);
		system("pause");
	}

	return 0;
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

void randomMapGenerate(){ //Генерация монеток
	int randomElementX = 0;
	int randomElementY = 0;
	srand(time(0));
	int countCoins = 44;

	while (countCoins > 0){
		randomElementX = 1 + rand() % (WIDTH_MAP - 1);
		randomElementY = 1 + rand() % (HEIGHT_MAP - 1);

		if (map1[randomElementY][randomElementX] == '0') {
			map1[randomElementY][randomElementX] = '2';
			//std::cout << "coordinate of Stone X:" << randomElementX << "\n" << "coordinate of Stone Y:" << randomElementY << "\n\n";
			countCoins--;
		}
	}
}