#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <vector>
#include <ctime>
using namespace std;
HWND hwnd = GetConsoleWindow();
HDC dc = GetDC(hwnd);
float playerX, playerY; // Координаты игрока
float enemyX = 150 + 11 * 64, enemyY = 50 + 3 * 64;
int enemyHealth = 500;
int damage = 50;
int playerHealth = 100;


/* Счетчики выстрелов */
vector <float> upFiresY;
vector <float> upFiresX;
vector <float> rightFiresY;
vector <float> rightFiresX;
vector <float> leftFiresY;
vector <float> leftFiresX;
vector <float> downFiresY;
vector <float> downFiresX;

vector <int> wallsXCoord;	//Стены, которые можно сломать
vector <int> wallsYCoord;

vector <int> blocksXCoord;	//Неразбиваемые блоки
vector <int> blocksYCoord;

vector <int> waterXCoord;	//Вода
vector <int> waterYCoord;
/* /////////////////////////////////////////////// */

void drawEnemy() {
	HBRUSH enemyBrush = CreateSolidBrush(RGB(255, 0, 0));
	/* Отрисовка различных положений танка, смотрящего ввех, вниз, вправо или влево */
	POINT lookUp[5] = { {enemyX, enemyY - 32}, {enemyX + 32, enemyY}, {enemyX + 32, enemyY + 32}, {enemyX - 32, enemyY + 32}, {enemyX - 32, enemyY} }; // Отрисовка вверх (первая координата - нос)
	POINT lookRight[5] = { {enemyX + 32, enemyY}, {enemyX, enemyY + 32}, {enemyX - 32, enemyY + 32}, {enemyX - 32, enemyY - 32}, {enemyX, enemyY - 32} }; // Отрисовка вправо (первая координата - нос)
	POINT lookLeft[5] = { {enemyX - 32, enemyY}, {enemyX, enemyY - 32}, {enemyX + 32, enemyY - 32}, {enemyX + 32, enemyY + 32}, {enemyX, enemyY + 32} };
	POINT lookDown[5] = { {enemyX, enemyY + 32}, {enemyX - 32, enemyY}, {enemyX - 32, enemyY - 32}, {enemyX + 32, enemyY - 32}, {enemyX + 32, enemyY} };
	SelectObject(dc, enemyBrush);
	Polygon(dc, lookDown, 5);
}

void enemyFireAction(unsigned short int prevEnemyDir, int& enemyReload) {
	switch (prevEnemyDir) {
	case 0: {
		upFiresX.push_back(enemyY - 54.f);
		upFiresY.push_back(enemyX - 6.f);
		enemyReload = 15;
		break;
	}
	case 1: {
		rightFiresY.push_back(enemyY - 6);
		rightFiresX.push_back(enemyX + 32);
		enemyReload = 15;
		break;
	}
	case 2: {

		leftFiresY.push_back(enemyY - 6);
		leftFiresX.push_back(enemyX - 54);
		enemyReload = 15;
		break;
	}
	case 3: {
		downFiresY.push_back(enemyY + 32);
		downFiresX.push_back(enemyX - 6);
		enemyReload = 15;
		break;
	}
	default:
		break;
	}
}

