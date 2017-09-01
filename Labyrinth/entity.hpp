#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "view.hpp"
#include "map.hpp"
#include <stdio.h>
#include <random>
#include <ctime>

using namespace sf;
using namespace std;

class Entity {
public:
	SoundBuffer sb;
	Sound sound;
	bool life = 1;
	float x, y, w, h, dx, dy, speed = 0;
	unsigned dir, id;
	Image image;
	Texture texture;
	Sprite sprite;

	Entity(float X, float Y, String imagePath){
		w = 28; h = 28;
		image.loadFromFile(imagePath);
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		x = X; y = Y;
		sprite.setTextureRect(IntRect(0, 0, w, h));
	}

	virtual void update(float time){};
	virtual void interactionWithMap(){};

	FloatRect getRect(){
		return FloatRect(x, y, w, h);
	}
};