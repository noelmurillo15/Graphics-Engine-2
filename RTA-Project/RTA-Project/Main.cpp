#pragma once

#include "RenderShape.h"
#include "TimerClass.h"

#include <ctime>
#include <dinput.h>
#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")

#define BUFFER_WIDTH	1024
#define BUFFER_HEIGHT	768


class RENDERER {

	//	Application
	HINSTANCE				application;
	WNDPROC					appWndProc;
	HWND					window;

	//	DirectX Devices
	ID3D11Device*			Device = nullptr;
	ID3D11RenderTargetView* RtView = nullptr;
	IDXGISwapChain*			SwapChain = nullptr;
	ID3D11DeviceContext*	DevContext = nullptr;
	ID3D11Buffer*			cbPerObjBuffer = nullptr;

	ID3D11DepthStencilView* DsView = nullptr;
	ID3D11DepthStencilState*pDSState = nullptr;
	ID3D11Texture2D*		depthStencil = nullptr;
	ID3D11SamplerState*		pSamplerState = nullptr;
	ID3D11RasterizerState*	RasterStateFC = nullptr;
	ID3D11BlendState*		textureBlendState = nullptr;
	D3D11_VIEWPORT			Viewport;

	//	Input Data
	IDirectInputDevice8*	DIKeyboard;
	LPDIRECTINPUT8			DirectInput;

	//	Misc
	RenderShape*			grid = nullptr;
	RenderShape*			cube = nullptr;

	TimerClass				timeTracker;

	cbPerObject				cbperobj;
	MATRIX4X4				camView;
	MATRIX4X4				camProj;

public:

	RENDERER(HINSTANCE hinst, WNDPROC proc);

	bool Update();
	bool Render();
	void ShutDown();

	void UpdateCB(MATRIX4X4 world);

	bool InitDirectInput(HINSTANCE hInstance);
	void DetectInput(double time, float w, float h);
};


RENDERER* pApp = nullptr;
D3D_FEATURE_LEVEL features[3];


RENDERER::RENDERER(HINSTANCE hinst, WNDPROC proc) {
	HRESULT result;

#pragma region App & Window
	pApp = this;
	application = hinst;
	appWndProc = proc;

	WNDCLASSEX  wndClass;
	ZeroMemory(&wndClass, sizeof(wndClass));
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.lpfnWndProc = appWndProc;
	wndClass.lpszClassName = L"DirectXApplication";
	wndClass.hInstance = application;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOWFRAME);
	RegisterClassEx(&wndClass);

	RECT window_size = { 0, 0, BUFFER_WIDTH, BUFFER_HEIGHT };
	AdjustWindowRect(&window_size, WS_OVERLAPPEDWINDOW, false);

	window = CreateWindow(L"DirectXApplication", L"Real-Time Animation", WS_OVERLAPPEDWINDOW /*& ~(WS_THICKFRAME | WS_MAXIMIZEBOX)*/,
		CW_USEDEFAULT, CW_USEDEFAULT, window_size.right - window_size.left, window_size.bottom - window_size.top,
		NULL, NULL, application, this);

	ShowWindow(window, SW_SHOW);
#pragma endregion

#pragma region Initialize Tracker
	timeTracker.Initialize();

	if (!InitDirectInput(hinst)) {
		MessageBox(0, L"Direct Input Initialization - Failed",
			L"Error", MB_OK);
		return;
	}
#pragma endregion

#pragma region Device&SwapChain
	D3D_FEATURE_LEVEL pFeatureLevels;
	features[0] = D3D_FEATURE_LEVEL_11_0;
	features[1] = D3D_FEATURE_LEVEL_10_1;
	features[2] = D3D_FEATURE_LEVEL_10_0;

	DXGI_MODE_DESC buffDesc;
	ZeroMemory(&buffDesc, sizeof(DXGI_MODE_DESC));
	buffDesc.Width = BUFFER_WIDTH;
	buffDesc.Height = BUFFER_HEIGHT;
	buffDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	buffDesc.RefreshRate.Denominator = 1;
	buffDesc.RefreshRate.Numerator = 0;

	DXGI_SWAP_CHAIN_DESC swapchainDesc;
	ZeroMemory(&swapchainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapchainDesc.BufferDesc = buffDesc;
	swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchainDesc.BufferCount = 1;
	swapchainDesc.Flags = 0;
	swapchainDesc.SampleDesc.Count = 1;
	swapchainDesc.SampleDesc.Quality = 0;
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL;
	swapchainDesc.OutputWindow = window;
	swapchainDesc.Windowed = TRUE;

	UINT deviceFlags = 0;
#ifdef _DEBUG
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	result = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		deviceFlags,
		features,
		3,
		D3D11_SDK_VERSION,
		&swapchainDesc,
		&SwapChain,
		&Device,
		&pFeatureLevels,
		&DevContext
		);
