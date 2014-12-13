#pragma once
#include <vector>
#include "GameManager.h"
#include "AtlasMaterial.h"

class GameEntity;
class Camera;
class UiManager
{
public:
	static void InitUi(Camera* uiCamera);
	static void Update(GameState currentState);
	static void DrawUi(ID3D11RenderTargetView *const *renderTargetView, ID3D11DepthStencilView* depthStencilView, ID3D11DeviceContext* deviceContext, DirectX::XMFLOAT4X4& viewData, DirectX::XMFLOAT4X4& projectionData);
	static void SetScore(int score);
	static void Release();

private:
	static int _score;
	static GameEntity* _scoreBack;
	static GameEntity* _title;
	static Atlas _titleData;
	static GameEntity* _paused;
	static Atlas _pausedData;
	static std::vector<GameEntity*>* _scoreNums;
	static std::vector<Atlas*>* _scoreData;
	static GameEntity* _gameOver;
	static Atlas _gameOverData;
	static Camera* _uiCamera;
	

};

