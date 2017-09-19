#include <math.h>
#include "player.hpp"

using namespace sf;
using namespace std;

class Enemy : public Actor { // класс Врага
private:
	void updateSprite(){
		sprite.setTexture(texture);
	}

	void death(){
		printf("Pendos killed\n");
		speed = 0;
		setX(-getRect().width);
		setY(-getRect().height);
		life = 0;
	}

	Player *p;

	void interactionWithMap(){
		for (int i = getRect().top / 64; i < (getRect().top + getRect().height) / 64; i++)
			for (int j = getRect().left / 64; j < (getRect().left + getRect().width) / 64; j++){
				if (map1[i][j] == '1'){
					if (dy > 0){
						setY(i * 64 - getRect().height);
						dir = 3;
					}
					if (dy < 0){
						setY(i * 64 + 64);
						dir = 2;
					}
					if (dx > 0){
						setX(j * 64 - getRect().width);
						dir = 1;
						sprite.setTextureRect(IntRect(56, 0, getRect().width, getRect().height));
					}
					if (dx < 0){
						setX(j * 64 + 64);
						dir = 0;
						sprite.setTextureRect(IntRect(0, 0, getRect().width, getRect().height));
					}
				}
			}
		if (collide(p->getRect(), getRect()) && p->dir == dir && p->moved){
			if (dir == 0 && p->getRect().left + getRect().width - 1 < getRect().left) death();
			if (dir == 1 && p->getRect().left - getRect().width + 1 > getRect().left) death();
			if (dir == 2 && p->getRect().top + getRect().height - 1 < getRect().top) death();
			if (dir == 3 && p->getRect().top - getRect().height + 1 > getRect().top) death();
		}
		else if (collide(p->getRect(), getRect())){
			if (p->life) {
				if (p->colId != id){
					sb.loadFromFile("sounds/Enemy.wav");
					sound.setBuffer(sb);
					sound.play();
					p->decScore(500);
					p->colId = id;
					printf("You respawned. Don't collide with pendos\n");
				}
				p->setHealth(p->getHealth() - 1.0f);
				speed = 0;
			}
		}
		else if (!collide(p->getRect(), getRect())){
			speed = 0.126;
		}

		for (size_t i = 0; i < p->bullets.size(); i++){
			if (collide(p->bullets[i]->getRect(), getRect())){
				life = 0; p->bullets[i]->life = 0;
				printf("Pendos shooted\n");
				killedByBullet = 1;
			}
		}
	}

public:
	Enemy(float X, float Y, String imagePath, Player *P, bool horizont, unsigned ID) : Actor(X, Y, imagePath){
		TYPE = Actor::enemy;
		setHealth(100);
		p = P;
		id = ID;
		setWidth(56); setHeight(56);
		sprite.setTextureRect(IntRect(0, 0, getRect().width, getRect().height));
		if (horizont) { setX(X); setY(Y + 4); dir = 0; }
		else { setX(X + 4); setY(Y); dir = 2; }
		printf("Enemy created at %f : %f : %d : %u\n", getRect().left, getRect().top, horizont, ID);
		speed = 0.126;
	}

	void update(float &time)
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
			setX(getRect().left + dx * time);
			setY(getRect().top + dy * time);
		}
		sprite.setPosition(getRect().left, getRect().top);
		interactionWithMap();
	}
};