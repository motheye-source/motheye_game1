#pragma once
#include "action_frame.h"

#include <motheye/engine/input/frame_input.h>

namespace game
{
	using motheye::platform::input::ControllerEvent;
	using motheye::platform::input::ControllerAnalogState;
	using motheye::engine::input::FrameInput;

	class ControllerBinder
	{
	public:
		ActionFrame Resolve(InputContext context, const FrameInput::Controller& snapshot, float deltaSeconds) const;

	private:
		ActionFrame ResolveGameplay(const FrameInput::Controller& snapshot, float deltaSeconds) const;
		ActionFrame ResolveMenu(const FrameInput::Controller& snapshot) const;

		// Tuned by feel; stick displacement is a rate (how fast to turn while held),
		// so this is scaled by deltaSeconds, unlike mouse sensitivity.
		static constexpr float controllerLookSensitivity_{ 3.0f };
	};

	inline ActionFrame ControllerBinder::Resolve(InputContext context, const FrameInput::Controller& snapshot, float deltaSeconds) const
	{
		switch (context)
		{
		case InputContext::Gameplay:
			return ResolveGameplay(snapshot, deltaSeconds);
		case InputContext::Menu:
			return ResolveMenu(snapshot);
		case InputContext::Undefined:
		default:
			return ActionFrame{};
		}
	}

	inline ActionFrame ControllerBinder::ResolveGameplay(const FrameInput::Controller& snapshot, float deltaSeconds) const
	{
		ActionFrame frame;
		frame.context = InputContext::Gameplay;

		const auto& analog = snapshot.Analog();
		frame.moveX = analog.leftStickX;
		frame.moveY = analog.leftStickY;
		frame.moveZ = 
			(snapshot.IsButtonHeld(ControllerEvent::ButtonMask::A) ? 1.0f : 0.0f) -
			(snapshot.IsButtonHeld(ControllerEvent::ButtonMask::Y) ? 1.0f : 0.0f);
		frame.lookYaw = analog.rightStickX * controllerLookSensitivity_ * deltaSeconds;
		frame.lookPitch = analog.rightStickY * controllerLookSensitivity_ * deltaSeconds;
		frame.openMenuPressed = snapshot.WasButtonPressed(ControllerEvent::ButtonMask::Start);

		return frame;
	}

	inline ActionFrame ControllerBinder::ResolveMenu(const FrameInput::Controller& snapshot) const
	{
		ActionFrame frame;
		frame.context = InputContext::Menu;

		frame.backPressed = snapshot.WasButtonPressed(ControllerEvent::ButtonMask::B);

		return frame;
	}
}