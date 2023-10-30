#include "moveComponent.h"
#include "player.h"
#include "scene.h"
#include "manager.h"

void MoveComponent::Init(GameObject* owner)
{
	m_Owner = owner;
	m_AngularSpeed = 0.0f;
	m_ForwardSpeed = 0.0f;
	m_UpSpeed = 0.0f;
}

void MoveComponent::Update()
{
	D3DXVECTOR3 unitY = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXQUATERNION q = m_Owner->GetQuaternion();
	float angle = m_AngularSpeed;
	D3DXQUATERNION inc;

	D3DXQuaternionRotationAxis(&inc, &unitY, angle);
	
	q = Concatenate(q, inc);
	m_Owner->SetQuaternion(q);

	D3DXVECTOR3 pos = m_Owner->GetPosition();
	pos += m_Owner->GetForward() * m_ForwardSpeed;
	pos += m_Owner->GetRight() * m_StrafeSpeed;
	//pos += m_Owner->GetUp() * m_UpSpeed;
	m_Owner->SetPosition(pos);
}


//åãçá
D3DXQUATERNION MoveComponent::Concatenate(const D3DXQUATERNION& q, const D3DXQUATERNION& p)
{
	D3DXQUATERNION retVal;
	D3DXVECTOR3 tmp1;

	D3DXVECTOR3 qv(q.x, q.y, q.z);
	D3DXVECTOR3 pv(p.x, p.y, p.z);
	D3DXVec3Cross(&tmp1, &pv, &qv);

	D3DXVECTOR3 newVec = p.w * qv + q.w * pv + tmp1;

	retVal.x = newVec.x;
	retVal.y = newVec.y;
	retVal.z = newVec.z;

	D3DXVec3Dot(&pv, &qv);
	retVal.w = p.w * q.w - D3DXVec3Dot(&pv, &qv);

	return retVal;
}