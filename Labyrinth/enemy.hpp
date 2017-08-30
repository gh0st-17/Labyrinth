////////////////////////////////////////////////////����� �����////////////////////////
#include <math.h>
#include "player.hpp"

using namespace sf;
using namespace std;

class Enemy : public Entity { // ����� ������
private:
	void updateSprite(){
		sprite.setTexture(texture);
	}
	Player *p;
public:

	Enemy(float X, float Y, String imagePath, Player *P, bool horizont) : Entity(X, Y, imagePath){
		p = P;
		w = 28; h = 28;//������ � ������
		image.loadFromFile(imagePath);
		texture.loadFromImage(image);//���������� ���� ����������� � ��������
		sprite.setTexture(texture);//�������� ������ ���������
		sprite.setTextureRect(IntRect(0, 0, w, h));
		if (horizont) { x = X; y = Y + 2; dir = 0; }
		else { x = X + 2; y = Y; dir = 2; }
		printf("Enemy created at %f : %f : %d\n", x, y, horizont);
		speed = 0.07;
	}

	void update(float time) //������� "���������" ������� ������. update - ����������. ��������� � ���� ����� SFML , ���������� ���� �������� ����������, ����� ��������� ��������.
	{
		switch (dir)//��������� ��������� � ����������� �� �����������. (������ ����� ������������� �����������)
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
		sprite.setPosition(x, y); //������� ������ � ������� x y , ����������. ���������� ������� � ���� �������, ����� �� ��� ������ ����� �� �����.
		interactionWithMap();//�������� �������, ���������� �� �������������� � ������
	}

	void death(){
		sb.loadFromFile("sounds/Death.wav");
		sound.setBuffer(sb);
		sound.play();
		printf("Pendos killed\n");
		life = 0;
		speed = 0;
		x = -w;
		y = -h;
		p->enemies--;
		p->enemiesC++;
		p->incScore(1000);
		if (p->enemies == 0 && p->totalMoney == 0) p->done = 1;
	}

	bool playerCollide(){
		if (fabs(p->x - x) < w && fabs(p->y - y) < h) return 1;
		if (p->x - x > w && p->y - y > h) return 0;
		return 0;
	}

	void interactionWithMap()//�-��� �������������� � ������
	{
		for (int i = y / 32; i < (y + h) / 32; i++)//���������� �� ��������, �������������� � �������,, �� ���� �� ���� ����������� ������� 32*32, ������� �� ���������� � 9 �����. ��� ������� ������� ����.
		for (int j = x / 32; j<(x + w) / 32; j++)//��� ����� �� 32, ��� ����� �������� ����� ���������, � ������� �������� �������������. (�� ���� ������ ������� 32*32, ������� ����� ������������ ������ �� ���������� ���������). � j<(x + w) / 32 - ������� ����������� ��������� �� ����. �� ���� ���������� ������ ������� ��������, ������� ������������� � ����������. ����� ������� ���� � ����� ����� ������� �� ����, ������� �� �� ������ �������� (���������������� � ������), �� ������� �������� (���������������� � ������)
		{
			if (map1[i][j] == '1')//���� ��� ��������� ������������� ������� 0 (�����), �� ��������� "����������� ��������" ���������:
			{
				if (dy>0)//���� �� ��� ����,
				{
					y = i * 32 - h;//�� �������� ���������� ����� ���������. ������� �������� ���������� ������ ���������� �� �����(�����) � ����� �������� �� ������ ������� ���������.
					dir = 3;
				}
				if (dy<0)
				{
					y = i * 32 + 32;//���������� � ������� �����. dy<0, ������ �� ���� ����� (���������� ���������� ������)
					dir = 2;
				}
				if (dx>0)
				{
					x = j * 32 - w;//���� ���� ������, �� ���������� � ����� ����� (������ 0) ����� ������ ���������
					dir = 1;
					sprite.setTextureRect(IntRect(28, 0, w, h));
				}
				if (dx < 0)
				{
					x = j * 32 + 32;//���������� ���� �����
					dir = 0;
					sprite.setTextureRect(IntRect(0, 0, w, h));
				}
			}

			if (playerCollide() && p->dir != dir){
				if (p->life) {
					sb.loadFromFile("sounds/Enemy.wav");
					sound.setBuffer(sb);
					sound.play();
					p->dieCounter++;
					p->decScore(500);
					p->life = 0;
					printf("You respawned. Don't collide with pendos\n");
				}
			}
			else if (playerCollide() && p->dir == dir){
				if (dir == 0 && p->x + w - 1 < x) death();
				if (dir == 1 && p->x - w + 1 > x) death();
				if (dir == 2 && p->y + h - 1 < y) death();
				if (dir == 3 && p->y - h + 1 > y) death();
			}
		}
	}
};