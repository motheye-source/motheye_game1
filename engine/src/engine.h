#pragma once

#include <motheye/platform/window/i_frame_handler.h>
#include <motheye/platform/input/mouse_keyboard_event.h>
#include <motheye/platform/input/controller_event.h>

#include <motheye/engine/i_engine.h>
#include <motheye/engine/input/input_state.h>
#include <motheye/model/model.h>

#include "renderer/renderer.h"

#include "clock_source.h"
#include "src/world/entity_manager.h"
#include "src/world/world.h"

#include <memory>
#include <filesystem>

namespace motheye::engine
{
	using motheye::platform::window::IFrameHandler;
	using motheye::platform::input::MouseKeyboardEvent;
	using motheye::platform::input::ControllerEvent;

	using motheye::model::Model;

	class Engine : public IFrameHandler, public IEngine
	{
	public:
		void Start(HWND hWnd);
		void Stop();

		void OnInitFrame() override;
		void OnRunFrame() override;

		void OnMouseKeyboardEvent(const MouseKeyboardEvent& event) override;
		void OnControllerEvent(const ControllerEvent& event) override;

		void OnResize(unsigned int, unsigned int) override;
		void OnReleaseInput() override;
			
		void RegisterEntityFactory(std::unique_ptr<world::IEntityFactory>&& factory) override;
		void LoadWorld(const motheye::model::Model& model, const std::filesystem::path& defaultTexture) override;

	private:
		void InitRenderer(HWND hWnd);
		void RenderScene();

	private:
		ClockSource clock_;
		input::InputState inputState_;
		
		std::unique_ptr<motheye::dx12::DXGIAdapter> adapter_;
		std::unique_ptr<renderer::Renderer> renderer_;

		std::unique_ptr<world::World> world_;
		world::EntityManager entityManager_;
	};
	
	inline void Engine::OnInitFrame()
	{
		inputState_.BeginFrame();
	}

	inline void Engine::OnMouseKeyboardEvent(const input::MouseKeyboardEvent& event)
	{
		inputState_.ApplyMouseKeyboardEvent(event);
	}

	inline void Engine::OnControllerEvent(const input::ControllerEvent& event)
	{
		inputState_.ApplyControllerEvent(event);
	}

	inline void Engine::OnResize(unsigned int width, unsigned int height)
	{
		renderer_->Resize(width, height);
	}

	inline void Engine::OnReleaseInput()
	{
		inputState_.ReleaseActiveHeldInput();

		// TODO: How to notify App so that input mode can be switched to legacy if needed?
	}

	inline void Engine::RegisterEntityFactory(std::unique_ptr<world::IEntityFactory>&& factory)
	{
		entityManager_.RegisterFactory(std::move(factory));
	}

}
