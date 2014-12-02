
#include "Camera.h"
#include "GameEntity.h"


using namespace DirectX;
Camera::Camera(void)
{
	_fieldOfView = 0.5f * 3.1415926535f;
	_near = 0.1f;
	_far = 100.0f;

	_width = 15.0f;

	_orthographic = false;

	_clearColor[0] = 0.4f;
	_clearColor[1] = 0.6f;
	_clearColor[2] = 0.75f;
	_clearColor[3] = 0.0f;

	_position = XMFLOAT4(0, 0, -5, 0);
	_lookAt = XMFLOAT4(0, 0, 0, 0);
	_up = XMFLOAT4(0, 1, 0, 0);

	_cullingMask = 1;

	_clearDepth = true;
	_clearStencil = true;
	_clearRenderTarget = true;

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

	//Get forward
	XMVECTOR forward = XMVector4Normalize(-position + target);
	XMStoreFloat4(&_forward, forward);
}

void Camera::Resize(float aspectRatio)
{
	_aspectRatio = aspectRatio;
	XMMATRIX P;
	if (!_orthographic)
	{
		P = XMMatrixPerspectiveFovLH(_fieldOfView, _aspectRatio, _near, _far);
	}
	else
	{
		P = XMMatrixOrthographicLH(_width,_width / _aspectRatio, _near, _far);
	}
	XMStoreFloat4x4(&_projection, XMMatrixTranspose(P));
}

void Camera::RenderScene(GameEntity** entities, int numEntities, ID3D11RenderTargetView* renderTargetView, ID3D11DepthStencilView* depthStencilView, ID3D11DeviceContext* deviceContext, XMFLOAT4X4& viewData, XMFLOAT4X4& projectionData)
{
	viewData = _view;
	projectionData = _projection;

	// Clear the buffer
	if (_clearRenderTarget)
		deviceContext->ClearRenderTargetView(renderTargetView, _clearColor);
	unsigned int clearDepth = _clearDepth * D3D11_CLEAR_DEPTH;
	unsigned int clearStencil = _clearStencil * D3D11_CLEAR_STENCIL;
	deviceContext->ClearDepthStencilView(
		depthStencilView,
		clearDepth | clearStencil,
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

void Camera::SetBackground(GameEntity* background)
{
	_background = background;

	//Set the background's position to the far plane
	XMVECTOR pos = XMLoadFloat4(&_position);
	//XMVECTOR lookAt = XMLoadFloat4(&_lookAt);
	XMVECTOR forward = XMLoadFloat4(&_forward);//XMVector4Normalize(-pos + lookAt);
	XMVECTOR newPosVec = pos + (forward *_far * .95);
	XMFLOAT4 newQuadPos;
	XMStoreFloat4(&newQuadPos, newPosVec);
	_background->position(newQuadPos);

	//Rotate the background to face the camera
	//We assume that the quad starts out facing up
	/*
	XMFLOAT4 quadForward = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR quadForWardVec = XMLoadFloat4(&quadForward);
	//Need to rotate the quad's forward to match the opposite of the camera's forward
	//First get the cross product of the two vectors
	XMVECTOR axis = XMVector3Cross(quadForWardVec, -forward);
	//Secondly, find the angle between the two vectors
	XMVECTOR angleVec = XMVector4AngleBetweenVectors(quadForWardVec, -forward);
	float angle;
	XMStoreFloat(&angle, angleVec);
	//Use the axis and the angle to create a rotation quaternion
	XMVECTOR rot = XMQuaternionRotationRollPitchYawFromVector(angle * axis);

	XMFLOAT4 quadRot;
	XMStoreFloat4(&quadRot, rot);
	_background->rotation(quadRot);
	*/
	_background->rotation(RotateToCamera(XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f)));

	//Now scale the background
	//The mesh starts out with a width and height of 1
	//So therefore the scale values should be set to the desired with and height
	//The width and height are based on the current view frustum
	float yScale = 2 * _far * tanf(_fieldOfView / 2);
	float xScale = yScale * _aspectRatio;
	_background->scale(XMFLOAT4(xScale, yScale, 1.0f, 0.0f));
}

XMFLOAT4 Camera::RotateToCamera(DirectX::XMFLOAT4 rotateThis)
{
	XMVECTOR forward = XMLoadFloat4(&_forward);
	//XMFLOAT4 quadForward = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR quadForWardVec = XMLoadFloat4(&rotateThis);
	//Need to rotate the quad's forward to match the opposite of the camera's forward
	//First get the cross product of the two vectors
	XMVECTOR axis = XMVector3Cross(quadForWardVec, -forward);
	//Secondly, find the angle between the two vectors
	XMVECTOR angleVec = XMVector4AngleBetweenVectors(quadForWardVec, -forward);
	float angle;
	XMStoreFloat(&angle, angleVec);
	//Use the axis and the angle to create a rotation quaternion
	XMVECTOR rotVec = XMQuaternionRotationRollPitchYawFromVector(angle * axis);

	XMFLOAT4 rot;
	XMStoreFloat4(&rot, rotVec);
	return rot;
}

XMFLOAT4X4 Camera::view() { return _view; }
XMFLOAT4X4 Camera::projection() { return _projection; }
unsigned int Camera::cullingMask() { return _cullingMask; }
void Camera::cullingMask(int newMask) { _cullingMask = newMask; }
XMFLOAT4 Camera::position() { return _position; }
void Camera::position(DirectX::XMFLOAT4 newPosition) { _position = newPosition; }

XMFLOAT4 Camera::forward() { return _forward; }

bool Camera::orthographic() { return _orthographic; }
void Camera::orthographic(bool isOrthographic) { _orthographic = isOrthographic; }
bool Camera::clearDepth() { return _clearDepth; }
void Camera::clearDepth(bool isClearDepth) { _clearDepth = isClearDepth; }
bool Camera::clearStencil() { return _clearStencil; }
void Camera::clearStencil(bool isClearStencil) { _clearStencil = isClearStencil; }
bool Camera::clearRenderTarget() { return _clearRenderTarget; }
void Camera::clearRenderTarget(bool isClearRenderTarget) { _clearRenderTarget = isClearRenderTarget; }