void enemyCollisionCheckEvent(int dir, bool& canGo) {
	float dPlayerX, dPlayerY;
	switch (dir) {	//Коллизия
	case 0: {
		dPlayerX = enemyX;
		dPlayerY = enemyY- 8;

		if ((dPlayerX - 32 > playerX - 32 && dPlayerX - 32 < playerX + 32 || dPlayerX + 32 > playerX - 32 && dPlayerX < playerX + 32 || dPlayerX - 32 == playerX - 32 && dPlayerX + 32 == playerX + 32)
			&& dPlayerY - 32 < playerY + 32 && dPlayerY - 32 > playerY - 32) {
			canGo = false;
		}
		else
			canGo = true;

		if (canGo) {
			for (int i = 0; i < blocksXCoord.size(); i++) {		// Коллизия неразрушаемых блоков
				if (((dPlayerX - 32 >= blocksXCoord[i] && dPlayerX - 32 <= blocksXCoord[i] + 64) || (dPlayerX + 32 >= blocksXCoord[i] && dPlayerX + 32 <= blocksXCoord[i] + 64))
					&& (dPlayerY - 32 <= blocksYCoord[i] + 64 && dPlayerY - 32 >= blocksYCoord[i])) {
					canGo = false;
					break;
				}
				else canGo = true;
			}
		}
		if (canGo) {
			for (int i = 0; i < wallsXCoord.size(); i++) {	// Коллизия разрушаемых блоков
				if (((dPlayerX - 32 > wallsXCoord[i] && dPlayerX - 32 < wallsXCoord[i] + 64) || (dPlayerX + 32 > wallsXCoord[i] && dPlayerX + 32 < wallsXCoord[i] + 64)
					|| (dPlayerX - 32 == wallsXCoord[i] && dPlayerX + 32 == wallsXCoord[i] + 64)) && (dPlayerY - 32 < wallsYCoord[i] + 64 && dPlayerY - 32 > wallsYCoord[i])) {
					canGo = false;
					break;
				}
				else canGo = true;
			}
		}
		if (canGo) {
			for (int i = 0; i < waterXCoord.size(); i++) {	// Коллизия воды
				if (((dPlayerX - 32 >= waterXCoord[i] && dPlayerX - 32 <= waterXCoord[i] + 64) || (dPlayerX + 32 >= waterXCoord[i] && dPlayerX + 32 <= waterXCoord[i] + 64))
					&& (dPlayerY - 32 <= waterYCoord[i] + 64 && dPlayerY - 32 >= waterYCoord[i])) {
					canGo = false;
					break;
				}
				else canGo = true;
			}
		}

		break;
	}
	case 3: {
		dPlayerX = enemyX;
		dPlayerY = enemyY + 8;

		if ((dPlayerX - 32 > playerX - 32 && dPlayerX - 32 < playerX + 32 || dPlayerX + 32 > playerX - 32 && dPlayerX < playerX + 32 || dPlayerX - 32 == playerX - 32 && dPlayerX + 32 == playerX + 32)
			&& (dPlayerY + 32 < playerY+ 32 && dPlayerY + 32 > playerY - 32)) {
			canGo = false;
		}
		else
			canGo = true;

		if (canGo) {
			for (int i = 0; i < blocksXCoord.size(); i++) {		// Коллизия неразрушаемых блоков
				if (((dPlayerX - 32 >= blocksXCoord[i] && dPlayerX - 32 <= blocksXCoord[i] + 64) || (dPlayerX + 32 >= blocksXCoord[i] && dPlayerX + 32 <= blocksXCoord[i] + 64))
					&& (dPlayerY + 32 >= blocksYCoord[i] && dPlayerY + 32 <= blocksYCoord[i] + 64)) {
					canGo = false;
					break;
				}
				else canGo = true;
			}
		}
		if (canGo) {
			for (int i = 0; i < wallsXCoord.size(); i++) {	// Коллизия разрушаемых блоков
				if (((dPlayerX - 32 > wallsXCoord[i] && dPlayerX - 32 < wallsXCoord[i] + 64) || (dPlayerX + 32 > wallsXCoord[i] && dPlayerX + 32 < wallsXCoord[i] + 64)
					|| (dPlayerX - 32 == wallsXCoord[i] && dPlayerX + 32 == wallsXCoord[i] + 64)) && (dPlayerY + 32 < wallsYCoord[i] + 64 && dPlayerY + 32 > wallsYCoord[i])) {
					canGo = false;
					break;
				}
				else canGo = true;
			}
		}
		if (canGo) {
			for (int i = 0; i < waterXCoord.size(); i++) {	// Коллизия воды
				if (((dPlayerX - 32 >= waterXCoord[i] && dPlayerX - 32 <= waterXCoord[i] + 64) || (dPlayerX + 32 >= waterXCoord[i] && dPlayerX + 32 <= waterXCoord[i] + 64))
					&& (dPlayerY + 32 <= waterYCoord[i] + 64 && dPlayerY + 32 >= waterYCoord[i])) {
					canGo = false;
					break;
				}
				else canGo = true;
			}
		}
		break;
	}
	case 2: {
		dPlayerX = enemyX - 8;
		dPlayerY = enemyY;

		if ((dPlayerY - 32 > playerY - 32 && dPlayerY - 32 < playerY + 32 || dPlayerY + 32 > playerY - 32 && dPlayerY < playerY + 32 || dPlayerY - 32 == playerY- 32 && dPlayerY + 32 == playerY + 32)
			&& (dPlayerX - 32 < playerX + 32 && dPlayerX - 32 > playerX - 32)) {
			canGo = false;
		}
		else
			canGo = true;

		if (canGo) {
			for (int i = 0; i < blocksXCoord.size(); i++) {		// Коллизия неразрушаемых блоков
				if (((dPlayerY + 32 >= blocksYCoord[i] && dPlayerY + 32 <= blocksYCoord[i] + 64) || (dPlayerY - 32 >= blocksYCoord[i] && dPlayerY - 32 <= blocksYCoord[i] + 64))
					&& (dPlayerX - 32 >= blocksXCoord[i] && dPlayerX - 32 <= blocksXCoord[i] + 64)) {
					canGo = false;
					break;
				}
				else canGo = true;
			}
		}
		if (canGo) {
			for (int i = 0; i < wallsXCoord.size(); i++) {	// Коллизия разрушаемых блоков
				if (((dPlayerY + 32 > wallsYCoord[i] && dPlayerY + 32 < wallsYCoord[i] + 64) || (dPlayerY - 32 > wallsYCoord[i] && dPlayerY - 32 < wallsYCoord[i] + 64)
					|| (dPlayerY - 32 == wallsYCoord[i] && dPlayerY + 32 == wallsYCoord[i] + 64)) && (dPlayerX - 32 > wallsXCoord[i] && dPlayerX - 32 < wallsXCoord[i] + 64)) {
					canGo = false;
					break;
				}
				else canGo = true;
			}
		}
		if (canGo) {
			for (int i = 0; i < waterXCoord.size(); i++) {	// Коллизия воды
				if (((dPlayerY + 32 >= waterYCoord[i] && dPlayerY + 32 <= waterYCoord[i] + 64) || (dPlayerY - 32 >= waterYCoord[i] && dPlayerY - 32 <= waterYCoord[i] + 64))
					&& (dPlayerX - 32 >= waterXCoord[i] && dPlayerX - 32 <= waterXCoord[i] + 64)) {
					canGo = false;
					break;
				}
				else canGo = true;
			}
		}
		break;
	}
	case 1: {
		dPlayerX = enemyX + 8;
		dPlayerY = enemyY;

		if ((dPlayerY - 32 > playerY - 32 && dPlayerY - 32 < playerY + 32 || dPlayerY + 32 > playerY - 32 && dPlayerY < playerY + 32 || dPlayerY - 32 == playerY - 32 && dPlayerY + 32 == playerY + 32)
			&& (dPlayerX + 32 < playerX + 32 && dPlayerX + 32 > playerX - 32)) {
			canGo = false;
		}
		else
			canGo = true;

		if (canGo) {
			for (int i = 0; i < blocksXCoord.size(); i++) {		// Коллизия неразрушаемых блоков
				if (((dPlayerY + 32 >= blocksYCoord[i] && dPlayerY + 32 <= blocksYCoord[i] + 64) || (dPlayerY - 32 >= blocksYCoord[i] && dPlayerY - 32 <= blocksYCoord[i] + 64))
					&& (dPlayerX + 32 >= blocksXCoord[i] && dPlayerX + 32 <= blocksXCoord[i] + 64)) {
					canGo = false;
					break;
				}
				else canGo = true;
			}
		}
		if (canGo) {
			for (int i = 0; i < wallsXCoord.size(); i++) {	// Коллизия разрушаемых блоков
				if (((dPlayerY + 32 > wallsYCoord[i] && dPlayerY + 32 < wallsYCoord[i] + 64) || (dPlayerY - 32 > wallsYCoord[i] && dPlayerY - 32 < wallsYCoord[i] + 64)
					|| (dPlayerY - 32 == wallsYCoord[i] && dPlayerY + 32 == wallsYCoord[i] + 64)) && (dPlayerX + 32 > wallsXCoord[i] && dPlayerX + 32 < wallsXCoord[i] + 64)) {
					canGo = false;
					break;
				}
				else canGo = true;
			}
		}
		if (canGo) {
			for (int i = 0; i < waterXCoord.size(); i++) {	// Коллизия воды
				if (((dPlayerY + 32 >= waterYCoord[i] && dPlayerY + 32 <= waterYCoord[i] + 64) || (dPlayerY - 32 >= waterYCoord[i] && dPlayerY - 32 <= waterYCoord[i] + 64))
					&& (dPlayerX + 32 >= waterXCoord[i] && dPlayerX + 32 <= waterXCoord[i] + 64)) {
					canGo = false;
					break;
				}
				else canGo = true;
			}
		}
		break;
	}
	default:
		break;
	}
}

