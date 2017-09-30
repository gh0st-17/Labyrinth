#include "actor.hpp"

using namespace sf;
using namespace std;

class Bullet : public Actor {
public:

	Bullet(float X, float Y, String imagePath, unsigned &direction) : Actor(X, Y, imagePath){
		printf("%s Bullet created at [%.3f, %.3f]\n", getTime().c_str(), getFloatRect().left, getFloatRect().top);
		TYPE = Actor::bullet;
		dir = direction;
		switch (dir)
		{
		case 0: {
					sprite.setTextureRect(IntRect(0, 0, 24, 24));
					setWidth(24); setHeight(24);
					setY(getFloatRect().top + ((64 - getFloatRect().height) / 2));
					break;
		}
		case 1: {
					sprite.setTextureRect(IntRect(24, 0, 24, 24));
					setWidth(24); setHeight(24);
					setY(getFloatRect().top + ((64 - getFloatRect().height) / 2));
					break;
		}
		case 2: {
					sprite.setTextureRect(IntRect(48, 0, 24, 22));
					setWidth(22); setHeight(24);
					setX(getFloatRect().left + ((64 - getFloatRect().width) / 2));
					break;
		}
		case 3: {
					sprite.setTextureRect(IntRect(70, 0, 24, 22));
					setWidth(22); setHeight(24);
					setX(getFloatRect().left + ((64 - getFloatRect().width) / 2));
					break;
		}
		}
		speed = 0.21f;
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
			setX(getFloatRect().left + dx * time);
			setY(getFloatRect().top + dy * time);
		}
		sprite.setPosition(getFloatRect().left, getFloatRect().top);
		interactionWithMap();
	}

	void interactionWithMap(){
		for (int i = getFloatRect().top / 64; i < (getFloatRect().top + getFloatRect().height) / 64; i++)
			for (int j = getFloatRect().left / 64; j < (getFloatRect().left + getFloatRect().width) / 64; j++){
				if (mapC[i][j] == '1'){
				life = 0;
			}
		}
	}

};
