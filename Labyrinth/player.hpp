#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <stdio.h>
#include <random>
#include <ctime>
#include "bullet.hpp"


using namespace sf;
using namespace std;

class Player : public Entity { // ����� ������
private:
	vector<Entity*>::iterator bulletsIt;
	unsigned bulletDelay = 200;

	void keys(){

		if ((Keyboard::isKeyPressed(Keyboard::Left) || (Keyboard::isKeyPressed(Keyboard::A)))){
			dir = 1; speed = 0.13;//dir =1 - ����������� �����, speed =0.125 - �������� ��������.
			sprite.setTextureRect(IntRect(56, 0, w, h));
		}
		else if ((Keyboard::isKeyPressed(Keyboard::Right) || (Keyboard::isKeyPressed(Keyboard::D)))){
			dir = 0; speed = 0.13;//����������� ������, �� ����
			sprite.setTextureRect(IntRect(0, 0, w, h));
		}
		else if ((Keyboard::isKeyPressed(Keyboard::Up) || (Keyboard::isKeyPressed(Keyboard::W)))){
			dir = 3; speed = 0.13;//����������� ����, �� ����
		}
		else if ((Keyboard::isKeyPressed(Keyboard::Down) || (Keyboard::isKeyPressed(Keyboard::S)))) {
			dir = 2; speed = 0.13;//����������� �����, �� ����
		}
		if ((Keyboard::isKeyPressed(Keyboard::Space))) {
			if (bulletDelay == 500){ bullets.push_back(new Bullet(x, y, "images/Bullet.png", dir)); bulletDelay = 0; }
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
		for (int i = y / 64; i < (y + h) / 64; i++)//���������� �� ��������, �������������� � �������,, �� ���� �� ���� ����������� ������� 32*32, ������� �� ���������� � 9 �����. ��� ������� ������� ����.
			for (int j = x / 64; j< (x + w) / 64; j++)//��� ����� �� 32, ��� ����� �������� ����� ���������, � ������� �������� �������������.
		{
			if (map1[i][j] == '1')//���� ��� ��������� ������������� ������� 0 (�����), �� ��������� "����������� ��������" ���������:
			{
				if (dy>0)//���� �� ��� ����,
				{
					y = i * 64 - h;//�� �������� ���������� ����� ���������. ������� �������� ���������� ������ ���������� �� �����(�����) � ����� �������� �� ������ ������� ���������.
				}
				if (dy<0)
				{
					y = i * 64 + 64;//���������� � ������� �����. dy<0, ������ �� ���� ����� (���������� ���������� ������)
				}
				if (dx>0)
				{
					x = j * 64 - w;//���� ���� ������, �� ���������� � ����� ����� (������ 0) ����� ������ ���������
				}
				if (dx < 0)
				{
					x = j * 64 + 64;//���������� ���� �����
				}
			}

			if (map1[i][j] == '2') { //���� ������ ����� 's' (������)
				map1[i][j] = '0';//������� ������, ���� ����� �����.
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
	vector<Entity*> bullets;
	float cacheX, cacheY;
	bool manual = 0, done = 0, moved;
	int score = 0;
	unsigned colId, dieCounter, totalMoney, enemies, enemiesC = 0;
	RenderWindow *ptrWindow;

	Player(float X, float Y, String imagePath, RenderWindow *ptrW) : Entity(X, Y, imagePath){  //����������� � �����������(�����������) ��� ������ Player. ��� �������� ������� ������ �� ����� �������� ��� �����, ���������� � � �, ������ � ������
		TYPE = Entity::player;
		health = 100;
		w = 28*2; h = 28*2;//������ � ������
		image.loadFromFile(imagePath);//���������� � image ���� ����������� ������ File �� ��������� ��, ��� �������� ��� �������� �������. � ����� ������ "hero.png" � ��������� ������ ���������� 	image.loadFromFile("images/hero/png");
		texture.loadFromImage(image);//���������� ���� ����������� � ��������
		sprite.setTexture(texture);//�������� ������ ���������
		x = X; y = Y;//���������� ��������� �������
		sprite.setTextureRect(IntRect(0, 0, w, h));  //������ ������� ���� ������������� ��� ������ ������ ����, � �� ���� ����� �����. IntRect - ���������� �����
		ptrWindow = ptrW;
		countMoneyEnemies();
	}


	void update(float time) //������� "���������" ������� ������. update - ����������. ��������� � ���� ����� SFML , ���������� ���� �������� ����������, ����� ��������� ��������.
	{
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
		if(life) keys();//����������
		if (cacheX == x && cacheY == y) moved = 0;
		else moved = 1;
		if (health = 0) { respawn(); health = 100; }
		for (size_t i = 0; i < bullets.size(); i++){
			if (bullets[i]->life){
				bullets[i]->update(time);
				ptrWindow->draw(bullets[i]->sprite);
			}
			else bulletsIt = bullets.erase(bullets.begin() + i);
		}
	}

	void respawn(){
		life = 1;
		x = 68;
		y = 68;
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