#include "RenderContext.h"
#include "RenderShape.h"
#include "RenderMesh.h"

#include "VS_Cube.csh"
#include "PS_Cube.csh"
#include "VS_Grid.csh"
#include "PS_Grid.csh"


RenderContext::RenderContext() {

}

RenderContext::~RenderContext() {	
	if (IndexBuffer != nullptr) IndexBuffer->Release();
	if (VertexBuffer != nullptr) VertexBuffer->Release();
	if (VShader != nullptr) VShader->Release();
	if (PShader != nullptr) PShader->Release();
	if (SRView != nullptr) SRView->Release();
	if (SamplerState != nullptr) SamplerState->Release();
	if (InputLayout != nullptr) InputLayout->Release();
	if (T2DBuffer != nullptr) T2DBuffer->Release();
}

UINT RenderContext::FindNumIndicies(ID3D11Buffer* b) {
	D3D11_BUFFER_DESC ibufferDesc;
	b->GetDesc(&ibufferDesc);

	UINT numIndicies = ibufferDesc.ByteWidth / sizeof(UINT);

	return numIndicies;
}

void RenderContext::InitializeModel(ID3D11Device* _device, RenderMesh* _mesh) {
	HRESULT result = NULL;
	D3D11_BUFFER_DESC buffDesc;
	D3D11_SUBRESOURCE_DATA subData;

	UINT iSize = _mesh->data.indicies.size();
	UINT vSize = _mesh->data.interleaved.size();

	//	Vertex Buffer
	ZeroMemory(&buffDesc, sizeof(D3D11_BUFFER_DESC));
	buffDesc.Usage = D3D11_USAGE_IMMUTABLE;
	buffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffDesc.ByteWidth = sizeof(Vert) * vSize;
	ZeroMemory(&subData, sizeof(D3D11_SUBRESOURCE_DATA));
	subData.pSysMem = &_mesh->data.interleaved[0];
	result = _device->CreateBuffer(&buffDesc, &subData, &VertexBuffer);	//	VBuffer cube

	//	Index Buffer
	ZeroMemory(&buffDesc, sizeof(D3D11_BUFFER_DESC));
	buffDesc.Usage = D3D11_USAGE_DEFAULT;
	buffDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buffDesc.ByteWidth = sizeof(unsigned int) * iSize;
	ZeroMemory(&subData, sizeof(D3D11_SUBRESOURCE_DATA));
	subData.pSysMem = &_mesh->data.indicies[0];
	result = _device->CreateBuffer(&buffDesc, &subData, &IndexBuffer);	//	IBuffer cube

	D3D11_INPUT_ELEMENT_DESC vLayout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	result = _device->CreateInputLayout(vLayout, 4, VS_Cube, sizeof(VS_Cube), &InputLayout);	//	Layout cube
	result = _device->CreateVertexShader(VS_Cube, sizeof(VS_Cube), NULL, &VShader);	//	VShader cube
	result = _device->CreatePixelShader(PS_Cube, sizeof(PS_Cube), NULL, &PShader);		//	PShader cube
}

