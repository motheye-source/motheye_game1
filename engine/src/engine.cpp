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
		//game_.Resolve(inputState_, deltaSeconds);
		RenderScene();
	}

	void Engine::LoadWorld(const motheye::model::Model& model, const std::filesystem::path& defaultTexture)
	{
		world_ = world::WorldLoader::Load(*renderer_, model, defaultTexture);
	}

	void Engine::RenderScene()
	{
		world::Transform transform;

		renderer::Frame& frame = renderer_->GetFrame();

		for (const auto& object : world_->GetRoot())
		{
			switch (object.kind)
			{
			case motheye::model::EntityKind::kCamera:
			{
				const auto camera = object.instance.As<renderer::CameraInstanceHandle>();
				renderer_->SetInstanceMatrix(camera, transform.ToXMMatrix());

				frame.SetCamera(camera);
			}
			break;

			case motheye::model::EntityKind::kSolid:
				renderer_->SetInstanceMatrix(object.instance.As<renderer::SolidInstanceHandle>(), object.ComputeWorldMatrix());
				break;
			case motheye::model::EntityKind::kLight:
				renderer_->SetInstanceMatrix(object.instance.As<renderer::LightInstanceHandle>(), object.ComputeWorldMatrix());
				break;
			}
		}

		frame.SetAmbientLightColor({ 0.3f, 0.3f, 0.3f, 1.0f });

		renderer_->SetRenderTargetClearColor({ 0.45f, 0.55f, 0.60f, 1.00f });
		renderer_->BeginRender();
		renderer_->RenderScene();
		renderer_->EndRender();
	}
}