#pragma once
#include <DirectXMath.h>

static class InputManager
{
public:
	static void Update();

	static void OnMouseDown(int x, int y);
	static void OnMouseUp(int x, int y);
	static void OnMouseMove(int x, int y);
	static DirectX::XMFLOAT2 getDeltaMousePos();

	// Input flags
	static bool wKey;
	static bool aKey;
	static bool sKey;
	static bool dKey;
	static bool rArrowKey;
	static bool lArrowKey;
	static bool uArrowKey;
	static bool dArrowKey;
	static bool mouseDown;

	// Mouse Data
	static DirectX::XMFLOAT2 prevMousePos;
	static DirectX::XMFLOAT2 mousePos;
};

