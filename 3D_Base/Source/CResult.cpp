#include "CResult.h"
#include "CSoundManager.h"

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

	if (FAILED(m_pDbgText->Init(*m_pDx11)))
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

	m_mouseSeudoPos = { WND_W / 2, (WND_H / 2) - 150 };

}

void CResult::Update()
{
	CScene::Update();

	CSoundManager::PlayLoop(CSoundManager::BGM_Menu);

	POINT mousePos = GetMouseSeudoPos();
	m_pCursor->SetPosition(mousePos.x, mousePos.y, 0.f);

	if (mousePos.x <= 100 + 50 * 6 && mousePos.x >= 100 &&
		mousePos.y <= 400 + 50 && mousePos.y >= 400)
	{
		MenuColor = Color(1.f, 0.f, 0.f);
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			m_pManager->ChangeScene("GameMain");
		}
	}
	else
		MenuColor = Color(1.f, 1.f, 1.f);

}

void CResult::Draw()
{
	m_pTitleSprite->Render();
	m_pCursor->Draw();

	int score = CCommon::SCORE;

	TCHAR dbgText[64];
	m_pDbgText->SetColor(1.0, 1.0, 1.0);
	_stprintf_s(dbgText, _T("RESULT"));
	m_pDbgText->Render(dbgText, 100, 140, 50);
	_stprintf_s(dbgText, _T("Score: %d"), int(score));
	m_pDbgText->Render(dbgText, 100, 240, 50);
	_stprintf_s(dbgText, _T("AGAIN?"));
	m_pDbgText->SetColor(MenuColor.r, MenuColor.g, MenuColor.b);
	m_pDbgText->Render(dbgText, 100, 400, 50);

}
