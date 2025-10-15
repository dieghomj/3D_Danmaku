#include "CGame.h"
#include "CSoundManager.h"
#include "CEffect.h"	//Effekseer���g�����߂̃N���X

//�R���X�g���N�^.
CGame::CGame( CDirectX9& pDx9, CDirectX11& pDx11, HWND hWnd, CTime& pTime )
	: m_pDx9			( &pDx9 )
	, m_pDx11			( &pDx11 )
	, m_pDbgText		( nullptr )
	, m_pRayY			( nullptr )
	, m_pCrossRay		()
	, m_hWnd			( hWnd )
	, m_mView			()
	, m_mProj			()


	, m_Camera			()
	, m_Light			()

	, m_pSpriteGround	( nullptr )
	, m_pSpritePlayer	( nullptr )
	, m_pSpriteExplosion( nullptr )

	, m_pSprite2DPmon	( nullptr )

	, m_pStaticMeshFighter	( nullptr )
	, m_pStaticMeshGround	( nullptr )
	, m_pStaticMeshRoboA	( nullptr )
	, m_pStaticMeshRoboB	( nullptr )
	, m_pSpriteBullet	( nullptr )
	, m_pStaticMeshBSphere	( nullptr )

	, m_pSkinMeshZako	( nullptr ) 
	, m_ZakoAnimNo		()
	, m_ZakoAnimTime	()
	, m_ZakoBonePos		()

	, m_pExplosion		( nullptr )

	, m_pPmon			( nullptr )
	, m_pBeedrill		( nullptr )
	, m_pParasect		( nullptr )
	, m_pScyther		( nullptr )

	, m_pStcMeshObj		( nullptr )
	, m_pPlayer			( nullptr )
	, m_pEnemy			( nullptr )
	, m_pEnemies		()
	, m_ppEnemies		()
	, m_EnemyMax		( 100 )

	, m_pGround			( nullptr )

	, m_Shot			()

	, m_pZako			( nullptr )

	, m_Zako			()
	, m_mousePos		({0,0})
	, m_mouseBeforePos	({0,0})
	, m_mouseDelta		({ 0,0 })
	, m_mouseSense		(0.01f)

	, m_pTime			(&pTime)
	, m_shotCd			(0)
{
	//�J�������W.
	m_Camera.vPosition	= D3DXVECTOR3( 0.0f, 2.0f, 0.0f );
	m_Camera.vLook		= D3DXVECTOR3( 0.0f, 2.0f, 10.0f );

	//���C�g���
	m_Light.vDirection = D3DXVECTOR3( 1.5f, 1.f, -1.f );	//���C�g����
}