void enemyControl(const HBRUSH& bgBrush, const HBRUSH& playerBrush, unsigned short int& prevEnemyDir, int& enemyTimer, int& enemyReload) { // Каждые несколько итераций противник выбирает, в каком направление двигаться
	int fire = 1;
	if (enemyReload < 2) {
		enemyFireAction(prevEnemyDir, enemyReload);
	}
	float dx = 0, dy = 0; // Передвижение игрока
	POINT lookUp[5] = { {enemyX, enemyY - 32}, {enemyX + 32, enemyY}, {enemyX + 32, enemyY + 32}, {enemyX - 32, enemyY + 32}, {enemyX - 32, enemyY} };
	POINT lookRight[5] = { {enemyX + 32, enemyY}, {enemyX, enemyY + 32}, {enemyX - 32, enemyY + 32}, {enemyX - 32, enemyY - 32}, {enemyX, enemyY - 32} }; 
	POINT lookLeft[5] = { {enemyX - 32, enemyY}, {enemyX, enemyY - 32}, {enemyX + 32, enemyY - 32}, {enemyX + 32, enemyY + 32}, {enemyX, enemyY + 32} };
	POINT lookDown[5] = { {enemyX, enemyY + 32}, {enemyX - 32, enemyY}, {enemyX - 32, enemyY - 32}, {enemyX + 32, enemyY - 32}, {enemyX + 32, enemyY} };
	
	int randDir = prevEnemyDir;
	/* Если таймер закончился, то выполняется условие и генерируется новое направление движения врага */
	if (enemyTimer <= 2) {
		randDir = 0 + rand() % 4; // от 0 до 3
	}
	bool canGo = true;
	enemyCollisionCheckEvent(randDir, canGo);
	if (canGo) {

		// Удаление с предыдущего положенияя
		SelectObject(dc, bgBrush);
		switch (prevEnemyDir)
		{
		case 0: // Смотрел вверх
			Polygon(dc, lookUp, 5);
			break;
		case 1: // Смотрел вправо
			Polygon(dc, lookRight, 5);
			break;
		case 2: // Смотрел влево
			Polygon(dc, lookLeft, 5);
			break;
		case 3: // Смотрел вниз
			Polygon(dc, lookDown, 5);
			break;
		default:
			break;
		}
		DeleteObject(lookUp);
		DeleteObject(lookRight);
		DeleteObject(lookLeft);
		DeleteObject(lookDown);

		switch (randDir) {
		case 0: // Поедет вверх
			dy = -4;
			break;
		case 1: // Поедет впарво
			dx = 4;
			break;
		case 2: // Поедет влево
			dx = -4;
			break;
		case 3: // Поедет вниз
			dy = 4;
			break;
		default:
			break;
		}
		enemyX += dx;
		enemyY += dy;
		dx = 0;
		dy = 0;

		/* Перерисовка на новом месте */
		SelectObject(dc, playerBrush);

		POINT lookU[5] = { {enemyX, enemyY - 32}, {enemyX + 32, enemyY}, {enemyX + 32, enemyY + 32}, {enemyX - 32, enemyY + 32}, {enemyX - 32, enemyY} };
		POINT lookR[5] = { {enemyX + 32, enemyY}, {enemyX, enemyY + 32}, {enemyX - 32, enemyY + 32}, {enemyX - 32, enemyY - 32}, {enemyX, enemyY - 32} };
		POINT lookL[5] = { {enemyX - 32, enemyY}, {enemyX, enemyY - 32}, {enemyX + 32, enemyY - 32}, {enemyX + 32, enemyY + 32}, {enemyX, enemyY + 32} };
		POINT lookD[5] = { {enemyX, enemyY + 32}, {enemyX - 32, enemyY}, {enemyX - 32, enemyY - 32}, {enemyX + 32, enemyY - 32}, {enemyX + 32, enemyY} };

		switch (randDir) {
		case 0: // Поедет вверх
			Polygon(dc, lookU, 5);
			prevEnemyDir = 0;
			break;
		case 1: // Поедет впарво
			Polygon(dc, lookR, 5);
			prevEnemyDir = 1;
			break;
		case 2: // Поедет влево
			Polygon(dc, lookL, 5);
			prevEnemyDir = 2;
			break;
		case 3: // Поедет вниз
			Polygon(dc, lookD, 5);
			prevEnemyDir = 3;
			break;
		default:
			break;
		}
		DeleteObject(lookU);
		DeleteObject(lookL);
		DeleteObject(lookR);
		DeleteObject(lookD);
	}
	else
		enemyTimer = 2;

}


/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/* Создание объекта выстрела в разных направлениях */
void fireAction(unsigned short int prevLookDirection, int& fireTimer) {
	switch (prevLookDirection) {
	case 0:
		upFiresY.push_back(playerY - 54);
		upFiresX.push_back(playerX - 6);
		fireTimer = 15;
		break;
	case 1:
		rightFiresY.push_back(playerY - 6);
		rightFiresX.push_back(playerX + 32);
		fireTimer = 15;
		break;
	case 2:
		leftFiresY.push_back(playerY - 6);
		leftFiresX.push_back(playerX - 54);
		fireTimer = 15;
		break;

	case 3:
		downFiresY.push_back(playerY + 32);
		downFiresX.push_back(playerX - 6);
		fireTimer = 15;
		break;
	}
}

/* Уничтожение стены */
void removeWall(int x, int y, int i) {
	HBRUSH bgBrush = CreateSolidBrush(RGB(0, 0, 0));
	HBRUSH explosionBrush_2 = CreateSolidBrush(RGB(255, 60, 0));
	HBRUSH explosionBrush_1 = CreateSolidBrush(RGB(255, 140, 0));

	SelectObject(dc, explosionBrush_1);
	Rectangle(dc, wallsXCoord[i], wallsYCoord[i], wallsXCoord[i] + 64, wallsYCoord[i] + 64);
	Sleep(22);

	SelectObject(dc, explosionBrush_2);
	Rectangle(dc, wallsXCoord[i], wallsYCoord[i], wallsXCoord[i] + 64, wallsYCoord[i] + 64);
	Sleep(22);

	SelectObject(dc, bgBrush);
	Rectangle(dc, wallsXCoord[i], wallsYCoord[i], wallsXCoord[i] + 64, wallsYCoord[i] + 64);

	wallsXCoord.erase(wallsXCoord.begin() + i, wallsXCoord.begin() + i + 1);
	wallsYCoord.erase(wallsYCoord.begin() + i, wallsYCoord.begin() + i + 1);

	DeleteObject(bgBrush);
	DeleteObject(explosionBrush_1);
	DeleteObject(explosionBrush_2);
}

