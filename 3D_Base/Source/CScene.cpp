#include "CScene.h"

CScene::CScene(CDirectX9& pDx9, CDirectX11& pDx11, HWND hWnd, CTime& pTime)
	: m_pDx9(&pDx9)
	, m_pDx11(&pDx11)
	, m_hWnd(hWnd)
	, m_pTime(&pTime)

	, m_mousePos		({0,0})
	, m_mouseBeforePos	({0,0})
	, m_mouseDelta		({ 0,0 })
	, m_mouseSense		( 0.01f )
{
}

CScene::~CScene()
{

	//äOïîÇ≈çÏê¨ÇµÇƒÇ¢ÇÈÇÃÇ≈ÅAÇ±Ç±Ç≈ÇÕîjä¸ÇµÇ»Ç¢
	m_hWnd = nullptr;
	m_pDx11 = nullptr;
	m_pDx9 = nullptr;
}

void CScene::Update()
{
	UpdateMousePos();
}

void CScene::UpdateMousePos()
{
	POINT mousePos;
	GetCursorPos(&mousePos);
	ScreenToClient(m_hWnd, &mousePos);

	// Calculate delta from center
	POINT center = { WND_W / 2, WND_H / 2 };
	m_mouseDelta.x = mousePos.x - center.x;
	m_mouseDelta.y = mousePos.y - center.y;

	// Reset cursor to center
	ClientToScreen(m_hWnd, &center);
	SetCursorPos(center.x, center.y);
}
