
#include "main.h"
#include "renderer.h"
#include "fade.h"
#include "sprite.h"

void Fade::Init()
{
	//�����ɃV�F�[�_�[�t�@�C���̃��[�h��ǉ�
	//CRenderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "unlitColorVS.cso");
	//CRenderer::CreatePixelShader(&m_PixelShader, "unlitColorPS.cso");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	m_Sprite = AddComponent<Sprite>(this);
	m_Sprite->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\black.png");
	//AddComponent<Sprite>()->Init(100.0f, 300.0f, 300.0f, 200.0f, "asset\\texture\\boss_position.PNG");
}


void Fade::Uninit()
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


void Fade::Update()
{
	GameObject::Update();
	if (!m_FadeOut)
	{
		m_Alpha -= 0.05f;
		if (m_Alpha < 0.0f) m_Alpha = 0.0f;
	}
	else
	{
		m_Alpha += 0.05f;
		if (m_Alpha > 1.0f)
		{
			m_Alpha = 1.0f;
			m_FadeFinish = true;
		}
	}

	m_Sprite->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_Alpha));
}


void Fade::Draw()
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