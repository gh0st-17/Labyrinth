#include <SFML/Graphics.hpp>
using namespace sf;

sf::View view;//объ€вили sfml объект "вид", который и €вл€етс€ камерой

void getPlayerView(float x, float y) { //функци€ дл€ считывани€ координат игрока
	float tempX = x; float tempY = y;//считываем коорд игрока и провер€ем их, чтобы убрать кра€

	if (x < 32 * 4) tempX = 32 * 4;//left
	if (x > 512 + 32 * 4) tempX = 512 + 32 * 4;//right
	if (y < 32 * 4) tempY = 32 * 4;//top
	if (y > 512 + 32 * 4) tempY = 512 + 32 * 4;//bottom	

	view.setCenter(tempX, tempY); 
}


void viewmap(float time) {
	if (Keyboard::isKeyPressed(Keyboard::W)) view.move(0, -0.1*time);
	if (Keyboard::isKeyPressed(Keyboard::A)) view.move(-0.1*time, 0);
	if (Keyboard::isKeyPressed(Keyboard::S)) view.move(0, 0.1*time);
	if (Keyboard::isKeyPressed(Keyboard::D)) view.move(0.1*time, 0);
}
