#pragma once
#include <DirectXMath.h>
#include <d3d11.h>

class GameEntity;
class Camera
{
public:
	Camera();
	~Camera(void);

	void Resize(float aspectRatio);
	void RenderScene(GameEntity** entities, int numEntities, ID3D11RenderTargetView* renderTargetView, ID3D11DepthStencilView* depthStencilView, ID3D11DeviceContext* deviceContext);

	void SetViewParameters(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 lookAt, DirectX::XMFLOAT3 up);

	DirectX::XMFLOAT4X4 view();
	DirectX::XMFLOAT4X4 projection();

private:
	void Update();

	DirectX::XMFLOAT4X4 _view;
	DirectX::XMFLOAT4X4 _projection;

	DirectX::XMFLOAT4 _position;
	DirectX::XMFLOAT4 _lookAt;
	DirectX::XMFLOAT4 _up;

	float _clearColor[4];
	float _fieldOfView;
	float _near, _far;
	float _width, _height;
	bool _orthographic;
	

};