void RenderContext::InitializeGrid(ID3D11Device* d) {
	HRESULT result;
	SIMPLE_GRID grid[400];
	unsigned int iGrid[80];

	for (int x = 0; x < 20; ++x) {
		for (int y = 0; y < 20; ++y) {
			int index = Convert2D_1D(y, x, 20);
			grid[index].pos = FLOAT4((x / 20.0f), 0.0f, (y / 20.0f), 1);
			grid[index].color = FLOAT4(0, 1, 0, 1);
		}
	}

	//	Rows								//	Columns		         y  x 
	iGrid[0] = Convert2D_1D(0, 0, 20);      iGrid[40] = Convert2D_1D(0, 0, 20);
	iGrid[1] = Convert2D_1D(19, 0, 20);		iGrid[41] = Convert2D_1D(0, 19, 20);
	iGrid[2] = Convert2D_1D(0, 1, 20);		iGrid[42] = Convert2D_1D(1, 0, 20);
	iGrid[3] = Convert2D_1D(19, 1, 20);		iGrid[43] = Convert2D_1D(1, 19, 20);
	iGrid[4] = Convert2D_1D(0, 2, 20);		iGrid[44] = Convert2D_1D(2, 0, 20);
	iGrid[5] = Convert2D_1D(19, 2, 20);		iGrid[45] = Convert2D_1D(2, 19, 20);
	iGrid[6] = Convert2D_1D(0, 3, 20);		iGrid[46] = Convert2D_1D(3, 0, 20);
	iGrid[7] = Convert2D_1D(19, 3, 20);		iGrid[47] = Convert2D_1D(3, 19, 20);
	iGrid[8] = Convert2D_1D(0, 4, 20);		iGrid[48] = Convert2D_1D(4, 0, 20);
	iGrid[9] = Convert2D_1D(19, 4, 20);		iGrid[49] = Convert2D_1D(4, 19, 20);

	iGrid[10] = Convert2D_1D(0, 5, 20);		iGrid[50] = Convert2D_1D(5, 0, 20);
	iGrid[11] = Convert2D_1D(19, 5, 20);	iGrid[51] = Convert2D_1D(5, 19, 20);
	iGrid[12] = Convert2D_1D(0, 6, 20);		iGrid[52] = Convert2D_1D(6, 0, 20);
	iGrid[13] = Convert2D_1D(19, 6, 20);	iGrid[53] = Convert2D_1D(6, 19, 20);
	iGrid[14] = Convert2D_1D(0, 7, 20);		iGrid[54] = Convert2D_1D(7, 0, 20);
	iGrid[15] = Convert2D_1D(19, 7, 20);	iGrid[55] = Convert2D_1D(7, 19, 20);
	iGrid[16] = Convert2D_1D(0, 8, 20);		iGrid[56] = Convert2D_1D(8, 0, 20);
	iGrid[17] = Convert2D_1D(19, 8, 20);	iGrid[57] = Convert2D_1D(8, 19, 20);
	iGrid[18] = Convert2D_1D(0, 9, 20);		iGrid[58] = Convert2D_1D(9, 0, 20);
	iGrid[19] = Convert2D_1D(19, 9, 20);	iGrid[59] = Convert2D_1D(9, 19, 20);

	iGrid[20] = Convert2D_1D(0, 10, 20);	iGrid[60] = Convert2D_1D(10, 0, 20);
	iGrid[21] = Convert2D_1D(19, 10, 20);	iGrid[61] = Convert2D_1D(10, 19, 20);
	iGrid[22] = Convert2D_1D(0, 11, 20);	iGrid[62] = Convert2D_1D(11, 0, 20);
	iGrid[23] = Convert2D_1D(19, 11, 20);	iGrid[63] = Convert2D_1D(11, 19, 20);
	iGrid[24] = Convert2D_1D(0, 12, 20);	iGrid[64] = Convert2D_1D(12, 0, 20);
	iGrid[25] = Convert2D_1D(19, 12, 20);	iGrid[65] = Convert2D_1D(12, 19, 20);
	iGrid[26] = Convert2D_1D(0, 13, 20);	iGrid[66] = Convert2D_1D(13, 0, 20);
	iGrid[27] = Convert2D_1D(19, 13, 20);	iGrid[67] = Convert2D_1D(13, 19, 20);
	iGrid[28] = Convert2D_1D(0, 14, 20);	iGrid[68] = Convert2D_1D(14, 0, 20);
	iGrid[29] = Convert2D_1D(19, 14, 20);	iGrid[69] = Convert2D_1D(14, 19, 20);

	iGrid[30] = Convert2D_1D(0, 15, 20);	iGrid[70] = Convert2D_1D(15, 0, 20);
	iGrid[31] = Convert2D_1D(19, 15, 20);	iGrid[71] = Convert2D_1D(15, 19, 20);
	iGrid[32] = Convert2D_1D(0, 16, 20);	iGrid[72] = Convert2D_1D(16, 0, 20);
	iGrid[33] = Convert2D_1D(19, 16, 20);	iGrid[73] = Convert2D_1D(16, 19, 20);
	iGrid[34] = Convert2D_1D(0, 17, 20);	iGrid[74] = Convert2D_1D(17, 0, 20);
	iGrid[35] = Convert2D_1D(19, 17, 20);	iGrid[75] = Convert2D_1D(17, 19, 20);
	iGrid[36] = Convert2D_1D(0, 18, 20);	iGrid[76] = Convert2D_1D(18, 0, 20);
	iGrid[37] = Convert2D_1D(19, 18, 20);	iGrid[77] = Convert2D_1D(18, 19, 20);
	iGrid[38] = Convert2D_1D(0, 19, 20);	iGrid[78] = Convert2D_1D(19, 0, 20);
	iGrid[39] = Convert2D_1D(19, 19, 20);	iGrid[79] = Convert2D_1D(19, 19, 20);


	D3D11_BUFFER_DESC vbuffDesc_grid;
	ZeroMemory(&vbuffDesc_grid, sizeof(D3D11_BUFFER_DESC));
	vbuffDesc_grid.Usage = D3D11_USAGE_IMMUTABLE;
	vbuffDesc_grid.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbuffDesc_grid.ByteWidth = sizeof(SIMPLE_GRID) * 400;

	D3D11_SUBRESOURCE_DATA vsubData_grid;
	ZeroMemory(&vsubData_grid, sizeof(D3D11_SUBRESOURCE_DATA));
	vsubData_grid.pSysMem = &grid;

	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	D3D11_BUFFER_DESC iBuffdesc_grid;
	ZeroMemory(&iBuffdesc_grid, sizeof(D3D11_BUFFER_DESC));
	iBuffdesc_grid.Usage = D3D11_USAGE_DEFAULT;
	iBuffdesc_grid.BindFlags = D3D11_BIND_INDEX_BUFFER;
	iBuffdesc_grid.ByteWidth = sizeof(unsigned int) * 80;

	D3D11_SUBRESOURCE_DATA isubData_grid;
	ZeroMemory(&isubData_grid, sizeof(D3D11_SUBRESOURCE_DATA));
	isubData_grid.pSysMem = &iGrid;

	result = d->CreateBuffer(&vbuffDesc_grid, &vsubData_grid, &VertexBuffer);	//	VBuffer grid
	result = d->CreateBuffer(&iBuffdesc_grid, &isubData_grid, &IndexBuffer);	//	IBuffer	grid
	result = d->CreateInputLayout(layout, 2, VS_Grid, sizeof(VS_Grid), &InputLayout);	//	Layout grid
	result = d->CreateVertexShader(VS_Grid, sizeof(VS_Grid), NULL, &VShader);	//	VShader grid
	result = d->CreatePixelShader(PS_Grid, sizeof(PS_Grid), NULL, &PShader);		//	PShader grid
#pragma endregion
}

