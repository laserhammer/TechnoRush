/*
Author: Ben Robbins
Loosely based on code from http://msdn.microsoft.com/en-us/library/windows/apps/hh452773.aspx

*/

#include "DebugCameraController.h"
#include "InputManager.h"
#include "Camera.h"

using namespace DirectX;

XMFLOAT3 DebugCameraController::_position = XMFLOAT3(0.0f, 0.0f, -5.0f);
float DebugCameraController::_pitch = 0.0f;
float DebugCameraController::_yaw = 0.0f;

Camera* DebugCameraController::_camera = nullptr;

XMFLOAT3 DebugCameraController::_moveCommand = XMFLOAT3(0.0f, 0.0f, 0.0f);

bool DebugCameraController::_forward, DebugCameraController::_back = false;
bool DebugCameraController::_left, DebugCameraController::_right = false;
bool DebugCameraController::_up, DebugCameraController::_down= false;

void DebugCameraController::Update(float dt)
{
	// Listen for updates
	_forward = InputManager::wKey;
	_back = InputManager::sKey;
	_left = InputManager::aKey;
	_right = InputManager::dKey;
	_up = InputManager::shiftKey;
	_down = InputManager::ctrlKey;

	XMFLOAT2 rotation = XMFLOAT2(0.0f, 0.0f);

	rotation.y -= 1.0f * InputManager::uArrowKey;
	rotation.y += 1.0f * InputManager::dArrowKey;
	rotation.x += 1.0f * InputManager::lArrowKey;
	rotation.x -= 1.0f * InputManager::rArrowKey;

	OnPointerMoved(rotation, dt);

	// Poll state bits set by the keyboard event inputs
	_moveCommand.y += 1.0f * _forward;
	_moveCommand.y -= 1.0f * _back;
	_moveCommand.x -= 1.0f * _right;
	_moveCommand.x += 1.0f * _left;
	_moveCommand.z += 1.0f * _up;
	_moveCommand.z -= 1.0f * _down;

	// Make sure that 45 degree cases are not faster
	XMFLOAT3 command = _moveCommand;
	if (command.x > 0.1f || command.y > 0.1f || command.z > 0.1f)
	{
		// NORMALIZE
		float mag = sqrtf(command.x * command.x + command.y * command.y + command.z * command.z);
		command = XMFLOAT3(command.x / mag, command.y / mag, command.z / mag);
	}

	// Rotate command to align with our direction (world coordinates)
	XMFLOAT3 wCommand = XMFLOAT3();
	wCommand.x = command.x*cosf(_yaw) - command.y*sinf(_yaw);
	wCommand.y = command.x*sinf(_yaw) + command.y*cosf(_yaw);
	wCommand.z = command.z;

	// Scale for sensitivity adjustment
	wCommand = XMFLOAT3(wCommand.x * MOVEMENT_GAIN * dt, wCommand.y * MOVEMENT_GAIN * dt, wCommand.z * MOVEMENT_GAIN * dt);

	// Our vleocity is based on the command, also not that y is on the up-down axis
	XMFLOAT3 velocity = XMFLOAT3(-wCommand.x, wCommand.z, wCommand.y);

	// Integrate
	_position = XMFLOAT3(_position.x + velocity.x, _position.y + velocity.y, _position.z + velocity.z);

	// Clear movement input accumulator for use during next frame
	_moveCommand = XMFLOAT3(0.0f, 0.0f, 0.0f);

	_camera->SetViewParameters(_position, get_LookPoint(), XMFLOAT3(0, 1, 0));
}

void DebugCameraController::OnPointerMoved(XMFLOAT2 deltaPosition, float dt)
{
	XMFLOAT2 rotationDelta = XMFLOAT2(deltaPosition.x * ROTATION_GAIN * dt, deltaPosition.y * ROTATION_GAIN * dt);

	_pitch -= rotationDelta.y;
	_yaw -= rotationDelta.x;

	if (_pitch > XM_PI / 2.0f)
		_pitch = XM_PI / 2.0f;
	if (_pitch < -XM_PI / 2.0f)
		_pitch = -XM_PI / 2.0f;
}

void DebugCameraController::init(Camera* camera)
{
	_camera = camera;
}

void DebugCameraController::SetPosition(XMFLOAT3 pos)
{
	_position = pos;
}

void DebugCameraController::SetOrientation(float pitch, float yaw)
{
	_pitch = pitch;
	_yaw = yaw;
}

DirectX::XMFLOAT3 DebugCameraController::get_Position()
{
	return _position;
}

DirectX::XMFLOAT3 DebugCameraController::get_LookPoint()
{
	float y = sinf(_pitch);
	float r = cosf(_pitch);
	float z = r*cosf(_yaw);
	float x = r*sinf(_yaw);

	return XMFLOAT3(_position.x + x, _position.y + y, _position.z + z);
}