//�f�X�g���N�^.
CGame::~CGame()
{
	//�U�R�����̔j��
	// rbegin()	:�������w���t�C�e���[�^���擾
	// rend()	:�擪���w���t�C�e���[�^���擾
	for (auto it = m_Zako.rbegin(); it != m_Zako.rend(); ++it)
	{
		SAFE_DELETE( *it );
	}


	//�U�R�̔j��
	SAFE_DELETE( m_pZako );

	//�e�̔j��
	if (m_pShot != nullptr) {
		for (int No = BULLET_MAX - 1; No >= 0; No--) {
			SAFE_DELETE(m_pShot[No]);
		}
	}

	//�n�ʂ̔j��.
	SAFE_DELETE( m_pGround );


	//�G�l�~�[�̔j��
#if 1
	if (m_ppEnemies != nullptr) {
		for (int No = m_EnemyMax - 1; No >= 0; No--) {
			SAFE_DELETE(m_ppEnemies[No]);
		}
		delete[] m_ppEnemies;
		m_ppEnemies = nullptr;
	}
#else
	for (int No = ENEMY_MAX - 1; No >= 0; No--) {
		SAFE_DELETE( m_pEnemies[No] );
	}
#endif
	SAFE_DELETE( m_pEnemy );

	//�v���C���[�̔j��.
	SAFE_DELETE( m_pPlayer );

	//�X�^�e�B�b�N���b�V���I�u�W�F�N�g�̔j��
	SAFE_DELETE( m_pStcMeshObj );

	//UI�I�u�W�F�N�g�̔j��
	SAFE_DELETE( m_pScyther );
	SAFE_DELETE( m_pParasect );
	SAFE_DELETE( m_pBeedrill );
	SAFE_DELETE( m_pPmon );

	//�����̔j��.
	SAFE_DELETE( m_pExplosion );

	//�X�L�����b�V���̔j��
	SAFE_DELETE( m_pSkinMeshZako );

	//�X�^�e�B�b�N���b�V���̔j��
	SAFE_DELETE( m_pStaticMeshBSphere );
	SAFE_DELETE( m_pSpriteBullet );
	SAFE_DELETE( m_pStaticMeshRoboB );
	SAFE_DELETE( m_pStaticMeshRoboA );
	SAFE_DELETE( m_pStaticMeshGround );
	SAFE_DELETE( m_pStaticMeshFighter );

	//�X�v���C�g2D�̔j��
	SAFE_DELETE( m_pSprite2DPmon );

	//�����X�v���C�g�̉��.
	SAFE_DELETE( m_pSpriteExplosion );
	//�v���C���[�X�v���C�g�̉��.
	SAFE_DELETE( m_pSpritePlayer );
	//�n�ʃX�v���C�g�̉��.
	SAFE_DELETE( m_pSpriteGround );

	//���C�\���N���X�̔j��
	for (int dir = CROSSRAY::max - 1; dir >= 0; dir--) {
		SAFE_DELETE( m_pCrossRay[dir] );
	}
	SAFE_DELETE( m_pRayY );

	//�f�o�b�O�e�L�X�g�̔j��
	SAFE_DELETE( m_pDbgText );

	//�O���ō쐬���Ă���̂ŁA�����ł͔j�����Ȃ�
	m_hWnd = nullptr;
	m_pDx11 = nullptr;
	m_pDx9 = nullptr;
}

//�\�z.
void CGame::Create()
{
	//�f�o�b�O�e�L�X�g�̃C���X�^���X�쐬
	m_pDbgText = new CDebugText();

	//���C�\���N���X�̃C���X�^�X�쐬
	m_pRayY = new CRay();
	for (int dir = 0; dir < CROSSRAY::max; dir++) {
		m_pCrossRay[dir] = new CRay();
	}

	//�X�v���C�g�̃C���X�^���X�쐬.
	m_pSpriteGround		= new CSprite3D();
	m_pSpritePlayer		= new CSprite3D();
	m_pSpriteExplosion	= new CSprite3D();
	m_pSpriteBullet		= new CSprite3D();

	//�X�v���C�g2D�̃C���X�^���X�쐬
	m_pSprite2DPmon	= new CSprite2D();

	//�X�^�e�B�b�N���b�V���̃C���X�^���X�쐬
	m_pStaticMeshFighter	= new CStaticMesh();
	m_pStaticMeshGround		= new CStaticMesh();
	m_pStaticMeshRoboA		= new CStaticMesh();
	m_pStaticMeshRoboB		= new CStaticMesh();
	m_pStaticMeshBSphere	= new CStaticMesh();

	//�X�L�����b�V���̃C���X�^���X�쐬
	m_pSkinMeshZako		= new CSkinMesh();

	//�X�v���C�g�I�u�W�F�N�g�N���X�̃C���X�^�X�쐬.
	m_pExplosion	= new CExplosion();

	//UI�I�u�W�F�N�g�̃C���X�^�X�쐬
	m_pPmon			= new CUIObject();
	m_pBeedrill		= new CUIObject();
	m_pParasect		= new CUIObject();
	m_pScyther		= new CUIObject();

	//�X�^�e�B�b�N���b�V���I�u�W�F�N�g�̃C���X�^���X�쐬
	m_pStcMeshObj	= new CStaticMeshObject();

	//�L�����N�^�[�N���X�̃C���X�^���X�쐬
	m_pPlayer		= new CPlayer();
	m_pEnemy		= new CEnemy();
#if 1
	//�G�l�~�[�𓮓I�Ɋm��
	m_ppEnemies = new CEnemy*[m_EnemyMax]();
	for (int No = 0; No < m_EnemyMax; No++) {
		m_ppEnemies[No] = new CEnemy();
	}
#else
	for (int No = 0; No < ENEMY_MAX; No++) {
		m_pEnemies[No] = new CEnemy();
	}
#endif

	//�n�ʃN���X�̃C���X�^���X�쐬
	m_pGround = new CGround();
	m_pGround->SetPlayer(*m_pPlayer);	//���C�̔���ŕK�v

	//�e�N���X�̃C���X�^���X�쐬
	for (int No = 0; No < BULLET_MAX; No++)
	{
		m_pShot[No] = new CShot();
		m_Shot.push(m_pShot[No]);
	}
	//�U�R�N���X�̃C���X�^�X�쐬
	m_pZako = new CZako();

	//�U�R����
	for (int i = 0; i < 3; i++) {
		//push_back(�l)	:�z��̖����֗v�f��ǉ�
		//size()		:�z��̗v�f�����擾
		m_Zako.push_back(new CZako());
	}

	//Effect�N���X
	CEffect::GetInstance()->Create(
		m_pDx11->GetDevice(),
		m_pDx11->GetContext());
}

