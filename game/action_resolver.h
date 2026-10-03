#pragma once
#include "action_frame.h"
#include "mouse_keyboard_binder.h"
#include "controller_binder.h"

#include <motheye/engine/input/frame_input.h>
#include <motheye/engine/input/input_device.h>

namespace game
{
	using motheye::engine::input::FrameInput;
	using motheye::engine::input::InputDevice;

	class ActionResolver
	{
	public:
		const ActionFrame& CurrentFrame() const;

		InputContext CurrentContext() const;
		void SetContext(InputContext context);

		const ActionFrame& Resolve(const FrameInput& input, float deltaSeconds);

	private:
		const ActionFrame& ApplyResolvedFrame(const ActionFrame& frame);

		MouseKeyboardBinder mouseKeyboardBinder_;
		ControllerBinder controllerBinder_;
		InputContext context_{ InputContext::Undefined };
		ActionFrame lastFrame_{};
	};

	inline const ActionFrame& ActionResolver::Resolve(const FrameInput& input, float deltaSeconds)
	{
		if (input.GetActiveDevice() == InputDevice::MouseKeyboard)
		{
			return ApplyResolvedFrame(mouseKeyboardBinder_.Resolve(context_, input.GetMouseKeyboard(), deltaSeconds));
		}

		return ApplyResolvedFrame(controllerBinder_.Resolve(context_, input.GetController(), deltaSeconds));
	}

	inline const ActionFrame& ActionResolver::ApplyResolvedFrame(const ActionFrame& frame)
	{
		lastFrame_ = frame;

		if (context_ == InputContext::Gameplay && lastFrame_.openMenuPressed)
		{
			SetContext(InputContext::Menu);
		}
		else if (context_ == InputContext::Menu && lastFrame_.backPressed)
		{
			SetContext(InputContext::Gameplay);
		}

		return lastFrame_;
	}

	inline const ActionFrame& ActionResolver::CurrentFrame() const
	{
		return lastFrame_;
	}

	inline InputContext ActionResolver::CurrentContext() const
	{
		return context_;
	}

	inline void ActionResolver::SetContext(InputContext context)
	{
		if (context == context_)
		{
			return;
		}

		context_ = context;
	}
}