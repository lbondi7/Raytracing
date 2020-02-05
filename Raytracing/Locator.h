#pragma once

struct MeshLoading;

struct Locator
{
	static void InitMeshes(MeshLoading* _meshes) { meshes = _meshes; }

	static MeshLoading* GetMeshes() { return meshes; }



private:

	static MeshLoading* meshes;

};