/* Коллизия выстрелов */
bool shotsCollision(int i, vector<float> fireDirectionX, vector<float> fireDirectionY){
	float dFireX, dFireY;
	if (fireDirectionX == upFiresX && fireDirectionY == upFiresY) {
		dFireX = fireDirectionX[i];
		dFireY = fireDirectionY[i] - 20;

		/* проверка на попадание в противника */
		if ((dFireX >= enemyX - 32 && dFireX <= enemyX + 32) && (dFireY <= enemyY + 32 && dFireY >= enemyY - 32)) {
			enemyHealth -= damage;
			return false;
		}
		if ((dFireX >= playerX - 32 && dFireX <= playerX + 32) && (dFireY <= playerY + 32 && dFireY >= playerY - 32)) {
			playerHealth -= damage;
			return false;
		}
		for (int j = 0; j < blocksXCoord.size(); j++) {
			if ((dFireX >= blocksXCoord[j] && dFireX <= blocksXCoord[j] + 64) && (dFireY < blocksYCoord[j] + 64 && dFireY > blocksYCoord[j])) { // Если сталкивается
				return false;
			}
		}
		for (int i = 0; i < wallsXCoord.size(); i++) {
			if ((dFireX >= wallsXCoord[i] && dFireX <= wallsXCoord[i] + 64) && (dFireY < wallsYCoord[i] + 64 && dFireY > wallsYCoord[i])) {	// Столкновение с разрушаемыми объектами
				removeWall(wallsXCoord[i], wallsYCoord[i], i);
				return false;
			}
		}
		return true;
	}
	if (fireDirectionX == rightFiresX && fireDirectionY == rightFiresY) {
		dFireX = fireDirectionX[i] + 20;
		dFireY = fireDirectionY[i];
		
		/* проверка на попадание в противника */
		if ((dFireX >= enemyX - 32 && dFireX <= enemyX + 32) && (dFireY <= enemyY + 32 && dFireY >= enemyY - 32)) {
			enemyHealth -= damage;
			return false;
		}
		if ((dFireX >= playerX - 32 && dFireX <= playerX + 32) && (dFireY <= playerY + 32 && dFireY >= playerY - 32)) {
			playerHealth -= damage;
			return false;
		}
		
		for (int i = 0; i < blocksXCoord.size(); i++) {
			if (dFireY <= blocksYCoord[i] + 64 && dFireY >= blocksYCoord[i] && dFireX <= blocksXCoord[i] + 64 && dFireX >= blocksXCoord[i]) {
				return false;
			}
		}
		for (int i = 0; i < wallsXCoord.size(); i++) {
			if (dFireY <= wallsYCoord[i] + 64 && dFireY >= wallsYCoord[i] && dFireX <= wallsXCoord[i] + 64 && dFireX >= wallsXCoord[i]) { // Столкновение с разрушаемыми объектами
				removeWall(wallsXCoord[i], wallsYCoord[i], i);
				return false;
			}
		}
		return true;
	}
	if (fireDirectionX == leftFiresX && fireDirectionY == leftFiresY) {
		dFireX = fireDirectionX[i] - 20;
		dFireY = fireDirectionY[i];
		
		/* проверка на попадание в противника */
		if ((dFireX >= enemyX - 32 && dFireX <= enemyX + 32) && (dFireY <= enemyY + 32 && dFireY >= enemyY - 32)) {
			enemyHealth -= damage;
			return false;
		}
		if ((dFireX >= playerX - 32 && dFireX <= playerX + 32) && (dFireY <= playerY + 32 && dFireY >= playerY - 32)) {
			playerHealth -= damage;
			return false;
		}

		for (int i = 0; i < blocksXCoord.size(); i++) {
			if (dFireY <= blocksYCoord[i] + 64 && dFireY >= blocksYCoord[i] && dFireX <= blocksXCoord[i] + 64 && dFireX >= blocksXCoord[i]) {
				return false;
			}
		}
		for (int i = 0; i < wallsXCoord.size(); i++) {
			if (dFireY <= wallsYCoord[i] + 64 && dFireY >= wallsYCoord[i] && dFireX <= wallsXCoord[i] + 64 && dFireX >= wallsXCoord[i]) { // Столкновение с разрушаемыми объектами
				removeWall(wallsXCoord[i], wallsYCoord[i], i);
				return false;
			}
		}
		return true;
	}
	if (fireDirectionX == downFiresX && fireDirectionY == downFiresY) {
		dFireX = fireDirectionX[i];
		dFireY = fireDirectionY[i] + 20;
		
		/* проверка на попадание в противника */
		if ((dFireX >= enemyX - 32 && dFireX <= enemyX + 32) && (dFireY <= enemyY + 32 && dFireY >= enemyY - 32)) {
			enemyHealth -= damage;
			return false;
		}
		if ((dFireX >= playerX - 32 && dFireX <= playerX + 32) && (dFireY <= playerY + 32 && dFireY >= playerY - 32)) {
			playerHealth -= damage;
			return false;
		}

		for (int i = 0; i < blocksXCoord.size(); i++){
			if ((dFireX >= blocksXCoord[i] && dFireX <= blocksXCoord[i] + 64) && (dFireY < blocksYCoord[i] + 64 && dFireY > blocksYCoord[i])) {
				return false;
			}
		}
		for (int i = 0; i < wallsXCoord.size(); i++) {
			if ((dFireX >= wallsXCoord[i] && dFireX <= wallsXCoord[i] + 64) && (dFireY < wallsYCoord[i] + 64 && dFireY > wallsYCoord[i])) { // Столкновение с разрушаемыми объектами
				removeWall(wallsXCoord[i], wallsYCoord[i], i);
				return false;
			}
		}
		return true;
	}
	return false;
}

