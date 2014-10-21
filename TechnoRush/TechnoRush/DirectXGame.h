
// This class copied from the DirectX11_Starter
#pragma once

#include "Windows.h"
#include <string>
#include <d3d11.h>
#include <assert.h>

#include "dxerr.h"
#include "GameTimer.h"

#define ReleaseMacro(x) {{if(x) x->Release(); x = 0; } }

#if defined(DEBUG) | defined(_DEBUG)
	#ifndef HR
	#define HR(x)												\
	{															\
		HRESULT hr = (x);										\
		if (FAILED(hr))											\
		{														\
			DXTrace(__FILEW__, (DWORD)__LINE__, hr, L#x, true);	\
			PostQuitMessage(0);									\
		}														\
	}															
	#endif
#else
	#ifndef HR
	#define HR(x) (x)
	#endif
#endif

class DirectXGame
{
public:
	DirectXGame(HINSTANCE hInstance);
	virtual ~DirectXGame(void);

	// Window Aspect ratio
	float AspectRatio()const;

	// Game loop
	int Run();

	virtual bool Init();
	virtual void OnResize();
	virtual void UpdateScene(float dt) = 0;
	virtual void DrawScene() = 0;
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	// Handles window and Direct3D initialization
	bool InitMainWindow();
	bool InitDirect3D();

	void CalculateFrameStats();

protected:

	HINSTANCE hAppInst;
	HWND hMainWnd;

	// Game and window state tracking
	bool	gamePaused;
	bool	minimized;
	bool	maximized;
	bool	resizing;

	// Timer for running game on delta time
	GameTimer timer;

	UINT msaa4xQuality;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	IDXGISwapChain* swapChain;
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11DepthStencilView* depthStencilView;
	D3D11_VIEWPORT viewport;
	D3D_DRIVER_TYPE driverType;
	D3D_FEATURE_LEVEL featureLevel;

	std::wstring windowCaption;
	int windowWidth;
	int windowHeight;
	bool enable4xMsaa;
};

