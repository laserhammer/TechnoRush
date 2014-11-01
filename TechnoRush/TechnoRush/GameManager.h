#pragma once
class Camera;

class GameManager 
{
public:
	GameManager();
	~GameManager();

	void Update(float dt);

	Camera* mainCamera();

private:
	Camera* _camera;
};