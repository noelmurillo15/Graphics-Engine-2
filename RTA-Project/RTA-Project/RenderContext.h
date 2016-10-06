#ifndef _RENDERCONTEXT_H_
#define _RENDERCONTEXT_H_

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

class RenderShape;
class RenderMesh;

class RenderContext {

	ID3D11Buffer*				IndexBuffer = nullptr;
	ID3D11Buffer*				VertexBuffer = nullptr;
	ID3D11InputLayout*			InputLayout = nullptr;
	ID3D11VertexShader*			VShader = nullptr;
	ID3D11PixelShader*			PShader = nullptr;

	ID3D11ShaderResourceView*	SRView = nullptr;
	ID3D11SamplerState*			SamplerState = nullptr;
	ID3D11Texture2D*			T2DBuffer = nullptr;

public:
	
	RenderContext();
	~RenderContext();

	void InitializeGrid(ID3D11Device* d);
	void InitializeModel(ID3D11Device* _device, RenderMesh* _mesh);

	UINT FindNumIndicies(ID3D11Buffer* b);

	static void RenderGrid(ID3D11DeviceContext* _devContext, RenderShape* _model);
	static void RenderModel(ID3D11DeviceContext* _devContext, RenderShape* _model);
};
#endif