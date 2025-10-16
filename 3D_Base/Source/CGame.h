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
*	ゲームクラス.
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

	static constexpr int ENEMY_MAX = 3;		//エネミーの最大数
	static constexpr int BULLET_MAX = 100;	//弾の最大

	CGame( CDirectX9& pDx9, CDirectX11& pDx11, HWND hWnd, CTime& pTime );
	~CGame();

	void Create();
	HRESULT LoadData();
	void Release();
	void Update();
	void Draw();

private:
	//カメラ関数.
	void Camera();
	//プロジェクション関数.
	void Projection();

	void CreateElite(CEnemy*& pE);

	//三人称カメラ
	void ThirdPersonCamera(
		CAMERA* pCamera, const D3DXVECTOR3& TargetPos, float TargetRotY);

	void TopDownCamera(
		CAMERA* pCamera, const D3DXVECTOR3& TargetPos, float TargetRotY);

	//マウスの移動によってカメラの更新
	void CameraRotToMouse(CAMERA* pCamera, const D3DXVECTOR3& TargetPos, POINT delta, float sense);

	//自機の発射を管理する
	void HandlePlayerShot();
	//N‐WAYショットを管理する
	void HandleNWayShot(int bulletCount);

	float GetNWayRot(float spreadDeg, int bulletCount, int bulletNo);
	
	void HandleBossShot();
	void HandleChargedShot();

private:
	CDebugText*		m_pDbgText;	//デバッグテキスト

	//レイ表示クラス
	CRay*			m_pRayY;	//Y方向(垂直)
	CRay*			m_pCrossRay[CROSSRAY::max];	//十字

	//カメラ情報.
	CAMERA			m_Camera;
	//ライト情報
	LIGHT			m_Light;

	//行列.
	D3DXMATRIX		m_mView;	//ビュー(カメラ)行列.
	D3DXMATRIX		m_mProj;	//射影（プロジェクション）行列.

	//ゲームで扱うスプライトデータ(使いまわす資源).
	CSprite3D*		m_pSpriteGround;
	CSprite3D*		m_pSpritePlayer;
	CSprite3D*		m_pSpriteExplosion;
	CSprite3D*		m_pSpriteBullet;	//弾
	CSprite3D*		m_pSpriteBossBullet;	//弾

	//スタティックメッシュ(使いまわす資源)
	CStaticMesh*	m_pStaticMeshFighter;	//自機
	CStaticMesh*	m_pStaticMeshGround;	//地面
	CStaticMesh*	m_pStaticMeshRoboA;		//ロボA
	CStaticMesh*	m_pStaticMeshRoboB;		//ロボB
	CStaticMesh*	m_pStaticMeshBSphere;	//バウンディングスフィア(当たり判定用)
	CStaticMesh*	m_pStaticMeshBoss;

	//スプライトオブジェクトクラス.
	CSpriteObject*		m_pExplosion;

	//キャラクタークラス
	CPlayer*			m_pPlayer;
	CEnemy*				m_pEnemy;
	CEnemy*				m_pEnemies[ENEMY_MAX];
	CEnemy**			m_ppEnemies;
	int					m_EnemyMax;

	//ボース
	CBoss*				m_pBoss;

	//地面クラス
	CGround*			m_pGround;

	//弾クラス
	CShot*				m_pShot[BULLET_MAX];
	CShot*				m_pBossShot[BULLET_MAX];
	std::queue<CShot*>	m_ShotQue;
	std::queue<CShot*>	m_BossShotQue;

	//スーコア
	int m_Score;

	//クールタイム
	float	m_shotCd;
	float	m_bossCd;


private:
	//=delete「削除定義」と呼ばれる機能.
	//指定された場合、その関数は呼び出せなくなる.
	CGame() = delete;	//デフォルトコンストラクタ禁止.
	CGame( const CGame& ) = delete;
	CGame& operator = (const CGame& rhs ) = delete;
};