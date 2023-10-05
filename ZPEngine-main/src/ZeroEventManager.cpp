#include "ZeroEventManager.h"
#include "ZeroGameObject.h"
#include "UIRenderer.h"
#include "RectTransform.h"
#include "ZeroSystem.h"
#include "ZeroScene.h"

#define NO_TARGET -1

ZeroEventManager::ZeroEventManager()
{
	targets.clear();

	isClicked = false;

	hoverObjectIndex = NO_TARGET;
	clickedObjectIndex = NO_TARGET;
	currentScene = ZERO_SCENE_MGR->GetCurrentScene();

	currentCursorPos = Vec2::zero;
	pastCursorPos = Vec2::zero;
	clickedCursorPos = Vec2::zero;
	lastEndCursorPos = Vec2::zero;
}

ZeroEventManager::~ZeroEventManager()
{
	targets.clear();
}

void ZeroEventManager::Update()
{
	currentCursorPos = ZERO_INPUT_MGR->GetCursorPos();

	CheckHoverMouseRay();
	CheckClickMouseRay();
}

void ZeroEventManager::EndScene()
{
	targets.erase(std::remove_if(targets.begin(), targets.end(),
		[](RayTarget target) {
			return target.gameObject->GetIsDestroy();
		}), targets.end());

	pastCursorPos = currentCursorPos;
}

void ZeroEventManager::AddTargetObject(ZeroGameObject* obj)
{
	RayTarget target;
	target.gameObject = obj;
	target.ray = obj->GetComponent<UIRenderer>();
	target.rect = obj->GetComponent<RectTransform>();

	targets.push_back(target);
}

void ZeroEventManager::CheckHoverMouseRay()
{
	RayTarget target;
	Rect tmpRect;

	for (std::vector<RayTarget>::iterator i = targets.end(); i != targets.begin(); i--) {
		auto index = i - targets.begin() - 1;
		
		target = targets[index];

		if (target.ray->GetIsRayTarget()) {
			tmpRect = target.rect->GetRect();
			tmpRect = tmpRect.offset(target.rect->GetWorldPos());

			if (tmpRect.Contain(currentCursorPos)) {
				hoverObjectIndex = index;
				return;
			}
		}
	}

	hoverObjectIndex = NO_TARGET;
}

void ZeroEventManager::CheckClickMouseRay()
{	
	if (hoverObjectIndex != NO_TARGET) {
		// 마우스가 NONE 상태
		if (ZERO_INPUT_MGR->GetKeyState(INPUT_MOUSE_LBUTTON) == KeyState::NONE ||
			(ZERO_INPUT_MGR->GetKeyState(INPUT_MOUSE_LBUTTON) == KeyState::PRESS && hoverObjectIndex != clickedObjectIndex)) {
			auto iter = currentScene->FindEntityComponents(targets[hoverObjectIndex].gameObject->GetEntityID());

			for (auto i : iter)
				i->OnCursorHover();
		}

		// 마우스가 PRESS 상태
		if (ZERO_INPUT_MGR->GetKeyState(INPUT_MOUSE_LBUTTON) == KeyState::DOWN) {
			auto iter = currentScene->FindEntityComponents(targets[hoverObjectIndex].gameObject->GetEntityID());

			clickedObjectIndex = hoverObjectIndex;

			clickedCursorPos = currentCursorPos;

			for (auto i : iter)
				i->OnCursorEnter();
		}
	}

	if (clickedObjectIndex != NO_TARGET) {
		// 마우스가 DOWN 상태
		if (ZERO_INPUT_MGR->GetKeyState(INPUT_MOUSE_LBUTTON) == KeyState::PRESS) {
			auto iter = currentScene->FindEntityComponents(targets[clickedObjectIndex].gameObject->GetEntityID());

			for (auto i : iter)
				i->OnCursorStay();
		}

		// 마우스가 UP 상태
		if (ZERO_INPUT_MGR->GetKeyState(INPUT_MOUSE_LBUTTON) == KeyState::UP) {
			auto iter = currentScene->FindEntityComponents(targets[clickedObjectIndex].gameObject->GetEntityID());

			lastEndCursorPos = currentCursorPos;
			clickedCursorPos = Vec2::zero;

			clickedObjectIndex = NO_TARGET;

			for (auto i : iter)
				i->OnCursorExit();
		}
	}
}

void ZeroEventManager::ClearAllTargetObject()
{
	targets.clear();
}