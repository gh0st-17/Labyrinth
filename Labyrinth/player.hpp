#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <stdio.h>
#include <random>
#include <ctime>
#include "Entity.hpp"


using namespace sf;
using namespace std;

class Player : public Entity { // ����� ������
private:
	void moving(){
		if ((Keyboard::isKeyPressed(Keyboard::Left) || (Keyboard::isKeyPressed(Keyboard::A)))){
			dir = 1; speed = 0.1275;//dir =1 - ����������� �����, speed =0.125 - �������� ��������.
			sprite.setTextureRect(IntRect(28, 0, w, h));
		}
		else if ((Keyboard::isKeyPressed(Keyboard::Right) || (Keyboard::isKeyPressed(Keyboard::D)))){
			dir = 0; speed = 0.1275;//����������� ������, �� ����
			sprite.setTextureRect(IntRect(0, 0, w, h));
		}
		else if ((Keyboard::isKeyPressed(Keyboard::Up) || (Keyboard::isKeyPressed(Keyboard::W)))){
			dir = 3; speed = 0.1275;//����������� ����, �� ����
		}
		else if ((Keyboard::isKeyPressed(Keyboard::Down) || (Keyboard::isKeyPressed(Keyboard::S)))) {
			dir = 2; speed = 0.1275;//����������� �����, �� ����
		}
	}

public:
	bool manual = 0, done = 0;
	int score = 0;
	unsigned dieCounter, totalMoney, enemies, enemiesC = 0; //����������� (direction) �������� ������
	RenderWindow *ptrWindow;

	Player(float X, float Y, String imagePath, RenderWindow *ptrW) : Entity(X, Y, imagePath){  //����������� � �����������(�����������) ��� ������ Player. ��� �������� ������� ������ �� ����� �������� ��� �����, ���������� � � �, ������ � ������
		w = 28; h = 28;//������ � ������
		image.loadFromFile(imagePath);//���������� � image ���� ����������� ������ File �� ��������� ��, ��� �������� ��� �������� �������. � ����� ������ "hero.png" � ��������� ������ ���������� 	image.loadFromFile("images/hero/png");
		texture.loadFromImage(image);//���������� ���� ����������� � ��������
		sprite.setTexture(texture);//�������� ������ ���������
		x = X; y = Y;//���������� ��������� �������
		sprite.setTextureRect(IntRect(0, 0, w, h));  //������ ������� ���� ������������� ��� ������ ������ ����, � �� ���� ����� �����. IntRect - ���������� �����
		ptrWindow = ptrW;
		countMoneyEnemies();
	}

	void countMoneyEnemies(){
		for (int i = 0; i < 24; i++)
		for (int j = 0; j < 24; j++)
		{
			if (map1[i][j] == '2') totalMoney++;
			if (map1[i][j] == '5' || map1[i][j] == '6') enemies++;
		}
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
		getPlayerView(x + (w / 2), y + (h / 2));//������ ����
		moving();//����������
	}

	void interactionWithMap()//�-��� �������������� � ������
	{
		for (int i = y / 32; i < (y + h) / 32; i++)//���������� �� ��������, �������������� � �������,, �� ���� �� ���� ����������� ������� 32*32, ������� �� ���������� � 9 �����. ��� ������� ������� ����.
		for (int j = x / 32; j< (x + w) / 32; j++)//��� ����� �� 32, ��� ����� �������� ����� ���������, � ������� �������� �������������.
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
				score += 150;
				if (enemies == 0 && totalMoney == 0) done = 1;
			}

			if (map1[i][j] == '3') if (done) ptrWindow->close();

			if (map1[i][j] == '4' && !manual) {
				printf("Hello, Player!\nYour mission is kill all enemies and collect all coins before you go to \"EXIT\".\nPress \"I\" for info.\nPress \"ECS\" to EXIT\n");
				manual = 1;
			}
		}
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

	void incScore(unsigned inc){
		score += inc;
	}
	void decScore(unsigned dec){
		score -= dec;
	}
};