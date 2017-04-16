#include "stdafx.h"
#include "EnemyBullet.h"
#include "MainFrm.h"
#include "PlaneWarView.h"

EnemyBullet::EnemyBullet(CPoint p, int speed){
	image.Load(L"res/enemy_bullet.png");
	image_height = image.GetHeight();
	image_width = image.GetWidth();
	this->position.y = p.y + 81;
	this->position.x = p.x + 25;
	this->speed = speed*2;
	this->damage = 1;
}

void EnemyBullet::Draw(CDC * pDC){
	image.Draw(pDC->m_hDC, position.x, position.y);
}

void EnemyBullet::Move(bool & out){
	position.y += speed;
	if (position.y > window_height)
		out = true;
}

bool EnemyBullet::HitDetection(){
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	CPlaneWarView* pView = (CPlaneWarView*)pFrame->GetActiveView();

	CPoint plane_position = pView->plane.getPosition();
	int plane_height = pView->plane.getImageHeight();
	int plane_width = pView->plane.getImageWidth();

	if (position.y + image_height > plane_position.y && position.y < plane_position.y + plane_height) {
		if (position.x + image_width > plane_position.x && position.x < plane_position.x + plane_width) {
			int blood = pView->plane.getBlood();
			if (blood - damage <= 0) {
				pView->state = END;
				pView->KillTimers();
				// GAME OVER
				// exit(0);
			}
			else {
				pView->plane.BloodDown(damage);
			}
			return true;
		}
	}
	return false;
}
