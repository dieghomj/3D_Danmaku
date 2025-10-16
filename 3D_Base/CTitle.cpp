#include "CTitle.h"

CTitle::CTitle(CDirectX9& pDx9, CDirectX11& pDx11, HWND hWnd, CTime& pTime)
		: CScene(pDx9, pDx11, hWnd, pTime)
		, m_pTitleBackground	(nullptr)
		, m_pTitleFont			(nullptr)
{
}

CTitle::~CTitle()
{
}

void CTitle::Create()
{
	m_pTitleSprite		= new CSprite2D();
	m_pTitleBackground	= new CUIObject();
	m_pTitleFont		= new CSprite2D();
}

HRESULT CTitle::LoadData()
{
	CSprite2D::SPRITE_STATE SSGround;
	SSGround.Disp.w = WND_W;
	SSGround.Disp.h = WND_H;
	SSGround.Base.w = 256.f;
	SSGround.Base.h = 256.f;
	SSGround.Stride.w = 256.f;
	SSGround.Stride.h = 256.f;
	m_pTitleSprite->Init(*m_pDx11,
		_T("Data\\Texture\\Ground.png"), SSGround);
	m_pTitleBackground->AttachSprite(*m_pTitleSprite);

	return S_OK;
}

void CTitle::Release()
{
}

void CTitle::Update()
{
	CScene::Update();

	if (GetAsyncKeyState(VK_RETURN & 0x0001) || m_mouseDelta.x != 0)
	{

	}
}

void CTitle::Draw()
{
	m_pTitleBackground->Draw();
}
