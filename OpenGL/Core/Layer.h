#pragma once

#include "../GLCommon.h"
#include "Event.h"


namespace GL {

	class Layer {
	public:
		Layer() = default;
		Layer(const std::string& name) : m_DebugName(name) {};
		virtual ~Layer() = default;

		virtual void OnAdd() {};
		virtual void OnRemove() {};

		virtual void OnUpdate(float deltaTime) {};
		virtual void OnEvent(Event& event) {};
		virtual void OnOverlayRender() {};

		std::string GetName() { return m_DebugName; };

	private:
		std::string m_DebugName;
	};

}

