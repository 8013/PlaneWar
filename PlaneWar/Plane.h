#pragma once
#include "GameObject.h"
#include "Bullet.h"

class Plane : public GameObject{
private:
	int blood;
	int damage;
	int MoveDirection();
	bool CheckBoundary(int direction);
	void HitDetection();
public:
	Plane();
	int getBlood();
	int getDamage();
	void setBlood(int blood);
	void BloodDown(int damage);
	void DamageUp();
	void Draw(CDC *pDC);
	void Move();
	void Shot();
};