void RenderContext::RenderGrid(ID3D11DeviceContext* _devContext, RenderShape* _model) {
	UINT num = _model->GetModelContext()->FindNumIndicies(_model->GetModelContext()->IndexBuffer);
	UINT strideValue = sizeof(SIMPLE_GRID);
	UINT offset = 0;

	_devContext->IASetVertexBuffers(0, 1, &_model->GetModelContext()->VertexBuffer, &strideValue, &offset);
	_devContext->IASetIndexBuffer(_model->GetModelContext()->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	_devContext->VSSetShader(_model->GetModelContext()->VShader, NULL, 0);
	_devContext->PSSetShader(_model->GetModelContext()->PShader, NULL, 0);
	_devContext->IASetInputLayout(_model->GetModelContext()->InputLayout);
	_devContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
	_devContext->DrawIndexed(num, 0, 0);
}

void RenderContext::RenderModel(ID3D11DeviceContext* _devContext, RenderShape* shape) {
	UINT num = shape->GetModelContext()->FindNumIndicies(shape->GetModelContext()->IndexBuffer);
	UINT strideValue = sizeof(Vert);
	UINT offset = 0;

	_devContext->IASetVertexBuffers(0, 1, &shape->GetModelContext()->VertexBuffer, &strideValue, &offset);
	_devContext->IASetIndexBuffer(shape->GetModelContext()->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	_devContext->IASetInputLayout(shape->GetModelContext()->InputLayout);
	_devContext->VSSetShader(shape->GetModelContext()->VShader, NULL, 0);
	_devContext->PSSetShader(shape->GetModelContext()->PShader, NULL, 0);
	_devContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_devContext->Draw(num, 0);
}