#include "entity.hpp"

using namespace sf;
using namespace std;

class Bullet : public Entity {
public:

	Bullet(float X, float Y, String imagePath, unsigned &direction) : Entity(X, Y, imagePath){
		printf("Bullet created at %1f %1f\n", x, y);
		TYPE = Entity::bullet;
		dir = direction;
		image.loadFromFile(imagePath);
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		x = X; y = Y;
		switch (dir)
		{
		case 0: {
					sprite.setTextureRect(IntRect(0, 0, 24, 24));
					w = 24; h = 24;
					y += (64 - h)/2;
					break;
		}
		case 1: {
					sprite.setTextureRect(IntRect(24, 0, 24, 24));
					w = 24; h = 24;
					y += (64 - h) / 2;
					break;
		}
		case 2: {
					sprite.setTextureRect(IntRect(48, 0, 24, 22));
					w = 22; h = 24;
					x += (64 - w) / 2;
					break;
		}
		case 3: {
					sprite.setTextureRect(IntRect(70, 0, 24, 22));
					w = 22; h = 24;
					x += (64 - w) / 2;
					break;
		}
		}
		speed = 0.19;
	}

	void update(float time){
		switch (dir)
		{
		case 0: dx = speed; dy = 0;   break;//персонаж идет только вправо
		case 1: dx = -speed; dy = 0;   break;//персонаж идет только влево
		case 2: dx = 0; dy = speed;   break;//персонаж идет только вниз
		case 3: dx = 0; dy = -speed;   break;//персонаж идет только вверх
		}

		if (life) {
			x += dx*time;
			y += dy*time;
		}
		sprite.setPosition(x, y);
		interactionWithMap();
	}

	void interactionWithMap(){
		for (int i = y / 64; i < (y + h) / 64; i++)
		for (int j = x / 64; j < (x + w) / 64; j++){
			if (map1[i][j] == '1'){
				life = 0;
			}
		}
	}

};
