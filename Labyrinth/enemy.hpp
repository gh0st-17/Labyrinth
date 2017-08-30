////////////////////////////////////////////////////КЛАСС ВРАГА////////////////////////
#include <math.h>
#include "player.hpp"

using namespace sf;
using namespace std;

class Enemy : public Entity { // класс Игрока
private:
	void updateSprite(){
		sprite.setTexture(texture);
	}
	Player *p;
public:

	Enemy(float X, float Y, String imagePath, Player *P, bool horizont) : Entity(X, Y, imagePath){
		p = P;
		w = 28; h = 28;//высота и ширина
		image.loadFromFile(imagePath);
		texture.loadFromImage(image);//закидываем наше изображение в текстуру
		sprite.setTexture(texture);//заливаем спрайт текстурой
		sprite.setTextureRect(IntRect(0, 0, w, h));
		if (horizont) { x = X; y = Y + 2; dir = 0; }
		else { x = X + 2; y = Y; dir = 2; }
		printf("Enemy created at %f : %f : %d\n", x, y, horizont);
		speed = 0.07;
	}

	void update(float time) //функция "оживления" объекта класса. update - обновление. принимает в себя время SFML , вследствие чего работает бесконечно, давая персонажу движение.
	{
		switch (dir)//реализуем поведение в зависимости от направления. (каждая цифра соответствует направлению)
		{
		case 0: dx = speed; dy = 0;   break;//персонаж идет только вправо
		case 1: dx = -speed; dy = 0;   break;//персонаж идет только влево
		case 2: dx = 0; dy = speed;   break;//персонаж идет только вниз
		case 3: dx = 0; dy = -speed;   break;//персонаж идет только вверх
		}

		updateSprite();
		if (life) {
			x += dx*time;
			y += dy*time;
		}
		sprite.setPosition(x, y); //выводим спрайт в позицию x y , посередине. бесконечно выводим в этой функции, иначе бы наш спрайт стоял на месте.
		interactionWithMap();//вызываем функцию, отвечающую за взаимодействие с картой
	}

	void death(){
		sb.loadFromFile("sounds/Death.wav");
		sound.setBuffer(sb);
		sound.play();
		printf("Pendos killed\n");
		life = 0;
		speed = 0;
		x = -w;
		y = -h;
		p->enemies--;
		p->enemiesC++;
		p->incScore(1000);
		if (p->enemies == 0 && p->totalMoney == 0) p->done = 1;
	}

	bool playerCollide(){
		if (fabs(p->x - x) < w && fabs(p->y - y) < h) return 1;
		if (p->x - x > w && p->y - y > h) return 0;
		return 0;
	}

	void interactionWithMap()//ф-ция взаимодействия с картой
	{
		for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по тайликам, контактирующим с игроком,, то есть по всем квадратикам размера 32*32, которые мы окрашивали в 9 уроке. про условия читайте ниже.
		for (int j = x / 32; j<(x + w) / 32; j++)//икс делим на 32, тем самым получаем левый квадратик, с которым персонаж соприкасается. (он ведь больше размера 32*32, поэтому может одновременно стоять на нескольких квадратах). А j<(x + w) / 32 - условие ограничения координат по иксу. то есть координата самого правого квадрата, который соприкасается с персонажем. таким образом идем в цикле слева направо по иксу, проходя по от левого квадрата (соприкасающегося с героем), до правого квадрата (соприкасающегося с героем)
		{
			if (map1[i][j] == '1')//если наш квадратик соответствует символу 0 (стена), то проверяем "направление скорости" персонажа:
			{
				if (dy>0)//если мы шли вниз,
				{
					y = i * 32 - h;//то стопорим координату игрек персонажа. сначала получаем координату нашего квадратика на карте(стены) и затем вычитаем из высоты спрайта персонажа.
					dir = 3;
				}
				if (dy<0)
				{
					y = i * 32 + 32;//аналогично с ходьбой вверх. dy<0, значит мы идем вверх (вспоминаем координаты паинта)
					dir = 2;
				}
				if (dx>0)
				{
					x = j * 32 - w;//если идем вправо, то координата Х равна стена (символ 0) минус ширина персонажа
					dir = 1;
					sprite.setTextureRect(IntRect(28, 0, w, h));
				}
				if (dx < 0)
				{
					x = j * 32 + 32;//аналогично идем влево
					dir = 0;
					sprite.setTextureRect(IntRect(0, 0, w, h));
				}
			}

			if (playerCollide() && p->dir != dir){
				if (p->life) {
					sb.loadFromFile("sounds/Enemy.wav");
					sound.setBuffer(sb);
					sound.play();
					p->dieCounter++;
					p->decScore(500);
					p->life = 0;
					printf("You respawned. Don't collide with pendos\n");
				}
			}
			else if (playerCollide() && p->dir == dir){
				if (dir == 0 && p->x + w - 1 < x) death();
				if (dir == 1 && p->x - w + 1 > x) death();
				if (dir == 2 && p->y + h - 1 < y) death();
				if (dir == 3 && p->y - h + 1 > y) death();
			}
		}
	}
};