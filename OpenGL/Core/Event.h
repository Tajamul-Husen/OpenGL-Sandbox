#pragma once

#include "../GLCommon.h"


namespace GL {

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize,
		KeyPressed,
		MouseButtonPressed, MouseMoved, MouseScrolled
	};

#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#define EVENT_TYPE(type) static EventType GetStaticType() { return EventType::##type; }; \
						EventType GetEventType() const override { return GetStaticType(); };


	class Event {
	public:
		Event() = default;

		virtual EventType GetEventType() const = 0;
		virtual bool IsHandled() const { return m_Handled; };

		mutable bool m_Handled = false;
	};


	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event) : m_Event(event) {};

		template<typename T, typename F>
		bool Dispatch(const F& fn)
		{
			if (m_Event.GetEventType() == T::GetStaticType() && !m_Event.IsHandled()) {
				m_Event.m_Handled = fn(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		};

	private:
		Event& m_Event;
	};


	class WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() {};

		EVENT_TYPE(WindowClose);
	};


	class WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(int width, int height) : m_Width(width), m_Height(height) {};

		unsigned int GetWidth() const { return m_Width; };
		unsigned int GetHeight() const { return m_Height; };

		EVENT_TYPE(WindowResize);

	private:
		unsigned int m_Width, m_Height;
	};


	class KeyPressedEvent : public Event {
	public:
		KeyPressedEvent(int keyCode, int m_RepeatCount) : m_KeyCode(keyCode), m_RepeatCount(m_RepeatCount) {};

		int GetKeyCode() { return m_KeyCode; };
		int GetRepeatCount() { return m_RepeatCount; };

		EVENT_TYPE(KeyPressed);
	private:
		int m_KeyCode, m_RepeatCount;
	};


	class MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(float xPos, float yPos) : m_XPos(xPos), m_YPos(yPos) {};

		float GetXPos() const { return m_XPos; };
		float GetYPos() const { return m_YPos; };

		EVENT_TYPE(MouseMoved);
	private:
		float m_XPos, m_YPos;
	};


	class MouseScrolledEvent : public Event {
	public:
		MouseScrolledEvent(float xoffset, float yoffset) : m_XOffset(xoffset), m_YOffset(yoffset) {};

		float GetXOffset() const { return m_XOffset; };
		float GetYOffset() const { return m_YOffset; };

		EVENT_TYPE(MouseScrolled);
	private:
		float m_XOffset, m_YOffset;
	};

}
