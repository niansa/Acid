#include "CameraFps.hpp"

#include <Devices/Mouse.hpp>
#include <Inputs/Input.hpp>
#include <Scenes/Scenes.hpp>
#include <Scenes/Entity.inl>
#include <Maths/Maths.hpp>
#include <Graphics/Graphics.hpp>
#include <Physics/CollisionObject.hpp>
#include "PlayerFps.hpp"

namespace test {
static const Vector3f ViewOffset(0.0f, 1.8f, 0.0f);

CameraFps::CameraFps() {
	m_nearPlane = 0.1f;
	m_farPlane = 4098.0f;
	m_fieldOfView = Maths::Radians(70.0f);
}

void CameraFps::Start() {
}

void CameraFps::Update() {
	auto delta = Engine::Get()->GetDelta().AsSeconds();

	if (auto scenePlayer = Scenes::Get()->GetScene()->GetComponents<PlayerFps>()[0]) {
		if (auto transformPlayer = scenePlayer->GetEntity()->GetComponent<Transform>()) {
			m_velocity = (transformPlayer->GetPosition() - m_position) / delta;
			m_position = transformPlayer->GetPosition() + ViewOffset;
		}
	}

	if (!Scenes::Get()->IsPaused()) {
		auto rotationDelta = Mouse::Get()->IsCursorHidden() * Vector2f(Input::Get()->GetAxis("mouseX")->GetAmount(),
			Input::Get()->GetAxis("mouseY")->GetAmount());

		m_rotation.m_y += rotationDelta.m_x;
		m_rotation.m_x += rotationDelta.m_y;
		m_rotation.m_x = std::clamp(m_rotation.m_x, Maths::Radians(90.0f), Maths::Radians(270.0f));
	}

	m_viewMatrix = Matrix4::ViewMatrix(m_position, m_rotation);
	m_projectionMatrix = Matrix4::PerspectiveMatrix(GetFieldOfView(), Window::Get()->GetAspectRatio(), GetNearPlane(), GetFarPlane());

	m_viewFrustum.Update(m_viewMatrix, m_projectionMatrix);
	m_viewRay.Update(m_position, {0.5f, 0.5f}, m_viewMatrix, m_projectionMatrix);
}
}
