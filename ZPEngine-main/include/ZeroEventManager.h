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
// - UIRenderer���� ���콺 �Է�(Ŭ��, ���� ���� ��..)�� ���������� ó���ϱ� ���� �޴��� �Դϴ�.
// - UIRenderer�� �������ϴ� ������� �־�� ���������� �����մϴ�. (�ڵ����� ZeroRenderManager�� ���ε��� ������� �߰��˴ϴ�.)
// - UIRenderer�� isRayTarget�� true�� �Ǿ� �־�� ������ �����մϴ�. (false�� ������ �������� �ʽ��ϴ�.)
class ZeroEventManager {
public:
	ZeroEventManager();
	~ZeroEventManager();

	// life cycle...
	void Update();
	void EndScene();

	// Ÿ�� ������Ʈ�� �߰��մϴ�.
	void AddTargetObject(ZeroGameObject* obj);
	
	// ���콺�� ��ġ�� ���� hover������Ʈ�� �����մϴ�.
	void CheckHoverMouseRay();

	// ���콺���� Ŭ���� ���θ� ���� hoverObject ����
	void CheckClickMouseRay();

	// ������ ��� ������Ʈ�� �����մϴ�.
	void ClearAllTargetObject();
	

private:
	// ������ Ÿ�� ������Ʈ
	std::vector<RayTarget> targets;

	bool isClicked;
	Vec2 currentCursorPos;
	Vec2 pastCursorPos;
	Vec2 clickedCursorPos;
	Vec2 lastEndCursorPos;

	// ȣ�� ������ ������Ʈ
	int hoverObjectIndex;

	//Ŭ�� ������ ������Ʈ
	int clickedObjectIndex;

	ZeroScene* currentScene;
};
