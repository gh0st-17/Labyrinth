#include "bullet.hpp"


using namespace sf;
using namespace std;

class Player : public Actor { // ����� ������
private:
	vector<Actor*>::iterator bulletsIt;
	unsigned bulletDelay = 500;
	float * ptrTime;
	Bar * ShootBar = new Bar(getRect().left, getRect().top, "images/ShootBar.png");
	void speedChanger(){
		if ((Keyboard::isKeyPressed(Keyboard::LControl))) speed = 0.126;
		else speed = 0.17;
	}

	void keys(){

		if ((Keyboard::isKeyPressed(Keyboard::Left) || (Keyboard::isKeyPressed(Keyboard::A)))){
			dir = 1; speedChanger();//dir =1 - ����������� �����, speed =0.125 - �������� ��������.
			sprite.setTextureRect(IntRect(56, 0, getRect().width, getRect().height));
		}
		else if ((Keyboard::isKeyPressed(Keyboard::Right) || (Keyboard::isKeyPressed(Keyboard::D)))){
			dir = 0; speedChanger();//����������� ������, �� ����
			sprite.setTextureRect(IntRect(0, 0, getRect().width, getRect().height));
		}
		else if ((Keyboard::isKeyPressed(Keyboard::Up) || (Keyboard::isKeyPressed(Keyboard::W)))){
			dir = 3; speedChanger();//����������� ����, �� ����
		}
		else if ((Keyboard::isKeyPressed(Keyboard::Down) || (Keyboard::isKeyPressed(Keyboard::S)))) {
			dir = 2; speedChanger();//����������� �����, �� ����
		}

		if ((Keyboard::isKeyPressed(Keyboard::Space))) {
			if (bulletDelay == 500){
				bullets.push_back(new Bullet(getRect().left, getRect().top, "images/Bullet.png", dir));
				bulletDelay = 0;
			}
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

	void interactionWithMap()//�-��� �������������� � ������
	{

		for (int i = getRect().top / 64; i < (getRect().top + getRect().height) / 64; i++)//���������� �� ��������, �������������� � �������,, �� ���� �� ���� ����������� ������� 32*32, ������� �� ���������� � 9 �����. ��� ������� ������� ����.
			for (int j = getRect().left / 64; j < (getRect().left + getRect().width) / 64; j++)//��� ����� �� 32, ��� ����� �������� ����� ���������, � ������� �������� �������������.
		{
			if (map1[i][j] == '1')//���� ��� ��������� ������������� ������� 0 (�����), �� ��������� "����������� ��������" ���������:
			{
				if (dy>0)//���� �� ��� ����,
				{
					setY(i * 64 - getRect().height);//�� �������� ���������� ����� ���������. ������� �������� ���������� ������ ���������� �� �����(�����) � ����� �������� �� ������ ������� ���������.
				}
				if (dy<0)
				{
					setY(i * 64 + 64);//���������� � ������� �����. dy<0, ������ �� ���� ����� (���������� ���������� ������)
				}
				if (dx>0)
				{
					setX(j * 64 - getRect().width);//���� ���� ������, �� ���������� � ����� ����� (������ 0) ����� ������ ���������
				}
				if (dx < 0)
				{
					setX(j * 64 + 64);//���������� ���� �����
				}
			}
			if (map1[i][j] == '2') {
				map1[i][j] = '0';
				totalMoney--;
				sb.loadFromFile("sounds/Money.wav");
				sound.setBuffer(sb);
				sound.play();
				score += 200;
				if (enemies == 0 && totalMoney == 0) done = 1;
			}
			if (map1[i][j] == '3') if (done) ptrWindow->close();
			if (map1[i][j] == '4' && !manual) {
				printf("Hello, Player!\nYour mission is kill all enemies and collect all coins before you go to \"EXIT\".\nPress \"TAB\" for info.\nPress \"R\" for Restart.\nPress \"ECS\" to EXIT\n");
				manual = 1;
			}
			if (map1[i][j] == 'h' && getHealth() < 100) {
				map1[i][j] = '0';
				sb.loadFromFile("sounds/Healing.wav");
				sound.setBuffer(sb);
				sound.play();
				setHealth(100);
			}
		}
	}

	void drawBars(float &time){
		HealthBar->setPercentage(getHealth());
		HealthBar->setBarPos(getRect().left, getRect().top);
		HealthBar->update(time);
		ptrWindow->draw(HealthBar->getOverlay());
		ptrWindow->draw(HealthBar->getSprite());
		ptrWindow->draw(HealthBar->getBlackS());
		if ((Keyboard::isKeyPressed(Keyboard::Space)) && bulletDelay != 500) {
			ShootBar->setPercentage(bulletDelay / 5);
			ShootBar->setBarPos(getRect().left, getRect().top - 11);
			ShootBar->update(time);
			ptrWindow->draw(ShootBar->getOverlay());
			ptrWindow->draw(ShootBar->getSprite());
			ptrWindow->draw(ShootBar->getBlackS());
		}
	}

public:
	vector<Actor*> bullets;
	float cacheX, cacheY;
	bool manual = 0, done = 0, moved;
	int score = 0;
	unsigned colId, dieCounter, totalMoney, enemies, enemiesC = 0;

	Player(float X, float Y, String imagePath, RenderWindow *ptrW) : Actor(X, Y, imagePath){  //����������� � �����������(�����������) ��� ������ Player. ��� �������� ������� ������ �� ����� �������� ��� �����, ���������� � � �, ������ � ������
		TYPE = Actor::player;
		setHealth(100);
		setWidth(56); setHeight(56);
		sprite.setTextureRect(IntRect(0, 0, getRect().width, getRect().height));  //������ ������� ���� ������������� ��� ������ ������ ����, � �� ���� ����� �����. IntRect - ���������� �����
		ptrWindow = ptrW;
		countMoneyEnemies();
	}



	void update(float &time) //������� "���������" ������� ������. update - ����������. ��������� � ���� ����� SFML , ���������� ���� �������� ����������, ����� ��������� ��������.
	{
		switch (dir)//��������� ��������� � ����������� �� �����������. (������ ����� ������������� �����������)
		{
		case 0: dx = speed; dy = 0;   break;//�� ���� ������ ������������� ��������, �� ������ ��������. ��������, ��� �������� ���� ������ ������
		case 1: dx = -speed; dy = 0;   break;//�� ���� ������ ������������� ��������, �� ������ ��������. ����������, ��� �������� ���� ������ �����
		case 2: dx = 0; dy = speed;   break;//�� ���� ������ ������� ��������, �� ������ �������������. ����������, ��� �������� ���� ������ ����
		case 3: dx = 0; dy = -speed;   break;//�� ���� ������ ������� ��������, �� ������ �������������. ����������, ��� �������� ���� ������ �����
		}

		setX(getRect().left + dx * time);
		setY(getRect().top + dy * time);

		speed = 0;//�������� ��������, ����� �������� �����������.
		sprite.setPosition(getRect().left, getRect().top); //������� ������ � ������� x y , ����������. ���������� ������� � ���� �������, ����� �� ��� ������ ����� �� �����.
		interactionWithMap();//�������� �������, ���������� �� �������������� � ������
		if(life) keys();//����������
		if (cacheX == getRect().left && cacheY == getRect().top) moved = 0;
		else moved = 1;

		for (size_t i = 0; i < bullets.size(); i++){
			if (bullets[i]->life){
				bullets[i]->update(time);
				ptrWindow->draw(bullets[i]->sprite);
			}
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