/* Изменение координат каждого выстрела и отсчет времени перезарядки */
void shotsMoving(const HBRUSH& mainFieldBgBrush, const HBRUSH& ballBrush) {
	
	/* обновление местоположения снаряда с каждой итерацией */
	for (int i = 0; i < upFiresY.size(); i++) {
		if (shotsCollision(i, upFiresX, upFiresY)) { // Если пуля не сталкивается, то она перерисовывается
			SelectObject(dc, mainFieldBgBrush);
			Ellipse(dc, upFiresX[i], upFiresY[i], upFiresX[i] + 12, upFiresY[i] + 24);
			upFiresY[i] -= 20;
			SelectObject(dc, ballBrush);
			Ellipse(dc, upFiresX[i], upFiresY[i], upFiresX[i] + 12, upFiresY[i] + 24);
		}
		else { // Если пуля сталкивается, то она взрывается и удаляется из вектора
			HBRUSH explosionBrush_1 = CreateSolidBrush(RGB(255, 60, 0));
			HBRUSH explosionBrush_2 = CreateSolidBrush(RGB(255, 140, 0));
			SelectObject(dc, explosionBrush_1);
			Ellipse(dc, upFiresX[i], upFiresY[i], upFiresX[i] + 24, upFiresY[i] + 24);
			SelectObject(dc, explosionBrush_2);
			Sleep(15);
			Ellipse(dc, upFiresX[i], upFiresY[i], upFiresX[i] + 24, upFiresY[i] + 24);
			Sleep(15);
			SelectObject(dc, mainFieldBgBrush);
			Ellipse(dc, upFiresX[i] -1, upFiresY[i] -1, upFiresX[i] + 25, upFiresY[i] + 25);
			upFiresX[i] = NULL;
			upFiresY[i] = NULL;
			upFiresX.erase(upFiresX.begin() + i, upFiresX.begin() + i + 1);
			upFiresY.erase(upFiresY.begin() + i, upFiresY.begin() + i + 1);
		}
	}
	for (int i = 0; i < rightFiresY.size(); i++) {
		if (shotsCollision(i, rightFiresX, rightFiresY)) {
			SelectObject(dc, mainFieldBgBrush);
			Ellipse(dc, rightFiresX[i], rightFiresY[i], rightFiresX[i] + 24, rightFiresY[i] + 12);
			rightFiresX[i] += 20;
			SelectObject(dc, ballBrush);
			Ellipse(dc, rightFiresX[i], rightFiresY[i], rightFiresX[i] + 24, rightFiresY[i] + 12);
		}
		else {
			HBRUSH explosionBrush_1 = CreateSolidBrush(RGB(255, 60, 0));
			HBRUSH explosionBrush_2 = CreateSolidBrush(RGB(255, 140, 0));
			SelectObject(dc, explosionBrush_1);
			Ellipse(dc, rightFiresX[i], rightFiresY[i], rightFiresX[i] + 24, rightFiresY[i] + 24);
			SelectObject(dc, explosionBrush_2);
			Sleep(15);
			Ellipse(dc, rightFiresX[i], rightFiresY[i], rightFiresX[i] + 24, rightFiresY[i] + 24);
			Sleep(15);
			SelectObject(dc, mainFieldBgBrush);
			Ellipse(dc, rightFiresX[i] - 1, rightFiresY[i] - 1, rightFiresX[i] + 25, rightFiresY[i] + 25);
			rightFiresX[i] = NULL;
			rightFiresY[i] = NULL;
			rightFiresX.erase(rightFiresX.begin() + i, rightFiresX.begin() + i + 1);
			rightFiresY.erase(rightFiresY.begin() + i, rightFiresY.begin() + i + 1);
		}
	}
	for (int i = 0; i < leftFiresY.size(); i++) {
		if (shotsCollision(i, leftFiresX, leftFiresY)) {
			SelectObject(dc, mainFieldBgBrush);
			Ellipse(dc, leftFiresX[i], leftFiresY[i], leftFiresX[i] + 24, leftFiresY[i] + 12);
			leftFiresX[i] -= 20;
			SelectObject(dc, ballBrush);
			Ellipse(dc, leftFiresX[i], leftFiresY[i], leftFiresX[i] + 24, leftFiresY[i] + 12);
		}
		else {
			HBRUSH explosionBrush_1 = CreateSolidBrush(RGB(255, 60, 0));
			HBRUSH explosionBrush_2 = CreateSolidBrush(RGB(255, 140, 0));
			SelectObject(dc, explosionBrush_1);
			Ellipse(dc, leftFiresX[i], leftFiresY[i], leftFiresX[i] + 24, leftFiresY[i] + 24);
			SelectObject(dc, explosionBrush_2);
			Sleep(15);
			Ellipse(dc, leftFiresX[i], leftFiresY[i], leftFiresX[i] + 24, leftFiresY[i] + 24);
			Sleep(15);
			SelectObject(dc, mainFieldBgBrush);
			Ellipse(dc, leftFiresX[i] - 1, leftFiresY[i] - 1, leftFiresX[i] + 25, leftFiresY[i] + 25);
			leftFiresX[i] = NULL;
			leftFiresY[i] = NULL;
			leftFiresX.erase(leftFiresX.begin() + i, leftFiresX.begin() + i + 1);
			leftFiresY.erase(leftFiresY.begin() + i, leftFiresY.begin() + i + 1);
		}
	}
	for (int i = 0; i < downFiresY.size(); i++) {
		if (shotsCollision(i, downFiresX, downFiresY)) {
			SelectObject(dc, mainFieldBgBrush);
			Ellipse(dc, downFiresX[i], downFiresY[i], downFiresX[i] + 12, downFiresY[i] + 24);
			downFiresY[i] += 20;
			SelectObject(dc, ballBrush);
			Ellipse(dc, downFiresX[i], downFiresY[i], downFiresX[i] + 12, downFiresY[i] + 24);
		}
		else {
			HBRUSH explosionBrush_1 = CreateSolidBrush(RGB(255, 60, 0));
			HBRUSH explosionBrush_2 = CreateSolidBrush(RGB(255, 140, 0));
			SelectObject(dc, explosionBrush_1);
			Ellipse(dc, downFiresX[i], downFiresY[i], downFiresX[i] + 24, downFiresY[i] + 24);
			SelectObject(dc, explosionBrush_2);
			Sleep(15);
			Ellipse(dc, downFiresX[i], downFiresY[i], downFiresX[i] + 24, downFiresY[i] + 24);
			Sleep(15);
			SelectObject(dc, mainFieldBgBrush);
			Ellipse(dc, downFiresX[i] - 1, downFiresY[i] - 1, downFiresX[i] + 25, downFiresY[i] + 25);
			downFiresX[i] = NULL;
			downFiresY[i] = NULL;
			downFiresX.erase(downFiresX.begin() + i, downFiresX.begin() + i + 1);
			downFiresY.erase(downFiresY.begin() + i, downFiresY.begin() + i + 1);
		}
	}
}

