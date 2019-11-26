#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <vector>
#include <ctime>
using namespace std;
HWND hwnd = GetConsoleWindow();
HDC dc = GetDC(hwnd);
float playerX, playerY; // ���������� ������
float enemyX = 150 + 11 * 64, enemyY = 50 + 3 * 64;
int enemyHealth = 500;
int damage = 50;
int playerHealth = 100;


/* �������� ��������� */
vector <float> upFiresY;
vector <float> upFiresX;
vector <float> rightFiresY;
vector <float> rightFiresX;
vector <float> leftFiresY;
vector <float> leftFiresX;
vector <float> downFiresY;
vector <float> downFiresX;

vector <int> wallsXCoord;	//�����, ������� ����� �������
vector <int> wallsYCoord;

vector <int> blocksXCoord;	//������������� �����
vector <int> blocksYCoord;

vector <int> waterXCoord;	//����
vector <int> waterYCoord;
/* /////////////////////////////////////////////// */

void drawEnemy() {
	HBRUSH enemyBrush = CreateSolidBrush(RGB(255, 0, 0));
	/* ��������� ��������� ��������� �����, ���������� ����, ����, ������ ��� ����� */
	POINT lookUp[5] = { {enemyX, enemyY - 32}, {enemyX + 32, enemyY}, {enemyX + 32, enemyY + 32}, {enemyX - 32, enemyY + 32}, {enemyX - 32, enemyY} }; // ��������� ����� (������ ���������� - ���)
	POINT lookRight[5] = { {enemyX + 32, enemyY}, {enemyX, enemyY + 32}, {enemyX - 32, enemyY + 32}, {enemyX - 32, enemyY - 32}, {enemyX, enemyY - 32} }; // ��������� ������ (������ ���������� - ���)
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
	switch (dir) {	//��������
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
			for (int i = 0; i < blocksXCoord.size(); i++) {		// �������� ������������� ������
				if (((dPlayerX - 32 >= blocksXCoord[i] && dPlayerX - 32 <= blocksXCoord[i] + 64) || (dPlayerX + 32 >= blocksXCoord[i] && dPlayerX + 32 <= blocksXCoord[i] + 64))
					&& (dPlayerY - 32 <= blocksYCoord[i] + 64 && dPlayerY - 32 >= blocksYCoord[i])) {
					canGo = false;
					break;
				}
				else canGo = true;
			}
		}
		if (canGo) {
			for (int i = 0; i < wallsXCoord.size(); i++) {	// �������� ����������� ������
				if (((dPlayerX - 32 > wallsXCoord[i] && dPlayerX - 32 < wallsXCoord[i] + 64) || (dPlayerX + 32 > wallsXCoord[i] && dPlayerX + 32 < wallsXCoord[i] + 64)
					|| (dPlayerX - 32 == wallsXCoord[i] && dPlayerX + 32 == wallsXCoord[i] + 64)) && (dPlayerY - 32 < wallsYCoord[i] + 64 && dPlayerY - 32 > wallsYCoord[i])) {
					canGo = false;
					break;
				}
				else canGo = true;
			}
		}
		if (canGo) {
			for (int i = 0; i < waterXCoord.size(); i++) {	// �������� ����
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
			for (int i = 0; i < blocksXCoord.size(); i++) {		// �������� ������������� ������
				if (((dPlayerX - 32 >= blocksXCoord[i] && dPlayerX - 32 <= blocksXCoord[i] + 64) || (dPlayerX + 32 >= blocksXCoord[i] && dPlayerX + 32 <= blocksXCoord[i] + 64))
					&& (dPlayerY + 32 >= blocksYCoord[i] && dPlayerY + 32 <= blocksYCoord[i] + 64)) {
					canGo = false;
					break;
				}
				else canGo = true;
			}
		}
		if (canGo) {
			for (int i = 0; i < wallsXCoord.size(); i++) {	// �������� ����������� ������
				if (((dPlayerX - 32 > wallsXCoord[i] && dPlayerX - 32 < wallsXCoord[i] + 64) || (dPlayerX + 32 > wallsXCoord[i] && dPlayerX + 32 < wallsXCoord[i] + 64)
					|| (dPlayerX - 32 == wallsXCoord[i] && dPlayerX + 32 == wallsXCoord[i] + 64)) && (dPlayerY + 32 < wallsYCoord[i] + 64 && dPlayerY + 32 > wallsYCoord[i])) {
					canGo = false;
					break;
				}
				else canGo = true;
			}
		}
		if (canGo) {
			for (int i = 0; i < waterXCoord.size(); i++) {	// �������� ����
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
			for (int i = 0; i < blocksXCoord.size(); i++) {		// �������� ������������� ������
				if (((dPlayerY + 32 >= blocksYCoord[i] && dPlayerY + 32 <= blocksYCoord[i] + 64) || (dPlayerY - 32 >= blocksYCoord[i] && dPlayerY - 32 <= blocksYCoord[i] + 64))
					&& (dPlayerX - 32 >= blocksXCoord[i] && dPlayerX - 32 <= blocksXCoord[i] + 64)) {
					canGo = false;
					break;
				}
				else canGo = true;
			}
		}
		if (canGo) {
			for (int i = 0; i < wallsXCoord.size(); i++) {	// �������� ����������� ������
				if (((dPlayerY + 32 > wallsYCoord[i] && dPlayerY + 32 < wallsYCoord[i] + 64) || (dPlayerY - 32 > wallsYCoord[i] && dPlayerY - 32 < wallsYCoord[i] + 64)
					|| (dPlayerY - 32 == wallsYCoord[i] && dPlayerY + 32 == wallsYCoord[i] + 64)) && (dPlayerX - 32 > wallsXCoord[i] && dPlayerX - 32 < wallsXCoord[i] + 64)) {
					canGo = false;
					break;
				}
				else canGo = true;
			}
		}
		if (canGo) {
			for (int i = 0; i < waterXCoord.size(); i++) {	// �������� ����
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
			for (int i = 0; i < blocksXCoord.size(); i++) {		// �������� ������������� ������
				if (((dPlayerY + 32 >= blocksYCoord[i] && dPlayerY + 32 <= blocksYCoord[i] + 64) || (dPlayerY - 32 >= blocksYCoord[i] && dPlayerY - 32 <= blocksYCoord[i] + 64))
					&& (dPlayerX + 32 >= blocksXCoord[i] && dPlayerX + 32 <= blocksXCoord[i] + 64)) {
					canGo = false;
					break;
				}
				else canGo = true;
			}
		}
		if (canGo) {
			for (int i = 0; i < wallsXCoord.size(); i++) {	// �������� ����������� ������
				if (((dPlayerY + 32 > wallsYCoord[i] && dPlayerY + 32 < wallsYCoord[i] + 64) || (dPlayerY - 32 > wallsYCoord[i] && dPlayerY - 32 < wallsYCoord[i] + 64)
					|| (dPlayerY - 32 == wallsYCoord[i] && dPlayerY + 32 == wallsYCoord[i] + 64)) && (dPlayerX + 32 > wallsXCoord[i] && dPlayerX + 32 < wallsXCoord[i] + 64)) {
					canGo = false;
					break;
				}
				else canGo = true;
			}
		}
		if (canGo) {
			for (int i = 0; i < waterXCoord.size(); i++) {	// �������� ����
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

void enemyControl(const HBRUSH& bgBrush, const HBRUSH& playerBrush, unsigned short int& prevEnemyDir, int& enemyTimer, int& enemyReload) { // ������ ��������� �������� ��������� ��������, � ����� ����������� ���������
	int fire = 1;
	if (enemyReload < 2) {
		enemyFireAction(prevEnemyDir, enemyReload);
	}
	float dx = 0, dy = 0; // ������������ ������
	POINT lookUp[5] = { {enemyX, enemyY - 32}, {enemyX + 32, enemyY}, {enemyX + 32, enemyY + 32}, {enemyX - 32, enemyY + 32}, {enemyX - 32, enemyY} };
	POINT lookRight[5] = { {enemyX + 32, enemyY}, {enemyX, enemyY + 32}, {enemyX - 32, enemyY + 32}, {enemyX - 32, enemyY - 32}, {enemyX, enemyY - 32} }; 
	POINT lookLeft[5] = { {enemyX - 32, enemyY}, {enemyX, enemyY - 32}, {enemyX + 32, enemyY - 32}, {enemyX + 32, enemyY + 32}, {enemyX, enemyY + 32} };
	POINT lookDown[5] = { {enemyX, enemyY + 32}, {enemyX - 32, enemyY}, {enemyX - 32, enemyY - 32}, {enemyX + 32, enemyY - 32}, {enemyX + 32, enemyY} };
	
	int randDir = prevEnemyDir;
	/* ���� ������ ����������, �� ����������� ������� � ������������ ����� ����������� �������� ����� */
	if (enemyTimer <= 2) {
		randDir = 0 + rand() % 4; // �� 0 �� 3
	}
	bool canGo = true;
	enemyCollisionCheckEvent(randDir, canGo);
	if (canGo) {

		// �������� � ����������� ����������
		SelectObject(dc, bgBrush);
		switch (prevEnemyDir)
		{
		case 0: // ������� �����
			Polygon(dc, lookUp, 5);
			break;
		case 1: // ������� ������
			Polygon(dc, lookRight, 5);
			break;
		case 2: // ������� �����
			Polygon(dc, lookLeft, 5);
			break;
		case 3: // ������� ����
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
		case 0: // ������ �����
			dy = -4;
			break;
		case 1: // ������ ������
			dx = 4;
			break;
		case 2: // ������ �����
			dx = -4;
			break;
		case 3: // ������ ����
			dy = 4;
			break;
		default:
			break;
		}
		enemyX += dx;
		enemyY += dy;
		dx = 0;
		dy = 0;

		/* ����������� �� ����� ����� */
		SelectObject(dc, playerBrush);

		POINT lookU[5] = { {enemyX, enemyY - 32}, {enemyX + 32, enemyY}, {enemyX + 32, enemyY + 32}, {enemyX - 32, enemyY + 32}, {enemyX - 32, enemyY} };
		POINT lookR[5] = { {enemyX + 32, enemyY}, {enemyX, enemyY + 32}, {enemyX - 32, enemyY + 32}, {enemyX - 32, enemyY - 32}, {enemyX, enemyY - 32} };
		POINT lookL[5] = { {enemyX - 32, enemyY}, {enemyX, enemyY - 32}, {enemyX + 32, enemyY - 32}, {enemyX + 32, enemyY + 32}, {enemyX, enemyY + 32} };
		POINT lookD[5] = { {enemyX, enemyY + 32}, {enemyX - 32, enemyY}, {enemyX - 32, enemyY - 32}, {enemyX + 32, enemyY - 32}, {enemyX + 32, enemyY} };

		switch (randDir) {
		case 0: // ������ �����
			Polygon(dc, lookU, 5);
			prevEnemyDir = 0;
			break;
		case 1: // ������ ������
			Polygon(dc, lookR, 5);
			prevEnemyDir = 1;
			break;
		case 2: // ������ �����
			Polygon(dc, lookL, 5);
			prevEnemyDir = 2;
			break;
		case 3: // ������ ����
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



/* �������� ������� �������� � ������ ������������ */
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

/* ����������� ����� */
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

/* �������� ��������� */
bool shotsCollision(int i, vector<float> fireDirectionX, vector<float> fireDirectionY){
	float dFireX, dFireY;
	if (fireDirectionX == upFiresX && fireDirectionY == upFiresY) {
		dFireX = fireDirectionX[i];
		dFireY = fireDirectionY[i] - 20;

		/* �������� �� ��������� � ���������� */
		if ((dFireX >= enemyX - 32 && dFireX <= enemyX + 32) && (dFireY <= enemyY + 32 && dFireY >= enemyY - 32)) {
			enemyHealth -= damage;
			return false;
		}
		if ((dFireX >= playerX - 32 && dFireX <= playerX + 32) && (dFireY <= playerY + 32 && dFireY >= playerY - 32)) {
			playerHealth -= damage;
			return false;
		}
		for (int j = 0; j < blocksXCoord.size(); j++) {
			if ((dFireX >= blocksXCoord[j] && dFireX <= blocksXCoord[j] + 64) && (dFireY < blocksYCoord[j] + 64 && dFireY > blocksYCoord[j])) { // ���� ������������
				return false;
			}
		}
		for (int i = 0; i < wallsXCoord.size(); i++) {
			if ((dFireX >= wallsXCoord[i] && dFireX <= wallsXCoord[i] + 64) && (dFireY < wallsYCoord[i] + 64 && dFireY > wallsYCoord[i])) {	// ������������ � ������������ ���������
				removeWall(wallsXCoord[i], wallsYCoord[i], i);
				return false;
			}
		}
		return true;
	}
	if (fireDirectionX == rightFiresX && fireDirectionY == rightFiresY) {
		dFireX = fireDirectionX[i] + 20;
		dFireY = fireDirectionY[i];
		
		/* �������� �� ��������� � ���������� */
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
			if (dFireY <= wallsYCoord[i] + 64 && dFireY >= wallsYCoord[i] && dFireX <= wallsXCoord[i] + 64 && dFireX >= wallsXCoord[i]) { // ������������ � ������������ ���������
				removeWall(wallsXCoord[i], wallsYCoord[i], i);
				return false;
			}
		}
		return true;
	}
	if (fireDirectionX == leftFiresX && fireDirectionY == leftFiresY) {
		dFireX = fireDirectionX[i] - 20;
		dFireY = fireDirectionY[i];
		
		/* �������� �� ��������� � ���������� */
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
			if (dFireY <= wallsYCoord[i] + 64 && dFireY >= wallsYCoord[i] && dFireX <= wallsXCoord[i] + 64 && dFireX >= wallsXCoord[i]) { // ������������ � ������������ ���������
				removeWall(wallsXCoord[i], wallsYCoord[i], i);
				return false;
			}
		}
		return true;
	}
	if (fireDirectionX == downFiresX && fireDirectionY == downFiresY) {
		dFireX = fireDirectionX[i];
		dFireY = fireDirectionY[i] + 20;
		
		/* �������� �� ��������� � ���������� */
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
			if ((dFireX >= wallsXCoord[i] && dFireX <= wallsXCoord[i] + 64) && (dFireY < wallsYCoord[i] + 64 && dFireY > wallsYCoord[i])) { // ������������ � ������������ ���������
				removeWall(wallsXCoord[i], wallsYCoord[i], i);
				return false;
			}
		}
		return true;
	}
	return false;
}

/* ��������� ��������� ������� �������� � ������ ������� ����������� */
void shotsMoving(const HBRUSH& mainFieldBgBrush, const HBRUSH& ballBrush) {
	
	/* ���������� �������������� ������� � ������ ��������� */
	for (int i = 0; i < upFiresY.size(); i++) {
		if (shotsCollision(i, upFiresX, upFiresY)) { // ���� ���� �� ������������, �� ��� ����������������
			SelectObject(dc, mainFieldBgBrush);
			Ellipse(dc, upFiresX[i], upFiresY[i], upFiresX[i] + 12, upFiresY[i] + 24);
			upFiresY[i] -= 20;
			SelectObject(dc, ballBrush);
			Ellipse(dc, upFiresX[i], upFiresY[i], upFiresX[i] + 12, upFiresY[i] + 24);
		}
		else { // ���� ���� ������������, �� ��� ���������� � ��������� �� �������
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

/* ��������� ��������� ���� ����������� �������� �� ����� */
void drawObjects() {
	/* ��������� �������� �� ���� */

	for (int i = 150; i < 1622; i += 64) {	// ���������� �� ������
		blocksXCoord.push_back(i);
		blocksYCoord.push_back(1010 - 64);
		blocksXCoord.push_back(i);
		blocksYCoord.push_back(50);
	}
	for (int i = 50; i < 1010; i += 64) {	// ���������� �� ������
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

/* �������, ������������ �� ��������� ������ � ������������ */
void collisionCheckEvent(char c, bool &canGo) {
	float dPlayerX, dPlayerY;
	switch (c) {	//��������
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
			for (int i = 0; i < blocksXCoord.size(); i++) {		// �������� ������������� ������
				if (((dPlayerX - 32 >= blocksXCoord[i] && dPlayerX - 32 <= blocksXCoord[i] + 64) || (dPlayerX + 32 >= blocksXCoord[i] && dPlayerX + 32 <= blocksXCoord[i] + 64))
					&& (dPlayerY - 32 <= blocksYCoord[i] + 64 && dPlayerY - 32 >= blocksYCoord[i])) {
					canGo = false;
					break;
				}
				else canGo = true;
			}
		}
		if (canGo) {
			for (int i = 0; i < wallsXCoord.size(); i++) {	// �������� ����������� ������
				if (((dPlayerX - 32 > wallsXCoord[i] && dPlayerX - 32 < wallsXCoord[i] + 64) || (dPlayerX + 32 > wallsXCoord[i] && dPlayerX + 32 < wallsXCoord[i] + 64)
					 || (dPlayerX - 32 == wallsXCoord[i] && dPlayerX + 32 == wallsXCoord[i] + 64)) && (dPlayerY - 32 < wallsYCoord[i] + 64 && dPlayerY - 32 > wallsYCoord[i])) {
					canGo = false;
					break;
				}
				else canGo = true;
			}
		}
		if (canGo) {
			for (int i = 0; i < waterXCoord.size(); i++) {	// �������� ����
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
			for (int i = 0; i < blocksXCoord.size(); i++) {		// �������� ������������� ������
				if (((dPlayerX - 32 >= blocksXCoord[i] && dPlayerX - 32 <= blocksXCoord[i] + 64) || (dPlayerX + 32 >= blocksXCoord[i] && dPlayerX + 32 <= blocksXCoord[i] + 64))
					&& (dPlayerY + 32 >= blocksYCoord[i] && dPlayerY + 32 <= blocksYCoord[i] + 64)) {
					canGo = false;
					break;
				}
				else canGo = true;
			}
		}
		if (canGo) {
			for (int i = 0; i < wallsXCoord.size(); i++) {	// �������� ����������� ������
				if (((dPlayerX - 32 > wallsXCoord[i] && dPlayerX - 32 < wallsXCoord[i] + 64) || (dPlayerX + 32 > wallsXCoord[i] && dPlayerX + 32 < wallsXCoord[i] + 64) 
					 || (dPlayerX - 32 == wallsXCoord[i] && dPlayerX + 32 == wallsXCoord[i] + 64)) && (dPlayerY + 32 < wallsYCoord[i] + 64 && dPlayerY + 32 > wallsYCoord[i])) {
					canGo = false;
					break;
				}
				else canGo = true;
			}
		}
		if (canGo) {
			for (int i = 0; i < waterXCoord.size(); i++) {	// �������� ����
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
			for (int i = 0; i < blocksXCoord.size(); i++) {		// �������� ������������� ������
				if (((dPlayerY + 32 >= blocksYCoord[i] && dPlayerY + 32 <= blocksYCoord[i] + 64) || (dPlayerY - 32 >= blocksYCoord[i] && dPlayerY - 32 <= blocksYCoord[i] + 64))
					&& (dPlayerX - 32 >= blocksXCoord[i] && dPlayerX - 32 <= blocksXCoord[i] + 64)) {
					canGo = false;
					break;
				}
				else canGo = true;
			}
		}
		if (canGo) {
			for (int i = 0; i < wallsXCoord.size(); i++) {	// �������� ����������� ������
				if (((dPlayerY + 32 > wallsYCoord[i] && dPlayerY + 32 < wallsYCoord[i] + 64) || (dPlayerY - 32 > wallsYCoord[i] && dPlayerY - 32 < wallsYCoord[i] + 64)
					 || (dPlayerY - 32 == wallsYCoord[i] && dPlayerY + 32 == wallsYCoord[i] + 64)) && (dPlayerX - 32 > wallsXCoord[i] && dPlayerX - 32 < wallsXCoord[i] + 64)) {
					canGo = false;
					break;
				}
				else canGo = true;
			}
		}
		if (canGo) {
			for (int i = 0; i < waterXCoord.size(); i++) {	// �������� ����
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
			for (int i = 0; i < blocksXCoord.size(); i++) {		// �������� ������������� ������
				if (((dPlayerY + 32 >= blocksYCoord[i] && dPlayerY + 32 <= blocksYCoord[i] + 64) || (dPlayerY - 32 >= blocksYCoord[i] && dPlayerY - 32 <= blocksYCoord[i] + 64))
					&& (dPlayerX + 32 >= blocksXCoord[i] && dPlayerX + 32 <= blocksXCoord[i] + 64)) {
					canGo = false;
					break;
				}
				else canGo = true;
			}
		}
		if (canGo) {
			for (int i = 0; i < wallsXCoord.size(); i++) {	// �������� ����������� ������
				if (((dPlayerY + 32 > wallsYCoord[i] && dPlayerY + 32 < wallsYCoord[i] + 64) || (dPlayerY - 32 > wallsYCoord[i] && dPlayerY - 32 < wallsYCoord[i] + 64)
					 || (dPlayerY - 32 == wallsYCoord[i] && dPlayerY + 32 == wallsYCoord[i] + 64)) && (dPlayerX + 32 > wallsXCoord[i] && dPlayerX + 32 < wallsXCoord[i] + 64)) {
					canGo = false;
					break;
				}
				else canGo = true;
			}
		}
		if (canGo) {
			for (int i = 0; i < waterXCoord.size(); i++) {	// �������� ����
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

/* ���������� ������� ������ ��� ������� ������ */
void firstPlayerControl(const HBRUSH& mainFieldBgBrush, const HBRUSH& playerBrush, unsigned short int& prevLookDirection, char c) {
	float dx = 0, dy = 0; // ������������ ������
	POINT lookUp[5] = { {playerX, playerY - 32}, {playerX + 32, playerY}, {playerX + 32, playerY + 32}, {playerX - 32, playerY + 32}, {playerX - 32, playerY} }; // ��������� ����� (������ ���������� - ���)
	POINT lookRight[5] = { {playerX + 32, playerY}, {playerX, playerY + 32}, {playerX - 32, playerY + 32}, {playerX - 32, playerY - 32}, {playerX, playerY - 32} }; // ��������� ������ (������ ���������� - ���)
	POINT lookLeft[5] = { {playerX - 32, playerY}, {playerX, playerY - 32}, {playerX + 32, playerY - 32}, {playerX + 32, playerY + 32}, {playerX, playerY + 32} };
	POINT lookDown[5] = { {playerX, playerY + 32}, {playerX - 32, playerY}, {playerX - 32, playerY - 32}, {playerX + 32, playerY - 32}, {playerX + 32, playerY} };

	if (c == 'w' || c == 'a' || c == 's' || c == 'd') {
		bool canGo = true;
		collisionCheckEvent(c, canGo);
		if (canGo) {
			/* �������� ����������� ��������� ������ */
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

			switch (c) { // ��������� ���������
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

			POINT lookUp[5] = { {playerX, playerY - 32}, {playerX + 32, playerY}, {playerX + 32, playerY + 32}, {playerX - 32, playerY + 32}, {playerX - 32, playerY} }; // ��������� ����� (������ ���������� - ���)
			POINT lookRight[5] = { {playerX + 32, playerY}, {playerX, playerY + 32}, {playerX - 32, playerY + 32}, {playerX - 32, playerY - 32}, {playerX, playerY - 32} }; // ��������� ������ (������ ���������� - ���)
			POINT lookLeft[5] = { {playerX - 32, playerY}, {playerX, playerY - 32}, {playerX + 32, playerY - 32}, {playerX + 32, playerY + 32}, {playerX, playerY + 32} };
			POINT lookDown[5] = { {playerX, playerY + 32}, {playerX - 32, playerY}, {playerX - 32, playerY - 32}, {playerX + 32, playerY - 32}, {playerX + 32, playerY} };

			/* ����������� ������ �� ����� ����� */
			SelectObject(dc, playerBrush);
			switch (c)
			{
			case 'w': //��������			
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
	/* ������������� ������ ����� � ����� ���������� ���� */
	COORD d;
	SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), 1, &d);
	RECT winCoord = {};
	GetWindowRect(hwnd, &winCoord);

	/* ������ ����� ��� */
	HBRUSH sideBgBrush = CreateSolidBrush(RGB(166, 166, 166));
	SelectObject(dc, sideBgBrush);
	Rectangle(dc, 0, 0, winCoord.right, winCoord.bottom);

	/* ������ ������ ��� ��� �������� ���� */
	HBRUSH mainFieldBgBrush = CreateSolidBrush(RGB(0, 0, 0));
	SelectObject(dc, mainFieldBgBrush);
	Rectangle(dc, 150, 50, 1622, 1010);

	/* ������ ��������� ���������� ������ */
	playerX = 150 + 10 * 64;
	playerY = 50 + 13 * 64;

	/* ��������� ��������� ��������� �����, ���������� ����, ����, ������ ��� ����� */
	POINT lookUp[5] = { {playerX, playerY - 32}, {playerX + 32, playerY}, {playerX + 32, playerY + 32}, {playerX - 32, playerY + 32}, {playerX - 32, playerY} }; // ��������� ����� (������ ���������� - ���)
	POINT lookRight[5] = { {playerX + 32, playerY}, {playerX, playerY + 32}, {playerX - 32, playerY + 32}, {playerX - 32, playerY - 32}, {playerX, playerY - 32} }; // ��������� ������ (������ ���������� - ���)
	POINT lookLeft[5] = { {playerX - 32, playerY}, {playerX, playerY - 32}, {playerX + 32, playerY - 32}, {playerX + 32, playerY + 32}, {playerX, playerY + 32} };
	POINT lookDown[5] = { {playerX, playerY + 32}, {playerX - 32, playerY}, {playerX - 32, playerY - 32}, {playerX + 32, playerY - 32}, {playerX+32, playerY} };

	/* ��������� ��������� ������ */
	HBRUSH playerBrush = CreateSolidBrush(RGB(255, 0, 0));
	SelectObject(dc, playerBrush);
	Polygon(dc, lookUp, 5);
	unsigned short int prevLookDirection = 0; // 0 - ������� �����, 1 - ������� ������, 2 - ������� �����, 3 - ������� ����
	int fireTimer = 0; // ����� ����������� ��������
	char c; // ���������� ��� ���������� ������ �������
	HBRUSH ballBrush = CreateSolidBrush(RGB(0, 0, 255));

	drawObjects();
	drawEnemy();
	unsigned short int prevEnemyDir = 3;
	int enemyTimer = 40;
	int enemyReload = 0;
	while(true){
		
		/* ��������� ������� ������� �������� � �������� */
		if (_kbhit()) {
			c = _getch();
			if (c != 'f') {
				firstPlayerControl(mainFieldBgBrush, playerBrush, prevLookDirection, c);
			}
			else if ( c == 'f' && fireTimer < 1) {
				fireAction(prevLookDirection, fireTimer);
			}
		}

		// ����������� �����
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

		/* ��������� ��������� ������� �������� � ������ ������� ����������� */
		shotsMoving(mainFieldBgBrush, ballBrush);
		/* ����������� ������ */
		if (fireTimer > -2) {
			fireTimer--;
		}
		if (enemyReload > -2) {
			enemyReload--;
		}
		if (playerHealth < 1) {
			break;
		}
		/* ������� ���(30fps) */
		Sleep(1000 / 30);
	}
	cout << "End game";

	return 0;
}