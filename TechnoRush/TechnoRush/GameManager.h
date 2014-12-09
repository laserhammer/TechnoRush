#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>

class Camera;
class GameEntity;
class WorldManager;

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

	void LoadData(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	void Update(float dt);

	Camera* mainCamera();

	void RenderScene(ID3D11RenderTargetView *const *renderTargetView, ID3D11DepthStencilView* depthStencilView, ID3D11DeviceContext* deviceContext);

	void Resize(float aspectRatio);
	GameState getGameState();

private:
	void UpdateFSM();
	void QuitGame();
	void ChangeGamestate(GameState newGameState);
	void ToggleDebug();
	void InitCameras();
	DirectX::XMFLOAT4 GetColorFromSpeed(float speed);

private:
	Camera* _debugCamera;
	Camera* _gameCamera;
	Camera* _uiCamera;
	
	GameState _currentGameState;

	bool _debugActive;
	float _score;
	float _fov;

	WorldManager* _worldManager;
	std::vector<GameEntity*>* _entities;
};