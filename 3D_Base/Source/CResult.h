#pragma once

#include "CScene.h"
#include "CSprite2D.h"
#include "CUIObject.h"
#include "CFont.h"

class CResult : public CScene
{

public:
	CResult(CDirectX9& pDx9, CDirectX11& pDx11, HWND hWnd, CTime& pTime, CSceneManager& pManager);
	virtual ~CResult();
	void Create() override;
	HRESULT LoadData() override;
	void Release() override;
	void Start() override;
	void Update() override;
	void Draw() override;

private:
	// Resources specific to the result screen
	CSprite2D*   m_pTitleSprite;
	CUIObject*   m_pTitleBackground;
	CSprite2D*   m_pCursorSprite;
	CUIObject*   m_pCursor;
	CFont*       m_pDbgText;
	struct Color
	{
		float r;
		float g;
		float b;
	};
	Color MenuColor;

};