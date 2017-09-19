#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <stdio.h>
#include <random>
#include <ctime>
#include "bullet.hpp"


using namespace sf;
using namespace std;

class Player : public Actor { // класс Игрока
private:
	vector<Actor*>::iterator bulletsIt;
	unsigned bulletDelay = 200, colIdDelay = 100;

	void keys(){

		if ((Keyboard::isKeyPressed(Keyboard::Left) || (Keyboard::isKeyPressed(Keyboard::A)))){
			dir = 1; speed = 0.17;//dir =1 - направление вверх, speed =0.125 - скорость движения.
			sprite.setTextureRect(IntRect(56, 0, getRect().width, getRect().height));
		}
		else if ((Keyboard::isKeyPressed(Keyboard::Right) || (Keyboard::isKeyPressed(Keyboard::D)))){
			dir = 0; speed = 0.17;//направление вправо, см выше
			sprite.setTextureRect(IntRect(0, 0, getRect().width, getRect().height));
		}
		else if ((Keyboard::isKeyPressed(Keyboard::Up) || (Keyboard::isKeyPressed(Keyboard::W)))){
			dir = 3; speed = 0.17;//направление вниз, см выше
		}
		else if ((Keyboard::isKeyPressed(Keyboard::Down) || (Keyboard::isKeyPressed(Keyboard::S)))) {
			dir = 2; speed = 0.17;//направление вверх, см выше
		}
		if ((Keyboard::isKeyPressed(Keyboard::Space))) {
			if (bulletDelay == 500){ bullets.push_back(new Bullet(getRect().left, getRect().top, "images/Bullet.png", dir)); bulletDelay = 0; }
			else bulletDelay += 20;
		}
		else
		{
			bulletDelay = 0;
		}
	}

	void countMoneyEnemies(){
		for (int i = 0; i < 24; i++)
		for (int j = 0; j < 24; j++)
		{
			if (map1[i][j] == '2') totalMoney++;
			if (map1[i][j] == '5' || map1[i][j] == '6') enemies++;
		}
	}

	void interactionWithMap()//ф-ция взаимодействия с картой
	{
		for (int i = getRect().top / 64; i < (getRect().top + getRect().height) / 64; i++)//проходимся по тайликам, контактирующим с игроком,, то есть по всем квадратикам размера 32*32, которые мы окрашивали в 9 уроке. про условия читайте ниже.
			for (int j = getRect().left / 64; j < (getRect().left + getRect().width) / 64; j++)//икс делим на 32, тем самым получаем левый квадратик, с которым персонаж соприкасается.
		{
			if (map1[i][j] == '1')//если наш квадратик соответствует символу 0 (стена), то проверяем "направление скорости" персонажа:
			{
				if (dy>0)//если мы шли вниз,
				{
					setY(i * 64 - getRect().height);//то стопорим координату игрек персонажа. сначала получаем координату нашего квадратика на карте(стены) и затем вычитаем из высоты спрайта персонажа.
				}
				if (dy<0)
				{
					setY(i * 64 + 64);//аналогично с ходьбой вверх. dy<0, значит мы идем вверх (вспоминаем координаты паинта)
				}
				if (dx>0)
				{
					setX(j * 64 - getRect().width);//если идем вправо, то координата Х равна стена (символ 0) минус ширина персонажа
				}
				if (dx < 0)
				{
					setX(j * 64 + 64);//аналогично идем влево
				}
			}

			if (map1[i][j] == '2') { //если символ равен 's' (камень)
				map1[i][j] = '0';//убираем камень, типа взяли бонус.
				totalMoney--;
				sb.loadFromFile("sounds/Money.wav");
				sound.setBuffer(sb);
				sound.play();
				score += 150;
				if (enemies == 0 && totalMoney == 0) done = 1;
			}

			if (map1[i][j] == '3') if (done) ptrWindow->close();

			if (map1[i][j] == '4' && !manual) {
				printf("Hello, Player!\nYour mission is kill all enemies and collect all coins before you go to \"EXIT\".\nPress \"TAB\" for info.\nPress \"R\" for Restart.\nPress \"ECS\" to EXIT\n");
				manual = 1;
			}
		}
	}

public:
	vector<Actor*> bullets;
	float cacheX, cacheY;
	bool manual = 0, done = 0, moved;
	int score = 0;
	unsigned colId, dieCounter, totalMoney, enemies, enemiesC = 0;

	Player(float X, float Y, String imagePath, RenderWindow *ptrW) : Actor(X, Y, imagePath){  //Конструктор с параметрами(формальными) для класса Player. При создании объекта класса мы будем задавать имя файла, координату Х и У, ширину и высоту
		TYPE = Actor::player;
		setHealth(100);
		setWidth(56); setHeight(56);
		sprite.setTextureRect(IntRect(0, 0, getRect().width, getRect().height));  //Задаем спрайту один прямоугольник для вывода одного льва, а не кучи львов сразу. IntRect - приведение типов
		ptrWindow = ptrW;
		countMoneyEnemies();
	}


	void update(float &time) //функция "оживления" объекта класса. update - обновление. принимает в себя время SFML , вследствие чего работает бесконечно, давая персонажу движение.
	{
		//cout << colId << endl;
		switch (dir)//реализуем поведение в зависимости от направления. (каждая цифра соответствует направлению)
		{
		case 0: dx = speed; dy = 0;   break;//по иксу задаем положительную скорость, по игреку зануляем. получаем, что персонаж идет только вправо
		case 1: dx = -speed; dy = 0;   break;//по иксу задаем отрицательную скорость, по игреку зануляем. получается, что персонаж идет только влево
		case 2: dx = 0; dy = speed;   break;//по иксу задаем нулевое значение, по игреку положительное. получается, что персонаж идет только вниз
		case 3: dx = 0; dy = -speed;   break;//по иксу задаем нулевое значение, по игреку отрицательное. получается, что персонаж идет только вверх
		}

		setX(getRect().left + dx * time);
		setY(getRect().top + dy * time);

		speed = 0;//зануляем скорость, чтобы персонаж остановился.
		sprite.setPosition(getRect().left, getRect().top); //выводим спрайт в позицию x y , посередине. бесконечно выводим в этой функции, иначе бы наш спрайт стоял на месте.
		interactionWithMap();//вызываем функцию, отвечающую за взаимодействие с картой
		if(life) keys();//Управление
		if (cacheX == getRect().left && cacheY == getRect().top) moved = 0;
		else moved = 1;
		if (getHealth() == 0) {
			respawn();
			dieCounter++;
			setHealth(100);
		}
		for (size_t i = 0; i < bullets.size(); i++){
			if (bullets[i]->life){
				bullets[i]->update(time);
				ptrWindow->draw(bullets[i]->sprite);
			}
			else bulletsIt = bullets.erase(bullets.begin() + i);
		}

		HealthBar->setPercentage(getHealth());
		HealthBar->setBarPos(getRect().left, getRect().top);
		HealthBar->update(time);
		ptrWindow->draw(HealthBar->sprite);
		ptrWindow->draw(HealthBar->blackS);

		if (colId){
			if (colIdDelay){
				colIdDelay--;
			}
			else{
				colId = 0;
				colIdDelay = 100;
			}
		}
	}

	void respawn(){
		life = 1;
		setX(68);
		setY(68);
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

	void incScore(unsigned inc){
		score += inc;
	}
	void decScore(unsigned dec){
		score -= dec;
	}
};