#pragma endregion

#pragma region Setup_RTV
	ID3D11Resource* pBB;
	SwapChain->GetBuffer(0, __uuidof(pBB),
		reinterpret_cast<void**>(&pBB));
	result = Device->CreateRenderTargetView(pBB, NULL, &RtView);
	pBB->Release();
#pragma endregion

#pragma region Setup ConstBuffer
	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));
	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cbPerObject);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	result = Device->CreateBuffer(&cbbd, NULL, &cbPerObjBuffer);
#pragma endregion

#pragma region Setup DepthStencil
	D3D11_TEXTURE2D_DESC depthBufferdesc;
	ZeroMemory(&depthBufferdesc, sizeof(D3D11_TEXTURE2D_DESC));
	depthBufferdesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferdesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferdesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthBufferdesc.Width = BUFFER_WIDTH;
	depthBufferdesc.Height = BUFFER_HEIGHT;
	depthBufferdesc.MipLevels = 1;
	depthBufferdesc.ArraySize = 1;
	depthBufferdesc.SampleDesc.Count = 1;

	D3D11_DEPTH_STENCIL_DESC dsDesc;
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = Device->CreateDepthStencilState(&dsDesc, &pDSState);	//	Stencil state
	result = Device->CreateTexture2D(&depthBufferdesc, NULL, &depthStencil);	//	Depth Stencil	
	result = Device->CreateDepthStencilView(depthStencil, 0, &DsView);	//	Stencil view

#pragma endregion

#pragma region Setup SamplerState
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MinLOD = (-FLT_MAX);
	samplerDesc.MaxLOD = (FLT_MAX);
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	result = Device->CreateSamplerState(&samplerDesc, &pSamplerState);
#pragma endregion

#pragma region Initialize ViewPort
	ZeroMemory(&Viewport, sizeof(D3D11_VIEWPORT));
	Viewport.Height = BUFFER_HEIGHT;
	Viewport.Width = BUFFER_WIDTH;
	Viewport.MaxDepth = 1.0f;
#pragma endregion

#pragma region Setup_Blendstate
	D3D11_BLEND_DESC blendDesc;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	result = Device->CreateBlendState(&blendDesc, &textureBlendState);
#pragma endregion

#pragma region RasterState
	D3D11_RASTERIZER_DESC frontcullNorm;
	ZeroMemory(&frontcullNorm, sizeof(D3D11_RASTERIZER_DESC));
	frontcullNorm.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	frontcullNorm.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;	//	front cull
	frontcullNorm.DepthClipEnable = TRUE;

	result = Device->CreateRasterizerState(&frontcullNorm, &RasterStateFC);	//	Rasterizer State
#pragma endregion

#pragma region Camera
	Mat_Identity(camView);
	Mat_Identity(camProj);

	FLOAT4 Position = FLOAT4(0.0f, 2.0f, -5.0f, 0.0f);
	FLOAT4 Target = FLOAT4(0.0f, 1.0f, 0.0f, 0.0f);
	FLOAT4 Up = FLOAT4(0.0f, 1.0f, 0.0f, 0.0f);

	camView = CreateViewMatrix(Position, Target, Up);
	camProj = CreateProjectionMatrix(100.0f, 0.1f, 72.0f, (BUFFER_WIDTH / BUFFER_HEIGHT));
#pragma endregion

#pragma region RenderShapes
	grid = new RenderShape;
	cube = new RenderShape;
	grid->InitializeGrid(Device);
	cube->InitializeModel(Device, "Link.obj");

	cube->ScaleModel(FLOAT3(0.1f, 0.1f, 0.1f));
#pragma endregion
}

bool RENDERER::Update() {
	timeTracker.Frame();

	DetectInput(timeTracker.GetTime(), (float)BUFFER_WIDTH, (float)BUFFER_HEIGHT);
	
	return Render();
}

