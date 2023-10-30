#pragma once
#include"scene.h"
#include"mouse.h"


class Result : public Scene
{
private:
	class Fade* m_Fade{};
	Mouse_State m_mouseState;

public:
	void Init() override;
	void Update() override;
};