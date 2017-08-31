////////////////////////////////////////////////////КЛАСС ИГРОКА////////////////////////
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "map.h"
#include <stdio.h>
#include <random>
#include <ctime>

using namespace sf;
using namespace std;

class Entity {
public:
	SoundBuffer sb;
	Sound sound;
	bool life = 1;
	float x, y, w, h, dx, dy, speed = 0; //координаты х и у, высота ширина, ускорение по х у, скорость
	unsigned dir; //направление (direction) движения
	Image image;//сфмл изображение
	Texture texture;//сфмл текстура
	Sprite sprite;//сфмл спрайт

	Entity(float X, float Y, String imagePath){
		w = 28; h = 28;//высота и ширина
		image.loadFromFile(imagePath);
		texture.loadFromImage(image);//закидываем наше изображение в текстуру
		sprite.setTexture(texture);//заливаем спрайт текстурой
		x = X; y = Y;//координата появления спрайта
		sprite.setTextureRect(IntRect(0, 0, w, h));
	}

	void update(float time); //функция "оживления" объекта класса. update - обновление.
	void interactionWithMap();//ф-ция взаимодействия с картой

	FloatRect getRect(){
		return FloatRect(x, y, w, h);
	}
};

////////////////////////////////////////////////////КЛАСС ВРАГА////////////////////////

class Enemy : public Entity {
private:
	void updateSprite(){
		sprite.setTexture(texture);
	}
	unsigned deathDelay = 1000;
public:
	Enemy(float X, float Y, String imagePath, bool horizont) : Entity(X, Y, imagePath){
		w = 28; h = 28;
		image.loadFromFile(imagePath);
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setTextureRect(IntRect(0, 0, w, h));
		if (horizont) { x = X; y = Y + 2; dir = 0; }
		else { x = X + 2; y = Y; dir = 2; }
		printf("Enemy created at %f : %f : %d\n", x, y, horizont);
		speed = 0.07;
	}

	void update(float time)
	{
		switch (dir)
		{
		case 0: dx = speed; dy = 0;   break;//персонаж идет только вправо
		case 1: dx = -speed; dy = 0;   break;//персонаж идет только влево
		case 2: dx = 0; dy = speed;   break;//персонаж идет только вниз
		case 3: dx = 0; dy = -speed;   break;//персонаж идет только вверх
		}

		updateSprite();
		if (life) {
			x += dx*time;
			y += dy*time;
		}
		sprite.setPosition(x, y);
		interactionWithMap();//вызываем функцию, отвечающую за взаимодействие с картой
	}

	void interactionWithMap()
	{
		for (int i = y / 32; i < (y + h) / 32; i++)
		for (int j = x / 32; j<(x + w) / 32; j++)
		{
			if (map1[i][j] == '1')
			{
				if (dy>0)
				{
					y = i * 32 - h;
					dir = 3;
				}
				if (dy<0)
				{
					y = i * 32 + 32;
					dir = 2;
				}
				if (dx>0)
				{
					x = j * 32 - w;
					dir = 1;
					sprite.setTextureRect(IntRect(28, 0, w, h));
				}
				if (dx < 0)
				{
					x = j * 32 + 32;
					dir = 0;
					sprite.setTextureRect(IntRect(0, 0, w, h));
				}
			}
		}
	}

};

////////////////////////////////////////////////////КЛАСС ИГРОКА////////////////////////

class Player : public Entity { // класс Игрока
private:
	vector<Enemy>::iterator it;

	bool playerCollide() {
		for (it = ptrCollidingEnemy->begin(); it != ptrCollidingEnemy->end();) {
			if (!(*it).life) return 0;
			else if (fabs(x - (*it).x) < w && fabs(y - (*it).y) < h) return 1;
			else if (x - (*it).x > w && y - (*it).y > h) return 0;
			else it++;
			return 0;
		}
	}

	void killEnemy() {
		sb.loadFromFile("sounds/Death.wav");
		sound.setBuffer(sb);
		sound.play();
		printf("Pendos killed\n");
		//life = 0;
		//speed = 0;
		enemies--;
		enemiesC++;
		incScore(1000);
		if (enemies == 0 && totalMoney == 0) done = 1;
	}

	void incScore(unsigned inc) { score += inc; }
	void decScore(unsigned dec) { score -= dec; }

public:
	bool manual = 0, moving, done = 0;
	int score = 0;
	unsigned dieCounter, totalMoney, enemies, enemiesC = 0; //направление (direction) движения игрока
	RenderWindow *ptrWindow;
	vector<Enemy> *ptrCollidingEnemy;

	Player(float X, float Y, String imagePath, RenderWindow *ptrW) : Entity(X, Y, imagePath) {
		w = 28; h = 28;//высота и ширина
		image.loadFromFile(imagePath);
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		x = X; y = Y;
		sprite.setTextureRect(IntRect(0, 0, w, h));
		ptrWindow = ptrW;
		countMoneyEnemies();
	}

	void countMoneyEnemies() {
		for (int i = 0; i < 24; i++)
		for (int j = 0; j < 24; j++)
		{
			if (map1[i][j] == '2') totalMoney++;
			if (map1[i][j] == '5' || map1[i][j] == '6') enemies++;
		}
	}

