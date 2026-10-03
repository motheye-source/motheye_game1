#include "mouse_keyboard_handler.h"

namespace motheye::engine::input
{
	void MouseKeyboardHandler::Apply(const MouseKeyboardEvent& event)
	{
		switch (event.kind)
		{
			case MouseKeyboardEvent::Kind::KeyDown:
			{
				const size_t idx = static_cast<size_t>(event.keyCode);
				state_.heldKeys.set(idx);
				state_.pressedKeys.set(idx);
				break;
			}
			case MouseKeyboardEvent::Kind::KeyUp:
			{
				const size_t idx = static_cast<size_t>(event.keyCode);
				state_.heldKeys.reset(idx);
				state_.releasedKeys.set(idx);
				break;
			}
			case MouseKeyboardEvent::Kind::MouseButtonDown:
			{
				state_.heldButtons |= event.buttonFlags;
				state_.pressedButtons |= event.buttonFlags;
				break;
			}
			case MouseKeyboardEvent::Kind::MouseButtonUp:
			{
				state_.heldButtons &= ~event.buttonFlags;
				state_.releasedButtons |= event.buttonFlags;
				break;
			}
			case MouseKeyboardEvent::Kind::MouseMove:
			{
				state_.mouseDeltaX += event.dxPixels;
				state_.mouseDeltaY += event.dyPixels;
				break;
			}
			case MouseKeyboardEvent::Kind::MouseWheel:
			default:
			{
				break;
			}
		}
	}
}