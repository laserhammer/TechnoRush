#pragma once
#include <DirectXMath.h>
#include <d3d11.h>

#define DEFAULT_FOV 1.57079633f

class GameEntity;
class Camera
{
public:
	Camera();
	~Camera(void);

	void InitPostProcRenderTarget(ID3D11Device* device);

	void Resize(float aspectRatio, float fov);

	void RenderScene(GameEntity** entities, UINT numEntities, ID3D11RenderTargetView *const *renderTargetViews, UINT numViews, ID3D11DepthStencilView* depthStencilView, ID3D11DeviceContext* deviceContext, DirectX::XMFLOAT4X4& viewData, DirectX::XMFLOAT4X4& projectionData);

	void SetViewParameters(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 lookAt, DirectX::XMFLOAT3 up);

	void SetBackground(GameEntity* background);

	void SetFullScreen(GameEntity* quad, float distance);

	DirectX::XMFLOAT4 RotateToCamera(DirectX::XMFLOAT4 rotateThis);

	DirectX::XMFLOAT4X4 view();
	DirectX::XMFLOAT4X4 projection();
	unsigned int cullingMask();
	void cullingMask(int newMask);
	DirectX::XMFLOAT4 position();
	void position(DirectX::XMFLOAT4 newPosition);

	DirectX::XMFLOAT4 forward();

	bool orthographic();
	void orthographic(bool isOrthographic);
	bool clearDepth();
	void clearDepth(bool isClearDepth);
	bool clearStencil();
	void clearStencil(bool isClearStencil);
	bool clearRenderTarget();
	void clearRenderTarget(bool isClearRenderTarget);
	float aspectRatio();

private:
	void Update();

	void RenderHDR(GameEntity** entities, UINT numEntities);

	DirectX::XMFLOAT4X4 _view;
	DirectX::XMFLOAT4X4 _projection;

	DirectX::XMFLOAT4 _position;
	DirectX::XMFLOAT4 _lookAt;
	DirectX::XMFLOAT4 _up;

	DirectX::XMFLOAT4 _forward;

	float _clearColor[4];
	float _fieldOfView;
	float _near, _far;
	float _width;
	float _aspectRatio;
	bool _orthographic;
	bool _clearDepth;
	bool _clearStencil;
	bool _clearRenderTarget;
	bool _hdr;

	unsigned int _cullingMask;

	GameEntity* _background;

	// Post-Processing

	ID3D11RenderTargetView* _pPRenderTargetView0;
	ID3D11RenderTargetView* _pPRenderTargetView1;
	ID3D11RenderTargetView* _pPRenderTargetView2;
	GameEntity* _pPQuad0;
	GameEntity* _pPQuad1;
	GameEntity* _pPQuad2;
};

