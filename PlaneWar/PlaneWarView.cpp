#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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

// CPlaneWarView ����/����

CPlaneWarView::CPlaneWarView(){}

CPlaneWarView::~CPlaneWarView(){}

BOOL CPlaneWarView::PreCreateWindow(CREATESTRUCT& cs){
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CPlaneWarView ���

#ifdef _DEBUG
void CPlaneWarView::AssertValid() const{
	CView::AssertValid();
}

void CPlaneWarView::Dump(CDumpContext& dc) const{
	CView::Dump(dc);
}

CPlaneWarDoc* CPlaneWarView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPlaneWarDoc)));
	return (CPlaneWarDoc*)m_pDocument;
}
#endif //_DEBUG

// CPlaneWarView ����

// ������Ϸ״̬���в�ͬ�Ļ���
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

// ��ʼ����
void CPlaneWarView::StartUI(CDC *pDC){

	starting.Draw(pDC->m_hDC, 0, 0);

	int r = rand() % 256, g = rand() % 256, b = rand() % 256;
	CString str;
	pDC->SetTextColor(RGB(r, g, b));
	pDC->SetBkMode(TRANSPARENT);
	str.Format(L"�ƶ����������ҷ����");
	pDC->TextOutW(300, 650, str);
	str.Format(L"������ո��");
	pDC->TextOutW(300, 700, str);
	str.Format(L"�����˺���D����400�ֿ�������");
	pDC->TextOutW(300, 750, str);
	str.Format(L"���������ʼ��Ϸ");
	pDC->TextOutW(300, 800, str);
}

// ����ʱ����
void CPlaneWarView::RunningUI(CDC *pDC){
	// ���±���
	UpdateBackground(pDC);
	// ���Լ��ķɻ�
	plane.Draw(pDC);
	// ���Լ����ӵ�
	for (unsigned int i = 0; i < bullets.size(); i++) 
		bullets[i]->Draw(pDC);
	// ���л����ӵ�
	for (unsigned int i = 0; i < enemy_bullets.size(); i++) 
		enemy_bullets[i]->Draw(pDC);
	// ���л�
	for (unsigned int i = 0; i < enemies.size(); i++) 
		enemies[i]->Draw(pDC);
	
	CString str;
	pDC->SetTextColor(RGB(255, 255, 0));
	pDC->SetBkMode(TRANSPARENT);
	str.Format(L"������%5d", score);
	pDC->TextOutW(0, 0, str);
	str.Format(L"������%5d", plane.getBlood());
	pDC->TextOutW(0, 25, str);
	str.Format(L"�˺���%5d", plane.getDamage());
	pDC->TextOutW(0, 50, str);
	str.Format(L"�л������ٶȣ�%5dms", create_speed);
	pDC->TextOutW(0, 75, str);
}

// ��������
void CPlaneWarView::EndUI(CDC *pDC){
	ending.Draw(pDC->m_hDC, 0, 0);

	int r = rand() % 256, g = rand() % 256, b = rand() % 256;
	CString str;
	pDC->SetTextColor(RGB(r, g, b));
	pDC->SetBkMode(TRANSPARENT);
	str.Format(L"������Ϸ�÷֣�%d", score);
	pDC->TextOutW(330, 400, str);

	str.Format(L"���¿�ʼ��R");
	pDC->TextOutW(330, 750, str);
	str.Format(L"�˳���Ϸ��ESC");
	pDC->TextOutW(330, 800, str);
}

// ���ö�ʱ��
void CPlaneWarView::SetTimers(){
	SetTimer(ID_BACKGROUND, 32, 0);				// �����ƶ�
	SetTimer(ID_MYPLANEMOVE, 16, 0);				// �ɻ��ƶ�
	SetTimer(ID_MYPLANESHOT, 32, 0);				// �ɻ����
	SetTimer(ID_BULLET, 16, 0);								// �ӵ��ƶ�����ײ���
	SetTimer(ID_BULLET_ENEMY, 32, 0);								// �ӵ��ƶ�����ײ���
	SetTimer(ID_CREATEENEMY, create_speed, 0);			// �����µĵл�
	SetTimer(ID_ENEMYMOVE, 64, 0);					// �л��ƶ�
	SetTimer(ID_ENEMYSHOT, 1000, 0);				// �л����
}

// ȡ����ʱ��
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

// ��Ϸ�Ѷ�����
void CPlaneWarView::LevelUP(){
	if (create_speed > 10) {
		create_speed -= 10;
		KillTimer(ID_CREATEENEMY);
		SetTimer(ID_CREATEENEMY, create_speed, 0);
	}
}

// ʹ��Ϸ����ʱ�ı���������Ч����
void CPlaneWarView::UpdateBackground(CDC *pDC) {
	//���������߽磬�ͻ�ͼ
	if (background_py == background_first.GetHeight()) {
		background_py = 0;
		isFirst = isFirst ? 0 : 1;
	}
	//�����һ��ͼ��ǰ�棬���Ȼ��Ƶ�һ��
	if (isFirst) {
		background_first.Draw(pDC->m_hDC, 0, background_py);
		background_second.Draw(pDC->m_hDC, 0, background_py - background_first.GetHeight());
	}
	//�ڶ���ͼ��ǰ�棬�Ȼ��Ƶڶ���
	else {
		background_second.Draw(pDC->m_hDC, 0, background_py);
		background_first.Draw(pDC->m_hDC, 0, background_py - background_first.GetHeight());
	}
}

// �����Լ��ɻ������������ӵ�
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

// ����л������������ӵ�
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

// �����µĵл�
void CPlaneWarView::CreateEnemy(){
	Enemy *e=new Enemy();
	enemies.push_back(e);
}

// �л��ƶ�
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

// �л����
void CPlaneWarView::EnemyShot(){
	for (vector<Enemy*>::iterator it = enemies.begin(); it != enemies.end(); it++) 
		(*it)->Shot();
}

// �������
void CPlaneWarView::Clear(){
	score = 0;
	create_speed = 1000;
	background_py = 0;
	isFirst = true;
	bullets.clear();
	enemies.clear();
	enemy_bullets.clear();
}

// CPlaneWarView ��Ϣ�������

// ��ʱ����Ӧ����
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

// ���򴴽�ʱ�ĳ�ʼ������
int CPlaneWarView::OnCreate(LPCREATESTRUCT lpCreateStruct){
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// һЩ��Ҫ�ĳ�ʼ��
	srand((unsigned)time(NULL));
	score = 0;
	create_speed = 1000;

	// ���ؿ�ʼ�����������ͼƬ
	starting.Load(L"res/starting.jpg");
	ending.Load(L"res/ending.jpg");

	// ��������ʱ����
	background_first.Load(L"res/background1.jpg");
	background_second.Load(L"res/background2.jpg");
	background_py = 0;
	isFirst = true;

	// ������Ϸ״̬
	state = START;

	// ����һ������ֹͣ�Ķ�ʱ��
	SetTimer(ID_FRESH, 500, 0);
	SetTimer(ID_LEVELUP, 5000, 0);
	return 0;
}

// ��Ӧ���̲���
void CPlaneWarView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (state == START) {
		state = RUNNING;
		// ���ö�ʱ��
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
