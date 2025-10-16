#pragma once
#include "CScene.h"
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
#include "CBoss.h"
#include "CShot.h"
#include "CSkinMesh.h"
#include "CZako.h"
#include "CRay.h"
#include <vector>
#include <queue>

/********************************************************************************
*	�Q�[���N���X.
**/
class CGame : public CScene
{
public:
	
	enum enGameScene
	{
		Title,
		GameOver,
		GameMain,
		Result,
	};

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

	void CreateElite(CEnemy*& pE);

	//�O�l�̃J����
	void ThirdPersonCamera(
		CAMERA* pCamera, const D3DXVECTOR3& TargetPos, float TargetRotY);

	void TopDownCamera(
		CAMERA* pCamera, const D3DXVECTOR3& TargetPos, float TargetRotY);

	//�}�E�X�̈ړ��ɂ���ăJ�����̍X�V
	void CameraRotToMouse(CAMERA* pCamera, const D3DXVECTOR3& TargetPos, POINT delta, float sense);

	//���@�̔��˂��Ǘ�����
	void HandlePlayerShot();
	//N�]WAY�V���b�g���Ǘ�����
	void HandleNWayShot(int bulletCount);

	float GetNWayRot(float spreadDeg, int bulletCount, int bulletNo);
	
	void HandleBossShot();
	void HandleChargedShot();

private:
	CDebugText*		m_pDbgText;	//�f�o�b�O�e�L�X�g

	//���C�\���N���X
	CRay*			m_pRayY;	//Y����(����)
	CRay*			m_pCrossRay[CROSSRAY::max];	//�\��

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
	CSprite3D*		m_pSpriteBossBullet;	//�e

	//�X�^�e�B�b�N���b�V��(�g���܂킷����)
	CStaticMesh*	m_pStaticMeshFighter;	//���@
	CStaticMesh*	m_pStaticMeshGround;	//�n��
	CStaticMesh*	m_pStaticMeshRoboA;		//���{A
	CStaticMesh*	m_pStaticMeshRoboB;		//���{B
	CStaticMesh*	m_pStaticMeshBSphere;	//�o�E���f�B���O�X�t�B�A(�����蔻��p)
	CStaticMesh*	m_pStaticMeshBoss;

	//�X�v���C�g�I�u�W�F�N�g�N���X.
	CSpriteObject*		m_pExplosion;

	//�L�����N�^�[�N���X
	CPlayer*			m_pPlayer;
	CEnemy*				m_pEnemy;
	CEnemy*				m_pEnemies[ENEMY_MAX];
	CEnemy**			m_ppEnemies;
	int					m_EnemyMax;

	//�{�[�X
	CBoss*				m_pBoss;

	//�n�ʃN���X
	CGround*			m_pGround;

	//�e�N���X
	CShot*				m_pShot[BULLET_MAX];
	CShot*				m_pBossShot[BULLET_MAX];
	std::queue<CShot*>	m_ShotQue;
	std::queue<CShot*>	m_BossShotQue;

	//�X�[�R�A
	int m_Score;

	//�N�[���^�C��
	float	m_shotCd;
	float	m_bossCd;


private:
	//=delete�u�폜��`�v�ƌĂ΂��@�\.
	//�w�肳�ꂽ�ꍇ�A���̊֐��͌Ăяo���Ȃ��Ȃ�.
	CGame() = delete;	//�f�t�H���g�R���X�g���N�^�֎~.
	CGame( const CGame& ) = delete;
	CGame& operator = (const CGame& rhs ) = delete;
};