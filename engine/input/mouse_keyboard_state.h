#pragma once
#include <motheye/platform/input/mouse_keyboard_event.h>
#include <motheye/platform/input/key_code.h>

#include <bitset>
#include <cstdint>

namespace motheye::engine::input
{
	using motheye::platform::input::MouseKeyboardEvent;
	using motheye::platform::input::KeyCode;
	using motheye::platform::input::KeyCodeLimits;

	// Owns the authoritative held-state for mouse/keyboard, plus this-frame press/release
	// transitions, updated by applying a stream of MouseKeyboardEvent (real or synthetic).
	// Platform independent; adapters (motheye_platform) only ever produce events, never hold
	// this state.
	class MouseKeyboardState
	{
	public:
		// Applies one event's effect to held-state and this-frame transitions.
		void Apply(const MouseKeyboardEvent& event);

		// Clears this-frame transitions; preserves held-state. Call once per Run() iteration
		// before pumping messages for that iteration.
		void BeginFrame();

		// Marks everything currently held as released this frame, then clears held-state.
		// Used on focus loss.
		void ReleaseAllHeld();

		bool IsKeyHeld(KeyCode key) const;
		bool WasKeyPressed(KeyCode key) const;
		bool WasKeyReleased(KeyCode key) const;

		uint32_t HeldButtonState() const;
		uint32_t ButtonsPressedThisFrame() const;
		uint32_t ButtonsReleasedThisFrame() const;

		int32_t MouseDeltaX() const;
		int32_t MouseDeltaY() const;

	private:
		std::bitset<KeyCodeLimits::Size> heldKeys_{};
		std::bitset<KeyCodeLimits::Size> pressedKeys_{};
		std::bitset<KeyCodeLimits::Size> releasedKeys_{};

		uint32_t heldButtons_{ MouseKeyboardEvent::ButtonMask::None };
		uint32_t pressedButtons_{ MouseKeyboardEvent::ButtonMask::None };
		uint32_t releasedButtons_{ MouseKeyboardEvent::ButtonMask::None };

		int32_t mouseDeltaX_{ 0 };
		int32_t mouseDeltaY_{ 0 };
	};

	inline void MouseKeyboardState::Apply(const MouseKeyboardEvent& event)
	{
		switch (event.kind)
		{
		case MouseKeyboardEvent::Kind::KeyDown:
		{
			const size_t idx = static_cast<size_t>(event.keyCode);
			heldKeys_.set(idx);
			pressedKeys_.set(idx);
			break;
		}
		case MouseKeyboardEvent::Kind::KeyUp:
		{
			const size_t idx = static_cast<size_t>(event.keyCode);
			heldKeys_.reset(idx);
			releasedKeys_.set(idx);
			break;
		}
		case MouseKeyboardEvent::Kind::MouseButtonDown:
		{
			heldButtons_ |= event.buttonFlags;
			pressedButtons_ |= event.buttonFlags;
			break;
		}
		case MouseKeyboardEvent::Kind::MouseButtonUp:
		{
			heldButtons_ &= ~event.buttonFlags;
			releasedButtons_ |= event.buttonFlags;
			break;
		}
		case MouseKeyboardEvent::Kind::MouseMove:
		{
			mouseDeltaX_ += event.dxPixels;
			mouseDeltaY_ += event.dyPixels;
			break;
		}
		case MouseKeyboardEvent::Kind::MouseWheel:
		default:
		{
			break;
		}
		}
	}

	inline void MouseKeyboardState::BeginFrame()
	{
		pressedKeys_.reset();
		releasedKeys_.reset();
		pressedButtons_ = MouseKeyboardEvent::ButtonMask::None;
		releasedButtons_ = MouseKeyboardEvent::ButtonMask::None;
		mouseDeltaX_ = 0;
		mouseDeltaY_ = 0;
	}

	inline void MouseKeyboardState::ReleaseAllHeld()
	{
		releasedKeys_ |= heldKeys_;
		heldKeys_.reset();

		releasedButtons_ |= heldButtons_;
		heldButtons_ = MouseKeyboardEvent::ButtonMask::None;
	}

	inline bool MouseKeyboardState::IsKeyHeld(KeyCode key) const
	{
		return heldKeys_.test(static_cast<size_t>(key));
	}

	inline bool MouseKeyboardState::WasKeyPressed(KeyCode key) const
	{
		return pressedKeys_.test(static_cast<size_t>(key));
	}

	inline bool MouseKeyboardState::WasKeyReleased(KeyCode key) const
	{
		return releasedKeys_.test(static_cast<size_t>(key));
	}

	inline uint32_t MouseKeyboardState::HeldButtonState() const
	{
		return heldButtons_;
	}

	inline uint32_t MouseKeyboardState::ButtonsPressedThisFrame() const
	{
		return pressedButtons_;
	}

	inline uint32_t MouseKeyboardState::ButtonsReleasedThisFrame() const
	{
		return releasedButtons_;
	}

	inline int32_t MouseKeyboardState::MouseDeltaX() const
	{
		return mouseDeltaX_;
	}

	inline int32_t MouseKeyboardState::MouseDeltaY() const
	{
		return mouseDeltaY_;
	}
}