/* Первичная отрисовка всех неподвижных объектов на карте */
void drawObjects() {
	/* Отрисовка объектов на поле */

	for (int i = 150; i < 1622; i += 64) {	// Окаймление по ширине
		blocksXCoord.push_back(i);
		blocksYCoord.push_back(1010 - 64);
		blocksXCoord.push_back(i);
		blocksYCoord.push_back(50);
	}
	for (int i = 50; i < 1010; i += 64) {	// Окаймление по высоте
		blocksYCoord.push_back(i);
		blocksXCoord.push_back(150);
		blocksYCoord.push_back(i);
		blocksXCoord.push_back(1622 - 64);
	}

	HBRUSH blocksBrush = CreateSolidBrush(RGB(165, 165, 165));
	SelectObject(dc, blocksBrush);
	for (int i = 0; i < blocksYCoord.size(); i++) {
		Rectangle(dc, blocksXCoord[i], blocksYCoord[i], blocksXCoord[i] + 64, blocksYCoord[i] + 64);
	}

	wallsXCoord.push_back(150 + 5 * 64);
	wallsXCoord.push_back(150 + 5 * 64);
	wallsXCoord.push_back(150 + 6 * 64);
	wallsXCoord.push_back(150 + 6 * 64);
	wallsXCoord.push_back(150 + 7 * 64);
	wallsXCoord.push_back(150 + 7 * 64);
	wallsXCoord.push_back(150 + 8 * 64);
	wallsXCoord.push_back(150 + 8 * 64);
	wallsXCoord.push_back(150 + 9 * 64);
	wallsXCoord.push_back(150 + 9 * 64);

	wallsXCoord.push_back(150 + 11 * 64);
	wallsXCoord.push_back(150 + 12 * 64);
	wallsXCoord.push_back(150 + 13 * 64);
	wallsXCoord.push_back(150 + 12 * 64);
	wallsXCoord.push_back(150 + 12 * 64);

	wallsXCoord.push_back(150 + 15 * 64);
	wallsXCoord.push_back(150 + 16 * 64);
	wallsXCoord.push_back(150 + 17 * 64);
	wallsXCoord.push_back(150 + 16 * 64);
	wallsXCoord.push_back(150 + 16 * 64);

	wallsYCoord.push_back(50 + 8 * 64);
	wallsYCoord.push_back(50 + 9 * 64);
	wallsYCoord.push_back(50 + 7 * 64);
	wallsYCoord.push_back(50 + 10 * 64);
	wallsYCoord.push_back(50 + 6 * 64);
	wallsYCoord.push_back(50 + 11 * 64);
	wallsYCoord.push_back(50 + 6 * 64);
	wallsYCoord.push_back(50 + 11 * 64);
	wallsYCoord.push_back(50 + 7 * 64);
	wallsYCoord.push_back(50 + 10 * 64);

	wallsYCoord.push_back(50 + 9 * 64);
	wallsYCoord.push_back(50 + 9 * 64);
	wallsYCoord.push_back(50 + 9 * 64);
	wallsYCoord.push_back(50 + 8 * 64);
	wallsYCoord.push_back(50 + 10 * 64);

	wallsYCoord.push_back(50 + 9 * 64);
	wallsYCoord.push_back(50 + 9 * 64);
	wallsYCoord.push_back(50 + 9 * 64);
	wallsYCoord.push_back(50 + 8 * 64);
	wallsYCoord.push_back(50 + 10 * 64);

	HBRUSH wallBrush = CreateSolidBrush(RGB(160, 82, 45));
	SelectObject(dc, wallBrush);
	for (int i = 0; i < wallsXCoord.size(); i++) {
		Rectangle(dc, wallsXCoord[i], wallsYCoord[i], wallsXCoord[i] + 64, wallsYCoord[i] + 64);
	}

	for (int i = 1; i <= 8; i++) {
		waterXCoord.push_back(150 + i * 64);
		waterYCoord.push_back(50 + 3 * 64);
	}
	for (int i = 14; i < 22; i++) {
		waterXCoord.push_back(150 + i * 64);
		waterYCoord.push_back(50 + 3 * 64);
	}

	HBRUSH waterBrush = CreateSolidBrush(RGB(0, 0, 255));
	SelectObject(dc, waterBrush);
	for (int i = 0; i < waterXCoord.size(); i++) {
		Rectangle(dc, waterXCoord[i], waterYCoord[i], waterXCoord[i] + 64, waterYCoord[i] + 64);
	}
}

