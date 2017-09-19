#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <random>
#include <ctime>

using namespace sf;
using namespace std;

class Entity {
private:
	float x, y, w, h;
public:
	Image image;
	Texture texture;
	Sprite sprite;

	Entity(float X, float Y, String imagePath){
		image.loadFromFile(imagePath);
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		x = X; y = Y;
		sprite.setTextureRect(IntRect(0, 0, w, h));
	}

	virtual void update(float &time){};
	virtual void interactionWithMap(){};

	bool collide(FloatRect &A, FloatRect &B){ return A.intersects(B); }

	FloatRect getRect(){ return FloatRect(x, y, w, h); }
	//Sprite getSprite(){ return sprite; }

	//void setImage(Image &i){ image = i; }
	//void setTexture(Texture &t){ texture = t; }
	//void setSprite(Sprite &s){ sprite = s; }
	void setX(float f){ x = f; }
	void setY(float f){ y = f; }
	void setWidth(float f){ w = f; }
	void setHeight(float f){ h = f; }
};