//���[�h�f�[�^�֐�.
HRESULT CGame::LoadData()
{
	//�f�o�b�O�e�L�X�g�̓ǂݍ���
	if (FAILED(m_pDbgText->Init( *m_pDx11 ))){
		return E_FAIL;
	}

	//�T�E���h�f�[�^�̓ǂݍ���
	if (CSoundManager::GetInstance()->Load(m_hWnd) == false) {
		return E_FAIL;
	}

	//Effect�N���X
	if (FAILED(CEffect::GetInstance()->LoadData())) {
		return E_FAIL;
	}

	//���C�\���N���X�̏�����
	RAY ray = m_pPlayer->GetRayY();
	m_pRayY->Init(*m_pDx11, ray);
	for (int dir = 0; dir < CROSSRAY::max; dir++) {
		ray = m_pPlayer->GetCrossRay().Ray[dir];
		m_pCrossRay[dir]->Init(*m_pDx11, ray);
	}

	//�n�ʃX�v���C�g�̍\����
	CSprite3D::SPRITE_STATE SSGround;
	SSGround.Disp.w = 1.f;
	SSGround.Disp.h = 1.f;
	SSGround.Base.w = 256.f;
	SSGround.Base.h = 256.f;
	SSGround.Stride.w = 256.f;
	SSGround.Stride.h = 256.f;
	//�n�ʃX�v���C�g�̓ǂݍ���.
	m_pSpriteGround->Init( *m_pDx11,
		_T( "Data\\Texture\\Ground.png" ), SSGround);

	//�v���C���[�X�v���C�g�̍\����
	CSprite3D::SPRITE_STATE SSPlayer =
		{ 1.f, 1.f, 64.f, 64.f, 64.f, 64.f };
	//�v���C���[�X�v���C�g�̓ǂݍ���.
	m_pSpritePlayer->Init( *m_pDx11,
		_T( "Data\\Texture\\Player.png" ), SSPlayer);

	//�����X�v���C�g�̍\����
	CSprite3D::SPRITE_STATE SSExplosion =
		{ 1.f, 1.f, 256.f, 256.f, 32.f, 32.f };
		//�����X�v���C�g�̓ǂݍ���.
	m_pSpriteExplosion->Init( *m_pDx11,
		_T( "Data\\Texture\\explosion.png" ), SSExplosion);

	CSprite3D::SPRITE_STATE SSBullet =
	{ 1.f, 1.f, 64.f, 64.f, 64.f, 64.f };
	m_pSpriteBullet->Init(*m_pDx11,
		_T("Data\\Texture\\Bullet\\bullet.png"), SSBullet);



#if 1
	//P�����X�v���C�g�̍\����
	CSprite2D::SPRITE_STATE SSPmon =
	{ 64.f, 64.f, 896.f, 560.f, 896.f / 16.f, 560.f / 10.f };
#else
	//P�����X�v���C�g�̍\����
	CSprite2D::SPRITE_STATE SSPmon;
	SSPmon.Disp.w = 896.f;
	SSPmon.Disp.h = 560.f;
	SSPmon.Base = SSPmon.Stride = SSPmon.Disp;
#endif
	//P�����X�v���C�g�̓ǂݍ���
	m_pSprite2DPmon->Init( *m_pDx11,
		_T( "Data\\Texture\\pmon.png" ), SSPmon );

	//�X�^�e�B�b�N���b�V���̓ǂݍ���
	m_pStaticMeshFighter->Init( *m_pDx9, *m_pDx11,
		_T( "Data\\Mesh\\Static\\Fighter\\Fighter.x" ) );
	m_pStaticMeshGround->Init( *m_pDx9, *m_pDx11,
//		_T("Data\\Mesh\\Static\\Ground\\ground.x" ));
		_T("Data\\Mesh\\Static\\Stage\\stage.x"));
	m_pStaticMeshRoboA->Init(*m_pDx9, *m_pDx11,
		_T("Data\\Mesh\\Static\\Robo\\RobotA_pivot.x"));
	m_pStaticMeshRoboB->Init(*m_pDx9, *m_pDx11,
		_T("Data\\Mesh\\Static\\Robo\\RobotB_pivot.x"));

	//�o�E���f�B���O�X�t�B�A(�����蔻��p)
	m_pStaticMeshBSphere->Init(*m_pDx9, *m_pDx11,
		_T("Data\\Collision\\Sphere.x"));

	//�X�L�����b�V���̓ǂݍ���
	m_pSkinMeshZako->Init(*m_pDx9, *m_pDx11,
		_T("Data\\Mesh\\Skin\\zako\\zako.x"));

	//�����X�v���C�g��ݒ�.
	m_pExplosion->AttachSprite( *m_pSpriteExplosion );
	for (int No = 0; No < BULLET_MAX; No++) {
		m_pShot[No]->AttachSprite(*m_pSpriteBullet);

	}

	//P�����X�v���C�g��ݒ�
	m_pPmon->AttachSprite( *m_pSprite2DPmon );
	m_pBeedrill->AttachSprite( *m_pSprite2DPmon );
	m_pParasect->AttachSprite( *m_pSprite2DPmon );
	m_pScyther->AttachSprite( *m_pSprite2DPmon );

	//�X�^�e�B�b�N���b�V����ݒ�
	m_pStcMeshObj->AttachMesh( *m_pStaticMeshFighter );
	m_pPlayer->AttachMesh( *m_pStaticMeshFighter );
	m_pGround->AttachMesh( *m_pStaticMeshGround );
	m_pEnemy->AttachMesh( *m_pStaticMeshRoboB );

	//�X�L�����b�V����ݒ�
	m_pZako->AttachMesh( *m_pSkinMeshZako );
	m_pZako->SetScale( 0.002f );
	m_pZako->SetPosition( 0.f, 0.f, 8.f );

	//�U�R����
	//�͈�for��
	//auto:�����Ō^�𐄘_�A&������ƎQ�ƂɂȂ�s�v�ȃR�s�[��������
	for (auto& e : m_Zako)
	{
		int i = static_cast<int>(&e - &m_Zako[0]);	//���݂̃C���f�b�N�X�ԍ��Z�o

		e->SetPosition( -3.f + ( i * 3.f ), 0.f, 12.f);
		e->SetScale( 0.002f );
		e->AttachMesh( *m_pSkinMeshZako );
		e->SetAnimSpeed( 0.0001 + ( i * 0.0001 ) );
	}

	//P�������ꂼ��̉摜�p�^�[����ݒ�
	m_pBeedrill->SetPatternNo( 14, 0 );
	m_pParasect->SetPatternNo( 14, 2 );
	m_pScyther->SetPatternNo( 10, 7 );

	//P�������ꂼ��̈ʒu��ݒ�
	const float size = SSPmon.Disp.w * 0.5f;	// 64.f
	const float pos_y = static_cast<float>(WND_H) - SSPmon.Disp.h;
	m_pBeedrill->SetPosition( size * 0.f, pos_y, 0.f );
	m_pParasect->SetPosition( size * 1.f, pos_y, 0.f );
	m_pScyther->SetPosition(  size * 2.f, pos_y, 0.f );

	//�o�E���f�B���O�X�t�B�A�̍쐬
	m_pPlayer->CreateBSphereForMesh(*m_pStaticMeshBSphere);
	m_pEnemy->CreateBSphereForMesh(*m_pStaticMeshBSphere);
	//m_pShot->CreateBSphereForMesh(*m_pStaticMeshBullet);

	//�L�����N�^�[�̏������W��ݒ�
	m_pPlayer->SetPosition( 0.f, 1.f, 6.f );
	m_pEnemy->SetPosition( 0.f, 1.f, 16.f );

	//�G�l�~�[�����ݒ�
#if 1
	for (int No = 0; No < m_EnemyMax; No++) {
		auto& pE = m_ppEnemies[No];
		pE->AttachMesh(*m_pStaticMeshRoboA);
		pE->CreateBSphereForMesh(*m_pStaticMeshBSphere);
		pE->SetPosition(-3.f + (No * 3.f), 1.f, 10.f);
		pE->SetScale(2.0f);
	}
#else
	for (int No = 0; No < ENEMY_MAX; No++) {
		auto& pE = m_pEnemies[No];
		pE->AttachMesh(*m_pStaticMeshRoboA);
		pE->CreateBSphereForMesh(*m_pStaticMeshBSphere);
		pE->SetPosition(-3.f + (No * 3.f), 1.f, 10.f);
	}
#endif

	return S_OK;
}

