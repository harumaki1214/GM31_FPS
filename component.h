#pragma once

class Component
{
protected:
	//コンポーネントの所有者を設定
	class GameObject* m_Owner{};

public:
	virtual void Init(class GameObject* owner) { m_Owner = owner; }  //仮想関数
	void Init() {}
	virtual void Uninit() {}
	virtual void Update() {}
	virtual void Draw() {}

};