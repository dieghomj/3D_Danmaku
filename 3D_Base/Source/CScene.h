#pragma once
#include "CDirectX9.h"
#include "CDirectX11.h"
#include "CTime.h"
#include "CSceneManager.h"
#include "CSprite2D.h"

/********************************************************************************
*	�Q�[���V�[���N���X.
**/
class CScene
{
public:

	CScene(CDirectX9& pDx9, CDirectX11& pDx11, HWND hWnd, CTime& pTime, CSceneManager& m_pManager);

	virtual ~CScene();

	virtual void Create() = 0;
	virtual HRESULT LoadData() = 0;
	virtual void Release() = 0;

	virtual void Start() = 0;
	virtual void Update();
	virtual void Draw() = 0;
	POINT GetMouseSeudoPos();

private :
	void UpdateMousePos();

protected:
	CDirectX9* m_pDx9;
	CDirectX11* m_pDx11;
	//�E�B���h�E�n���h��.
	HWND			m_hWnd;
	CTime*			m_pTime;
	CSceneManager*	m_pManager;

	//mouse����
	POINT m_mousePos;
	POINT m_mouseSeudoPos;
	POINT m_mouseBeforePos;
	POINT m_mouseDelta;
	float m_mouseSense;

private:
	//=delete�u�폜��`�v�ƌĂ΂��@�\.
	//�w�肳�ꂽ�ꍇ�A���̊֐��͌Ăяo���Ȃ��Ȃ�.
	CScene() = delete;	//�f�t�H���g�R���X�g���N�^�֎~.
	CScene(const CScene&) = delete;
	CScene& operator = (const CScene& rhs) = delete;
};