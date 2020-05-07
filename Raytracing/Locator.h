#pragma once

struct MeshLoading;
class ThreadManager;

struct Locator
{
	static void InitMeshes(MeshLoading* _meshes) { meshes = _meshes; }

	static MeshLoading* GetMeshes() { return meshes; }

	static void InitTM(ThreadManager* _tm) { if(!tm) tm = _tm; }

	static ThreadManager* GetTM() { return tm; }

private:

	static MeshLoading* meshes;

	static ThreadManager* tm;
};