bool RENDERER::Render() {
	HRESULT result = NULL;
	UINT stride = 0, offset = 0;

#pragma region Reset Views
	FLOAT RGBA[4]; RGBA[0] = 0; RGBA[1] = 0; RGBA[2] = 0; RGBA[3] = 1;

	DevContext->ClearDepthStencilView(DsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	DevContext->ClearRenderTargetView(RtView, RGBA);

	DevContext->OMSetDepthStencilState(pDSState, 1);
	DevContext->OMSetRenderTargets(1, &RtView, DsView);
	DevContext->RSSetViewports(1, &Viewport);
#pragma endregion

#pragma region Draw Grid
	UpdateCB(grid->GetWorldMatrix());
	grid->func(DevContext, grid);
#pragma endregion

#pragma region Draw Model
	UpdateCB(cube->GetWorldMatrix());
	cube->func(DevContext, cube);
#pragma endregion

	SwapChain->Present(0, 0);
	return true;
}

void RENDERER::UpdateCB(MATRIX4X4 world) {
	MATRIX4X4 WVP = Mult_4x4(world, camView);
	WVP = Mult_4x4(WVP, camProj);
	cbperobj.World = grid->GetWorldMatrix();
	cbperobj.WVP = WVP;
	DevContext->UpdateSubresource(cbPerObjBuffer, NULL, NULL, &cbperobj, NULL, NULL);
	DevContext->VSSetConstantBuffers(NULL, 1, &cbPerObjBuffer);
}

bool RENDERER::InitDirectInput(HINSTANCE hInstance) {

	HRESULT result;

	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&DirectInput, NULL);

	result = DirectInput->CreateDevice(GUID_SysKeyboard, &DIKeyboard, NULL);
	result = DIKeyboard->SetDataFormat(&c_dfDIKeyboard);
	result = DIKeyboard->SetCooperativeLevel(pApp->window, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	return true;
}

void RENDERER::DetectInput(double time, float w, float h) {

	BYTE keyboardState[256];

	DIKeyboard->Acquire();
	DIKeyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);

	//	Movement amount per frame
	float negMove = -(10.0f * (float)time);
	float posMove = 10.0f * (float)time;

	float negRotate = -(5.0f * (float)time);
	float posRotate = 5.0f * (float)time;

	if (keyboardState[DIK_ESCAPE] & 0x80) {
		PostMessage(pApp->window, WM_DESTROY, 0, 0);
	}

	if (keyboardState[DIK_R] & 0x80) {
		FLOAT4 Position = FLOAT4(0.0f, 2.0f, -5.0f, 0.0f);
		FLOAT4 Target = FLOAT4(0.0f, 1.0f, 0.0f, 0.0f);
		FLOAT4 Up = FLOAT4(0.0f, 1.0f, 0.0f, 0.0f);

		camView = CreateViewMatrix(Position, Target, Up);
	}

#pragma region Camera Movement
	if (keyboardState[DIK_W] & 0x80)
		Translate(camView, 0.0f, 0.0f, negMove);

	if (keyboardState[DIK_S] & 0x80)
		Translate(camView, 0.0f, 0.0f, posMove);

	if (keyboardState[DIK_A] & 0x80)
		Translate(camView, posMove, 0.0f, 0.0f);

	if (keyboardState[DIK_D] & 0x80)
		Translate(camView, negMove, 0.0f, 0.0f);


	if (keyboardState[DIK_F] & 0x80)
		Translate(camView, 0.0f, negMove, 0.0f);

	if (keyboardState[DIK_G] & 0x80)
		Translate(camView, 0.0f, posMove, 0.0f);


	if (keyboardState[DIK_UPARROW] & 0x80)
		RotateX(camView, negRotate);

	if (keyboardState[DIK_DOWNARROW] & 0x80)
		RotateX(camView, posRotate);

	if (keyboardState[DIK_LEFTARROW] & 0x80)
		RotateY(camView, negRotate);

	if (keyboardState[DIK_RIGHTARROW] & 0x80)
		RotateY(camView, posRotate);
#pragma endregion

	return;
}

void RENDERER::ShutDown() {
	//	Destroy all the things!
	Device->Release();
	RtView->Release();
	SwapChain->Release();
	DevContext->Release();
	cbPerObjBuffer->Release();
	RasterStateFC->Release();

	DsView->Release();
	pDSState->Release();
	depthStencil->Release();
	pSamplerState->Release();
	textureBlendState->Release();

	delete grid;
	delete cube;

	UnregisterClass(L"DirectXApplication", application);
}



int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int) {
	srand(unsigned int(time(0)));
	RENDERER myApp(hInstance, (WNDPROC)WndProc);
	MSG msg; ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT && myApp.Update()) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	myApp.ShutDown();
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	if (GetAsyncKeyState(VK_ESCAPE))
		message = WM_DESTROY;

	switch (message) {
	case (WM_DESTROY) : {
		PostQuitMessage(0); 
	}
	case (WM_SIZE) : {	
		if (pApp)
			//pApp->ResizeWin();
		break;
		}
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}