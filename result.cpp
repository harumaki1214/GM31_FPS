#include "main.h"
#include "renderer.h"
#include "manager.h"
#include"input.h"
#include "result.h"
#include "resultLogo.h"
#include "title.h"
#include "Polygon2D.h"
#include "fade.h"

void Result::Init()
{
	AddGameObject<ResultLogo>(2);
	m_Fade = AddGameObject<Fade>(2);
	//相対座標モードに変更
	Mouse_SetMode(MOUSE_POSITION_MODE_ABSOLUTE);
}

void Result::Update()
{
	Scene::Update();

	//マウスの状態を取得
	Mouse_GetState(&m_mouseState);

	if (Input::GetKeyTrigger(VK_SPACE))
	{
		m_Fade->FadeOut();
		//Manager::SetScene<Game>();
	}

	if (m_Fade->GetFadeFinish())
	{
		Manager::SetScene<Title>();
	}
}
