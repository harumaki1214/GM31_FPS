#pragma once
#include"gameObject.h"
#include"model.h"

class Enemy : public GameObject
{
private:
	Model* m_EnemyModel{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};
	float m_Speed;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	float Frand(void);

	//void SetPosition();
};