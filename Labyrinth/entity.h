////////////////////////////////////////////////////КЛАСС ИГРОКА////////////////////////
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <stdio.h>
#include <random>
#include <ctime>

using namespace sf;
using namespace std;

class Entity { // класс Игрока
public:
	SoundBuffer sb;
	Sound sound;
	bool life = 1;
	float x, y, w, h, dx, dy, speed = 0; //координаты х и у, высота ширина, ускорение по х у, скорость
	unsigned dir; //направление (direction) движения
	Image image;//сфмл изображение
	Texture texture;//сфмл текстура
	Sprite sprite;//сфмл спрайт

	Entity(float X, float Y, String imagePath){
		w = 28; h = 28;//высота и ширина
		image.loadFromFile(imagePath);
		texture.loadFromImage(image);//закидываем наше изображение в текстуру
		sprite.setTexture(texture);//заливаем спрайт текстурой
		x = X; y = Y;//координата появления спрайта
		sprite.setTextureRect(IntRect(0, 0, w, h));
	}

	void update(float time); //функция "оживления" объекта класса. update - обновление.
	void interactionWithMap();//ф-ция взаимодействия с картой
};