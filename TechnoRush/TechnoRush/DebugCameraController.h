/*
Author: Ben Robbins
Loosely based on code from http://msdn.microsoft.com/en-us/library/windows/apps/hh452773.aspx

*/

#pragma once
#include <DirectXMath.h>

#define ROTATION_GAIN 0.5f
#define MOVEMENT_GAIN 5.0f

class Camera;
class DebugCameraController
{
public:
	static void Update(float dt);

	static void OnPointerMoved(DirectX::XMFLOAT2 deltaPosition, float dt);

	static void init(Camera* camera);

	static void SetPosition(DirectX::XMFLOAT3 pos);

	static void SetOrientation(float pitch, float yaw);

	static DirectX::XMFLOAT3 get_Position();

	static DirectX::XMFLOAT3 get_LookPoint();

private:
	// Properties of the controller object
	static DirectX::XMFLOAT3 _position;
	static float _pitch, _yaw;

	static Camera* _camera;

	static DirectX::XMFLOAT3 _moveCommand;

	// States for movement
	static bool _forward, _back;
	static bool _left, _right;
	static bool _up, _down;

};

