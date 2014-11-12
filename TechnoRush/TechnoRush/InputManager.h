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
	static bool shiftKey;
	static bool ctrlKey;
	static bool rArrowKey;
	static bool lArrowKey;
	static bool uArrowKey;
	static bool dArrowKey;
	static bool mouseDown;
	static bool returnKey;
	static bool escKey;
	static bool pspaceKey;

	// Prev input flags
	static bool pwKey;
	static bool paKey;
	static bool psKey;
	static bool pdKey;
	static bool pshiftKey;
	static bool pctrlKey;
	static bool prArrowKey;
	static bool plArrowKey;
	static bool puArrowKey;
	static bool pdArrowKey;
	static bool pmouseDown;
	static bool preturnKey;
	static bool pescKey;
	static bool spaceKey;

	// Mouse Data
	static DirectX::XMFLOAT2 prevMousePos;
	static DirectX::XMFLOAT2 mousePos;
};

