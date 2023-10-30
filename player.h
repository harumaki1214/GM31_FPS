#pragma once

#include"model.h"
#include"gameObject.h"
#include "mouse.h"


class Player : public GameObject
{
private:
	Model* m_Model{};
	class Audio* m_ShotSE{};
	class Shadow* m_Shadow{};
	class MoveComponent* m_MoveComp{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	D3DXVECTOR3 m_Velocity{};
	Mouse_State m_MouseState;
	Mouse_State m_OldMouseState;

	int m_BulletCount;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void Shoot(class Camera*, class Scene*);
};
