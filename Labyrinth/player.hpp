#include "bullet.hpp"

using namespace sf;
using namespace std;

class Player : public Actor { // класс Игрока
private:
	vector<Actor*>::iterator bulletsIt;
	unsigned bulletDelay = 500, lowHP = 30;
	float * ptrTime;
	Bar * ShootBar = new Bar(getFloatRect().left, getFloatRect().top, "images/ShootBar.png");
	void speedChanger(){
		if ((Keyboard::isKeyPressed(Keyboard::LControl))) speed = 0.126f;
		else speed = 0.17f;
	}

	void keys(){

		if ((Keyboard::isKeyPressed(Keyboard::Left) || (Keyboard::isKeyPressed(Keyboard::A)))){
			dir = 1; speedChanger();//dir =1 - направление вверх, speed =0.125 - скорость движения.
			sprite.setTextureRect(IntRect(56, 0, getFloatRect().width, getFloatRect().height));
		}
		else if ((Keyboard::isKeyPressed(Keyboard::Right) || (Keyboard::isKeyPressed(Keyboard::D)))){
			dir = 0; speedChanger();//направление вправо, см выше
			sprite.setTextureRect(IntRect(0, 0, getFloatRect().width, getFloatRect().height));
		}
		else if ((Keyboard::isKeyPressed(Keyboard::Up) || (Keyboard::isKeyPressed(Keyboard::W)))){
			dir = 3; speedChanger();//направление вниз, см выше
		}
		else if ((Keyboard::isKeyPressed(Keyboard::Down) || (Keyboard::isKeyPressed(Keyboard::S)))) {
			dir = 2; speedChanger();//направление вверх, см выше
		}

		if ((Keyboard::isKeyPressed(Keyboard::Space))) {
			if (bulletDelay == 500){
				bullets.push_back(new Bullet(getFloatRect().left, getFloatRect().top, "images/Bullet.png", dir));
				bulletDelay = 0;
			}
			else bulletDelay += 20;
		}
		else
		{
			bulletDelay = 0;
		}
	}

	void countEnemies(){
		enemies = 0;
		for (int i = 1; i < 23; i++)
		for (int j = 1; j < 23; j++)
		{
			if (mapC[i][j] == '5' || mapC[i][j] == '6') enemies++;
		}
	}

	void interactionWithMap()//ф-ция взаимодействия с картой
	{
		for (int i = getFloatRect().top / 64; i < (getFloatRect().top + getFloatRect().height) / 64; i++)//проходимся по тайликам, контактирующим с игроком,, то есть по всем квадратикам размера 32*32, которые мы окрашивали в 9 уроке. про условия читайте ниже.
			for (int j = getFloatRect().left / 64; j < (getFloatRect().left + getFloatRect().width) / 64; j++)//икс делим на 32, тем самым получаем левый квадратик, с которым персонаж соприкасается.
		{
			if (mapC[i][j] == '1')//если наш квадратик соответствует символу 0 (стена), то проверяем "направление скорости" персонажа:
			{
				if (dy>0)//если мы шли вниз,
				{
					setY(i * 64 - getFloatRect().height);//то стопорим координату игрек персонажа. сначала получаем координату нашего квадратика на карте(стены) и затем вычитаем из высоты спрайта персонажа.
				}
				if (dy<0)
				{
					setY(i * 64 + 64);//аналогично с ходьбой вверх. dy<0, значит мы идем вверх (вспоминаем координаты паинта)
				}
				if (dx>0)
				{
					setX(j * 64 - getFloatRect().width);//если идем вправо, то координата Х равна стена (символ 0) минус ширина персонажа
				}
				if (dx < 0)
				{
					setX(j * 64 + 64);//аналогично идем влево
				}
			}
			if (mapC[i][j] == '2') {
				mapC[i][j] = '0';
				totalMoney--;
				sb.loadFromFile("sounds/Money.wav");
				sound.setBuffer(sb);
				sound.play();
				score += 200;
				if (enemies == 0 && totalMoney == 0) done = 1;
			}
			if (mapC[i][j] == '3' && done) {
				nextLvl = 1;
			}
			if (mapC[i][j] == '4' && !manual) {
				printf("Hello, Player!\nYour mission is kill all enemies and collect all coins before you go to \"EXIT\".\nPress \"TAB\" for info.\nPress \"R\" for Restart.\nPress \"ECS\" to EXIT\n");
				manual = 1;
			}
			if (mapC[i][j] == 'h' && getHealth() < 100) {
				mapC[i][j] = '0';
				sb.loadFromFile("sounds/Healing.wav");
				sound.setBuffer(sb);
				sound.play();
				setHealth(100);
			}
		}
	}

