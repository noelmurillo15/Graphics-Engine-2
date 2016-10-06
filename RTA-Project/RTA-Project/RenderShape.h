#ifndef _RENDERSHAPE_H_
#define _RENDERSHAPE_H_

#include "RenderNode.h"

class RenderMesh;
class RenderContext;
class ID3D11Device;


class RenderShape : public RenderNode {

	RenderMesh* mesh;
	RenderContext* context;

	MATRIX4X4 worldMat;		  

public:

	RenderShape();
	~RenderShape();

	void InitializeGrid(ID3D11Device* dev);
	void InitializeModel(ID3D11Device* dev, const char* path);

	void ScaleModel(FLOAT3 scale);
	void RotateModel(float dt, unsigned int axis);
	void TranslateModel(FLOAT3 trans);

	RenderMesh* GetModelMesh() { return mesh; }
	RenderContext* GetModelContext() { return context; }

	MATRIX4X4 GetWorldMatrix() { return worldMat; }
};
#endif
