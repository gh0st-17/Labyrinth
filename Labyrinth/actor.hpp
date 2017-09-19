#include "view.hpp"
#include "map.hpp"
#include "bar.hpp"
#include <stdio.h>

using namespace sf;
using namespace std;

class Actor : public Entity{
private:
	unsigned health;
public:
	enum { player, enemy, bullet } TYPE;
	SoundBuffer sb;
	Sound sound;
	Bar * HealthBar;
	bool life = 1, killedByBullet = 0;
	float dx, dy, speed = 0;
	unsigned dir, id;
	RenderWindow *ptrWindow;

	Actor(float X, float Y, String imagePath) : Entity(X, Y, imagePath){
		setWidth(28); setHeight(28);
		HealthBar = new Bar(X, Y - 10, "images/HealthBar.png", "images/HealthBarBlack.png", ptrWindow);
		setX(X); setY(Y);
		image.loadFromFile(imagePath);
		texture.loadFromImage(image);
		sprite.setTexture(texture);
	}

	void setHealth(float h){
		if (h < 0){
			health = 0;
		}
		else if (h >= 0){
			health = h;
		}
		if (health == 0) life = 0;
	}

	float getHealth(){ return health; }

};