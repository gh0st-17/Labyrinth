#include "actor.hpp"

using namespace sf;
using namespace std;

class Bullet : public Actor {
public:

	Bullet(float X, float Y, String imagePath, unsigned &direction) : Actor(X, Y, imagePath){
		printf("%s Bullet created at [%.3f, %.3f]\n", getTime().c_str(), getRect().left, getRect().top);
		TYPE = Actor::bullet;
		dir = direction;
		switch (dir)
		{
		case 0: {
					sprite.setTextureRect(IntRect(0, 0, 24, 24));
					setWidth(24); setHeight(24);
					setY(getRect().top + ((64 - getRect().height) / 2));
					break;
		}
		case 1: {
					sprite.setTextureRect(IntRect(24, 0, 24, 24));
					setWidth(24); setHeight(24);
					setY(getRect().top + ((64 - getRect().height) / 2));
					break;
		}
		case 2: {
					sprite.setTextureRect(IntRect(48, 0, 24, 22));
					setWidth(22); setHeight(24);
					setX(getRect().left + ((64 - getRect().width) / 2));
					break;
		}
		case 3: {
					sprite.setTextureRect(IntRect(70, 0, 24, 22));
					setWidth(22); setHeight(24);
					setX(getRect().left + ((64 - getRect().width) / 2));
					break;
		}
		}
		speed = 0.21;
	}

	void update(float &time){
		switch (dir)
		{
		case 0: dx = speed; dy = 0;   break;//персонаж идет только вправо
		case 1: dx = -speed; dy = 0;   break;//персонаж идет только влево
		case 2: dx = 0; dy = speed;   break;//персонаж идет только вниз
		case 3: dx = 0; dy = -speed;   break;//персонаж идет только вверх
		}

		if (life) {
			setX(getRect().left + dx * time);
			setY(getRect().top + dy * time);
		}
		sprite.setPosition(getRect().left, getRect().top);
		interactionWithMap();
	}

	void interactionWithMap(){
		for (int i = getRect().top / 64; i < (getRect().top + getRect().height) / 64; i++)
			for (int j = getRect().left / 64; j < (getRect().left + getRect().width) / 64; j++){
				if (mapC[i][j] == '1'){
				life = 0;
			}
		}
	}

};
