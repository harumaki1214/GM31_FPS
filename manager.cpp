#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "scene.h"
#include "title.h"
#include "audio.h"

Scene* Manager::m_Scene{};   //�ÓI�����o�ϐ��͍Đ錾���K�v
Scene* Manager::m_NextScene{};   //�ÓI�����o�ϐ��͍Đ錾���K�v

void Manager::Init()
{
	Renderer::Init();
	Input::Init();
	
	Audio::InitMaster();

	//m_Scene = new Title();
	//m_Scene->Init();
	SetScene<Title>();
}

void Manager::Uninit()
{
	m_Scene->Uninit();
	delete m_Scene;

	Audio::UninitMaster();
	Input::Uninit();
	Renderer::Uninit();
	
}

void Manager::Update()
{
	Input::Update();

	if(m_NextScene)
	{
		if (m_Scene)
		{
			m_Scene->Uninit();
			delete m_Scene;
		}
		m_Scene = m_NextScene;
		m_Scene->Init();

		m_NextScene = nullptr;
	}
	
	m_Scene->Update();
}

void Manager::Draw()
{
	Renderer::Begin();

	m_Scene->Draw();

	Renderer::End();
}
