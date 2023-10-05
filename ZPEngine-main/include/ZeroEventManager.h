#pragma once

#include "stdafx.h"
#include "ZeroMath.h"

class ZeroGameObject;
class RectTransform;
class UIRenderer;
class ZeroScene;

struct RayTarget {
	ZeroGameObject* gameObject;
	UIRenderer* ray;
	RectTransform* rect;
};

// -= ZeroEventManager =-
// - UIRenderer에서 마우스 입력(클릭, 위에 있음 등..)을 정상적으로 처리하기 위한 메니저 입니다.
// - UIRenderer가 렌더링하는 순서대로 넣어야 순차적으로 감지합니다. (자동으로 ZeroRenderManager에 바인딩한 순서대로 추가됩니다.)
// - UIRenderer의 isRayTarget이 true로 되어 있어야 감지를 진행합니다. (false면 감지를 진행하지 않습니다.)
class ZeroEventManager {
public:
	ZeroEventManager();
	~ZeroEventManager();

	// life cycle...
	void Update();
	void EndScene();

	// 타겟 오브젝트를 추가합니다.
	void AddTargetObject(ZeroGameObject* obj);
	
	// 마우스의 위치에 따른 hover오브젝트를 감지합니다.
	void CheckHoverMouseRay();

	// 마우스에서 클릭된 여부를 통해 hoverObject 관리
	void CheckClickMouseRay();

	// 감지할 모든 오브젝트를 제거합니다.
	void ClearAllTargetObject();
	

private:
	// 감지할 타겟 오브젝트
	std::vector<RayTarget> targets;

	bool isClicked;
	Vec2 currentCursorPos;
	Vec2 pastCursorPos;
	Vec2 clickedCursorPos;
	Vec2 lastEndCursorPos;

	// 호버 상태의 오브젝트
	int hoverObjectIndex;

	//클릭 샅애의 오브젝트
	int clickedObjectIndex;

	ZeroScene* currentScene;
};
