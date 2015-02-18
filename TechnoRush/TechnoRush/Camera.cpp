
#include "Camera.h"
#include "GameEntity.h"
#include "AssetLoader.h"
#include "HDRMaterial.h"

#define RADIAL_BLUR_COEFF 1000.0f

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

	_hdr = false;

	Update();
}


Camera::~Camera(void)
{
	ReleaseMacro(_pPRenderTargetView0);
	ReleaseMacro(_pPRenderTargetView1);
	ReleaseMacro(_pPRenderTargetView2);
	delete _pPQuad0;
	delete _pPQuad1;
	delete _pPQuad2;
}

void Camera::InitPostProcRenderTarget(ID3D11Device* device)
{
	XMFLOAT4 pos = this->forward();
	pos.y += 0.95f;
	_pPQuad0 = new GameEntity(AssetLoader::hDRMat0, AssetLoader::quad, &AssetLoader::vsData);
	//SetFullScreen(_pPQuad0, _far * 0.95f);
	//_pPQuad0->Update(1.0f);
	_pPQuad0->position(pos);
	_pPQuad0->rotation(this->RotateToCamera(XMFLOAT4(0.0f, 1.0f, 0.0f, 0.f)));
	_pPQuad0->scale(XMFLOAT4(_width, _width / _aspectRatio, 1.0f, 0.0f));
	_pPQuad0->Update(1.0f);

	_pPQuad1 = new GameEntity(AssetLoader::hDRMat1, AssetLoader::quad, &AssetLoader::vsData);
	//SetFullScreen(_pPQuad1, _far * 0.95f);
	//_pPQuad1->Update(1.0f);
	_pPQuad1->position(pos);
	_pPQuad1->rotation(this->RotateToCamera(XMFLOAT4(0.0f, 1.0f, 0.0f, 0.f)));
	_pPQuad1->scale(XMFLOAT4(_width, _width / _aspectRatio, 1.0f, 0.0f));
	_pPQuad1->Update(1.0f);

	_pPQuad2 = new GameEntity(AssetLoader::hDRMat2, AssetLoader::quad, &AssetLoader::vsData);
	//SetFullScreen(_pPQuad2, _far * 0.95f);
	//_pPQuad2->Update(1.0f);
	_pPQuad2->position(pos);
	_pPQuad2->rotation(this->RotateToCamera(XMFLOAT4(0.0f, 1.0f, 0.0f, 0.f)));
	_pPQuad2->scale(XMFLOAT4(_width, _width / _aspectRatio, 1.0f, 0.0f));
	_pPQuad2->Update(1.0f);

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetDesc;
	renderTargetDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	renderTargetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetDesc.Texture2D.MipSlice = 0;

	HR(device->CreateRenderTargetView(AssetLoader::pPTex2D0, &renderTargetDesc, &_pPRenderTargetView0));
	HR(device->CreateRenderTargetView(AssetLoader::pPTex2D1, &renderTargetDesc, &_pPRenderTargetView1));
	HR(device->CreateRenderTargetView(AssetLoader::pPTex2D2, &renderTargetDesc, &_pPRenderTargetView2));

	_hdr = true;
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

void Camera::Resize(float aspectRatio, float fov)
{
	_aspectRatio = aspectRatio;
	_fieldOfView = fov;
	XMMATRIX P;
	if (!_orthographic)
	{
		P = XMMatrixPerspectiveFovLH(fov, _aspectRatio, _near, _far);
	}
	else
	{
		P = XMMatrixOrthographicLH(_width,_width / _aspectRatio, _near, _far);
	}
	XMStoreFloat4x4(&_projection, XMMatrixTranspose(P));
	if (_background)
		SetFullScreen(_background, _far * 0.95f);
}

void Camera::RenderScene(GameEntity** entities, UINT numEntities, ID3D11RenderTargetView *const *renderTargetView, UINT numViews, ID3D11DepthStencilView* depthStencilView, ID3D11DeviceContext* deviceContext, XMFLOAT4X4& viewData, XMFLOAT4X4& projectionData)
{
	viewData = _view;
	projectionData = _projection;

	ID3D11RenderTargetView *const *RTV;
	// Bind the render target and depthstencil buffer
	if (_hdr)
		RTV = &_pPRenderTargetView0;
	else
		RTV = renderTargetView;

	deviceContext->OMSetRenderTargets(numViews, RTV, depthStencilView);

	// Clear the buffer
	for (UINT i = 0; i < numViews; ++i)
	{
		if (_clearRenderTarget)
			deviceContext->ClearRenderTargetView(RTV[i], _clearColor);
	}
	unsigned int clearDepth = _clearDepth * D3D11_CLEAR_DEPTH;
	unsigned int clearStencil = _clearStencil * D3D11_CLEAR_STENCIL;
	deviceContext->ClearDepthStencilView(
		depthStencilView,
		clearDepth | clearStencil,
		1.0f,
		0);

	
	for (UINT i = 0; i < numEntities; ++i)
	{
		if (_cullingMask & entities[i]->layer())	//Check the layer of the object against the cullling mask
			entities[i]->Draw(deviceContext);
	}
	if (_hdr)
	{
		XMStoreFloat4x4(&projectionData, XMMatrixTranspose(XMMatrixOrthographicLH(_width, _width / _aspectRatio, _near, _far)));

		((HDRMaterial*)_pPQuad2->mat())->SetStage(FILTER);
		((HDRMaterial*)_pPQuad2->mat())->UpdateBuffer();

		deviceContext->OMSetRenderTargets(numViews, &_pPRenderTargetView1, NULL);
		deviceContext->ClearDepthStencilView(depthStencilView, 3, 1.0f, 0);
		_pPQuad0->Draw(deviceContext);

		((HDRMaterial*)_pPQuad2->mat())->SetStage(HORIZONTAL_BLUR);
		((HDRMaterial*)_pPQuad2->mat())->UpdateBuffer();
		
		deviceContext->OMSetRenderTargets(numViews, &_pPRenderTargetView2, NULL);
		_pPQuad1->Draw(deviceContext);

		((HDRMaterial*)_pPQuad2->mat())->SetStage(VERTICAL_BLUR);
		((HDRMaterial*)_pPQuad2->mat())->UpdateBuffer();

		ID3D11ShaderResourceView* baseImage = _pPQuad0->mat()->textureView();
		deviceContext->PSSetShaderResources(2, 1, &baseImage);

		deviceContext->OMSetRenderTargets(numViews, &_pPRenderTargetView1, NULL);
		_pPQuad2->Draw(deviceContext);

		((HDRMaterial*)_pPQuad2->mat())->SetStage(MERGE);
		((HDRMaterial*)_pPQuad2->mat())->UpdateBuffer();

		//deviceContext->OMSetRenderTargets(numViews, renderTargetView, NULL);
		deviceContext->OMSetRenderTargets(numViews, &_pPRenderTargetView2, NULL);
		_pPQuad1->Draw(deviceContext);
		
		//Now for the radial blur
		float relativeFOV = abs(_fieldOfView - 1.57079632679f);
		((HDRMaterial*)_pPQuad2->mat())->SetRadialBlur(RADIAL_BLUR_COEFF * relativeFOV * relativeFOV);
		((HDRMaterial*)_pPQuad2->mat())->UpdateBuffer();

		deviceContext->OMSetRenderTargets(numViews, renderTargetView, NULL);
		_pPQuad2->Draw(deviceContext);
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

	SetFullScreen(_background, _far * 0.95f);
}

void Camera::SetFullScreen(GameEntity* quad, float distance)
{
	//Set the background's position to the far plane
	XMVECTOR pos = XMLoadFloat4(&_position);
	XMVECTOR forward = XMLoadFloat4(&_forward);
	XMVECTOR newPosVec = pos + (forward * distance);
	XMFLOAT4 newQuadPos;
	XMStoreFloat4(&newQuadPos, newPosVec);
	quad->position(newQuadPos);

	quad->rotation(RotateToCamera(XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f)));

	//Now scale the background
	//The mesh starts out with a width and height of 1
	//So therefore the scale values should be set to the desired with and height
	//The width and height are based on the current view frustum
	float yScale = 2 * _far * tanf(_fieldOfView / 2.0f);
	float xScale = yScale * _aspectRatio;
	quad->scale(XMFLOAT4(xScale, yScale, 1.0f, 0.0f));
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
float Camera::aspectRatio() { return _aspectRatio; }
