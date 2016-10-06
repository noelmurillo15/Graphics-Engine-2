#ifndef _RENDERMESH_H_
#define _RENDERMESH_H_

#include "Defines.h"


class RenderMesh {

public:
	MODEL data;

	RenderMesh();
	~RenderMesh();

	bool LoadOBJfromFile(const char* path);
};
#endif