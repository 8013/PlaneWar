#include "stdafx.h"
#include "Enemy.h"
#include "MainFrm.h"
#include "PlaneWarView.h"

Enemy::Enemy(){
	int n = (rand() % 6) + 1;
	switch (n) {
	case 1: image.Load(L"res/enemy1.png"); break;
	case 2: image.Load(L"res/enemy2.png"); break;
	case 3: image.Load(L"res/enemy3.png"); break;
	case 4: image.Load(L"res/enemy4.png"); break;
	case 5: image.Load(L"res/enemy5.png"); break;
	case 6: image.Load(L"res/enemy6.png"); break;
	default: image.Load(L"res/enemy1.png"); break;
	}
	image_height = image.GetHeight();
	image_width = image.GetWidth();
	this->position.x = rand()%(window_width-image_width);
	this->speed = rand()%15 + 5;
	this->blood = 8;
}

int Enemy::getBlood(){
	return blood;
}

void Enemy::BloodDown(int damage){
	blood-=damage;
}

void Enemy::Draw(CDC * pDC){
	image.Draw(pDC->m_hDC, position.x, position.y);
}

void Enemy::Move(bool &out){
	position.y += speed;
	if (position.y > window_height)
		out = true;
}

void Enemy::Shot(){
	EnemyBullet *bullet = new EnemyBullet(position, speed);
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	CPlaneWarView* pView = (CPlaneWarView*)pFrame->GetActiveView();
	pView->enemy_bullets.push_back(bullet);
}
