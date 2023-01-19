#include "PlayerControler.hpp"

namespace Gameplay::Player
{
	PlayerControler::PlayerControler(const float p_speed, const float p_jumpHeight)
		: forwardCam(nullptr)
		, rightCam(nullptr)
	{
		isEnable = false;
	}
}