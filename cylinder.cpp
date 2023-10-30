
#include "main.h"
#include "renderer.h"
#include "cylinder.h"


void Cylinder::Init()
{
	m_CylinderModel = new Model();
	m_CylinderModel->Load("asset\\model\\cylinder\\cylinder.obj");

	/*m_Position = D3DXVECTOR3(0.0f, 1.0f, 2.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);*/

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\vertexLightingPS.cso");
}

void Cylinder::Uninit()
{

	m_CylinderModel->Unload();
	delete m_CylinderModel;


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

void Cylinder::Update()
{
}

void Cylinder::Draw()
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

	m_CylinderModel->Draw();
}


