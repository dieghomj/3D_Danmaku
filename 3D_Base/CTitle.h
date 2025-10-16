#pragma once
#include "Source/CScene.h";
#include "Source/CSprite2D.h"
#include "Source/CUIObject.h"

/********************************************************************************
*	タイトルシーンクラス.
**/
class CTitle : public CScene
{

public:

	CTitle(CDirectX9& pDx9, CDirectX11& pDx11, HWND hWnd, CTime& pTime);
	virtual ~CTitle();

    void Create() override;
    HRESULT LoadData() override;
    void Release() override;
    void Update() override;
    void Draw() override;

private:
    // Resources specific to the title screen
    CSprite2D*   m_pTitleSprite;
    CUIObject*  m_pTitleBackground;
    CSprite2D*  m_pTitleFont;
};