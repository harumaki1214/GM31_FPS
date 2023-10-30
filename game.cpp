
#include "game.h"

#include"Field.h"
#include"manager.h"
#include"Player.h"
#include"Enemy.h"
#include"scene.h"
#include"cylinder.h"
#include"Box.h"
#include"Bullet.h"
#include"Polygon2D.h"
#include"score.h"
#include"audio.h"
#include "result.h"
#include "sky.h"
#include "fade.h"

void Game::Init()
{
	//相対座標モードに変更
	Mouse_SetMode(MOUSE_POSITION_MODE_RELATIVE);
	Bullet::Load();

	AddGameObject<Camera>(0);
	AddGameObject<Sky>(1);
	AddGameObject<Field>(1);
	AddGameObject<Player>(1)->SetPosition(D3DXVECTOR3(0.0f, 2.0f, -10.0f));

	AddGameObject<Enemy>(1)->SetPosition(D3DXVECTOR3(5.0f, 2.0f, 1.0f));
	AddGameObject<Enemy>(1)->SetPosition(D3DXVECTOR3(0.0f, 2.0f, 1.0f));
	AddGameObject<Enemy>(1)->SetPosition(D3DXVECTOR3(-5.0f, 2.0f, 1.0f));
	AddGameObject<Enemy>(1)->SetPosition(D3DXVECTOR3(0.0f, 2.0f, 10.0f));
	AddGameObject<Enemy>(1)->SetPosition(D3DXVECTOR3(10.0f, 2.0f, 10.0f));
	AddGameObject<Enemy>(1)->SetPosition(D3DXVECTOR3(4.0f, 2.0f, 10.0f));
	AddGameObject<Enemy>(1)->SetPosition(D3DXVECTOR3(-7.0f, 2.0f, -10.0f));
	AddGameObject<Enemy>(1)->SetPosition(D3DXVECTOR3(-20.0f, 2.0f, 10.0f));
	AddGameObject<Enemy>(1)->SetPosition(D3DXVECTOR3(-15.0f, 2.0f, -12.0f));

	
	/*Cylinder* cylinder = AddGameObject<Cylinder>(1);
	cylinder->SetPosition(D3DXVECTOR3(5.0f, 0.0f, 5.0f));
	cylinder->SetScale(D3DXVECTOR3(3.0f, 5.0f, 3.0f));*/

	/*Box* box = AddGameObject<Box>(1);
	box->SetPosition(D3DXVECTOR3(-5.0f, 0.0f, -5.0f));
	box->SetScale(D3DXVECTOR3(3.0f, 2.0f, 3.0f));*/

	AddGameObject<Score>(2);  //登録の順番に注意
	m_Fade = AddGameObject<Fade>(2);

	Audio* bgm;
	bgm = AddGameObject<GameObject>(0)->AddComponent<Audio>();
	bgm->Load("asset\\audio\\bgm.wav");
	bgm->Play(true);
}

void Game::Uninit()
{
	Scene::Uninit();
	Bullet::Unload();
}

void Game::Update()
{
	Scene::Update();


	Score* score = GetGameObject<Score>();
	if (score->GetCount() >= 9)
	{
		m_Fade->FadeOut();
	}
	if (m_Fade->GetFadeFinish())
	{
		Manager::SetScene<Result>();
	}
	//画面遷移コードをかく
}
