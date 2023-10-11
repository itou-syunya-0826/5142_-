#pragma once

typedef struct PLAYER {
	float posX;
	float posY;
	float radiusX;
	float radiusY;
	float speedY;
}PLAYER;
PLAYER Player{
	0.0f,
	0.0f,
	32.0f,
	32.0f,
	10.0f,
};

typedef struct ENEMY {
	int posX;
	int posY;
	int radiusX;
	int radiusY;
}ENEMY;
ENEMY Enemy{
	600,
	300,
	100,
	100,
};