	void drawBars(float &time){
		HealthBar->setPercentage(getHealth());
		HealthBar->setBarPos(getFloatRect().left, getFloatRect().top);
		HealthBar->update(time);
		ptrWindow->draw(HealthBar->getOverlay());
		if (getHealth() <= 20){
			if (lowHP > 15){
				lowHP--;
			}
			else if (lowHP > 0){
				ptrWindow->draw(HealthBar->getSprite());
				lowHP--;
			}
			else lowHP = 30;
		}
		else ptrWindow->draw(HealthBar->getSprite());
		ptrWindow->draw(HealthBar->getBlackS());
		if ((Keyboard::isKeyPressed(Keyboard::Space)) && bulletDelay != 500) {
			ShootBar->setPercentage(bulletDelay / 5.0f);
			ShootBar->setBarPos(getFloatRect().left, getFloatRect().top - 11);
			ShootBar->update(time);
			ptrWindow->draw(ShootBar->getOverlay());
			ptrWindow->draw(ShootBar->getSprite());
			ptrWindow->draw(ShootBar->getBlackS());
		}
	}

public:
	std::vector<Actor*> bullets;
	bool manual, done = 0, nextLvl = 0, moved;
	float cacheX, cacheY;
	int score = 0;
	unsigned colId, dieCounter = 0, totalMoney, enemies, enemiesC = 0, lifeCount = 3;

	Player(float X, float Y, String imagePath, RenderWindow *ptrW) : Actor(X, Y, imagePath){  //Конструктор с параметрами(формальными) для класса Player. При создании объекта класса мы будем задавать имя файла, координату Х и У, ширину и высоту
		TYPE = Actor::player;
		setHealth(100);
		setWidth(56); setHeight(56);
		sprite.setTextureRect(IntRect(0, 0, getFloatRect().width, getFloatRect().height));  //Задаем спрайту один прямоугольник для вывода одного льва, а не кучи львов сразу. IntRect - приведение типов
		ptrWindow = ptrW;
		countEnemies();
	}

	void update(float &time) //функция "оживления" объекта класса. update - обновление. принимает в себя время SFML , вследствие чего работает бесконечно, давая персонажу движение.
	{
		switch (dir)//реализуем поведение в зависимости от направления. (каждая цифра соответствует направлению)
		{
		case 0: dx = speed; dy = 0;   break;//по иксу задаем положительную скорость, по игреку зануляем. получаем, что персонаж идет только вправо
		case 1: dx = -speed; dy = 0;   break;//по иксу задаем отрицательную скорость, по игреку зануляем. получается, что персонаж идет только влево
		case 2: dx = 0; dy = speed;   break;//по иксу задаем нулевое значение, по игреку положительное. получается, что персонаж идет только вниз
		case 3: dx = 0; dy = -speed;   break;//по иксу задаем нулевое значение, по игреку отрицательное. получается, что персонаж идет только вверх
		}

		setX(getFloatRect().left + dx * time);
		setY(getFloatRect().top + dy * time);

		speed = 0;//зануляем скорость, чтобы персонаж остановился.
		sprite.setPosition(getFloatRect().left, getFloatRect().top); //выводим спрайт в позицию x y , посередине. бесконечно выводим в этой функции, иначе бы наш спрайт стоял на месте.
		interactionWithMap();//вызываем функцию, отвечающую за взаимодействие с картой
		if(life) keys();//Управление
		if (cacheX == getFloatRect().left && cacheY == getFloatRect().top) moved = 0;
		else moved = 1;

		for (size_t i = 0; i < bullets.size(); i++){
			if (bullets[i]->life){
				bullets[i]->update(time);
				ptrWindow->draw(bullets[i]->sprite);
			} // Deleting "dead" bullets
			else bulletsIt = bullets.erase(bullets.begin() + i);
		}
		drawBars(time);
	}

	void respawn(){
		life = 1;
		setHealth(100);
		setX(68);
		setY(68);
	}

	void resetActor(){
		done = 0;
		score = 0;
		manual = 0;
		dieCounter = 0;
		enemiesC = 0;
		lifeCount = 3;
		countEnemies();
		respawn();
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