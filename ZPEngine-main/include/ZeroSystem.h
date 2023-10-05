#ifndef ZERO_ENGINE_ZEROSYSTEM_H
#define ZERO_ENGINE_ZEROSYSTEM_H

#include "ZeroWindow.h"
#include "ZeroInputMgr.h"
#include "ZeroTimeMgr.h"
#include "ZeroResourcesManager.h"
#include "ZeroSoundDevice.h"
#include "ZeroRenderManager.h"
#include "ZeroSceneMgr.h"

#define ZERO_SYSTEM ZeroSystem::Instance()
#define ZERO_WINDOW ZeroSystem::Instance()->GetWindowManager()
#define ZERO_INPUT_MGR ZeroSystem::Instance()->GetInputManager()
#define ZERO_TIME_MGR ZeroSystem::Instance()->GetTimeManager()
#define ZERO_SOUND ZeroSystem::Instance()->GetSoundDeviceManager()
#define ZERO_RESOURCES_MGR ZeroSystem::Instance()->GetResourcesManager()
#define ZERO_RENDERER ZeroSystem::Instance()->GetRenderManager()
#define ZERO_SCENE_MGR ZeroSystem::Instance()->GetSceneManager()


class ZeroSystem {
public:
	ZeroSystem();
	~ZeroSystem();

	static ZeroSystem* Instance();

	void RegisterSystem(std::string appName, int width, int height, bool isFullScreen, float r = 0.0f, float g = 0.0f, float b = 0.0f);
	void InitializeSystem();

	int MainLoop();

	// ������ ���� �޴���
	ZeroWindow* GetWindowManager() { return window; };
	
	// �Է� ���� �޴���
	ZeroInputMgr* GetInputManager() { return input; };
	
	// �ð� ���� �޴���
	ZeroTimeMgr* GetTimeManager() { return time; };
	
	// ���� ���� �޴���
	ZeroSoundDevice* GetSoundDeviceManager() { return sound; };
	
	// ���ҽ� ���� �޴���
	ZeroResourcesManager* GetResourcesManager() { return resource; };

	// ������ ���� �޴���
	ZeroRenderManager* GetRenderManager() { return renderer; };

	// �� ���� �޴���
	ZeroSceneMgr* GetSceneManager() { return scene; }

private:
	void Start();
	void Update();
	void LateUpdate();
	void Render();
	void EndScene();
	void Release();

	void FixedUpdate();

	std::string * appName;
	int width;
	int height;
	bool isFullScreen;

	bool isDebug;

	ZeroWindow*				window;
	ZeroInputMgr*			input;
	ZeroTimeMgr*			time;
	ZeroSoundDevice*		sound;
	ZeroResourcesManager*	resource;
	ZeroRenderManager*		renderer;
	ZeroSceneMgr*			scene;
};

#endif //ZERO_ENGINE_ZEROSYSTEM_H