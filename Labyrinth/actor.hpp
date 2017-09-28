#include "view.hpp"
#include "mapProcessor.hpp"
#include "map.hpp"
#include "bar.hpp"
#include <stdio.h>

using namespace sf;
using namespace std;

class Actor : public Entity{
private:
	float health;

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
		HealthBar = new Bar(X, Y, "images/HealthBar.png");
		setX(X); setY(Y);
		image.loadFromFile(imagePath);
		texture.loadFromImage(image);
		sprite.setTexture(texture);
	}


	~Actor(){
		//HealthBar->~Bar();
		//image.~Image();
		//texture.~Texture();
		//sprite.~Sprite();
	}

	virtual void drawBars(float &time, RenderWindow &window){
		HealthBar->setPercentage(getHealth());
		HealthBar->setBarPos(getRect().left, getRect().top);
		HealthBar->update(time);
		window.draw(HealthBar->getOverlay());
		window.draw(HealthBar->getSprite());
		window.draw(HealthBar->getBlackS());
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