#include "CPlayer.h"
#include "CSoundManager.h"

CPlayer::CPlayer()
	: m_TurnSpeed	(0.1f)	//���������肽���ꍇ�̓��W�A���l��ݒ肷�邱��
	, m_MoveSpeed	(0.1f)
	, m_MoveState	(enMoveState::Stop)
	, m_shotMax		(10)
	, m_ShotNumber	(enShotNumber::Triple)
	, m_ShotType	(enShotType::Charged)
	, m_Cadence(100.0f)	//�A�ˑ��x
	, m_NWaySrpeadDeg(30.0f)	//NWay�e�̍L����p�x
	, m_ChargedShotTime(0.f)
	, m_CadenceTimer(0.f)
	, m_IsBomb(false)
	
{
}

CPlayer::~CPlayer() = default;

void CPlayer::Update()
{
	HandleInput();			//���͏���
	//���C�̈ʒu���v���C���[�̍��W�ɂ��낦��
	m_pRayY->Position = m_vPosition;
	//�n�ʂ߂荞�݉���̂��߃v���C���[�̈ʒu����������ɂ��Ă���
	m_pRayY->Position.y += 0.2f;
	m_pRayY->RotationY = m_vRotation.y;

	//�\���i�O�㍶�E�ɐL�΂����j���C�̐ݒ�
	for (int dir = 0; dir < CROSSRAY::max; dir++)
	{
		m_pCrossRay->Ray[dir].Position = m_vPosition;
		m_pCrossRay->Ray[dir].Position.y += 0.2f;
		m_pCrossRay->Ray[dir].RotationY = m_vRotation.y;
	}

	CCharacter::Update();
}	

void CPlayer::Draw(
	D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	CCharacter::Draw(View, Proj, Light, Camera);
}

//���W�I����
void CPlayer::RadioControl()
{
	//Z���x�N�g��(Z+�����ւ̒P�ʃx�N�g��)
	//���傫���i�����j���P�̃x�N�g����P�ʃx�N�g���Ƃ���
	D3DXVECTOR3 vecAxisZ(0.f, 0.f, 1.f);
	D3DXVECTOR3 vecAxisX(1.f, 0.f, 0.f);

	//Y�����̉�]�s��
	D3DXMATRIX mRotationY;
	//Y����]�s����쐬
	D3DXMatrixRotationY(
		&mRotationY,		//(out)�s��
		m_vRotation.y);		//�v���C���[��Y�����̉�]�l

	//Y����]�s����g����Z���x�N�g�������W�ϊ�����
	D3DXVec3TransformCoord(
		&vecAxisZ,		//(out)Z���x�N�g��
		&vecAxisZ,		//(in)Z���x�N�g��
		&mRotationY);	//Y����]�s��

	D3DXVec3TransformCoord(
		&vecAxisX,		//(out)Z���x�N�g��
		&vecAxisX,		//(in)Z���x�N�g��
		&mRotationY);	//Y����]�s��

	//�ړ���Ԃɂ���ď����𕪂���
	switch (m_MoveState) {
	case enMoveState::Forward:	//�O�i
		m_vPosition += vecAxisZ * m_MoveSpeed;
		break;
	case enMoveState::Backward:	//���
		m_vPosition -= vecAxisZ * m_MoveSpeed;
		break;
	case enMoveState::Leftward:	//���ړ�
		m_vPosition += vecAxisX * m_MoveSpeed;
		break;
	case enMoveState::Rightward:	//�E�ړ�
		m_vPosition -= vecAxisX * m_MoveSpeed;
		break;
	default:
		break;
	}
	//��L�̈ړ��������I���Β�~��Ԃɂ��Ă���
	m_MoveState = enMoveState::Stop;
}

void CPlayer::HandleInput()
{
#if 1
	//�O�i
	if (GetAsyncKeyState(VK_UP) & 0x8000 || GetAsyncKeyState('W') & 0x8000) {
		m_MoveState = enMoveState::Forward;
	}
	//���
	if (GetAsyncKeyState(VK_DOWN) & 0x8000 || GetAsyncKeyState('S') & 0x8000) {
		m_MoveState = enMoveState::Backward;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000 || GetAsyncKeyState('A') & 0x8000) {
		m_MoveState = enMoveState::Rightward;
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000 || GetAsyncKeyState('D') & 0x8000) {
		m_MoveState = enMoveState::Leftward;
	}
	RadioControl();
#else
	float add_value = 0.1f;
	if (GetAsyncKeyState(VK_UP) & 0x8000) {
		m_vPosition.y += add_value;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		m_vPosition.y -= add_value;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		m_vPosition.x += add_value;
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		m_vPosition.x -= add_value;
	}
#endif
	//�O��̃t���[���Œe���΂��Ă��邩���m��Ȃ��̂�false�ɂ���
	m_Shot = false;

	bool fireDown = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;

	//�e���΂�����!
	if (fireDown) 
	{
		m_IsCharging = true;

		m_ChargedShotTime += 1000.0f / FPS;
		m_ChargedShotTime = min(m_ChargedShotTime, m_ChargedShotMax);

		if (m_CadenceTimer <= 0.0f && m_ShotType == CCharacter::Simple)
		{
			m_Shot = true;
			//�A�ˑ��x�����Z�b�g
			ResetCadenceTimer();
		}

	}

	if (!fireDown && m_WasFireDown)
	{
		m_Shot = true;

		ChargedTime = m_ChargedShotTime;
		m_ChargedShotTime = 0.0f;
		m_IsCharging = false;
	}

	m_WasFireDown = fireDown;

}
