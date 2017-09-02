﻿#include <math.h>
#include "player.hpp"

using namespace sf;
using namespace std;




class Enemy : public Entity { // класс Врага
private:
	void updateSprite(){
		sprite.setTexture(texture);
	}
	Player *p;
public:
	Enemy(float X, float Y, String imagePath, Player *P, bool horizont) : Entity(X, Y, imagePath){
		TYPE = Entity::enemy;
		health = 100;
		p = P;
		w = 28; h = 28;
		image.loadFromFile(imagePath);
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setTextureRect(IntRect(0, 0, w, h));
		if (horizont) { x = X; y = Y + 2; dir = 0; }
		else { x = X + 2; y = Y; dir = 2; }
		printf("Enemy created at %f : %f : %d\n", x, y, horizont);
		speed = 0.085;
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
		interactionWithMap();
	}

	void death(){
		printf("Pendos killed\n");
		speed = 0;
		x = -w;
		y = -h;
		life = 0;
	}

	void interactionWithMap(){
		for (int i = y / 32; i < (y + h) / 32; i++)
		for (int j = x / 32; j<(x + w) / 32; j++){
			if (map1[i][j] == '1'){
				if (dy>0){
					y = i * 32 - h;
					dir = 3;
				}
				if (dy<0){
					y = i * 32 + 32;
					dir = 2;
				}
				if (dx>0){
					x = j * 32 - w;
					dir = 1;
					sprite.setTextureRect(IntRect(28, 0, w, h));
				}
				if (dx < 0){
					x = j * 32 + 32;
					dir = 0;
					sprite.setTextureRect(IntRect(0, 0, w, h));
				}
			}
		}
			if (collide(p->getRect(), getRect()) && p->dir == dir && p->moved){
				if (dir == 0 && p->x + w - 1 < x) death();
				if (dir == 1 && p->x - w + 1 > x) death();
				if (dir == 2 && p->y + h - 1 < y) death();
				if (dir == 3 && p->y - h + 1 > y) death();
			}
			else if (collide(p->getRect(), getRect())){
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
			for (size_t i = 0; i < p->bullets.size(); i++){
				if (collide(p->bullets[i]->getRect(), getRect())){ 
					life = 0; p->bullets[i]->life = 0; 
					printf("Pendos shooted\n");
					killedByBullet = 1;
				}
			}
	}
};