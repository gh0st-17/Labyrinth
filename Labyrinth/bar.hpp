#include "entity.hpp"

using namespace sf;
using namespace std;

class Bar : public Entity{
private:
	bool visible = 1;
	float percentage = 100;
	Texture blackT;
	Sprite blackS;
	Sprite overlay;

public:

	Bar(float X, float Y, String imagePath) : Entity(X, Y, imagePath){
		setWidth(100); setHeight(14);
		blackT.loadFromFile("images/BarBack.png");
		blackS.setTexture(blackT);
		overlay.setTexture(blackT);
		sprite.setTextureRect(IntRect(0, 0, 100, 8));
		overlay.setTextureRect(IntRect(0, 0, 104, 14));
	}

	void setBarPos(float X, float Y){
		overlay.setPosition(X - (100 - 56) / 2, Y - 20);
		sprite.setPosition(overlay.getPosition().x + 2, overlay.getPosition().y + 3);
	}

	void update(float &time, RenderWindow *ptrWindow){
		if (percentage >= 0){
			float offset = 100 - percentage;
			blackS.setTextureRect(IntRect(2, 3, offset, 8));
			blackS.setPosition(sprite.getPosition().x + (100 - offset), sprite.getPosition().y);
		}
		ptrWindow->draw(overlay);
		ptrWindow->draw(sprite);
		ptrWindow->draw(blackS);
	}

	float getPercentage(){ return percentage; }
	void setPercentage(float p){ percentage = p; }
	bool getVisible(){ return visible; }
	void setVisible(bool v){ visible = v; }


};