#pragma once
#include "GameObject.h"
class EnemyBullet : public GameObject{
private:
	int damage;
public:
	EnemyBullet(CPoint p, int speed);
	void Draw(CDC *pDC);
	void Move(bool &out);

	bool HitDetection();
};

