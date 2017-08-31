////////////////////////////////////////////////////����� ������////////////////////////
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
	float x, y, w, h, dx, dy, speed = 0; //���������� � � �, ������ ������, ��������� �� � �, ��������
	unsigned dir; //����������� (direction) ��������
	Image image;//���� �����������
	Texture texture;//���� ��������
	Sprite sprite;//���� ������

	Entity(float X, float Y, String imagePath){
		w = 28; h = 28;//������ � ������
		image.loadFromFile(imagePath);
		texture.loadFromImage(image);//���������� ���� ����������� � ��������
		sprite.setTexture(texture);//�������� ������ ���������
		x = X; y = Y;//���������� ��������� �������
		sprite.setTextureRect(IntRect(0, 0, w, h));
	}

	void update(float time); //������� "���������" ������� ������. update - ����������.
	void interactionWithMap();//�-��� �������������� � ������

	FloatRect getRect(){
		return FloatRect(x, y, w, h);
	}
};

////////////////////////////////////////////////////����� �����////////////////////////

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
		case 0: dx = speed; dy = 0;   break;//�������� ���� ������ ������
		case 1: dx = -speed; dy = 0;   break;//�������� ���� ������ �����
		case 2: dx = 0; dy = speed;   break;//�������� ���� ������ ����
		case 3: dx = 0; dy = -speed;   break;//�������� ���� ������ �����
		}

		updateSprite();
		if (life) {
			x += dx*time;
			y += dy*time;
		}
		sprite.setPosition(x, y);
		interactionWithMap();//�������� �������, ���������� �� �������������� � ������
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

////////////////////////////////////////////////////����� ������////////////////////////

class Player : public Entity { // ����� ������
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
	unsigned dieCounter, totalMoney, enemies, enemiesC = 0; //����������� (direction) �������� ������
	RenderWindow *ptrWindow;
	vector<Enemy> *ptrCollidingEnemy;

	Player(float X, float Y, String imagePath, RenderWindow *ptrW) : Entity(X, Y, imagePath) {
		w = 28; h = 28;//������ � ������
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
		switch (dir)//��������� ��������� � ����������� �� �����������. (������ ����� ������������� �����������)
		{
		case 0: dx = speed; dy = 0;   break;//�� ���� ������ ������������� ��������, �� ������ ��������. ��������, ��� �������� ���� ������ ������
		case 1: dx = -speed; dy = 0;   break;//�� ���� ������ ������������� ��������, �� ������ ��������. ����������, ��� �������� ���� ������ �����
		case 2: dx = 0; dy = speed;   break;//�� ���� ������ ������� ��������, �� ������ �������������. ����������, ��� �������� ���� ������ ����
		case 3: dx = 0; dy = -speed;   break;//�� ���� ������ ������� ��������, �� ������ �������������. ����������, ��� �������� ���� ������ �����
		}

		x += dx*time;//�� �������� �� �������� �����. ���� ��������� �� ����� �������� �������� ��������� � ��� ��������� ��������
		y += dy*time;//���������� �� ������

		speed = 0;//�������� ��������, ����� �������� �����������.
		sprite.setPosition(x, y); //������� ������ � ������� x y , ����������. ���������� ������� � ���� �������, ����� �� ��� ������ ����� �� �����.
		interactionWithMap();//�������� �������, ���������� �� �������������� � ������
		if (!speed) moving = 1;
		else moving = 0;
		for (it = ptrCollidingEnemy->begin(); it != ptrCollidingEnemy->end();) {
			if ((*it).life == false) it = ptrCollidingEnemy->erase(it);
			else it++;
		}
	}

	void interactionWithMap()//�-��� �������������� � ������
	{
		for (int i = y / 32; i < (y + h) / 32; i++)
		for (int j = x / 32; j<(x + w) / 32; j++)
		{
			if (map1[i][j] == '1')//���� ��� ��������� ������������� ������� 0 (�����), �� ��������� "����������� ��������" ���������:
			{
				if (dy>0)//���� �� ��� ����,
				{
					y = i * 32 - h;//�� �������� ���������� ����� ���������. ������� �������� ���������� ������ ���������� �� �����(�����) � ����� �������� �� ������ ������� ���������.
				}
				if (dy<0)
				{
					y = i * 32 + 32;//���������� � ������� �����. dy<0, ������ �� ���� ����� (���������� ���������� ������)
				}
				if (dx>0)
				{
					x = j * 32 - w;//���� ���� ������, �� ���������� � ����� ����� (������ 0) ����� ������ ���������
				}
				if (dx < 0)
				{
					x = j * 32 + 32;//���������� ���� �����
				}
			}

			if (map1[i][j] == '2') { //���� ������ ����� 's' (������)
				map1[i][j] = '0';//������� ������, ���� ����� �����.
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
		/*for (it = ptrCollidingEnemy->begin(); it != ptrCollidingEnemy->end(); it++)//���������� �� ��-��� ������
		{
			if (playerCollide())//���� ������������� ������� ������� ������������ � �������
			{
				if (dy > 0) { (*it).dx = 0; dy = -0.2; (*it).life = 0; }//���� �������� �� �����,�� ���� ����� �������� 0,����������� �� ���� ���� �����,���� ��� �������� 0
				else {
					respawn();	//����� ���� ������� � ��� ����� � ����� ����
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
