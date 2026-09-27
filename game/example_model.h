#pragma once
#include <motheye/model/plane_geometry.h>
#include <motheye/model/pyramid_geometry.h>
#include <motheye/model/cube_geometry.h>

#include <motheye/model/model.h>
#include <motheye/model/model_builder.h>
#include <motheye/math/math.h>

namespace game
{
	using motheye::math::Vector4;

    class ExampleModel
    {
    public:

        static std::unique_ptr<motheye::model::Model> Build(const std::filesystem::path& modelDirectory)
        {
            auto model = std::make_unique<motheye::model::Model>();
            motheye::model::ModelBuilder builder(*model, modelDirectory);

            builder.PushTexture("e7sbrickfloorbig", "textures/evillair/e7/e7sbrickfloorbig.dds");
            builder.PushTexture("e6panelsym", "textures/evillair/e6/evil6_support/e6panelsym.dds");
            builder.PushTexture("e6symbol_a", "textures/evillair/e6/evil6_trims/e6symbol_a.dds");
            builder.PushTexture("e6symbol_b", "textures/evillair/e6/evil6_trims/e6symbol_b.dds");
            builder.PushTexture("e6symbol_c", "textures/evillair/e6/evil6_trims/e6symbol_c.dds");
            builder.PushTexture("e6symbol_d", "textures/evillair/e6/evil6_trims/e6symbol_d.dds");

            builder.PushMaterial("ground", Vector4{ 1.0, 1.0, 1.0, 1.0 }, "e7sbrickfloorbig");
            builder.PushMaterial("pyramid", Vector4{ 1.0, 1.0, 1.0, 1.0 }, "e6panelsym");
            builder.PushMaterial("crateA", Vector4{ 1.0, 1.0, 1.0, 1.0 }, "e6symbol_a");
            builder.PushMaterial("crateB", Vector4{ 1.0, 1.0, 1.0, 1.0 }, "e6symbol_b");
            builder.PushMaterial("crateC", Vector4{ 1.0, 1.0, 1.0, 1.0 }, "e6symbol_c");
            builder.PushMaterial("crateD", Vector4{ 1.0, 1.0, 1.0, 1.0 }, "e6symbol_d");

            builder.PushMesh("plane", motheye::model::PlaneGeometry());
            builder.PushMesh("pyramid", motheye::model::PyramidGeometry());
            builder.PushMesh("cube", motheye::model::CubeGeometry());

            builder.PushSolid("ground", "plane", "ground");
            builder.PushSolid("pyramid", "pyramid", "pyramid");
            builder.PushSolid("cubeA", "cube", "crateA");
            builder.PushSolid("cubeB", "cube", "crateB");
            builder.PushSolid("cubeC", "cube", "crateC");
            builder.PushSolid("cubeD", "cube", "crateD");

            builder.PushLight("light1", Vector4(1.0, 1.0, 1.0, 1.0));
            builder.PushLight("light2", Vector4(1.0, 1.0, 1.0, 1.0));
            builder.PushLight("light3", Vector4(1.0, 1.0, 1.0, 1.0));
            builder.PushLight("light4", Vector4(1.0, 1.0, 1.0, 1.0));

            motheye::model::Transform transform;

            transform = { 0 };
            transform.sx = 6.0;
            transform.sy = 1.0;
            transform.sz = 6.0;
            builder.PushEntity("ground", motheye::model::EntityKind::kSolid, "ground", "", transform);

            transform = { 0 };
            transform.y = 2.0;
            transform.sx = 2.0;
            transform.sy = 2.0;
            transform.sz = 2.0;
            builder.PushEntity("pyramid", motheye::model::EntityKind::kSolid, "pyramid", "", transform);

            transform = { 0 };
            transform.x = 4.0;
            transform.y = 1.0;
            transform.sx = 1.0;
            transform.sy = 1.0;
            transform.sz = 1.0;
            builder.PushEntity("cubeA", motheye::model::EntityKind::kSolid, "cubeA", "", transform);

            transform = { 0 };
            transform.x = -4.0;
            transform.y = 1.0;
            transform.sx = 1.0;
            transform.sy = 1.0;
            transform.sz = 1.0;
            builder.PushEntity("cubeB", motheye::model::EntityKind::kSolid, "cubeB", "", transform);

            transform = { 0 };
            transform.z = 4.0;
            transform.y = 1.0;
            transform.sx = 1.0;
            transform.sy = 1.0;
            transform.sz = 1.0;
            builder.PushEntity("cubeC", motheye::model::EntityKind::kSolid, "cubeC", "", transform);

            transform = { 0 };
            transform.z = -4.0;
            transform.y = 1.0;
            transform.sx = 1.0;
            transform.sy = 1.0;
            transform.sz = 1.0;
            builder.PushEntity("cubeD", motheye::model::EntityKind::kSolid, "cubeD", "", transform);

            transform = { 0 };
            transform.x = 15.0;
            transform.y = 15.0;
            transform.z = -15.0;
            builder.PushEntity("light1", motheye::model::EntityKind::kLight, "light1", "", transform);

            transform = { 0 };
            transform.x = -15.0;
            transform.y = 15.0;
            transform.z = -15.0;
            builder.PushEntity("light2", motheye::model::EntityKind::kLight, "light2", "", transform);

            transform = { 0 };
            transform.x = -15.0;
            transform.y = 15.0;
            transform.z = 15.0;
            builder.PushEntity("light3", motheye::model::EntityKind::kLight, "light3", "", transform);

            transform = { 0 };
            transform.x = 15.0;
            transform.y = 15.0;
            transform.z = 15.0;
            builder.PushEntity("light4", motheye::model::EntityKind::kLight, "light4", "", transform);

            return model;
        }
    };
}