//����֐�.
void CGame::Release()
{
}


//�X�V����.
void CGame::Update()
{
	//BGM�̃��[�v�Đ�
	//CSoundManager::PlayLoop(CSoundManager::BGM_Bonus);
	
	POINT mousePos;
	GetCursorPos(&mousePos);
	ScreenToClient(m_hWnd, &mousePos);

	// Calculate delta from center
	POINT center = { WND_W / 2, WND_H / 2 };
	m_mouseDelta.x = mousePos.x - center.x;
	m_mouseDelta.y = mousePos.y - center.y;

	// Reset cursor to center
	ClientToScreen(m_hWnd, &center);
	SetCursorPos(center.x, center.y);

	m_pPlayer->Update();
	m_pGround->Update();

#if 1
	//�e���΂�����!
	int bulletCount = m_pPlayer->GetShotNumber(); // existing convention: enum value + 1
	const float spreadDeg = 60.0f; // total spread in degrees (change to widen/narrow pattern)
	const float spreadRad = (bulletCount > 1) ? D3DXToRadian(spreadDeg) : 0.0f;
	const float startAngle = -spreadRad * 0.5f; // start relative to forward (-half spread)
	const float angleStep = (bulletCount > 1) ? (spreadRad / (bulletCount - 1)) : 0.0f;

	if (m_pPlayer->IsShot() == true) {
		
		float cadence = m_shotCd;					//�A�ˑ��x
		m_shotCd -= m_pTime->GetFixedDeltaTime();	//�A�ˑ��x������
		
		//�A�ˑ��x�𒴂�����e�𔭎�
		if (m_shotCd <= 0.0f)
		{
			for (int No = 0; No <= bulletCount; No++)
			{
				CShot* bullet = m_Shot.front();		//�L���[�̐擪���擾

				const float rotY = m_pPlayer->GetRotation().y + startAngle + angleStep * No;

				bullet->Reload(
					m_pPlayer->GetPosition(),
					rotY);
				m_shotCd = bullet->GetCadence();

				//�e���L���[�̍Ō�Ɉړ�
				m_Shot.pop();
				m_Shot.push(bullet);
			}
		}

	}

#else
	//�e���΂�����!
	//dynamic_cast�F�e�N���X�̃|�C���^���q�N���X�̃|�C���^�ɕϊ�����
	if (dynamic_cast<CPlayer*>(m_pPlayer)->IsShot() == true) {
		m_pShot->Reload(m_pPlayer->GetPosition());
	}
#endif
	for (int sNo = 0; sNo < BULLET_MAX; sNo++) {

		m_pShot[sNo]->Update();
	}

	for (int No = 0; No < m_EnemyMax; No++) {
		m_ppEnemies[No]->SetTargetPos(m_pPlayer->GetPosition());
		m_ppEnemies[No]->Update();
	}
	
	m_pEnemy->Update();

#if 1
#else
	for (int No = 0; No < ENEMY_MAX; No++) {
		m_pEnemies[No]->Update();
	}
#endif

	m_pExplosion->Update();

	//----------------------------
	// �X�L�����b�V��
	//----------------------------
	m_pZako->Update();

	//�U�R����
	for (auto& e : m_Zako) {
		e->Update();
	}

	//Effect����
	{
		//�G�t�F�N�g�̃C���X�^���X���ƂɕK�v�ȃn���h��
		//���R�\�����Đ��䂷��Ȃ�R�K�v�ɂȂ�
		static ::EsHandle hEffect = -1;

		if (GetAsyncKeyState('Y') & 0x0001) {
			hEffect = CEffect::Play(CEffect::Test0, D3DXVECTOR3(0.f, 1.f, 0.f));

			//�g��k��
			CEffect::SetScale(hEffect, D3DXVECTOR3(0.8f, 0.8f, 0.8f));

			//��](Y����])
			CEffect::SetRotation(hEffect, D3DXVECTOR3(0.f, D3DXToRadian(90.0f), 0.f));

			//�ʒu���Đݒ�
			CEffect::SetLocation(hEffect, D3DXVECTOR3(0.f, 1.f, 1.f));
		}
		if (GetAsyncKeyState('T') & 0x0001) {
			CEffect::Stop(hEffect);
		}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
	}


	TopDownCamera(
		&m_Camera,
		m_pPlayer->GetPosition(),
		m_pPlayer->GetRotation().y);

	CameraRotToMouse(
		&m_Camera,
		m_pPlayer->GetPosition(),
		m_mouseDelta,
		m_mouseSense);

	float rotX = m_Camera.yaw;
	m_pPlayer->SetRotation(0, rotX, 0);

//	//�O�l�̃J����
//	ThirdPersonCamera(
//		&m_Camera,
//		m_pPlayer->GetPosition(),
//		m_pPlayer->GetRotation().y);
}