/* Провека, сталкивается ли подвижный объект с неподвижными */
void collisionCheckEvent(char c, bool &canGo) {
	float dPlayerX, dPlayerY;
	switch (c) {	//Коллизия
	case 'w': {
		dPlayerX = playerX;
		dPlayerY = playerY - 8;

		if ((dPlayerX - 32 > enemyX - 32 && dPlayerX - 32 < enemyX + 32 || dPlayerX + 32 > enemyX - 32 && dPlayerX < enemyX + 32 || dPlayerX - 32 == enemyX - 32 && dPlayerX + 32 == enemyX + 32)
			&& dPlayerY - 32 < enemyY + 32 && dPlayerY - 32 > enemyY - 32) {
			canGo = false;
		}
		else
			canGo = true;

		if (canGo) {
			for (int i = 0; i < blocksXCoord.size(); i++) {		// Коллизия неразрушаемых блоков
				if (((dPlayerX - 32 >= blocksXCoord[i] && dPlayerX - 32 <= blocksXCoord[i] + 64) || (dPlayerX + 32 >= blocksXCoord[i] && dPlayerX + 32 <= blocksXCoord[i] + 64))
					&& (dPlayerY - 32 <= blocksYCoord[i] + 64 && dPlayerY - 32 >= blocksYCoord[i])) {
					canGo = false;
					break;
				}
				else canGo = true;
			}
		}
		if (canGo) {
			for (int i = 0; i < wallsXCoord.size(); i++) {	// Коллизия разрушаемых блоков
				if (((dPlayerX - 32 > wallsXCoord[i] && dPlayerX - 32 < wallsXCoord[i] + 64) || (dPlayerX + 32 > wallsXCoord[i] && dPlayerX + 32 < wallsXCoord[i] + 64)
					 || (dPlayerX - 32 == wallsXCoord[i] && dPlayerX + 32 == wallsXCoord[i] + 64)) && (dPlayerY - 32 < wallsYCoord[i] + 64 && dPlayerY - 32 > wallsYCoord[i])) {
					canGo = false;
					break;
				}
				else canGo = true;
			}
		}
		if (canGo) {
			for (int i = 0; i < waterXCoord.size(); i++) {	// Коллизия воды
				if (((dPlayerX - 32 >= waterXCoord[i] && dPlayerX - 32 <= waterXCoord[i] + 64) || (dPlayerX + 32 >= waterXCoord[i] && dPlayerX + 32 <= waterXCoord[i] + 64))
					&& (dPlayerY - 32 <= waterYCoord[i] + 64 && dPlayerY - 32 >= waterYCoord[i])) {
					canGo = false;
					break;
				}
				else canGo = true;
			}
		}

		break;
	}
	case 's': {
		dPlayerX = playerX;
		dPlayerY = playerY + 8;

		if ((dPlayerX - 32 > enemyX - 32 && dPlayerX - 32 < enemyX + 32 || dPlayerX + 32 > enemyX - 32 && dPlayerX < enemyX + 32 || dPlayerX - 32 == enemyX - 32 && dPlayerX + 32 == enemyX + 32)
			&& (dPlayerY + 32 < enemyY + 32 && dPlayerY + 32 > enemyY - 32)) {
			canGo = false;
		}
		else
			canGo = true;

		if (canGo) {
			for (int i = 0; i < blocksXCoord.size(); i++) {		// Коллизия неразрушаемых блоков
				if (((dPlayerX - 32 >= blocksXCoord[i] && dPlayerX - 32 <= blocksXCoord[i] + 64) || (dPlayerX + 32 >= blocksXCoord[i] && dPlayerX + 32 <= blocksXCoord[i] + 64))
					&& (dPlayerY + 32 >= blocksYCoord[i] && dPlayerY + 32 <= blocksYCoord[i] + 64)) {
					canGo = false;
					break;
				}
				else canGo = true;
			}
		}
		if (canGo) {
			for (int i = 0; i < wallsXCoord.size(); i++) {	// Коллизия разрушаемых блоков
				if (((dPlayerX - 32 > wallsXCoord[i] && dPlayerX - 32 < wallsXCoord[i] + 64) || (dPlayerX + 32 > wallsXCoord[i] && dPlayerX + 32 < wallsXCoord[i] + 64) 
					 || (dPlayerX - 32 == wallsXCoord[i] && dPlayerX + 32 == wallsXCoord[i] + 64)) && (dPlayerY + 32 < wallsYCoord[i] + 64 && dPlayerY + 32 > wallsYCoord[i])) {
					canGo = false;
					break;
				}
				else canGo = true;
			}
		}
		if (canGo) {
			for (int i = 0; i < waterXCoord.size(); i++) {	// Коллизия воды
				if (((dPlayerX - 32 >= waterXCoord[i] && dPlayerX - 32 <= waterXCoord[i] + 64) || (dPlayerX + 32 >= waterXCoord[i] && dPlayerX + 32 <= waterXCoord[i] + 64))
					&& (dPlayerY + 32 <= waterYCoord[i] + 64 && dPlayerY + 32 >= waterYCoord[i])) {
					canGo = false;
					break;
				}
				else canGo = true;
			}
		}
		break;
	}
	case 'a': {
		dPlayerX = playerX - 8;
		dPlayerY = playerY;

		if ((dPlayerY - 32 > enemyY - 32 && dPlayerY - 32 < enemyY + 32 || dPlayerY + 32 > enemyY - 32 && dPlayerY < enemyY + 32 || dPlayerY - 32 == enemyY - 32 && dPlayerY + 32 == enemyY + 32)
			&& (dPlayerX - 32 < enemyX + 32 && dPlayerX - 32 > enemyX - 32)) {
			canGo = false;
		}
		else
			canGo = true;

		if (canGo) {
			for (int i = 0; i < blocksXCoord.size(); i++) {		// Коллизия неразрушаемых блоков
				if (((dPlayerY + 32 >= blocksYCoord[i] && dPlayerY + 32 <= blocksYCoord[i] + 64) || (dPlayerY - 32 >= blocksYCoord[i] && dPlayerY - 32 <= blocksYCoord[i] + 64))
					&& (dPlayerX - 32 >= blocksXCoord[i] && dPlayerX - 32 <= blocksXCoord[i] + 64)) {
					canGo = false;
					break;
				}
				else canGo = true;
			}
		}
		if (canGo) {
			for (int i = 0; i < wallsXCoord.size(); i++) {	// Коллизия разрушаемых блоков
				if (((dPlayerY + 32 > wallsYCoord[i] && dPlayerY + 32 < wallsYCoord[i] + 64) || (dPlayerY - 32 > wallsYCoord[i] && dPlayerY - 32 < wallsYCoord[i] + 64)
					 || (dPlayerY - 32 == wallsYCoord[i] && dPlayerY + 32 == wallsYCoord[i] + 64)) && (dPlayerX - 32 > wallsXCoord[i] && dPlayerX - 32 < wallsXCoord[i] + 64)) {
					canGo = false;
					break;
				}
				else canGo = true;
			}
		}
		if (canGo) {
			for (int i = 0; i < waterXCoord.size(); i++) {	// Коллизия воды
				if (((dPlayerY + 32 >= waterYCoord[i] && dPlayerY + 32 <= waterYCoord[i] + 64) || (dPlayerY - 32 >= waterYCoord[i] && dPlayerY - 32 <= waterYCoord[i] + 64))
					&& (dPlayerX - 32 >= waterXCoord[i] && dPlayerX - 32 <= waterXCoord[i] + 64)) {
					canGo = false;
					break;
				}
				else canGo = true;
			}
		}
		break;
	}
	case 'd': {
		dPlayerX = playerX + 8;
		dPlayerY = playerY;

		if ((dPlayerY - 32 > enemyY - 32 && dPlayerY - 32 < enemyY + 32 || dPlayerY + 32 > enemyY - 32 && dPlayerY < enemyY + 32 || dPlayerY - 32 == enemyY - 32 && dPlayerY + 32 == enemyY + 32)
			&& (dPlayerX + 32 < enemyX + 32 && dPlayerX + 32 > enemyX - 32)) {
			canGo = false;
		}
		else
			canGo = true;

		if (canGo) {
			for (int i = 0; i < blocksXCoord.size(); i++) {		// Коллизия неразрушаемых блоков
				if (((dPlayerY + 32 >= blocksYCoord[i] && dPlayerY + 32 <= blocksYCoord[i] + 64) || (dPlayerY - 32 >= blocksYCoord[i] && dPlayerY - 32 <= blocksYCoord[i] + 64))
					&& (dPlayerX + 32 >= blocksXCoord[i] && dPlayerX + 32 <= blocksXCoord[i] + 64)) {
					canGo = false;
					break;
				}
				else canGo = true;
			}
		}
		if (canGo) {
			for (int i = 0; i < wallsXCoord.size(); i++) {	// Коллизия разрушаемых блоков
				if (((dPlayerY + 32 > wallsYCoord[i] && dPlayerY + 32 < wallsYCoord[i] + 64) || (dPlayerY - 32 > wallsYCoord[i] && dPlayerY - 32 < wallsYCoord[i] + 64)
					 || (dPlayerY - 32 == wallsYCoord[i] && dPlayerY + 32 == wallsYCoord[i] + 64)) && (dPlayerX + 32 > wallsXCoord[i] && dPlayerX + 32 < wallsXCoord[i] + 64)) {
					canGo = false;
					break;
				}
				else canGo = true;
			}
		}
		if (canGo) {
			for (int i = 0; i < waterXCoord.size(); i++) {	// Коллизия воды
				if (((dPlayerY + 32 >= waterYCoord[i] && dPlayerY + 32 <= waterYCoord[i] + 64) || (dPlayerY - 32 >= waterYCoord[i] && dPlayerY - 32 <= waterYCoord[i] + 64))
					&& (dPlayerX + 32 >= waterXCoord[i] && dPlayerX + 32 <= waterXCoord[i] + 64)) {
					canGo = false;
					break;
				}
				else canGo = true;
			}
		}
		break;
	}
	default:
		break;
	}
}

