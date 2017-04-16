#include "stdafx.h"
#include "Bullet.h"
#include "MainFrm.h"
#include "PlaneWarView.h"

Bullet::Bullet(CPoint p, int speed, int damage){
	int n = (rand() % 7) + 1;
	switch (n) {
	case 1: image.Load(L"res/bullet1.png"); break;
	case 2: image.Load(L"res/bullet2.png"); break;
	case 3: image.Load(L"res/bullet3.png"); break;
	case 4: image.Load(L"res/bullet4.png"); break;
	case 5: image.Load(L"res/bullet5.png"); break;
	case 6: image.Load(L"res/bullet6.png"); break;
	case 7: image.Load(L"res/bullet7.png"); break;
	default: image.Load(L"res/bullet1.png"); break;
	}
	image_height = image.GetHeight();
	image_width = image.GetWidth();
	this->position.y = p.y - image_height;
	this->position.x = p.x + 21;
	this->speed = speed * 2;
	this->damage = damage;
}

void Bullet::Draw(CDC * pDC){
	image.Draw(pDC->m_hDC, position.x, position.y);
}

void Bullet::Move(bool &out){
	position.y -= speed;
	if (position.y < 0)
		out = true;
}

bool Bullet::HitDetection(){
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	CPlaneWarView* pView = (CPlaneWarView*)pFrame->GetActiveView();

	vector<Enemy*>::iterator it = pView->enemies.begin();
	while (it != pView->enemies.end()) {
		CPoint enemy_position = (*it)->getPosition();
		int enemy_width = (*it)->getImageWidth();
		int enemy_height = (*it)->getImageHeight();

		// 命中一架敌机
		if (position.y < enemy_position.y + enemy_height) {
			if (position.x + image_width > enemy_position.x && position.x < enemy_position.x + enemy_width) {
				int blood = (*it)->getBlood();
				// 摧毁敌机
				if (blood - damage <= 0) {
					pView->score += (*it)->getSpeed();
					delete (*it);
					pView->enemies.erase(it);
				}
				else {
					(*it)->BloodDown(damage);
				}
				return true;
			}
		}
		it++;
	}
	return false;
}
