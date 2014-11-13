/*
Author: Ben Robbins
Loosely based on code from http://msdn.microsoft.com/en-us/library/windows/apps/hh452773.aspx

*/

#pragma once
#include <DirectXMath.h>

#define ROTATION_GAIN 0.0006f
#define MOVEMENT_GAIN 0.01f

class Camera;
static class DebugCameraController
{
public:
	static void Update();

	static void OnPointerMoved(DirectX::XMFLOAT2 deltaPosition);

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