//�`�揈��.
void CGame::Draw()
{
	Camera();
	Projection();

#if 1
	m_pGround->Draw( m_mView, m_mProj, m_Light, m_Camera );

	m_pPlayer->Draw( m_mView, m_mProj, m_Light, m_Camera );


	m_pEnemy->Draw( m_mView, m_mProj, m_Light, m_Camera );

#if 1
	for (int No = 0; No < m_EnemyMax; No++) {
		m_ppEnemies[No]->Draw( m_mView, m_mProj, m_Light, m_Camera );
	}
#else
	for (int No = 0; No < ENEMY_MAX; No++) {
		m_pEnemies[No]->Draw( m_mView, m_mProj, m_Light, m_Camera );
	}
#endif

	//----------------------------
	// �X�L�����b�V��
	//----------------------------
	m_pZako->Draw(m_mView, m_mProj, m_Light, m_Camera);
//	//�{�[�����W�ɍ��킹�ċ��̂�\��
//	m_pStaticMeshBSphere->SetPosition(m_ZakoBonePos);
//	m_pStaticMeshBSphere->Render(m_mView, m_mProj, m_Light, m_Camera.vPosition);

	//�U�R����
	for (auto& e : m_Zako) {
		e->Draw(m_mView, m_mProj, m_Light, m_Camera);
	}

	m_pPmon->Draw();

	//�[�x�e�X�g�����ɂ��邱�ƂŁA�������Ԃŕ`�悳���邱�Ƃ��ł���
	m_pDx11->SetDepth(false);
	m_pBeedrill->Draw();
	m_pParasect->Draw();
	m_pScyther->Draw();
	//��肽�����Ƃ��I������̂ŁA�[�x�e�X�g��L���ɂ��Ă���
	m_pDx11->SetDepth(true);

	//�����蔻��̒��S���W���X�V����
	m_pPlayer->UpdateBSpherePos();
	m_pEnemy->UpdateBSpherePos();
	//m_pShot->UpdateBSpherePos();

	//�v���C���[�ƃG�l�~�[�̓����蔻��
	if (m_pPlayer->GetBSphere()->IsHit(*m_pEnemy->GetBSphere()))
	{
		SetWindowText(m_hWnd, _T("�Փ˂��Ă��܂�"));
	}
	else
	{
		SetWindowText(m_hWnd, _T(""));
	}

	//�e�ƃG�l�~�[�̓����蔻��

	for (int No = 0; No < BULLET_MAX; No++)
	{
		if (m_pShot[No]->IsHit(m_pEnemy, 0.1))
		{
			//����
			m_pExplosion->SetPosition(m_pEnemy->GetPosition());	//�G�l�~�[�̈ʒu�ɂ��낦��
			dynamic_cast<CExplosion*>(m_pExplosion)->ResetAnimation();//�A�j���[�V�������Z�b�g
			//�e
			m_pShot[No]->SetDisplay(false);
			m_pShot[No]->SetPosition(0.f, -10.f, 0.f);	//�n�ʂɖ��߂�
			//�G�l�~�[
			m_pEnemy->SetPosition(0.f, 1.f, 20.f);	//���֍Ĕz�u
		}

		for (int eNo = 0; eNo < m_EnemyMax; eNo++)
		{
			if (m_pShot[No]->IsHit(m_ppEnemies[eNo], 0.1))
			{
				m_pExplosion->SetPosition(m_ppEnemies[eNo]->GetPosition());	//�G�l�~�[�̈ʒu�ɂ��낦��
				dynamic_cast<CExplosion*>(m_pExplosion)->ResetAnimation();//�A�j���[�V�������Z�b�g
				m_pShot[No]->SetDisplay(false);
				m_pShot[No]->SetPosition(0.f, -10.f, 0.f);	//�n�ʂɖ��߂�
				m_ppEnemies[eNo]->SetDamagedValue(10.f);
			}
		}


	}

	m_pExplosion->Draw(m_mView, m_mProj);
	for (int No = 0; No < BULLET_MAX; No++) {
		m_pShot[No]->Draw(m_mView, m_mProj);
	}

#else
	//�Q�[���I�u�W�F�N�g�̃|�C���^�̃��X�g���쐬
	CGameObject* pObjList[] =
	{
		m_pGround,
		m_pPlayer,
		m_pExplosion,
		m_pPmon,
		m_pBeedrill,
		m_pParasect,
		m_pScyther,
	};
	//�z��̍ő�v�f�����Z�o
	const int ObjListMax = sizeof(pObjList) / sizeof(pObjList[0]);
	//�쐬�������X�g�Ń��[�v��������
	for (int i = 0; i < ObjListMax; i++) {
		pObjList[i]->Draw( m_mView, m_mProj, m_Light, m_Camera );
	}

#endif

	//Effect�N���X
	CEffect::GetInstance()->Draw(m_mView, m_mProj, m_Light, m_Camera);

	//���C�̕`��
	m_pRayY->Render(m_mView, m_mProj, m_pPlayer->GetRayY());
	for (int dir = 0; dir < CROSSRAY::max; dir++) {
		m_pCrossRay[dir]->Render(
			m_mView, m_mProj, m_pPlayer->GetCrossRay().Ray[dir]);
	}

	//�f�o�b�O�e�L�X�g�̕`��
	m_pDbgText->SetColor(0.9f, 0.6f, 0.f);	//�F�̐ݒ�
	m_pDbgText->Render(_T("ABCD"), 10, 100);

	//�f�o�b�O�e�L�X�g(���l����)�̕`��
	m_pDbgText->SetColor(1.f, 0.f, 0.f);
	TCHAR dbgText[64];
	_stprintf_s(dbgText, _T("Float:%f, %f"), 1.f, 2.2f );
	m_pDbgText->Render( dbgText, 10, 110 );

	m_pDbgText->SetColor(1.f, 1.f, 1.f);
	_stprintf_s(dbgText, _T("DELTA:%d, %d"), m_mouseDelta.x, m_mouseDelta.y);
	m_pDbgText->Render(dbgText, 10, 150);
}

