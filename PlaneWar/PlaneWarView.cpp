#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "PlaneWar.h"
#endif

#include "PlaneWarDoc.h"
#include "PlaneWarView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPlaneWarView

IMPLEMENT_DYNCREATE(CPlaneWarView, CView)

BEGIN_MESSAGE_MAP(CPlaneWarView, CView)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CPlaneWarView 构造/析构

CPlaneWarView::CPlaneWarView(){}

CPlaneWarView::~CPlaneWarView(){}

BOOL CPlaneWarView::PreCreateWindow(CREATESTRUCT& cs){
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CPlaneWarView 诊断

#ifdef _DEBUG
void CPlaneWarView::AssertValid() const{
	CView::AssertValid();
}

void CPlaneWarView::Dump(CDumpContext& dc) const{
	CView::Dump(dc);
}

CPlaneWarDoc* CPlaneWarView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPlaneWarDoc)));
	return (CPlaneWarDoc*)m_pDocument;
}
#endif //_DEBUG

// CPlaneWarView 绘制

// 根据游戏状态进行不同的绘制
void CPlaneWarView::OnDraw(CDC* pDC) {
	CPlaneWarDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CDC mem_dc;
	CBitmap bmp;
	CRect rect;
	GetClientRect(&rect);
	mem_dc.CreateCompatibleDC(pDC);
	bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	CBitmap *pOldBitmap = mem_dc.SelectObject(&bmp);

	switch (state) {
	case START:
		StartUI(&mem_dc);
		break;
	case RUNNING:
		RunningUI(&mem_dc);
		break;
	case PAUSE:
		break;
	case END:
		EndUI(&mem_dc);
		break;
	default:
		TRACE("PlaneWarView.cpp OnDraw()");
		break;
	}

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &mem_dc, 0, 0, SRCCOPY);
	mem_dc.SelectObject(pOldBitmap);
	mem_dc.DeleteDC();
	bmp.DeleteObject();
}

// 开始界面
void CPlaneWarView::StartUI(CDC *pDC){

	starting.Draw(pDC->m_hDC, 0, 0);

	int r = rand() % 256, g = rand() % 256, b = rand() % 256;
	CString str;
	pDC->SetTextColor(RGB(r, g, b));
	pDC->SetBkMode(TRANSPARENT);
	str.Format(L"移动：上下左右方向键");
	pDC->TextOutW(300, 650, str);
	str.Format(L"射击：空格键");
	pDC->TextOutW(300, 700, str);
	str.Format(L"提升伤害：D（满400分可提升）");
	pDC->TextOutW(300, 750, str);
	str.Format(L"按任意键开始游戏");
	pDC->TextOutW(300, 800, str);
}

// 运行时界面
void CPlaneWarView::RunningUI(CDC *pDC){
	// 更新背景
	UpdateBackground(pDC);
	// 画自己的飞机
	plane.Draw(pDC);
	// 画自己的子弹
	for (unsigned int i = 0; i < bullets.size(); i++) 
		bullets[i]->Draw(pDC);
	// 画敌机的子弹
	for (unsigned int i = 0; i < enemy_bullets.size(); i++) 
		enemy_bullets[i]->Draw(pDC);
	// 画敌机
	for (unsigned int i = 0; i < enemies.size(); i++) 
		enemies[i]->Draw(pDC);
	
	CString str;
	pDC->SetTextColor(RGB(255, 255, 0));
	pDC->SetBkMode(TRANSPARENT);
	str.Format(L"分数：%5d", score);
	pDC->TextOutW(0, 0, str);
	str.Format(L"生命：%5d", plane.getBlood());
	pDC->TextOutW(0, 25, str);
	str.Format(L"伤害：%5d", plane.getDamage());
	pDC->TextOutW(0, 50, str);
	str.Format(L"敌机生成速度：%5dms", create_speed);
	pDC->TextOutW(0, 75, str);
}

// 结束界面
void CPlaneWarView::EndUI(CDC *pDC){
	ending.Draw(pDC->m_hDC, 0, 0);

	int r = rand() % 256, g = rand() % 256, b = rand() % 256;
	CString str;
	pDC->SetTextColor(RGB(r, g, b));
	pDC->SetBkMode(TRANSPARENT);
	str.Format(L"本次游戏得分：%d", score);
	pDC->TextOutW(330, 400, str);

	str.Format(L"重新开始：R");
	pDC->TextOutW(330, 750, str);
	str.Format(L"退出游戏：ESC");
	pDC->TextOutW(330, 800, str);
}

// 设置定时器
void CPlaneWarView::SetTimers(){
	SetTimer(ID_BACKGROUND, 32, 0);				// 背景移动
	SetTimer(ID_MYPLANEMOVE, 16, 0);				// 飞机移动
	SetTimer(ID_MYPLANESHOT, 32, 0);				// 飞机射击
	SetTimer(ID_BULLET, 16, 0);								// 子弹移动和碰撞检测
	SetTimer(ID_BULLET_ENEMY, 32, 0);								// 子弹移动和碰撞检测
	SetTimer(ID_CREATEENEMY, create_speed, 0);			// 创建新的敌机
	SetTimer(ID_ENEMYMOVE, 64, 0);					// 敌机移动
	SetTimer(ID_ENEMYSHOT, 1000, 0);				// 敌机射击
}

// 取消定时器
void CPlaneWarView::KillTimers(){
	KillTimer(ID_BACKGROUND);
	KillTimer(ID_MYPLANEMOVE);
	KillTimer(ID_MYPLANESHOT);
	KillTimer(ID_BULLET);
	KillTimer(ID_BULLET_ENEMY);
	KillTimer(ID_CREATEENEMY);
	KillTimer(ID_ENEMYMOVE);
	KillTimer(ID_ENEMYSHOT);
}

