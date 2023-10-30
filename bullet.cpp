
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "enemy.h"
#include "bullet.h"
#include "explosion.h"
#include "score.h"
#include "audio.h"
#include "movecomponent.h"

Model* Bullet::m_BulletModel{};

void Bullet::Load()
{
	m_BulletModel = new Model();
	m_BulletModel->Load("asset\\model\\bullet.obj");
}

void Bullet::Unload()
{
	m_BulletModel->Unload();
	delete m_BulletModel;
}


void Bullet::Init()
{
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f) * 0.5f;
	m_Velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\vertexLightingPS.cso");

	m_MoveComp = AddComponent<MoveComponent>(this);
	m_MoveComp->SetForwardSpeed(0.7f);
	m_DestoroyFrame = 0.0f;

	m_ExplosionSE = AddComponent<Audio>(this);
	m_ExplosionSE->Load("asset\\audio\\explosion.wav");
}


void Bullet::Uninit()
{
	//�����ɃV�F�[�_�[�I�u�W�F�N�g�̉����ǉ�
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

void Bullet::Update()
{
	//�Ȃ���ball�ɕt����moveConponent��update���悾��Ȃ��̂ł����ɂ���
	m_MoveComp->Update();
	//6000�t���[���Ńf�X�g���C
	if (m_DestoroyFrame > 700.0f)
	{
		//�e�������\�������
		SetDestroy();
		return;
	}
	m_DestoroyFrame++;
	Scene* scene = Manager::GetScene();
	std::vector<Enemy*> enemies = scene->GetGameObjects<Enemy>();   //std::vector<Enemy*>��auto�ł���

	//�G�Ƃ̏Փ˔���
	for (Enemy* enemy : enemies)
	{
		D3DXVECTOR3 enemyPos = enemy->GetPosition();
		D3DXVECTOR3 direction = enemyPos - m_Position;
		float length = D3DXVec3Length(&direction);

		if (length < 2.0f)
		{
			scene->AddGameObject<Explosion>(1)->SetPosition(enemyPos);

			SetDestroy();
			enemy->SetDestroy();
			m_ExplosionSE->Play();

			Score* score = scene->GetGameObject<Score>();
			score->AddCount(1);
			return;
		}
	}
}

void Bullet::Draw()
{

	//�����ɃV�F�[�_�[�֘A�̕`�揀����ǉ� ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//�g�p����V�F�[�_��ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// �}�g���N�X�ݒ�
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);


	m_BulletModel->Draw();
}


