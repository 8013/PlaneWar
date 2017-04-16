#pragma once
#include "GameObject.h"
#include "EnemyBullet.h"

class Enemy : public GameObject{
private:
	int blood;
public:
	Enemy();
	int getBlood();
	void BloodDown(int damage);
	void Draw(CDC *pDC);
	void Move(bool &out);
	void Shot();
};

