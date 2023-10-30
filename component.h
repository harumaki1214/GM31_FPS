#pragma once

class Component
{
protected:
	//�R���|�[�l���g�̏��L�҂�ݒ�
	class GameObject* m_Owner{};

public:
	virtual void Init(class GameObject* owner) { m_Owner = owner; }  //���z�֐�
	void Init() {}
	virtual void Uninit() {}
	virtual void Update() {}
	virtual void Draw() {}

};