#pragma once

// Core
#include "InputsManager.hpp"
// Core::DataStructure
#include "IComponent.hpp"
// Physics
#include "Rigidbody.hpp"
#include "Transform.hpp"
#include "Collider.hpp"

namespace Gameplay::Player
{
	class PlayerControler : public Core::DataStructure::IComponent
	{
		// Attribute
	private:
		float coyoteTimer= 0.2f;

		// Methode
	public:
		bool canJump = false;
		float coyoteCountdown;
		Core::Maths::Vec3* forwardCam;
		Core::Maths::Vec3* rightCam;
		PlayerControler(const float p_speed = 15.f, const float p_jumpHeight = 15.f);

		// Get and Set
		void SetCam(Core::Maths::Vec3& p_forward, Core::Maths::Vec3& p_right) { forwardCam = &p_forward; rightCam = &p_right; };
		Core::Maths::Vec3* GetForwardCam() { return forwardCam; };
	};
}