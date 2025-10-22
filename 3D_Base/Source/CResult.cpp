#include "CResult.h"

CResult::CResult(CDirectX9& pDx9, CDirectX11& pDx11, HWND hWnd, CTime& pTime, CSceneManager& pManager)
	: CScene(pDx9, pDx11, hWnd, pTime, pManager)
	, m_pTitleBackground(nullptr)
	, m_pDbgText(nullptr)
	, m_pCursorSprite(nullptr)
	, m_pCursor(nullptr)
	, m_pTitleSprite(nullptr)
	, MenuColor({ 1.f, 1.f, 1.f })
{
}

CResult::~CResult()
{
}

void CResult::Create()
{
	m_pTitleSprite = new CSprite2D();
	m_pTitleBackground = new CUIObject();
	m_pCursor = new CUIObject();
	m_pCursorSprite = new CSprite2D();
	m_pDbgText = new CFont();
}

HRESULT CResult::LoadData()
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

	m_pTitleSprite->SetAlpha(0.5f);

	if (FAILED(m_pCursorSprite->Init(*m_pDx11,
		_T("Data\\Texture\\Player.png"), SSPlayer)))
	{
		return E_FAIL;
	}

	m_pTitleBackground->AttachSprite(*m_pTitleSprite);
	m_pTitleBackground->SetRotation(D3DXVECTOR3(0.1f, 0.5f, 1.f));
	m_pCursor->AttachSprite(*m_pCursorSprite);

	m_pTitleBackground->SetPosition(0.f, 0.f, 0.f);

	if (FAILED(m_pDbgText->Init(*m_pDx11)));
	{
		return E_FAIL;
	}

	return S_OK;
}

void CResult::Release()
{

}

void CResult::Start()
{
	m_pDx11->SetDepth(false);
}

void CResult::Update()
{
	CScene::Update();

	POINT mousePos = GetMouseSeudoPos();
	m_pCursor->SetPosition(mousePos.x, mousePos.y, 0.f);
}

void CResult::Draw()
{
	m_pTitleSprite->Render();
	m_pCursor->Draw();

	TCHAR dbgText[64];
	m_pDbgText->SetColor(MenuColor.r, MenuColor.g, MenuColor.b);
	_stprintf_s(dbgText, _T("RESULT"));
	m_pDbgText->Render(dbgText, 100, 140, 50);
	_stprintf_s(dbgText, _T("Score: 000000"));
	m_pDbgText->Render(dbgText, 100, 240, 50);
	_stprintf_s(dbgText, _T("Enemies Killed: 000"));
	m_pDbgText->Render(dbgText, 100, 280, 50);
	_stprintf_s(dbgText, _T("Boss Killed: Yes"));
	m_pDbgText->Render(dbgText, 100, 320, 50);
	_stprintf_s(dbgText, _T("CONTINUE?"));
	m_pDbgText->Render(dbgText, 100, 400, 50);

}
