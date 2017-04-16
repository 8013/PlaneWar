#pragma once
#include "PlaneWarDoc.h"
#include "Plane.h"
#include "Enemy.h"

#define ID_BACKGROUND 100
#define ID_MYPLANEMOVE 200
#define ID_MYPLANESHOT 201
#define ID_BULLET 300
#define ID_BULLET_ENEMY 301
#define ID_CREATEENEMY 400
#define ID_ENEMYMOVE 401
#define ID_ENEMYSHOT 402
#define ID_FRESH 500
#define ID_LEVELUP 501

enum STATE{ START, RUNNING, PAUSE, END };

class CPlaneWarView : public CView{
protected: // 仅从序列化创建
	CPlaneWarView();
	DECLARE_DYNCREATE(CPlaneWarView)

// 特性
public:
	CPlaneWarDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CPlaneWarView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	CImage starting, ending;
	CImage background_first;
	CImage background_second;
	int background_py;
	bool isFirst;
	int create_speed;
public:
	Plane plane;
	vector<Bullet*> bullets;
	vector<Enemy*> enemies;
	vector<EnemyBullet*> enemy_bullets;
	STATE state;
	int score;
// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	void StartUI(CDC *pDC);
	void RunningUI(CDC *pDC);
	void EndUI(CDC *pDC);
	void SetTimers();
	void KillTimers();
	void LevelUP();
	void UpdateBackground(CDC *pDC);
	void HandleBullets();
	void HandleEnemyBullets();
	void CreateEnemy();
	void EnemyMove();
	void EnemyShot();
	void Clear();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // PlaneWarView.cpp 中的调试版本
inline CPlaneWarDoc* CPlaneWarView::GetDocument() const
   { return reinterpret_cast<CPlaneWarDoc*>(m_pDocument); }
#endif

