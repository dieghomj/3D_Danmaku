#include "CTitle.h"

CTitle::CTitle(CDirectX9& pDx9, CDirectX11& pDx11, HWND hWnd, CTime& pTime, CSceneManager& pManager)
		: CScene(pDx9, pDx11, hWnd, pTime, pManager)
		, m_pTitleBackground	(nullptr)
		, m_pDbgText			(nullptr)
		, m_pCursorSprite		(nullptr)
		, m_pCursor				(nullptr)
{
}

CTitle::~CTitle()
{
}

void CTitle::Create()
{
	m_pTitleSprite		= new CSprite2D();
	m_pTitleBackground	= new CUIObject();
	m_pCursor			= new CUIObject();
	m_pCursorSprite		= new CSprite2D();
	m_pDbgText			= new CDebugText();
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

	CSprite2D::SPRITE_STATE SSPlayer;
	SSPlayer.Disp.w = 12.f;
	SSPlayer.Disp.h = 12.f;
	SSPlayer.Base.w = 64.f;
	SSPlayer.Base.h = 64.f;
	SSPlayer.Stride.w = 64.f;
	SSPlayer.Stride.h = 64.f;
		

	if (FAILED(m_pTitleSprite->Init(*m_pDx11,
		_T("Data\\Texture\\Ground.png"), SSGround)))
	{
		return E_FAIL;
	}
	if( FAILED(m_pCursorSprite->Init(*m_pDx11,
		_T("Data\\Texture\\Player.png"), SSPlayer)))
	{
		return E_FAIL;
	}
	
	m_pTitleBackground->AttachSprite(*m_pTitleSprite);
	m_pCursor->AttachSprite(*m_pCursorSprite);

	m_pTitleBackground->SetPosition(0.f, 0.f, 0.f);
	
	if ( FAILED( m_pDbgText->Init(*m_pDx11) ) );
	{
		return E_FAIL;
	}

	return S_OK;
}

void CTitle::Release()
{
}

void CTitle::Start()
{
}

void CTitle::Update()
{
	CScene::Update();

	POINT mousePos = GetMouseSeudoPos();
	m_pCursor->SetPosition(mousePos.x,mousePos.y,0.f);

	if (mousePos.x <= 100 + 50 * 6 && mousePos.x >= 100 &&
		mousePos.y <= 140 + 50 && mousePos.y >= 140)
	{
		startMenuColor = Color(1.f, 0.f, 0.f);
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			m_pManager->ChangeScene("GameMain");
		}
	}
	else
		startMenuColor = Color(1.f, 1.f, 1.f);
}

void CTitle::Draw()
{

	m_pCursor->Draw();

	TCHAR dbgText[64];
	m_pDbgText->SetColor(startMenuColor.r, startMenuColor.g, startMenuColor.b);
	_stprintf_s(dbgText, _T("START"));
	m_pDbgText->Render(dbgText, 100, 140, 50);

	m_pTitleBackground->Draw();
}