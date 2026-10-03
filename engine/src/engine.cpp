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

		world_.Stage(*renderer_);

		RenderScene();
	}

	void Engine::RenderScene()
	{
		renderer_->GetFrame().SetAmbientLightColor({ 0.3f, 0.3f, 0.3f, 1.0f });
		renderer_->SetRenderTargetClearColor({ 0.45f, 0.55f, 0.60f, 1.00f });
		renderer_->BeginRender();
		renderer_->RenderScene();
		renderer_->EndRender();
	}

	void Engine::LoadWorld(const motheye::model::Model& model, const std::filesystem::path& defaultTexture)
	{
		world_.Load(*renderer_, model, defaultTexture);
	}	
}