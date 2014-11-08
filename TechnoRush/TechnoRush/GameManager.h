#pragma once
#include <d3d11.h>
class Camera;
class GameEntity;

typedef enum
{
	Menu,
	Play,
	EndGame,
	Pause
}GameState;

class GameManager 
{
public:
	GameManager();
	~GameManager();

	void Update(float dt);

	Camera* mainCamera();

	void RenderScene(GameEntity** entities, int numEntities, ID3D11RenderTargetView* renderTargetView, ID3D11DepthStencilView* depthStencilView, ID3D11DeviceContext* deviceContext);

private:
	void UpdateFSM();
	void QuitGame();
	void ChangeGamestate(GameState newGameState);

private:
	Camera* _debugCamera;
	Camera* _gameCamera;
	Camera* _uiCamera;

	GameState _currentGameState;
};