#include "entity.hpp"

using namespace sf;
using namespace std;

class Bar : public Entity{
private:
	float percentage = 100;
	RenderWindow *ptrWindow;
	Texture blackT;

public:
	Sprite blackS;

	Bar(float X, float Y, String imagePath, String blackPath, RenderWindow *pWindow) : Entity(X, Y, imagePath){
		ptrWindow = pWindow;
		setWidth(100); setHeight(14);
		blackT.loadFromFile(blackPath);
		blackS.setTexture(blackT);
		sprite.setTextureRect(IntRect(0, 0, 100, 14));
	}

	void setBarPos(float X, float Y){
		sprite.setPosition(X - (100 - 56)/2, Y - 20);
	}

	void update(float &time){
		if (percentage >= 0){
			float offset = 100 - percentage;
			blackS.setTextureRect(IntRect(0, 0, offset, 14));
			blackS.setPosition(sprite.getPosition().x + (100 - offset), sprite.getPosition().y);
		}
	}

	float getPercentage(){ return percentage; }
	void setPercentage(float p){ percentage = p; }

};