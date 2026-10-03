#include "engine.h"
#include "world/world_loader.h"

using namespace motheye::renderer;

namespace motheye::engine
{
	void Engine::Start(HWND hWnd)
	{
		InitRenderer(hWnd);
	}
	
	void Engine::Stop()
	{
		renderer_->Destroy();
	}

	void Engine::InitRenderer(HWND hWnd)
	{
		try
		{
			adapter_ = motheye::dx12::DXGIFactory(true).GetHighPerformanceAdapter();
			renderer_ = std::make_unique<renderer::Renderer>(*adapter_.get(), hWnd);
			adapter_->OutputDebugVideoMemory("After Device::Initialize");
			renderer_->Resize(800, 600);
			adapter_->OutputDebugVideoMemory("After Device::Resize");
		}
		catch (motheye::dx12::DirectXException ex)
		{
			::OutputDebugStringA(ex.what());
		}
	}

	void Engine::OnRunFrame()
	{       
		const float deltaSeconds = clock_.Tick();

		// TODO:  forward inputState_ to game to resolve actions
		// game_.Resolve(inputState_, deltaSeconds);

		world_.Render(*renderer_);
	}

	void Engine::LoadWorld(const motheye::model::Model& model, const std::filesystem::path& defaultTexture)
	{
		world_.Load(*renderer_, model, defaultTexture);
	}	
}