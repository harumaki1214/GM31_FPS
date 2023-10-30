#pragma once
#include "Component.h"
#include <list>
#include "main.h"

class GameObject
{

protected:
	bool        m_Destroy = false;

	D3DXVECTOR3 m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_OldPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXQUATERNION m_Quaternion = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);

	D3DXVECTOR3 m_Velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	D3DXVECTOR3 m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	// m_Velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMATRIX  mtxWorld;

	std::list<Component*> m_Component;

public:
	void SetDestroy()
	{
		m_Destroy = true;
	}

	bool Destroy()
	{
		if (m_Destroy)
		{
			Uninit();
			delete this;
			return true;
		}
		else
		{
			return false;
		}
	}

	void SetPosition(D3DXVECTOR3 Position){ m_Position = Position; }
	void SetRotation(D3DXVECTOR3 Rotation){ m_Rotation = Rotation; }
	void SetQuaternion(D3DXQUATERNION Quaternion) { m_Quaternion = Quaternion; }

	//void SetVelocity(D3DXVECTOR3 Velocity) { m_Velocity = Velocity; }
	void SetScale(D3DXVECTOR3 Scale) { m_Scale = Scale; }

	D3DXVECTOR3 GetRotation(){ return m_Rotation; }
	D3DXVECTOR3 GetPosition() { return m_Position; }
	D3DXQUATERNION GetQuaternion(){ return m_Quaternion; }

	//D3DXVECTOR3 GetVelocity() { return m_Velocity; }
	D3DXVECTOR3 GetScale(){ return m_Scale; }

	//クォータニオンでベクトルを回転
	D3DXVECTOR3 Transform(const D3DXVECTOR3& v, const D3DXQUATERNION& q)
	{
		D3DXVECTOR3 qv = D3DXVECTOR3(q.x, q.y, q.z);
		D3DXVECTOR3 retVal = v;
		D3DXVECTOR3 tmp1, tmp2;
		D3DXVec3Cross(&tmp1, &qv, &v);
		tmp1 += q.w * v;
		D3DXVec3Cross(&tmp2, &qv, &tmp1);
		retVal += 2.0f * tmp2;

		return retVal;
	}

	/*D3DXVECTOR3 GetForward()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 forward;

		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;
	}

	D3DXVECTOR3 GetRight()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 forward;

		forward.x = rot._11;
		forward.y = rot._12;
		forward.z = rot._13;

		return forward;
	}

	D3DXVECTOR3 GetUp()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 forward;

		forward.x = rot._21;
		forward.y = rot._22;
		forward.z = rot._23;

		return forward;
	}*/

	D3DXVECTOR3 GetForward()
	{
		D3DXVECTOR3 forward;
		D3DXVECTOR3 unitZ = { 0.0f, 0.0f, 1.0f };
		forward = Transform(unitZ, m_Quaternion);

		return forward;
	}

	D3DXVECTOR3 GetRight()
	{
		D3DXVECTOR3 right;
		D3DXVECTOR3 unitX = { 1.0f, 0.0f, 0.0f };
		right = Transform(unitX, m_Quaternion);

		return right;
	}

	D3DXVECTOR3 GetUp()
	{
		D3DXVECTOR3 up;
		D3DXVECTOR3 unitY = { 0.0f, 1.0f, 0.0f };
		up = Transform(unitY, m_Quaternion);

		return up;
	}

	void RotateToNewForward(const D3DXVECTOR3& forward)
	{
		D3DXVECTOR3 unitX = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		D3DXVECTOR3 unitZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		
		float dot = D3DXVec3Dot(&unitX, &forward);
		float angle = acosf(dot);
		
		if (dot > 0.9999f)
		{
			SetQuaternion(D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f));
		}
		else if (dot < -0.9999f)
		{
			SetQuaternion(D3DXQUATERNION(unitZ.x, unitZ.y, unitZ.z, D3DX_PI));
		}
		else
		{
			D3DXVECTOR3 axis;
			D3DXVec3Cross(&axis, &unitX, &forward);
			D3DXVec3Normalize(&axis, &axis);
			SetQuaternion(D3DXQUATERNION(axis.x, axis.y, axis.z, angle));
		}
	}
	
	D3DXVECTOR3 TransformWithPerspDiv(const D3DXVECTOR3& vec, const D3DXMATRIX& mat, float w = 1.0f)
	{
		D3DXVECTOR3 retVal;
		retVal.x = vec.x * mat._11 + vec.y * mat._21 +
			vec.z * mat._31 + w * mat._41;
		retVal.y = vec.x * mat._12 + vec.y * mat._22 +
			vec.z * mat._31 + w * mat._42;
		retVal.z = vec.x * mat._13 + vec.y * mat._23 +
			vec.z * mat._33 + w * mat._43;
		float transformedW = vec.x * mat._14 + vec.y * mat._24 +
			vec.z * mat._34 + w * mat._44;

		transformedW = 1.0f / transformedW;
		retVal *= transformedW;

		return retVal;
	}


	virtual void Init() {}

	virtual void Uninit() 
	{
		for (Component* component : m_Component)    //範囲forループ
		{
			component->Uninit();
			delete component;
		}
		m_Component.clear();  //リスト構造の削除
	}

	virtual void Update()
	{
		for (Component* component : m_Component)    //範囲forループ
		{
			component->Update();
		}
	}

	virtual void Draw()
	{
		for (Component* component : m_Component)    //範囲forループ
		{
			component->Draw();
		}
	}

	template <typename T>  //テンプレート関数
	T* AddComponent()
	{
		Component* component = new T();
		m_Component.push_back(component);
		component->Init();

		return  (T*)component;
	}

	//引数：componentの所有者を指定
	template <typename T>  //テンプレート関数
	T* AddComponent(GameObject* owner)
	{
		Component* component = new T();
		m_Component.push_back(component);
		component->Init(owner);

		return  (T*)component;
	}

};
