
#include "main.h"
#include "renderer.h"
#include "enemy.h"


void Enemy::Init()
{
	m_EnemyModel = new Model();
	m_EnemyModel->Load("asset\\model\\torus.obj");

	/*m_Position = D3DXVECTOR3(0.0f, 1.0f, 2.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);*/
	m_Velocity = D3DXVECTOR3(Frand() - 0.5f, 0.0f, Frand() - 0.5f);
	m_Speed = 0.2f;

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\vertexLightingPS.cso");
}

void Enemy::Uninit()
{

	m_EnemyModel->Unload();
	delete m_EnemyModel;

	//ここにシェーダーオブジェクトの解放を追加
	if (m_VertexLayout != NULL)
	{
		m_VertexLayout->Release();
		m_VertexLayout = NULL;
	}

	if (m_VertexShader != NULL)
	{
		m_VertexShader->Release();
		m_VertexShader = NULL;
	}

	if (m_PixelShader != NULL)
	{
		m_PixelShader->Release();
		m_PixelShader = NULL;
	}
}

void Enemy::Update()
{

	m_Position += m_Velocity * m_Speed;

	//回転の更新
	m_Rotation.x += 0.05f;
	m_Rotation.z += 0.05f;

	if (m_Position.x > 50.0f)
		m_Velocity.x *= -1;

	if (m_Position.x < -50.0f)
		m_Velocity.x *= -1;

	if (m_Position.z > 50.0f)
		m_Velocity.z *= -1;

	if (m_Position.z < -50.0f)
		m_Velocity.z *= -1;
}

void Enemy::Draw()
{

	//ここにシェーダー関連の描画準備を追加 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//使用するシェーダを設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);

	m_EnemyModel->Draw();
}

float Enemy::Frand(void)
{
	return (float)rand() / RAND_MAX;
}