//�J�����֐�.
//���J������p�̃N���X���쐬������֗�����.
void CGame::Camera()
{
	D3DXVECTOR3 cam_pos	= m_Camera.vPosition;
	D3DXVECTOR3 cam_look = m_Camera.vLook;
	D3DXVECTOR3	vUpVec( 0.0f, 1.0f, 0.0f );	//����i�x�N�g���j.

	//�r���[�i�J�����j�ϊ�.
	D3DXMatrixLookAtLH(
		&m_mView,	//(out)�r���[�v�Z����.
		&cam_pos, &cam_look, &vUpVec );
}

//�v���W�F�N�V�����֐�.
void CGame::Projection()
{
	//y�����̎���p�B���l��傫�������王�삪�����Ȃ�.
	float fov_y	 = static_cast<FLOAT>( D3DXToRadian( 45.0 ) );	//���W�A���l.
	//�A�X�y�N�g�i���������j.
	float aspect = static_cast<FLOAT>( WND_W ) / static_cast<FLOAT>( WND_H );
	float near_z = 0.1f;
	float far_z	 = 100.0f;

	//�v���W�F�N�V�����i�ˉe�j�ϊ�.
	D3DXMatrixPerspectiveFovLH(
		&m_mProj,	//(out)�v���W�F�N�V�����v�Z����.
		fov_y,		//����p�iFOV�FField of View�j.
		aspect,		//�A�X�y�N�g.
		near_z,		//�߂��r���[���ʂ�z�l.
		far_z );	//�����r���[���ʂ�z�l.
}

