#include "Object.h"
#include "Locator.h"

constexpr float kEPSILON = 1e-8;

Object::Object(Vec3 _center)
{
	center = _center;
}

Object::~Object()
{
}

void Object::Load(const std::string& meshType)
{

	mesh = Locator::GetMeshes()->meshes[Locator::GetMeshes()->meshesID[meshType]];

    //for (size_t i = 0; i < mesh.indices.size(); i+=3)
    //{
    //    Triangle tri(center + mesh.vertices[mesh.indices[i]].pos,
    //        center + mesh.vertices[mesh.indices[i + 1]].pos,
    //        center + mesh.vertices[mesh.indices[i + 2]].pos);

    //    tris.push_back(tri);
    //}

    boundingBox.Load(mesh);
}

void Object::Load(const std::string& meshType, const Vec3& _center)
{
    center = _center;
    mesh = Locator::GetMeshes()->meshes[Locator::GetMeshes()->meshesID[meshType]];

    for (size_t i = 0; i < mesh.indices.size(); i += 3)
    {
        Triangle tri(&center, mesh.vertices[mesh.indices[i]],
            mesh.vertices[mesh.indices[i + 1]],
            mesh.vertices[mesh.indices[i + 2]]);

        tris.push_back(tri);
    }

    boundingBox.Load(mesh);
}

void Object::Load(const std::string& meshType, const Vec3& _center, int _id)
{
    id = _id;
    center = _center;
    mesh = Locator::GetMeshes()->meshes[Locator::GetMeshes()->meshesID[meshType]];
    int j = 0;
    for (size_t i = 0; i < mesh.indices.size(); i += 3)
    {
        Triangle tri(&center, mesh.vertices[mesh.indices[i]],
            mesh.vertices[mesh.indices[i + 1]],
            mesh.vertices[mesh.indices[i + 2]]);

        tri.tri_id = j;
        tri.obj_id = id;
        tris.push_back(tri);
        ++j;
    }

    boundingBox.Load(mesh);
}

void Object::Load(const std::string& meshType, const Transform& _tran, int _id)
{
    id = _id;
    transform = _tran;
    mesh = Locator::GetMeshes()->meshes[Locator::GetMeshes()->meshesID[meshType]];
    int j = 0;
    for (size_t i = 0; i < mesh.indices.size(); i += 3)
    {
        Triangle tri(&center, mesh.vertices[mesh.indices[i]],
            mesh.vertices[mesh.indices[i + 1]],
            mesh.vertices[mesh.indices[i + 2]]);

        tri.tri_id = j;
        tri.obj_id = id;
        tris.push_back(tri);
        ++j;
    }

    boundingBox.Load(mesh);
}

void Object::SetCenter(Vec3 _center)
{
    center = _center;

    for (auto& t : tris)
    {
        t.Update();
    }
}

void Object::Update()
{

    //for (auto& t : tris)
    //{
    //    t.Update();
    //}

}
