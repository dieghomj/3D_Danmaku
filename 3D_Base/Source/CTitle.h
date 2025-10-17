#pragma once
#include "CSprite2D.h"
#include "CUIObject.h"
#include "CScene.h"
#include "CDebugText.h"
/********************************************************************************
*	タイトルシーンクラス.
**/
class CTitle : public CScene
{
public:
    struct Color
    {
        float r;
        float g;
        float b;
    };
public:
	CTitle(CDirectX9& pDx9, CDirectX11& pDx11, HWND hWnd, CTime& pTime, CSceneManager& pManager);
	virtual ~CTitle();

    void Create() override;
    HRESULT LoadData() override;
    void Release() override;

    void Start() override;
    void Update() override;
    void Draw() override;

private:
    // Resources specific to the title screen
    CSprite2D*   m_pTitleSprite;
    CUIObject*   m_pTitleBackground;
    CSprite2D*   m_pCursorSprite;
    CUIObject*   m_pCursor;
    CDebugText*  m_pDbgText;

    Color startMenuColor;
};