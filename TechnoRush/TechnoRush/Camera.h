#pragma once
#include <DirectXMath.h>
#include <d3d11.h>

class GameEntity;
class Camera
{
public:
	Camera();
	~Camera(void);

	void Update();
	void Resize(float aspectRatio);
	void RenderScene(GameEntity** entities, int numEntities, ID3D11RenderTargetView* renderTargetView, ID3D11DepthStencilView* depthStencilView, ID3D11DeviceContext* deviceContext);
	
	DirectX::XMFLOAT4X4 view();
	DirectX::XMFLOAT4X4 projection();

	//float fieldOfView();
	//void fieldOfView(float newFieldOfView);

private:
	DirectX::XMFLOAT4X4 _view;
	DirectX::XMFLOAT4X4 _projection;
	DirectX::XMFLOAT4 _position;
	DirectX::XMFLOAT4 _rotation;
	float _clearColor[4];
	float _fieldOfView;
	float _near, _far;
	float _width, _height;
	bool _orthographic;
	

};

