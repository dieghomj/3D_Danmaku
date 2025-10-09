#include "CSpriteObject.h"

CSpriteObject::CSpriteObject()
	: m_pSprite		( nullptr )
{
}

CSpriteObject::~CSpriteObject()
{
	DetachSprite();
}

void CSpriteObject::Update()
{
	if( m_pSprite == nullptr ){
		return;
	}
}

bool CSpriteObject::IsHit(CGameObject* obj, float rad)
{
	POINTS pattern = m_pSprite->GetPatternMax();
	float selfRadius = min(pattern.x, pattern.y);

	D3DXVECTOR3 vObjPos = obj->GetPosition();
	D3DXVECTOR3 vDist;
	
	D3DXVec3Subtract(&vDist,&m_vPosition, &vObjPos);

	float dist = D3DXVec3LengthSq(&vDist);

	if (selfRadius + rad >= dist)
		return true;
	else
		return false;

}

void CSpriteObject::Draw( D3DXMATRIX& View, D3DXMATRIX& Proj )
{
	if( m_pSprite == nullptr ){
		return;
	}

	//�`�撼�O�ō��W���]���Ȃǂ��X�V.
	m_pSprite->SetPosition( m_vPosition );
	m_pSprite->SetRotation( m_vRotation );
	m_pSprite->SetScale( m_vScale );

	//�����_�����O.
	m_pSprite->Render( View, Proj );
}

void CSpriteObject::Draw(
	D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	Draw( View, Proj );
}