	void update(float time) {
		switch (dir)//реализуем поведение в зависимости от направления. (каждая цифра соответствует направлению)
		{
		case 0: dx = speed; dy = 0;   break;//по иксу задаем положительную скорость, по игреку зануляем. получаем, что персонаж идет только вправо
		case 1: dx = -speed; dy = 0;   break;//по иксу задаем отрицательную скорость, по игреку зануляем. получается, что персонаж идет только влево
		case 2: dx = 0; dy = speed;   break;//по иксу задаем нулевое значение, по игреку положительное. получается, что персонаж идет только вниз
		case 3: dx = 0; dy = -speed;   break;//по иксу задаем нулевое значение, по игреку отрицательное. получается, что персонаж идет только вверх
		}

		x += dx*time;//то движение из прошлого урока. наше ускорение на время получаем смещение координат и как следствие движение
		y += dy*time;//аналогично по игреку

		speed = 0;//зануляем скорость, чтобы персонаж остановился.
		sprite.setPosition(x, y); //выводим спрайт в позицию x y , посередине. бесконечно выводим в этой функции, иначе бы наш спрайт стоял на месте.
		interactionWithMap();//вызываем функцию, отвечающую за взаимодействие с картой
		if (!speed) moving = 1;
		else moving = 0;
		for (it = ptrCollidingEnemy->begin(); it != ptrCollidingEnemy->end();) {
			if ((*it).life == false) it = ptrCollidingEnemy->erase(it);
			else it++;
		}
	}

	void interactionWithMap()//ф-ция взаимодействия с картой
	{
		for (int i = y / 32; i < (y + h) / 32; i++)
		for (int j = x / 32; j<(x + w) / 32; j++)
		{
			if (map1[i][j] == '1')//если наш квадратик соответствует символу 0 (стена), то проверяем "направление скорости" персонажа:
			{
				if (dy>0)//если мы шли вниз,
				{
					y = i * 32 - h;//то стопорим координату игрек персонажа. сначала получаем координату нашего квадратика на карте(стены) и затем вычитаем из высоты спрайта персонажа.
				}
				if (dy<0)
				{
					y = i * 32 + 32;//аналогично с ходьбой вверх. dy<0, значит мы идем вверх (вспоминаем координаты паинта)
				}
				if (dx>0)
				{
					x = j * 32 - w;//если идем вправо, то координата Х равна стена (символ 0) минус ширина персонажа
				}
				if (dx < 0)
				{
					x = j * 32 + 32;//аналогично идем влево
				}
			}

			if (map1[i][j] == '2') { //если символ равен 's' (камень)
				map1[i][j] = '0';//убираем камень, типа взяли бонус.
				totalMoney--;
				sb.loadFromFile("sounds/Money.wav");
				sound.setBuffer(sb);
				sound.play();
				score += 100;
				if (enemies == 0 && totalMoney == 0) done = 1;
			}

			if (map1[i][j] == '3') if (done) ptrWindow->close();

			if (map1[i][j] == '4' && !manual) {
				printf("Hello, Player!\nYour mission is kill all enemies and collect all coins before you go to \"EXIT\".\nPress \"I\" for info.\nPress \"ECS\" to EXIT\n");
				manual = 1;
			}
		}

		//Colliding whith Enemy
		//It's a problem for now
		/*for (it = ptrCollidingEnemy->begin(); it != ptrCollidingEnemy->end(); it++)//проходимся по эл-там списка
		{
			if (playerCollide())//если прямоугольник спрайта объекта пересекается с игроком
			{
				if (dy > 0) { (*it).dx = 0; dy = -0.2; (*it).life = 0; }//если прыгнули на врага,то даем врагу скорость 0,отпрыгиваем от него чуть вверх,даем ему здоровье 0
				else {
					respawn();	//иначе враг подошел к нам сбоку и нанес урон
				}
			}
		}

		/*for (it = ptrCollidingEnemy->begin(); it != ptrCollidingEnemy->end();) {
			if (playerCollide() && (*it).dir != dir){
			if ((*it).life) {
			sb.loadFromFile("sounds/Enemy.wav");
			sound.setBuffer(sb);
			sound.play();
			dieCounter++;
			decScore(500);
			life = 0;
			printf("You respawned. Don't collide with pendos\n");
			}
			}
			else if (playerCollide() && (*it).dir == dir){
			if (dir == 0 && w + (*it).x - 1 < x) killEnemy();
			if (dir == 1 && w - (*it).x + 1 > x) killEnemy();
			if (dir == 2 && h + (*it).y - 1 < y) killEnemy();
			if (dir == 3 && h - (*it).y + 1 > y) killEnemy();
			}
			else it++;
			}*/
	}

	void respawn(){
		life = 1;
		x = 34;
		y = 34;
	}

	string getScoreString(){
		stringstream ss;
		ss << "Score: " << score;
		return ss.str();
	}

	string getInfoString(){
		stringstream ss;
		ss << "You died " << dieCounter;
		if (!done) ss << " times\nRemain: " << enemies << " enemies\n\t\t\t" << totalMoney << " coins";
		else ss << " times\nGo to EXIT";
		return ss.str();
	}
};
