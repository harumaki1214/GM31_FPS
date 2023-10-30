#pragma once
#include"gameObject.h"

class Camera : public GameObject
{

private:
	D3DXVECTOR3		    m_Target;
	D3DXMATRIX			m_ViewMatrix{};
	D3DXMATRIX          m_ProjectionMatrix{};

	float m_PitchSpeed;
	float m_MaxPitch;
	float m_Pitch;

public:
	void Init();
	void Update();
	void Draw(); 

	D3DXMATRIX GetViewMatrix() const { return m_ViewMatrix; }
	void SetViewMatrix(D3DXMATRIX matrix)  
	{ 
		D3DXMatrixInverse(&matrix, nullptr, &matrix);
		m_ViewMatrix = matrix; 
	}
	D3DXMATRIX GetProjectionMatrix() const { return m_ProjectionMatrix; }
	void SetProjectionMatrix(D3DXMATRIX matrix) 
	{ 
		D3DXMatrixInverse(&m_ProjectionMatrix, nullptr, &m_ProjectionMatrix);
		m_ProjectionMatrix = matrix; 
	}

	float GetPitch() const { return m_Pitch; }
	float GetPitchSpeed() const { return m_PitchSpeed; }
	float GetMaxPitch() const { return m_MaxPitch; }

	void SetPitchSpeed(float speed) { m_PitchSpeed = speed; }
	void SetMaxPitch(float pitch) { m_MaxPitch = pitch; }

	D3DXVECTOR3 Unproject(const D3DXVECTOR3& screenPoint);


};