#ifndef _RENDERNODE_H_
#define _RENDERNODE_H_

#include "MathFunc.h"

class RenderNode;
class RenderShape;
class ID3D11DeviceContext;
typedef void(*RenderFunc)(ID3D11DeviceContext*, RenderShape*);

class RenderNode {

public:
	RenderNode *next;  // Used to define list of nodes
	RenderFunc func;  // Function pointer

	// Process used by the Renderer
	inline void renderProcess(ID3D11DeviceContext* _devContext, RenderShape* _model) { func(_devContext, _model); };
};
#endif