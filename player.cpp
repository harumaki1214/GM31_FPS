
#include "main.h"
#include "renderer.h"
#include "manager.h"

#include "scene.h"
#include "player.h"
#include "input.h"
#include "bullet.h"
#include "cylinder.h"
#include "box.h"
#include "audio.h"
#include "moveComponent.h"


void Player::Init()
{
	//m_Model = new Model();
	//m_Model->Load("asset\\model\\player.obj");

	m_Position = D3DXVECTOR3(0.0f, 1.0f, -10.0f);
	m_Quaternion = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
	m_Scale = D3DXVECTOR3(30.0f, 30.0f, 30.0f);
	m_BulletCount = 0;

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\vertexLightingPS.cso");

	m_ShotSE = AddComponent<Audio>(this);
	m_ShotSE->Load("asset\\audio\\shoot.wav");

	m_MoveComp = AddComponent<MoveComponent>(this);
}


void Player::Uninit()
{
	GameObject::Uninit();

	if (m_Model != NULL)
	{
		m_Model->Unload();
		delete m_Model;
	}

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


void Player::Update()
{
	GameObject::Update();

	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();
	m_OldPosition = m_Position;

	//マウスの状態を取得
	Mouse_GetState(&m_MouseState);

	const int maxMouseSpeed = 2100;
	const float maxAngularSpeed = D3DX_PI * 10;
	float angularSpeed = 0.0f;

	float forwardSpeed = 0.0f;
	float strafeSpeed = 0.0f;

	int x = m_MouseState.x;
	int y = m_MouseState.y;

	if (x != 0)
	{
		angularSpeed = static_cast<float>(x) / maxMouseSpeed;
		angularSpeed *= maxAngularSpeed;
	}
	m_MoveComp->SetAngularSpeed(angularSpeed);

	const float maxPitchSpeed = D3DX_PI * 10;
	float pitchSpeed = 0.0f;

	if (y != 0)
	{
		pitchSpeed = static_cast<float>(y) / maxMouseSpeed;
		pitchSpeed *= maxPitchSpeed;
	}
	camera->SetPitchSpeed(pitchSpeed);

	//前後左右移動
	{
		if (Input::GetKeyPress('A'))
			strafeSpeed -= 0.2f;

		if (Input::GetKeyPress('D'))
			strafeSpeed += 0.2f;

		if (Input::GetKeyPress('W'))
			forwardSpeed += 0.2f;

		if (Input::GetKeyPress('S'))
			forwardSpeed -= 0.2f;
	}

	m_MoveComp->SetForwardSpeed(forwardSpeed);
	m_MoveComp->SetStrafeSpeed(strafeSpeed);

	//ジャンプ
	/*if (Input::GetKeyTrigger(VK_SPACE))
	{
		
	}*/

	//m_Velocity.y -= 0.010f;


	if (m_BulletCount <= 0 && m_MouseState.leftButton)
	{
		Shoot(camera, scene);
		m_BulletCount = 1;
		m_ShotSE->Play();
	}

	if (m_MouseState.leftButton == false && m_OldMouseState.leftButton == true)
		m_BulletCount = 0;

	m_OldMouseState.leftButton = m_MouseState.leftButton;

	//障害物との当たり判定
	float groundHeight = 0.0f;

	std::vector<Cylinder*> cylinders = scene->GetGameObjects<Cylinder>();   //std::vector<Enemy*>はautoでも可

	for (Cylinder* cylinder : cylinders)
	{
		D3DXVECTOR3 position = cylinder->GetPosition();
		D3DXVECTOR3 scale = cylinder->GetScale();

		D3DXVECTOR3 direction = m_Position - position;
		direction.y = 0.0f;
		float length = D3DXVec3Length(&direction);

		if (length < scale.x)
		{
			if (m_Position.y < position.y + scale.y - 0.5f)
			{
				m_Position.x = m_OldPosition.x;
				m_Position.z = m_OldPosition.z;
			}
			else
			{
				groundHeight = position.y + scale.y;
			}
			break;
		}
	}

	std::vector<Box*> boxes = scene->GetGameObjects<Box>();   //std::vector<Enemy*>はautoでも可

	for (Box* box : boxes)
	{
		D3DXVECTOR3 position = box->GetPosition();
		D3DXVECTOR3 scale = box->GetScale();

		if (position.x - scale.x - 0.5f < m_Position.x &&
			m_Position.x < position.x + scale.x + 0.5f &&
			position.z - scale.z - 0.5f < m_Position.z &&
			m_Position.z < position.z + scale.z + 0.5f)
		{
			if (m_Position.y < position.y + scale.y * 2.0f - 0.5f)
			{
				m_Position.x = m_OldPosition.x;
				m_Position.z = m_OldPosition.z;
			}
			else
			{
				groundHeight = position.y + scale.y * 2.0f;
			}
			break;
		}
	}

	//	
	//D3DXVECTOR3 shadowPosition = m_Position;
	///shadowPosition.y = groundHeight + 0.01f;
	//m_Shadow->SetPosition(shadowPosition);

	//接地
	if (m_Position.y < groundHeight && m_Velocity.y < 0.0f)
	{
		m_Position.y = groundHeight;
		m_Velocity.y = 0.0f;
	}
}


void Player::Draw()
{
	GameObject::Draw();

	//ここにシェーダー関連の描画準備を追加 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//使用するシェーダを設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	D3DXMATRIX scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y + D3DX_PI, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	mtxWorld = scale * rot * trans;

	Renderer::SetWorldMatrix(&mtxWorld);

	//m_Model->Draw();
}


void Player::Shoot(Camera* camera, Scene* scene)
{
	/*D3DXVECTOR3 screenPoint(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 start = camera->Unproject(screenPoint);
	
	screenPoint.z = 0.9f;
	D3DXVECTOR3 end = camera->Unproject(screenPoint);

	D3DXVECTOR3 dir = end - start;
	D3DXVec3Normalize(&dir, &dir);*/

	Bullet* bullet = scene->AddGameObject<Bullet>(1);
	D3DXVECTOR3 pos = m_Position;
	pos.y += 1.0f;
	bullet->SetPosition(pos);
	bullet->SetQuaternion(this->m_Quaternion);
}