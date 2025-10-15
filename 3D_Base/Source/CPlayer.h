#pragma once

#include "CCharacter.h"

/**************************************************
*	プレイヤークラス.
**/
class CPlayer
	: public CCharacter	//キャラクタークラスを継承.
{
public:

	//移動状態列挙型
	enum enMoveState
	{
		Stop = 0,	//停止
		Forward,	//前進
		Backward,	//後退
		Leftward,
		Rightward,
		TurnLeft,	//左回転
		TurnRight,	//右回転
	};
public:
	CPlayer();
	virtual ~CPlayer() override;

	virtual void Update() override;
	virtual void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera ) override;

	//ラジコン操作
	void RadioControl();

	bool IsBomb() const { return m_IsBomb; }

	int GetShotNumber() const { return m_ShotNumber; }
	int GetShotType()	const { return m_ShotType; }
	float GetShotCadence() const { return m_Cadence; }
	float GetNWaySpreadDeg() const { return m_NWaySrpeadDeg; }
	float GetChargedShotTime() const { return m_ChargedShotTime; }
	float GetChargedShotMax() const { return m_ChargedShotMax; }

	void IncCadenceTimer(float delta) { m_CadenceTimer += delta; }
	void DecCadenceTimer(float delta) { m_CadenceTimer -= delta; }
	void ResetCadenceTimer() { m_CadenceTimer = m_Cadence; }

	void SetPlayerCadence(float cad) { m_Cadence = cad; }
	void SetPlayerShotType(enShotType type) { m_ShotType = type; }

public:
	float			ChargedTime;

protected:

	int				m_shotMax;
	float			m_TurnSpeed;				//回転速度
	float			m_MoveSpeed;				//移動速度
	enMoveState		m_MoveState;				//移動状態
	enShotType		m_ShotType;					//弾の種類
	enShotNumber	m_ShotNumber;				//弾の数

	float			m_Cadence;					//連射速度
	float			m_CadenceTimer;				//連射タイマー	
	float			m_NWaySrpeadDeg; 	//NWay弾の広がり角度
	float			m_ChargedShotTime;	//チャージショット時間
	const float		m_ChargedShotMax = 5.0f;	//チャージショット最大時間

	bool			m_IsBomb;

private:

	void HandleInput();

	bool m_IsCharging;
	bool m_WasFireDown;
};