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
	_titleData.displacement = DirectX::XMFLOAT2(0.014f, 0.01f);

	//Paused
	_paused = new GameEntity(AssetLoader::uiMat, AssetLoader::quad, &AssetLoader::vsData);
	_paused->position(_uiCamera->forward());
	_paused->rotation(_uiCamera->RotateToCamera(XMFLOAT4(0.0f, 1.0f, 0.0f, 0.f)));
	_paused->scale(XMFLOAT4(20.0f * 0.6f, 20.0f * 0.075f, 1.0f, 0.0f));
	_pausedData.scale = DirectX::XMFLOAT2(0.6f, 0.075f);
	_pausedData.displacement = DirectX::XMFLOAT2(0.016f, .09f);

	//GameOver
	_gameOver = new GameEntity(AssetLoader::uiMat, AssetLoader::quad, &AssetLoader::vsData);
	_gameOver->position(_uiCamera->forward());
	_gameOver->rotation(_uiCamera->RotateToCamera(XMFLOAT4(0.0f, 1.0f, 0.0f, 0.f)));
	_gameOver->scale(XMFLOAT4(20.0f * 0.6f, 20.0f * 0.075f, 1.0f, 0.0f));
	_gameOverData.scale = DirectX::XMFLOAT2(0.6f, 0.075f);
	_gameOverData.displacement = DirectX::XMFLOAT2(0.01f, 0.17f);

	//Score Background
	_scoreBack = new GameEntity(AssetLoader::scoreBackMat, AssetLoader::quad, &AssetLoader::vsData);
	XMFLOAT4 forward = _uiCamera->forward();
	_scoreBack->position(XMFLOAT4(forward.x - 5, forward.y + 5, forward.z, forward.w));
	_scoreBack->rotation(_uiCamera->RotateToCamera(XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f)));
	_scoreBack->scale(XMFLOAT4(5.0f, 2.0f, 1.0f, 0.0f));

	//score display
	_scoreNums = new std::vector<GameEntity*>();
	_scoreData = new std::vector<Atlas*>();

	//nine quads are made to represent digits
	for (int i = 0; i < 9; i++)
	{
		_scoreNums->push_back(new GameEntity(AssetLoader::uiMat, AssetLoader::quad, &AssetLoader::vsData));
		_scoreData->push_back(new Atlas());
		_scoreNums->at(i)->position(XMFLOAT4((forward.x - 7.0f)+(0.5f*i), forward.y + 4.8f, forward.z, forward.w));
		_scoreNums->at(i)->rotation(_uiCamera->RotateToCamera(XMFLOAT4(0.0f, 1.0f, 0.0f, 0.f)));
		_scoreNums->at(i)->scale(XMFLOAT4(20.0f * 0.022f, 20.0f * 0.075f, 1.0f, 0.0f));
		_scoreData->at(i)->scale = DirectX::XMFLOAT2(0.044f, 0.075f);
		_scoreData->at(i)->displacement = DirectX::XMFLOAT2(0.021f+(0.044*i), .245f);
	}
}
void UiManager::Update(GameState currentState)
{
	switch (currentState)
	{
	case GameState::Play:
		for (int i = 0; i < _scoreNums->size(); i++)
		{
			_scoreNums->at(i)->layer(2);
		}
		_scoreBack->layer(2);
		_title->layer(0);
		_paused->layer(0);
		_gameOver->layer(0);
		break;
	case GameState::Menu:
		for (int i = 0; i < _scoreNums->size(); i++)
		{
			_scoreNums->at(i)->layer(0);
		}
		_scoreBack->layer(0);
		_title->layer(2);
		_paused->layer(0);
		_gameOver->layer(0);
		break;
	case GameState::Pause:
		for (int i = 0; i < _scoreNums->size(); i++)
		{
			_scoreNums->at(i)->layer(2);
		}
		_scoreBack->layer(0);
		_title->layer(0);
		_paused->layer(2);
		_gameOver->layer(0);
		break;
	case GameState::EndGame:
		for (int i = 0; i < _scoreNums->size(); i++)
		{
			_scoreNums->at(i)->layer(2);
		}
		_scoreBack->layer(2);
		_title->layer(0);
		_paused->layer(0);
		_gameOver->layer(2);
		break;
	default:
		break;
	}
	_title->Update(1.0f);
	_paused->Update(1.0f);
	_gameOver->Update(1.0f);
	_scoreBack->Update(1.0f);
	for (int i = 0; i < _scoreNums->size(); i++)
	{
		_scoreNums->at(i)->Update(1.0f);
	}
}
void UiManager::DrawUi(ID3D11RenderTargetView *const *renderTargetView, ID3D11DepthStencilView* depthStencilView, ID3D11DeviceContext* deviceContext, DirectX::XMFLOAT4X4& viewData, DirectX::XMFLOAT4X4& projectionData)
{
	((AtlasMaterial*)(_title->mat()))->SetScale(_titleData.scale);
	((AtlasMaterial*)(_title->mat()))->SetDisplacement(_titleData.displacement);
	((AtlasMaterial*)(_title->mat()))->UpdateBuffer();
	_uiCamera->RenderScene(&_title, 1, renderTargetView, 1, depthStencilView, deviceContext, viewData, projectionData);

	((AtlasMaterial*)(_paused->mat()))->SetScale(_pausedData.scale);
	((AtlasMaterial*)(_paused->mat()))->SetDisplacement(_pausedData.displacement);
	((AtlasMaterial*)(_paused->mat()))->UpdateBuffer();
	_uiCamera->RenderScene(&_paused, 1, renderTargetView, 1, depthStencilView, deviceContext, viewData, projectionData);

	((AtlasMaterial*)(_gameOver->mat()))->SetScale(_gameOverData.scale);
	((AtlasMaterial*)(_gameOver->mat()))->SetDisplacement(_gameOverData.displacement);
	((AtlasMaterial*)(_gameOver->mat()))->UpdateBuffer();
	_uiCamera->RenderScene(&_gameOver, 1, renderTargetView, 1, depthStencilView, deviceContext, viewData, projectionData);
	_uiCamera->RenderScene(&_scoreBack, 1, renderTargetView, 1, depthStencilView, deviceContext, viewData, projectionData);

	
	for (int i = 0; i < _scoreData->size(); i++)
	{
		((AtlasMaterial*)(_scoreNums->at(i)->mat()))->SetScale(_scoreData->at(i)->scale);
		((AtlasMaterial*)(_scoreNums->at(i)->mat()))->SetDisplacement(_scoreData->at(i)->displacement);
		((AtlasMaterial*)(_scoreNums->at(i)->mat()))->UpdateBuffer();
		_uiCamera->RenderScene(&_scoreNums->at(i), 1, renderTargetView, 1, depthStencilView, deviceContext, viewData, projectionData);
	}
}
void UiManager::SetScore(int score)
{
	_score = score;
	int tempScore = _score;
	
	//set the score display

	int num;
	for (int i = 0; i < _scoreData->size(); i++)
	{
		num = (int)(tempScore / pow(10,i)) % 10;
		tempScore -= num * (pow(10,i));
		_scoreData->at((_scoreData->size()-1)-i)->displacement = DirectX::XMFLOAT2(0.021f + (0.0474*num), .245f);
	}
}

void UiManager::Release()
{
	delete _scoreBack;
	delete _title;
	delete _paused;
	delete _gameOver;
	while (_scoreData->size() > 0)
	{
		Atlas* scoreDatum = (*_scoreData)[_scoreData->size() - 1];
		delete(scoreDatum);
		_scoreData->pop_back();
		GameEntity* scoreNum = (*_scoreNums)[_scoreNums->size() - 1];
		delete(scoreNum);
		_scoreNums->pop_back();
	}
	delete _scoreData;
	delete _scoreNums;
}

int UiManager::_score = 0;
GameEntity* UiManager::_scoreBack = NULL;
GameEntity* UiManager::_title = NULL;
Atlas UiManager::_titleData;
GameEntity* UiManager::_paused = NULL;
Atlas UiManager::_pausedData;
std::vector<GameEntity*>* UiManager::_scoreNums = NULL;
std::vector<Atlas*>* UiManager::_scoreData = NULL;
GameEntity* UiManager::_gameOver = NULL;
Atlas UiManager::_gameOverData;
Camera* UiManager::_uiCamera = NULL;


