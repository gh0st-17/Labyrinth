#include <math.h>
#include "player.hpp"

using namespace sf;
using namespace std;

class Enemy : public Actor { // класс Врага
private:
	bool killPlayer = 0;
	unsigned soundLimit = 0;
	void updateSprite(){
		sprite.setTexture(texture);
	}

	void playEDamageSound(){
		sb.loadFromFile("sounds/EnemyDamage.wav");
		sound.setBuffer(sb);
		sound.play();
	}

	void damage(){
		setHealth(getHealth() - 1.0f);
		if (soundLimit == 15){
			playEDamageSound();
			soundLimit = 0;
		}
		else soundLimit++;
	}

	void death(){
		if(killedByBullet) printf("%s Pendos shooted\n", getTime().c_str());
		else printf("%s Pendos killed\n", getTime().c_str());
		speed = 0;
		setX(-getRect().width);
		setY(-getRect().height);
		life = 0;
	}

	Player *p;

	void interactionWithMap(){
		for (int i = getRect().top / 64; i < (getRect().top + getRect().height) / 64; i++)
			for (int j = getRect().left / 64; j < (getRect().left + getRect().width) / 64; j++){
				if (mapC[i][j] == '1'){
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
			if (dir == 0 && p->getRect().left + getRect().width - 28 < getRect().left) damage();
			if (dir == 1 && p->getRect().left - getRect().width + 28 > getRect().left) damage();
			if (dir == 2 && p->getRect().top + getRect().height - 28 < getRect().top) damage();
			if (dir == 3 && p->getRect().top - getRect().height + 28 > getRect().top) damage();
		}
		else if (collide(p->getRect(), getRect())){
			if (p->getHealth() > 0) {
				sb.loadFromFile("sounds/Damage.wav");
				sound.setBuffer(sb);
				sound.play();
				p->decScore(1);
				p->setHealth(p->getHealth() - 0.75f);
				speed = 0;
				killPlayer = 0;
			}
			else if (p->getHealth() <= 0 && !killPlayer) {
				killPlayer = 1;
				p->dieCounter++;
			}
		}
		else if (!collide(p->getRect(), getRect())){
			speed = 0.126;
		}

		for (size_t i = 0; i < p->bullets.size(); i++){
			if (collide(p->bullets[i]->getRect(), getRect())){
				playEDamageSound();
				setHealth(getHealth() - 10.0f);
				p->bullets[i]->life = 0;
				if(getHealth() <= 0) killedByBullet = 1;
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
		printf("%s Enemy created at [%.3f, %.3f] dir: %d\n", getTime().c_str(), getRect().left, getRect().top, horizont, ID);
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
		if (!getHealth()) death();
	}
};