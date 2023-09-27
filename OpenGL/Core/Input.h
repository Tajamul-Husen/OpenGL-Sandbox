#pragma once

#include "../GLCommon.h"
#include "KeyCodes.h"
#include "MouseButtonCodes.h"


namespace GL {

	class Input {
	public:
		static bool IsKeyPressed(int key);
		static bool IsMouseButtonPressed(int button);

		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();

	};

};
