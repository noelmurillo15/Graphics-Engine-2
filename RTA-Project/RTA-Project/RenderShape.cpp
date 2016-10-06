#include "RenderShape.h"

#include "RenderMesh.h"
#include "RenderContext.h"

#include <fstream>
#include <string>


RenderShape::RenderShape() {
	Mat_Identity(worldMat);

	this->next = nullptr;
	this->func = NULL;

	mesh = new RenderMesh;
	context = new RenderContext;
}

RenderShape::~RenderShape() {
	delete mesh;
	delete context;
}

void RenderShape::InitializeModel(ID3D11Device* dev, const char* path) {
	this->func = RenderContext::RenderModel;
	Translate(worldMat, 0.5f, 0.5f, 2.0f);
	mesh->LoadOBJfromFile(path);
	context->InitializeModel(dev, mesh);
}

void RenderShape::ScaleModel(FLOAT3 scale) {
	Scale_4x4(worldMat, scale.x, scale.y, scale.z);
}

void RenderShape::RotateModel(float dt, unsigned int axis) {
	if (axis == 0)
		RotateX_Local(worldMat, dt);
	else if(axis == 1)
		RotateY_Local(worldMat, dt);
	else if (axis == 2)
		RotateZ_Local(worldMat, dt);
}

void RenderShape::TranslateModel(FLOAT3 trans) {
	Translate(worldMat, trans.x, trans.y, trans.z);
}

void RenderShape::InitializeGrid(ID3D11Device* dev) {
	this->func = RenderContext::RenderGrid;
	Mat_Identity(worldMat);
	context->InitializeGrid(dev);
}