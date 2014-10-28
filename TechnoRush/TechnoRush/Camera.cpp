#include "Camera.h"
#include "GameEntity.h"

using namespace DirectX;
Camera::Camera(void)
{
	_fieldOfView = 0.5f * 3.1415926535f;
	_near = 0.1f;
	_far = 100.0f;

	_width = 800;
	_height = 600;

	_orthographic = true;

	_clearColor[0] = 0.4f;
	_clearColor[1] = 0.6f;
	_clearColor[2] = 0.75f;
	_clearColor[3] = 0.0f;
}


Camera::~Camera(void)
{
}

void Camera::Update()
{
	XMVECTOR position = XMVectorSet(0, 0, -5, 0);
	XMVECTOR target = XMVectorSet(0, 0, 0, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	XMMATRIX V = XMMatrixLookAtLH(position, target, up);
	XMStoreFloat4x4(&_view, XMMatrixTranspose(V));
}

void Camera::Resize(float aspectRatio)
{
	XMMATRIX P;
	if (!_orthographic)
	{
		P = XMMatrixPerspectiveFovLH(_fieldOfView, aspectRatio, _near, _far);
	}
	else
	{
		P = XMMatrixOrthographicLH(_width, _height, _near, _far);
	}
	XMStoreFloat4x4(&_projection, XMMatrixTranspose(P));
}

void Camera::RenderScene(GameEntity** entities, int numEntities, ID3D11RenderTargetView* renderTargetView, ID3D11DepthStencilView* depthStencilView, ID3D11DeviceContext* deviceContext)
{
	// Clear the buffer
	deviceContext->ClearRenderTargetView(renderTargetView, _clearColor);
	deviceContext->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	for(int i = 0; i < numEntities; ++i)
	{
		entities[i]->Draw(deviceContext);
	}
}

XMFLOAT4X4 Camera::view() { return _view; }
XMFLOAT4X4 Camera::projection() { return _projection; }