/* Обработчик нажатий клавиш для первого игрока */
void firstPlayerControl(const HBRUSH& mainFieldBgBrush, const HBRUSH& playerBrush, unsigned short int& prevLookDirection, char c) {
	float dx = 0, dy = 0; // Передвижение игрока
	POINT lookUp[5] = { {playerX, playerY - 32}, {playerX + 32, playerY}, {playerX + 32, playerY + 32}, {playerX - 32, playerY + 32}, {playerX - 32, playerY} }; // Отрисовка вверх (первая координата - нос)
	POINT lookRight[5] = { {playerX + 32, playerY}, {playerX, playerY + 32}, {playerX - 32, playerY + 32}, {playerX - 32, playerY - 32}, {playerX, playerY - 32} }; // Отрисовка вправо (первая координата - нос)
	POINT lookLeft[5] = { {playerX - 32, playerY}, {playerX, playerY - 32}, {playerX + 32, playerY - 32}, {playerX + 32, playerY + 32}, {playerX, playerY + 32} };
	POINT lookDown[5] = { {playerX, playerY + 32}, {playerX - 32, playerY}, {playerX - 32, playerY - 32}, {playerX + 32, playerY - 32}, {playerX + 32, playerY} };

	if (c == 'w' || c == 'a' || c == 's' || c == 'd') {
		bool canGo = true;
		collisionCheckEvent(c, canGo);
		if (canGo) {
			/* Удаление предыдущего положения игрока */
			SelectObject(dc, mainFieldBgBrush);
			switch (prevLookDirection) {
			case 0: {
				Polygon(dc, lookUp, 5);
				break;
			}
			case 1: {
				Polygon(dc, lookRight, 5);
				break;
			}
			case 2: {
				Polygon(dc, lookLeft, 5);
				break;

			}
			case 3: {
				Polygon(dc, lookDown, 5);
				break;
			}
			}
			DeleteObject(lookUp);
			DeleteObject(lookRight);
			DeleteObject(lookLeft);
			DeleteObject(lookDown);

			switch (c) { // Изменение координат
			case 'w':
				dy = -8;
				break;
			case 's':
				dy = 8;
				break;
			case 'a':
				dx = -8;
				break;
			case 'd':
				dx = 8;
				break;
			default:
				break;
			}
			playerX += dx;
			playerY += dy;
			dx = 0;
			dy = 0;

			POINT lookUp[5] = { {playerX, playerY - 32}, {playerX + 32, playerY}, {playerX + 32, playerY + 32}, {playerX - 32, playerY + 32}, {playerX - 32, playerY} }; // Отрисовка вверх (первая координата - нос)
			POINT lookRight[5] = { {playerX + 32, playerY}, {playerX, playerY + 32}, {playerX - 32, playerY + 32}, {playerX - 32, playerY - 32}, {playerX, playerY - 32} }; // Отрисовка вправо (первая координата - нос)
			POINT lookLeft[5] = { {playerX - 32, playerY}, {playerX, playerY - 32}, {playerX + 32, playerY - 32}, {playerX + 32, playerY + 32}, {playerX, playerY + 32} };
			POINT lookDown[5] = { {playerX, playerY + 32}, {playerX - 32, playerY}, {playerX - 32, playerY - 32}, {playerX + 32, playerY - 32}, {playerX + 32, playerY} };

			/* Перерисовка игрока на новом месте */
			SelectObject(dc, playerBrush);
			switch (c)
			{
			case 'w': //движение			
				Polygon(dc, lookUp, 5);
				prevLookDirection = 0;
				break;
			case 's':
				Polygon(dc, lookDown, 5);
				prevLookDirection = 3;
				break;
			case 'a':
				Polygon(dc, lookLeft, 5);
				prevLookDirection = 2;
				break;
			case 'd':
				Polygon(dc, lookRight, 5);
				prevLookDirection = 1;
				break;
			default:
				break;
			}
			DeleteObject(lookUp);
			DeleteObject(lookLeft);
			DeleteObject(lookRight);
			DeleteObject(lookDown);
		}
	}
}

/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

int main() {
	srand(time(0));
	/* Устанавливаем полный экран и берем координаты окна */
	COORD d;
	SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), 1, &d);
	RECT winCoord = {};
	GetWindowRect(hwnd, &winCoord);

	/* Рисуем серый фон */
	HBRUSH sideBgBrush = CreateSolidBrush(RGB(166, 166, 166));
	SelectObject(dc, sideBgBrush);
	Rectangle(dc, 0, 0, winCoord.right, winCoord.bottom);

	/* Рисуем черный фон для игрового поля */
	HBRUSH mainFieldBgBrush = CreateSolidBrush(RGB(0, 0, 0));
	SelectObject(dc, mainFieldBgBrush);
	Rectangle(dc, 150, 50, 1622, 1010);

	/* Задаем начальные координаты игрока */
	playerX = 150 + 10 * 64;
	playerY = 50 + 13 * 64;

	/* Отрисовка различных положений танка, смотрящего ввех, вниз, вправо или влево */
	POINT lookUp[5] = { {playerX, playerY - 32}, {playerX + 32, playerY}, {playerX + 32, playerY + 32}, {playerX - 32, playerY + 32}, {playerX - 32, playerY} }; // Отрисовка вверх (первая координата - нос)
	POINT lookRight[5] = { {playerX + 32, playerY}, {playerX, playerY + 32}, {playerX - 32, playerY + 32}, {playerX - 32, playerY - 32}, {playerX, playerY - 32} }; // Отрисовка вправо (первая координата - нос)
	POINT lookLeft[5] = { {playerX - 32, playerY}, {playerX, playerY - 32}, {playerX + 32, playerY - 32}, {playerX + 32, playerY + 32}, {playerX, playerY + 32} };
	POINT lookDown[5] = { {playerX, playerY + 32}, {playerX - 32, playerY}, {playerX - 32, playerY - 32}, {playerX + 32, playerY - 32}, {playerX+32, playerY} };

	/* Первичная отрисовка игрока */
	HBRUSH playerBrush = CreateSolidBrush(RGB(255, 0, 0));
	SelectObject(dc, playerBrush);
	Polygon(dc, lookUp, 5);
	unsigned short int prevLookDirection = 0; // 0 - смотрит вверх, 1 - смотрит вправо, 2 - смотрит влево, 3 - смотрит вниз
	int fireTimer = 0; // Время перезарядки выстрела
	char c; // Переменная для управления первым игроком
	HBRUSH ballBrush = CreateSolidBrush(RGB(0, 0, 255));

	drawObjects();
	drawEnemy();
	unsigned short int prevEnemyDir = 3;
	int enemyTimer = 40;
	int enemyReload = 0;
	while(true){
		
		/* Обработка нажатия клавиши движения и стрельбы */
		if (_kbhit()) {
			c = _getch();
			if (c != 'f') {
				firstPlayerControl(mainFieldBgBrush, playerBrush, prevLookDirection, c);
			}
			else if ( c == 'f' && fireTimer < 1) {
				fireAction(prevLookDirection, fireTimer);
			}
		}

		// Уничтожение врага
		if (enemyHealth <= 0) {
			SelectObject(dc, mainFieldBgBrush);
			Rectangle(dc, enemyX - 32, enemyY - 32, enemyX + 32, enemyY + 32);
			enemyX = -100;
			enemyY = -100;
			break;
		}
		else {
			enemyTimer--;
			if (enemyTimer < 1) {
				enemyTimer = 40;
			}
			enemyControl(mainFieldBgBrush, playerBrush, prevEnemyDir, enemyTimer, enemyReload);
		}

		/* Изменение координат каждого выстрела и отсчет времени перезарядки */
		shotsMoving(mainFieldBgBrush, ballBrush);
		/* Перезарядка орудия */
		if (fireTimer > -2) {
			fireTimer--;
		}
		if (enemyReload > -2) {
			enemyReload--;
		}
		if (playerHealth < 1) {
			break;
		}
		/* Счетчик фпс(30fps) */
		Sleep(1000 / 30);
	}
	cout << "End game";

	return 0;
}