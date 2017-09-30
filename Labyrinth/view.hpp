#include <SFML/Graphics.hpp>
using namespace sf;

sf::View view;//объ¤вили sfml объект "вид", который и ¤вл¤етс¤ камерой

void getPlayerView(float x, float y) { //функци¤ дл¤ считывани¤ координат игрока
	float tempX = x; float tempY = y;//считываем коорд игрока и провер¤ем их, чтобы убрать кра¤

	if (x < 64 * 4) tempX = 64 * 4;//left
	if (x > 512 + 64 * 12) tempX = 512 + 64 * 12;//right
	if (y < 64 * 4) tempY = 64 * 4;//top
	if (y > 512 + 64 * 12) tempY = 512 + 64 * 12;//bottom	

	view.setCenter(tempX, tempY); 
}


void viewmap(float time) {
	if (Keyboard::isKeyPressed(Keyboard::W)) view.move(0.0f, -0.1f * time);
	if (Keyboard::isKeyPressed(Keyboard::A)) view.move(-0.1f * time, 0.0f);
	if (Keyboard::isKeyPressed(Keyboard::S)) view.move(0.0f, 0.1f * time);
	if (Keyboard::isKeyPressed(Keyboard::D)) view.move(0.1f * time, 0.0f);
}
