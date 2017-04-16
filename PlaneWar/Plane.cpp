#include "stdafx.h"
#include "Plane.h"
#include "MainFrm.h"
#include "PlaneWarView.h"

Plane::Plane() {
	image.Load(L"res/plane.png");
	image_height = image.GetHeight();
	image_width = image.GetWidth();
	position.x = (window_width - image_width) / 2;
	position.y = window_height - image_height;
	this->speed = 16;
	this->damage = 1;
}

int Plane::getBlood(){
	return blood;
}

int Plane::getDamage(){
	return damage;
}

void Plane::setBlood(int blood){
	this->blood = blood;
}

void Plane::BloodDown(int damage){
	blood -= damage;
}

void Plane::DamageUp(){
	damage++;
}

int Plane::MoveDirection(){
	// 返回移动方向
	int direction = 5;
	int up = GetKeyState(VK_UP);
	int down = GetKeyState(VK_DOWN);
	int left = GetKeyState(VK_LEFT);
	int right = GetKeyState(VK_RIGHT);
	if (up < 0) {
		if (left < 0) direction = 7;
		else if (right < 0) direction = 9;
		else direction = 8;
	}
	else if (down < 0) {
		if (left < 0) direction = 1;
		else if (right < 0) direction = 3;
		else direction = 2;
	}
	else if (left < 0) {
		if (up < 0) direction = 7;
		else if (down < 0) direction = 1;
		else direction = 4;
	}
	else if (right < 0) {
		if (up < 0) direction = 9;
		else if (down < 0) direction = 3;
		else direction = 6;
	}
	return direction;
}

bool Plane::CheckBoundary(int direction){
	int height = image.GetHeight(), width = image.GetWidth();
	switch (direction) {
	case 1:
		if (position.y + image_height < window_height && position.x  > 0)
			return true;
		break;
	case 2:
		if (position.y + image_height < window_height)
			return true;
		break;
	case 3:
		if (position.y + image_height < window_height && position.x + image_width  < window_width)
			return true;
		break;
	case 4:
		if (position.x > 0)
			return true;
		break;
	case 6:
		if (position.x + image_width  < window_width)
			return true;
		break;
	case 7:
		if(position.x  > 0 && position.y > 0)
			return true;
		break;
	case 8:
		if (position.y > 0)
			return true;
		break;
	case 9:
		if (position.x + image_width  < window_width && position.y>0)
			return true;
		break;
	default:
		break;
	}
	return false;
}

void Plane::HitDetection(){
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	CPlaneWarView* pView = (CPlaneWarView*)pFrame->GetActiveView();

	vector<Enemy*>::iterator it = pView->enemies.begin();
	while (it != pView->enemies.end()) {

		// 与一架敌机碰撞
		if ((*it)->Contains(position.x,position.y) || (*it)->Contains(position.x,position.y+image_height) || (*it)->Contains(position.x+image_width,position.y) || (*it)->Contains(position.x + image_width, position.y+image_height)) {
			delete (*it);
			pView->enemies.erase(it);
			blood--;
			if (blood <= 0) {
				pView->state = END;
				pView->KillTimers();
			}
			break;
		}
		else {
			it++;
		}
	}
}

void Plane::Draw(CDC * pDC){
	image.Draw(pDC->m_hDC, position.x, position.y);
}

void Plane::Move(){
	int direction = MoveDirection();

	if (CheckBoundary(direction) != true)
		return;

	switch (direction) {
	case 1:
		position.y+=speed; position.x-=speed; break;
	case 2:
		position.y+=speed; break;
	case 3:
		position.y+=speed; position.x+=speed; break;
	case 4:
		position.x-=speed; break;
	case 6:
		position.x+=speed; break;
	case 7:
		position.y-=speed; position.x-=speed; break;
	case 8:
		position.y-=speed; break;
	case 9:
		position.y-=speed; position.x+=speed; break;
	default:break;
	}
	HitDetection();
}

void Plane::Shot(){
	int space = GetKeyState(VK_SPACE);
	if (space < 0) {
		Bullet *bullet = new Bullet(position, speed, damage);
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		CPlaneWarView* pView = (CPlaneWarView*)pFrame->GetActiveView();
		pView->bullets.push_back(bullet);
	}
}
