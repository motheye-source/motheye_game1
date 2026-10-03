#include "game.h"
#include "example_model.h"

#include <motheye/model/model.h>
#include <motheye/blender/compiler.h>
#include <motheye/compiler/compiler.h>
#include <motheye/engine/world/i_entity_factory.h>

#include <DirectXMath.h>

namespace game
{
	void Game::Start(HWND hWnd)
	{
        engine_.Start(hWnd);

        engine_.RegisterEntityFactory(std::make_unique<EntityFactory>());

        LoadWorld();

        actionResolver_.SetContext(InputContext::Gameplay);
	}

	void Game::Stop()
	{
        engine_.Stop();
	}

	void Game::LoadWorld()
	{
        enum class WorldSource {
            Example,
            Blender,
            Map
        };

        auto sourcePath = std::filesystem::path(__FILE__).parent_path();
        auto assetsPath = sourcePath.parent_path().parent_path() / "motheye_assets";

        //const WorldSource worldSource{ WorldSource::Example };
        //const WorldSource worldSource{ WorldSource::Blender };
        const WorldSource worldSource{ WorldSource::Map };

        std::unique_ptr<Model> model;

        switch (worldSource)
        {
        case WorldSource::Example:
        {
            model = ExampleModel::Build(assetsPath);
        }
        break;

        case WorldSource::Blender:
        {
            //const auto scenePath = (assetsPath / "scenes/colorcube.json").make_preferred();
            const auto scenePath = (assetsPath / "scenes/light_blue_cube.json").make_preferred();
            model = motheye::blender::Compiler::Compile(scenePath);
        }
        break;

        case WorldSource::Map:
        {
            //const auto mapPath = (assetsPath / "maps/onebrush.map").make_preferred();
            const auto mapPath = (assetsPath / "maps/test1.map").make_preferred();
            //const auto mapPath = (assetsPath / "maps/light_blue_cube.map").make_preferred();
            //const auto mapPath = (assetsPath / "maps/oneroom.map").make_preferred();
            motheye::compiler::Compiler compiler(assetsPath);
            model = compiler.Compile(mapPath);
        }
        break;
        }

        LoadDefaultCamera(*model);

        auto defaultTexture = (assetsPath / "textures/default.dds").make_preferred();

        engine_.LoadWorld(*model, defaultTexture);
	}

    void Game::LoadDefaultCamera(Model& model)
    {
        using namespace motheye::model;

        // Default camera
        Camera camera{};
        camera.name = "default";
        camera.fov = 0.25f * DirectX::XM_PI;
        camera.nearClip = 1.0f;
        camera.farClip = 10000.0f;
        model.resources.cameras.emplace_back(camera);

        motheye::model::Entity entity{};
        entity.name = "@default_camera";
        entity.resource = "default";
        entity.classname = "fps_camera";
        entity.kind = EntityKind::kCamera;
        entity.transform = {};
        entity.transform.rotationKind = RotationKind::kEuler;
        entity.transform.rw = 1.0;
        entity.transform.sx = 1.0;
        entity.transform.sy = 1.0;
        entity.transform.sz = 1.0;
        model.entities.emplace_back(entity);

        model.rootnode.nodes.emplace_back(entity.name);
    }
}