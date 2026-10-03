#pragma once
#include "action_frame.h"

#include <motheye/engine/input/frame_input.h>
#include <motheye/platform/input/key_code.h>

namespace game
{
	using motheye::platform::input::KeyCode;
	using motheye::engine::input::FrameInput;

	class MouseKeyboardBinder
	{
	public:
		ActionFrame Resolve(InputContext context, const FrameInput::MouseKeyboard& snapshot, float deltaSeconds) const;

	private:
		ActionFrame ResolveGameplay(const FrameInput::MouseKeyboard& snapshot) const;
		ActionFrame ResolveMenu(const FrameInput::MouseKeyboard& snapshot) const;

		// Tuned by feel; mouse deltas are per-frame mickeys, not a rate, so this is
		// applied directly without deltaSeconds. Matches the camera's prior
		// lookSensitivity_ constant (0.15 degrees per mickey, in radians).
		static constexpr float mouseSensitivity_{ 0.15f * 3.14159265358979323846f / 180.0f };
	};

	inline ActionFrame MouseKeyboardBinder::Resolve(InputContext context, const FrameInput::MouseKeyboard& snapshot, float deltaSeconds) const
	{
		(void)deltaSeconds;

		switch (context)
		{
		case InputContext::Gameplay:
			return ResolveGameplay(snapshot);
		case InputContext::Menu:
			return ResolveMenu(snapshot);
		case InputContext::Undefined:
		default:
			return ActionFrame{};
		}
	}

	inline ActionFrame MouseKeyboardBinder::ResolveGameplay(const FrameInput::MouseKeyboard& snapshot) const
	{
		ActionFrame frame;
		frame.context = InputContext::Gameplay;

		frame.moveX = (snapshot.IsKeyHeld(KeyCode::D) ? 1.0f : 0.0f) - (snapshot.IsKeyHeld(KeyCode::A) ? 1.0f : 0.0f);
		frame.moveY = (snapshot.IsKeyHeld(KeyCode::W) ? 1.0f : 0.0f) - (snapshot.IsKeyHeld(KeyCode::S) ? 1.0f : 0.0f);
		frame.moveZ = (snapshot.IsKeyHeld(KeyCode::Q) ? 1.0f : 0.0f) - (snapshot.IsKeyHeld(KeyCode::Z) ? 1.0f : 0.0f);
		frame.lookYaw = static_cast<float>(snapshot.MouseDeltaX()) * mouseSensitivity_;
		frame.lookPitch = static_cast<float>(snapshot.MouseDeltaY()) * mouseSensitivity_;
		frame.openMenuPressed = snapshot.WasKeyPressed(KeyCode::Escape);
		frame.toggleFullscreenPressed = snapshot.WasKeyPressed(KeyCode::F11);

		return frame;
	}

	inline ActionFrame MouseKeyboardBinder::ResolveMenu(const FrameInput::MouseKeyboard& snapshot) const
	{
		ActionFrame frame;
		frame.context = InputContext::Menu;

		frame.backPressed = snapshot.WasKeyPressed(KeyCode::Escape);
		frame.toggleFullscreenPressed = snapshot.WasKeyPressed(KeyCode::F11);

		return frame;
	}
}