//�O�l�̃J����
void CGame::ThirdPersonCamera(
	CAMERA* pCamera, const D3DXVECTOR3& TargetPos, float TargetRotY)
{
	//Z���x�N�g���iZ+�����̒P�ʃx�N�g���j
	D3DXVECTOR3 vecAxisZ(0.f, 0.f, 1.f);

	//Y�����̉�]�s��
	D3DXMATRIX mRotationY;
	//Y����]�s����쐬
	D3DXMatrixRotationY(
		&mRotationY,	//(out)�s��
		TargetRotY);	//�Ώۂ�Y�����̉�]�l

	//Y����]�s����g����Z���x�N�g�������W�ϊ�����
	D3DXVec3TransformCoord(
		&vecAxisZ,		//(out)Z���x�N�g��
		&vecAxisZ,		//(in)Z���x�N�g��
		&mRotationY);	//Y����]�s��

	//�J�����̈ʒu�A�����ʒu��Ώۂɂ��낦��
	pCamera->vPosition	= TargetPos;
	pCamera->vLook		= TargetPos;

	//�J�����̈ʒu�A�����ʒu��Z���x�N�g����p���Ē���
	pCamera->vPosition	-= vecAxisZ * 4.f;	//�Ώۂ̔w����
	pCamera->vLook		+= vecAxisZ * 3.f;	//�Ώۂ�����Ō�������

	//�J�����̈ʒu�A�����ʒu�̍��������ꂼ�������
	pCamera->vPosition.y+= 2.f;
	pCamera->vLook.y	+= 0.5f;
}


