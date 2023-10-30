
#include "main.h"
#include "renderer.h"
#include "polygon2D.h"
#include "sprite.h"

void Polygon2D::Init()
{
	//�����ɃV�F�[�_�[�t�@�C���̃��[�h��ǉ�
	//CRenderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "unlitColorVS.cso");
	//CRenderer::CreatePixelShader(&m_PixelShader, "unlitColorPS.cso");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	//Sprite* sprite = AddComponent<Sprite>();
	AddComponent<Sprite>()->Init(50.0f, 50.0f, 100.0f, 50.0f, "asset\\texture\\boss_position.PNG");
	//AddComponent<Sprite>()->Init(100.0f, 300.0f, 300.0f, 200.0f, "asset\\texture\\boss_position.PNG");
}


void Polygon2D::Uninit()
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

	GameObject::Uninit();
}


void Polygon2D::Update()
{
	GameObject::Update();
}


void Polygon2D::Draw()
{
	//�����ɃV�F�[�_�[�֘A�̕`�揀����ǉ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//�g�p����V�F�[�_��ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// �}�g���N�X�ݒ�
	Renderer::SetWorldViewProjection2D();

	//���N���X�̃��\�b�h�Ăяo��
	GameObject::Draw();
}