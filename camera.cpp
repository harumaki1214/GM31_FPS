#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "player.h"
#include "scene.h"

#define	VALUE_MOVE_CAMERA	(20.0f)										// カメラの移動量
#define	VALUE_ROTATE_CAMERA	(D3DX_PI * 0.01f)							// カメラの回転量

void Camera::Init()
{
	m_Position = D3DXVECTOR3(0.0f, 5.0f, -10.0f);
	m_Target = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_PitchSpeed = 0.0f;
	m_MaxPitch = D3DX_PI / 3.0f;
	m_Pitch = 0.0f;
}

void Camera::Update()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	m_Target = player->GetPosition();
	D3DXQUATERNION q;

	m_Position = player->GetPosition() + D3DXVECTOR3(0.0f, 1.5f, 0.0f);
	m_Pitch += m_PitchSpeed;
	m_Pitch = min(max(m_Pitch, -m_MaxPitch), m_MaxPitch);
	D3DXVECTOR3 right = player->GetRight();
	D3DXQuaternionRotationAxis(&q, &right, m_Pitch);

	D3DXVECTOR3 viewForward = Transform(player->GetForward(), q);
	D3DXVECTOR3 target = m_Position + viewForward * 100.0f;
	D3DXVECTOR3 up = Transform(D3DXVECTOR3(0.0f, 1.0f, 0.0f), q);

	// ビューマトリックス設定
	D3DXMatrixLookAtLH(&m_ViewMatrix, &m_Position, &target, &up);

	// プロジェクションマトリックス設定
	D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, 1.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		1.0f, 1000.0f);
}

void Camera::Draw()
{
	// ビューマトリックス設定
	/*D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_ViewMatrix, &m_Position, &m_Target, &up);*/

	Renderer::SetViewMatrix(&m_ViewMatrix);
	Renderer::SetProjectionMatrix(&m_ProjectionMatrix);
}

D3DXVECTOR3 Camera::Unproject(const D3DXVECTOR3& screenPoint)
{
	D3DXVECTOR3 deviceCoord = screenPoint;
	deviceCoord.x /= SCREEN_WIDTH * 0.5f;
	deviceCoord.y /= SCREEN_HEIGHT * 0.5f;

	D3DXMATRIX unprojection = m_ViewMatrix * m_ProjectionMatrix;
	D3DXMatrixInverse(&unprojection, nullptr, &unprojection);

	return TransformWithPerspDiv(deviceCoord, unprojection);
}


