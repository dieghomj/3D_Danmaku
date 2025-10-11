#pragma once
#include "CDirectX9.h"
#include "CDirectX11.h"
#include "CDebugText.h"
#include "CSprite3D.h"
#include "CSprite2D.h"
#include "CSpriteObject.h"
#include "CExplosion.h"
#include "CUIObject.h"
#include "CStaticMesh.h"
#include "CStaticMeshObject.h"
#include "CCharacter.h"
#include "CPlayer.h"
#include "CGround.h"
#include "CEnemy.h"
#include "CShot.h"
#include "CSkinMesh.h"
#include "CZako.h"
#include "CRay.h"

#include <vector>
#include <queue>


/********************************************************************************
*	�Q�[���N���X.
**/
class CGame
{
public:
	enum enShotNumber
	{
		Zero,
		Single = 0,
		Double,
		Triple,
	};

	enum enShotType
	{
		Simple = 0,
		Rapid,
		Homing,
		Laser,
	};

public:
	static constexpr int ENEMY_MAX = 3;		//�G�l�~�[�̍ő吔
	static constexpr int BULLET_MAX = 100;	//�e�̍ő�

	CGame( CDirectX9& pDx9, CDirectX11& pDx11, HWND hWnd, CTime& pTime );
	~CGame();

	void Create();
	HRESULT LoadData();
	void Release();

	void Update();
	void Draw();

private:
	//�J�����֐�.
	void Camera();
	//�v���W�F�N�V�����֐�.
	void Projection();

	//�O�l�̃J����
	void ThirdPersonCamera(
		CAMERA* pCamera, const D3DXVECTOR3& TargetPos, float TargetRotY);

	void TopDownCamera(
		CAMERA* pCamera, const D3DXVECTOR3& TargetPos, float TargetRotY);

	void CameraRotToMouse(CAMERA* pCamera, const D3DXVECTOR3& TargetPos, POINT delta, float sense);

	const float NWayShotRot(float spreadDeg, int bulletCount, int bulletNo);
private:
	CDirectX9*		m_pDx9;
	CDirectX11*		m_pDx11;

	CDebugText*		m_pDbgText;	//�f�o�b�O�e�L�X�g

	//���C�\���N���X
	CRay*			m_pRayY;	//Y����(����)
	CRay*			m_pCrossRay[CROSSRAY::max];	//�\��

	//�E�B���h�E�n���h��.
	HWND			m_hWnd;

	//�J�������.
	CAMERA			m_Camera;
	//���C�g���
	LIGHT			m_Light;

	//�s��.
	D3DXMATRIX		m_mView;	//�r���[(�J����)�s��.
	D3DXMATRIX		m_mProj;	//�ˉe�i�v���W�F�N�V�����j�s��.

	//�Q�[���ň����X�v���C�g�f�[�^(�g���܂킷����).
	CSprite3D*		m_pSpriteGround;
	CSprite3D*		m_pSpritePlayer;
	CSprite3D*		m_pSpriteExplosion;
	CSprite3D*		m_pSpriteBullet;	//�e

	//�X�v���C�g2D�f�[�^(�g���܂킷����)
	CSprite2D*		m_pSprite2DPmon;

	//�X�^�e�B�b�N���b�V��(�g���܂킷����)
	CStaticMesh*	m_pStaticMeshFighter;	//���@
	CStaticMesh*	m_pStaticMeshGround;	//�n��
	CStaticMesh*	m_pStaticMeshRoboA;		//���{A
	CStaticMesh*	m_pStaticMeshRoboB;		//���{B
	CStaticMesh*	m_pStaticMeshBSphere;	//�o�E���f�B���O�X�t�B�A(�����蔻��p)

	//�X�L�����b�V��(�g���܂킷����)
	CSkinMesh*		m_pSkinMeshZako;		//�U�R
	int				m_ZakoAnimNo;			//�U�R�F�A�j���[�V�����ԍ�
	double			m_ZakoAnimTime;			//�U�R�F�A�j���[�V�����o�ߎ���
	D3DXVECTOR3		m_ZakoBonePos;			//�U�R�F�{�[�����W

	//�X�v���C�g�I�u�W�F�N�g�N���X.
	CSpriteObject*		m_pExplosion;

	//UI�I�u�W�F�N�g�N���X
	CUIObject*		m_pPmon;
	CUIObject*		m_pBeedrill;	//#015:�X�s�A�[
	CUIObject*		m_pParasect;	//#047:�p���Z�N�g
	CUIObject*		m_pScyther;		//#123:�X�g���C�N

	//�X�^�e�B�b�N���b�V���I�u�W�F�N�g�N���X
	CStaticMeshObject*	m_pStcMeshObj;

	//�L�����N�^�[�N���X
	CPlayer*			m_pPlayer;
	CEnemy*				m_pEnemy;
	CEnemy*				m_pEnemies[ENEMY_MAX];
	CEnemy**			m_ppEnemies;
	int					m_EnemyMax;

	//�n�ʃN���X
	CGround*			m_pGround;

	//�e�N���X
	CShot*				m_pShot[BULLET_MAX];
	std::queue<CShot*>	m_Shot;

	//�U�R�N���X
	CZako*				m_pZako;

	//std::vector<�錾�������^��> �ϐ���
	std::vector<CZako*>	m_Zako;

	//mouse����
	POINT m_mousePos;
	POINT m_mouseBeforePos;
	POINT m_mouseDelta;
	float m_mouseSense;

	//�^�C��
	CTime*	m_pTime;
	float	m_shotCd;

private:
	//=delete�u�폜��`�v�ƌĂ΂��@�\.
	//�w�肳�ꂽ�ꍇ�A���̊֐��͌Ăяo���Ȃ��Ȃ�.
	CGame() = delete;	//�f�t�H���g�R���X�g���N�^�֎~.
	CGame( const CGame& ) = delete;
	CGame& operator = (const CGame& rhs ) = delete;
};