
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

	_orthographic = false;

	_clearColor[0] = 0.4f;
	_clearColor[1] = 0.6f;
	_clearColor[2] = 0.75f;
	_clearColor[3] = 0.0f;

	_position = XMFLOAT4(0, 0, -5, 0);
	_lookAt = XMFLOAT4(0, 0, 0, 0);
	_up = XMFLOAT4(0, 1, 0, 0);

	_cullingMask = 1;

	Update();
}


Camera::~Camera(void)
{
}

void Camera::Update()
{
	XMVECTOR position = XMLoadFloat4(&_position);
	XMVECTOR target = XMLoadFloat4(&_lookAt);
	XMVECTOR up = XMLoadFloat4(&_up);
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
		if (_cullingMask & entities[i]->layer())	//Check the layer of the object against the cullling mask
			entities[i]->Draw(deviceContext);
	}
}

void Camera::SetViewParameters(XMFLOAT3 position, XMFLOAT3 lookAt, XMFLOAT3 up)
{
	_position = XMFLOAT4(position.x, position.y, position.z, 0);
	_lookAt = XMFLOAT4(lookAt.x, lookAt.y, lookAt.z, 0);
	_up = XMFLOAT4(up.x, up.y, up.z, 0);
	Update();
}


XMFLOAT4X4 Camera::view() { return _view; }
XMFLOAT4X4 Camera::projection() { return _projection; }
unsigned int Camera::cullingMask() { return _cullingMask; }
void Camera::cullingMask(int newMask) { _cullingMask = newMask; }
