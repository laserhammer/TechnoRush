#include "UiManager.h"
#include "AssetLoader.h"
#include "AtlasMaterial.h"
#include "Camera.h"


void UiManager::InitUi(Camera* uiCamera)
{
	_uiCamera = uiCamera;
	//Title
	_title = new GameEntity(AssetLoader::uiMat, AssetLoader::quad, &AssetLoader::vsData);
	_title->position(_uiCamera->forward());
	_title->rotation(_uiCamera->RotateToCamera(XMFLOAT4(0.0f, 1.0f, 0.0f, 0.f)));
	_title->scale(XMFLOAT4(20.0f * 0.6f, 20.0f * 0.075f, 1.0f, 0.0f));
	_titleData.scale = DirectX::XMFLOAT2(0.6f, 0.075f);
	_titleData.displacement = DirectX::XMFLOAT2(0.014f, 0.016f);

	//Paused
	_paused = new GameEntity(AssetLoader::uiMat, AssetLoader::quad, &AssetLoader::vsData);
	_paused->position(_uiCamera->forward());
	_paused->rotation(_uiCamera->RotateToCamera(XMFLOAT4(0.0f, 1.0f, 0.0f, 0.f)));
	_paused->scale(XMFLOAT4(20.0f * 0.6f, 20.0f * 0.075f, 1.0f, 0.0f));
	_pausedData.scale = DirectX::XMFLOAT2(0.6f, 0.075f);
	_pausedData.displacement = DirectX::XMFLOAT2(0.016, .096);
}
void UiManager::Update(GameState currentState)
{
	switch (currentState)
	{
	case GameState::Play:
		_title->layer(0);
		_paused->layer(0);
		break;
	case GameState::Menu:
		_title->layer(2);
		_paused->layer(0);
		break;
	case GameState::Pause:
		_title->layer(0);
		_paused->layer(2);
		break;
	case GameState::EndGame:
		_title->layer(0);
		_paused->layer(0);
		break;
	default:
		break;
	}
	_title->Update(1.0f);
	_paused->Update(1.0f);
}
void UiManager::DrawUi(ID3D11RenderTargetView* renderTargetView, ID3D11DepthStencilView* depthStencilView, ID3D11DeviceContext* deviceContext, DirectX::XMFLOAT4X4& viewData, DirectX::XMFLOAT4X4& projectionData)
{
	((AtlasMaterial*)(_title->mat()))->SetScale(_titleData.scale);
	((AtlasMaterial*)(_title->mat()))->SetDisplacement(_titleData.displacement);
	((AtlasMaterial*)(_title->mat()))->UpdateBuffer();
	_uiCamera->RenderScene(&_title, 1, renderTargetView, depthStencilView, deviceContext, viewData, projectionData);

	((AtlasMaterial*)(_paused->mat()))->SetScale(_pausedData.scale);
	((AtlasMaterial*)(_paused->mat()))->SetDisplacement(_pausedData.displacement);
	((AtlasMaterial*)(_paused->mat()))->UpdateBuffer();
	_uiCamera->RenderScene(&_paused, 1, renderTargetView, depthStencilView, deviceContext, viewData, projectionData);
}
void UiManager::SetScore(int score)
{
	_score = score;
}

void UiManager::Release()
{
	delete _title;
	delete _paused;
}

int UiManager::_score = 0;
GameEntity* UiManager::_title = NULL;
Atlas UiManager::_titleData;
GameEntity* UiManager::_paused = NULL;
Atlas UiManager::_pausedData;
Camera* UiManager::_uiCamera = NULL;
