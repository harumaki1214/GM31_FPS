#pragma once
#include"component.h"
#include"main.h"

class MoveComponent : public Component
{
protected:
	float m_AngularSpeed;
	float m_ForwardSpeed;
	float m_StrafeSpeed;
	float m_UpSpeed;

public:
	void Init(class GameObject* owner) override;
	//void Uninit() override;
	void Update() override;

	float GetAngularSpeed() const { return m_AngularSpeed; }
	float GetForwardSpeed() const { return m_ForwardSpeed; }
	float GetStrafeSpeed() const { return m_StrafeSpeed; }
	void SetAngularSpeed(float speed) { m_AngularSpeed = speed; }
	void SetForwardSpeed(float speed) { m_ForwardSpeed = speed; }
	void SetStrafeSpeed(float speed) { m_StrafeSpeed = speed; }

	D3DXQUATERNION Concatenate(const D3DXQUATERNION& q, const D3DXQUATERNION& p);
};