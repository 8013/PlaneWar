#pragma once
#include "GameObject.h"

class Bullet : public GameObject{
private:
	int damage;
public:
	Bullet(CPoint p, int speed, int damage);
	void Draw(CDC *pDC);
	void Move(bool &out);

	bool HitDetection();
};

