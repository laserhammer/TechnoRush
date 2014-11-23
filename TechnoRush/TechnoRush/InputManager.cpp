

#include "InputManager.h"
#include "Windows.h"

// Input flags
bool InputManager::wKey = false;
bool InputManager::aKey = false;
bool InputManager::sKey = false;
bool InputManager::dKey = false;
bool InputManager::shiftKey = false;
bool InputManager::ctrlKey = false;
bool InputManager::rArrowKey = false;
bool InputManager::lArrowKey = false;
bool InputManager::uArrowKey = false;
bool InputManager::dArrowKey = false;
bool InputManager::mouseDown = false;
bool InputManager::returnKey = false;
bool InputManager::escKey = false;
bool InputManager::pspaceKey = false;

bool InputManager::pwKey = false;
bool InputManager::paKey = false;
bool InputManager::psKey = false;
bool InputManager::pdKey = false;
bool InputManager::pshiftKey = false;
bool InputManager::pctrlKey = false;
bool InputManager::prArrowKey = false;
bool InputManager::plArrowKey = false;
bool InputManager::puArrowKey = false;
bool InputManager::pdArrowKey = false;
bool InputManager::pmouseDown = false;
bool InputManager::preturnKey = false;
bool InputManager::pescKey = false;
bool InputManager::spaceKey = false;

// Mouse Data
DirectX::XMFLOAT2 InputManager::prevMousePos = DirectX::XMFLOAT2(0.0f, 0.0f);
DirectX::XMFLOAT2 InputManager::mousePos = DirectX::XMFLOAT2(0.0f, 0.0f);

void InputManager::OnMouseDown(int x, int y)
{
	prevMousePos.x = mousePos.x;
	prevMousePos.y = mousePos.y;
	mousePos.x = (float)x;
	mousePos.y = (float)y;
	mouseDown = true;
}

void InputManager::OnMouseUp(int x, int y)
{
	mouseDown = false;
}

void InputManager::OnMouseMove(int x, int y)
{
	prevMousePos.x = mousePos.x;
	prevMousePos.y = mousePos.y;
	mousePos.x = (float)x;
	mousePos.y = (float)y;
}

DirectX::XMFLOAT2 InputManager::getDeltaMousePos()
{
	return DirectX::XMFLOAT2(mousePos.x - prevMousePos.x, mousePos.y - prevMousePos.y);
}

void InputManager::Update()
{
	//Check ALL the key states
	pwKey = wKey;
	paKey = aKey;
	psKey = sKey;
	pdKey = dKey;
	pshiftKey = shiftKey;
	pctrlKey = ctrlKey;
	puArrowKey = uArrowKey;
	pdArrowKey = dArrowKey;
	plArrowKey = lArrowKey;
	prArrowKey = rArrowKey;
	preturnKey = returnKey;
	pescKey = escKey;
	pspaceKey = spaceKey;

	wKey = GetKeyState(0x57) & 0x80;
	aKey = GetKeyState(0x41) & 0x80;
	sKey = GetKeyState(0x53) & 0x80;
	dKey = GetKeyState(0x44) & 0x80;
	shiftKey = GetKeyState(0x10) & 0x80;
	ctrlKey = GetKeyState(0x11) & 0x80;
	uArrowKey = GetKeyState(0x26) & 0x80;
	dArrowKey = GetKeyState(0x28) & 0x80;
	lArrowKey = GetKeyState(0x25) & 0x80;
	rArrowKey = GetKeyState(0x27) & 0x80;
	returnKey = GetKeyState(0X0D) & 0X80;
	escKey = GetKeyState(0x1B) & 0X80;
	spaceKey = GetKeyState(0x20) & 0x80;
}