// 游戏难度提升
void CPlaneWarView::LevelUP(){
	if (create_speed > 10) {
		create_speed -= 10;
		KillTimer(ID_CREATEENEMY);
		SetTimer(ID_CREATEENEMY, create_speed, 0);
	}
}

// 使游戏运行时的背景滚动（效果）
void CPlaneWarView::UpdateBackground(CDC *pDC) {
	//如果到了左边界，就换图
	if (background_py == background_first.GetHeight()) {
		background_py = 0;
		isFirst = isFirst ? 0 : 1;
	}
	//如果第一张图在前面，就先绘制第一张
	if (isFirst) {
		background_first.Draw(pDC->m_hDC, 0, background_py);
		background_second.Draw(pDC->m_hDC, 0, background_py - background_first.GetHeight());
	}
	//第二张图在前面，先绘制第二张
	else {
		background_second.Draw(pDC->m_hDC, 0, background_py);
		background_first.Draw(pDC->m_hDC, 0, background_py - background_first.GetHeight());
	}
}

// 处理自己飞机发出的所有子弹
void CPlaneWarView::HandleBullets(){
	vector<Bullet*>::iterator it = bullets.begin();
	while (it != bullets.end()) {
		bool out = false;
		(*it)->Move(out);
		if (out) {
			delete (*it);
			it = bullets.erase(it);
		}
		else {
			it++;
		}
	}

	it = bullets.begin();
	while (it != bullets.end()) {
		if ((*it)->HitDetection() == true) 
			it = bullets.erase(it);
		else 
			it++;
	}
}

// 处理敌机发出的所有子弹
void CPlaneWarView::HandleEnemyBullets(){
	vector<EnemyBullet*>::iterator it = enemy_bullets.begin();
	while (it != enemy_bullets.end()) {
		bool out = false;
		(*it)->Move(out);
		if (out) {
			delete (*it);
			it = enemy_bullets.erase(it);
		}
		else {
			it++;
		}
	}
	it = enemy_bullets.begin();
	while (it != enemy_bullets.end()) {
		if ((*it)->HitDetection() == true)
			it = enemy_bullets.erase(it);
		else
			it++;
	}
}

// 创建新的敌机
void CPlaneWarView::CreateEnemy(){
	Enemy *e=new Enemy();
	enemies.push_back(e);
}

// 敌机移动
void CPlaneWarView::EnemyMove(){
	vector<Enemy*>::iterator it = enemies.begin();
	while (it != enemies.end()) {
		bool out = false;
		(*it)->Move(out);
		if (out) {
			delete (*it);
			it = enemies.erase(it);
		}
		else {
			it++;
		}
	}
}

// 敌机射击
void CPlaneWarView::EnemyShot(){
	for (vector<Enemy*>::iterator it = enemies.begin(); it != enemies.end(); it++) 
		(*it)->Shot();
}

// 清除操作
void CPlaneWarView::Clear(){
	score = 0;
	create_speed = 1000;
	background_py = 0;
	isFirst = true;
	bullets.clear();
	enemies.clear();
	enemy_bullets.clear();
}

// CPlaneWarView 消息处理程序

// 定时器响应函数
void CPlaneWarView::OnTimer(UINT_PTR nIDEvent){
	
	switch (nIDEvent){
	case ID_BACKGROUND:
		background_py += 5;
		break;
	case ID_MYPLANEMOVE:
		plane.Move();
		break;
	case ID_MYPLANESHOT:
		plane.Shot();
		break;
	case ID_BULLET:
		HandleBullets();
		break;
	case ID_BULLET_ENEMY:
		HandleEnemyBullets();
		break;
	case ID_CREATEENEMY:
		CreateEnemy();
		break;
	case ID_ENEMYMOVE:
		EnemyMove();
		break;
	case ID_ENEMYSHOT:
		EnemyShot();
		break;
	case ID_LEVELUP:
		LevelUP();
		break;
	default:
		break;
	}

	InvalidateRect(NULL, FALSE);
	CView::OnTimer(nIDEvent);
}

// 程序创建时的初始化操作
int CPlaneWarView::OnCreate(LPCREATESTRUCT lpCreateStruct){
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 一些必要的初始化
	srand((unsigned)time(NULL));
	score = 0;
	create_speed = 1000;

	// 加载开始、结束界面的图片
	starting.Load(L"res/starting.jpg");
	ending.Load(L"res/ending.jpg");

	// 加载运行时背景
	background_first.Load(L"res/background1.jpg");
	background_second.Load(L"res/background2.jpg");
	background_py = 0;
	isFirst = true;

	// 设置游戏状态
	state = START;

	// 设置一个不会停止的定时器
	SetTimer(ID_FRESH, 500, 0);
	SetTimer(ID_LEVELUP, 5000, 0);
	return 0;
}

// 响应键盘操作
void CPlaneWarView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (state == START) {
		state = RUNNING;
		// 设置定时器
		SetTimers();
		plane.setBlood(9);
	}
	else if (state == END) {
		if (nChar == 'R') {
			state = START;
			Clear();
		}
		if (nChar == VK_ESCAPE) {
			exit(0);
		}
	}
	else {
		switch (nChar) {
		case 'P':
			if (state == RUNNING) {
				KillTimers();
				state = PAUSE;
			}
			else if (state == PAUSE) {
				SetTimers();
				state = RUNNING;
			}
			else { ; }
			break;
		case 'D':
			if (score > 400) {
				plane.DamageUp();
				score -= 400;
			}
			break;
		default:
			break;
		}
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