void CGame::TopDownCamera(
	CAMERA* pCamera, const D3DXVECTOR3& TargetPos, float TargetRotY)
{
	//Z���x�N�g���iZ+�����̒P�ʃx�N�g���j
	D3DXVECTOR3 vecAxisY(0.f, 1.f, 0.f);
	D3DXVECTOR3 vecAxisZ(0.f, 0.f, 1.f);

	//Y�����̉�]�s��
	D3DXMATRIX mRotationY;
	//Y����]�s����쐬
	D3DXMatrixRotationY(
		&mRotationY,	//(out)�s��
		TargetRotY);	//�Ώۂ�Y�����̉�]�l

	//Y����]�s����g����Z���x�N�g�������W�ϊ�����
	D3DXVec3TransformCoord(
		&vecAxisZ,		//(out)Z���x�N�g��
		&vecAxisZ,		//(in)Z���x�N�g��
		&mRotationY);	//Y����]�s��

	pCamera->vPosition	= TargetPos;
	pCamera->vLook		= TargetPos;

	pCamera->vPosition	+= vecAxisY * 25.f ;
	pCamera->vLook		+= vecAxisZ * 8.f - vecAxisY * 1.f;

}

void CGame::CameraRotToMouse(CAMERA* pCamera, const D3DXVECTOR3& TargetPos, POINT delta, float sense)
{

	pCamera->yaw += (float)delta.x * sense;

	D3DXVECTOR3 lookDirection;
	lookDirection.x = cosf(pCamera->pitch) * sinf(pCamera->yaw);
	lookDirection.y = sinf(pCamera->pitch);
	lookDirection.z = cosf(pCamera->pitch) * cosf(pCamera->yaw);

	D3DXVECTOR3 position = D3DXVECTOR3(TargetPos.x, 0, TargetPos.z);

	pCamera->vLook =  position + lookDirection;

}
