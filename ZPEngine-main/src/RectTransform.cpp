#include "RectTransform.h"
#include "ZeroGameObject.h"
#include "ZeroComponent.h"
#include "ZeroMath.h"
#include "ZeroWindow.h"
#include "Camera2D.h"
#include "ZeroSceneMgr.h"
#include "ZeroScene.h"
#include "ZeroSystem.h"

using namespace ZeroRect;

RectTransform::RectTransform()
{
	anchorMin = Vec2::zero;
	anchorMax = Vec2::zero;
	sizeDelta = Vec2(100,100);
	pivot = Vec2::zero;
	anchoredPosition = Vec2::zero;
}

RectTransform::~RectTransform()
{
}

void RectTransform::Start()
{
	Component::Start();

}

void RectTransform::Update()
{
	Component::Update();

	scaleCenter = pivot;
	
}

void RectTransform::LateUpdate()
{
	Component::LateUpdate();

}

void RectTransform::Render()
{
	Component::Render();

	if (!parent) {
		// 1. ��Ʈ�� ũ�⸦ ���Ѵ�
		UpdateRectSize();
		// 2. ��Ʈ�� ���� ��ġ�� ���Ѵ�.
		UpdateRectPosition(); 
		// 3. ���������� ��Ʈ������ ����Ѵ�.
		UpdateTransformation();
	}
}

void RectTransform::SetRectPreset(RectPreset pivot, RectPreset anchor)
{
	SetPivotPreset(pivot);
	SetAnchorPreset(anchor);
}

void RectTransform::SetPivotPreset(RectPreset preset)
{
	Vec2 tmpPivot(0.5f, 0.5f);

	if ((preset & RECT_PIVOT_CENTER) == RECT_PIVOT_CENTER) {
		pivot = tmpPivot;
		return;
	}

	if ((preset & RECT_PIVOT_LEFT) == RECT_PIVOT_LEFT)
		tmpPivot.x -= 0.5f;
	if ((preset & RECT_PIVOT_RIGHT) == RECT_PIVOT_RIGHT)
		tmpPivot.x += 0.5f;
	if ((preset & RECT_PIVOT_TOP) == RECT_PIVOT_TOP)
		tmpPivot.y -= 0.5f;
	if ((preset & RECT_PIVOT_BOTTOM) == RECT_PIVOT_BOTTOM)
		tmpPivot.y += 0.5f;

	pivot = tmpPivot;
}

void RectTransform::SetAnchorPreset(RectPreset preset)
{
	Vec2 tmpAnchorMin(0.5f, 0.5f);
	Vec2 tmpAnchorMax(0.5f, 0.5f);

	if ((preset & RECT_ANCHOR_MAX) == RECT_ANCHOR_MAX) {
		anchorMin.x = 0.0f;
		anchorMin.y = 0.0f;
		anchorMax.x = 1.0f;
		anchorMax.y = 1.0f;
		return;
	}
	if ((preset & RECT_ANCHOR_CENTER) == RECT_ANCHOR_CENTER) {
		anchorMin = tmpAnchorMin;
		anchorMax = tmpAnchorMax;
		return;
	}

	if ((preset & RECT_ANCHOR_LEFT) == RECT_ANCHOR_LEFT) {
		tmpAnchorMin.x -= 0.5f;
		tmpAnchorMax.x -= 0.5f;
	}
	if ((preset & RECT_ANCHOR_RIGHT) == RECT_ANCHOR_RIGHT) {
		tmpAnchorMin.x += 0.5f;
		tmpAnchorMax.x += 0.5f;
	}
	if ((preset & RECT_ANCHOR_HORIZONTAL) == RECT_ANCHOR_HORIZONTAL) {
		tmpAnchorMin.x = 0.0f;
		tmpAnchorMax.x = 1.0f;
	}
	if ((preset & RECT_ANCHOR_TOP) == RECT_ANCHOR_TOP) {
		tmpAnchorMin.y -= 0.5f;
		tmpAnchorMax.y -= 0.5f;
	}
	if ((preset & RECT_ANCHOR_BOTTOM) == RECT_ANCHOR_BOTTOM) {
		tmpAnchorMin.y += 0.5f;
		tmpAnchorMax.y += 0.5f;
	}
	if ((preset & RECT_ANCHOR_VERTICAL) == RECT_ANCHOR_VERTICAL) {
		tmpAnchorMin.y = 0.0f;
		tmpAnchorMax.y = 1.0f;
	}

	anchorMin = tmpAnchorMin;
	anchorMax = tmpAnchorMax;
}

Vec2 RectTransform::GetParentSize() {
	Vec2 size;

	if (parent) {
		//�θ� ��ü�� ������ �θ� ũ�⸦ ������
		size = ((RectTransform*)parent)->GetMySize();
	}
	else {
		//�θ� ������ ȭ�� ũ�⸦ ������
		size.x = ZERO_WINDOW->GetWidth();
		size.y = ZERO_WINDOW->GetHeight();
	}

	return size;
}

Vec2 RectTransform::GetMySize()
{
	return rect.Size();
}

void RectTransform::UpdateRectSize()
{
	//sizeDelta, anchorMin, anchorMax�� �θ��� ũ�⿡�� ���� ũ�⸦ ���Ѵ�. -> rect ��ȯ
	Vec2 tmpParentSizeWithAnchor;
	Vec2 tmpParentSize;

	tmpParentSize = GetParentSize();

	tmpParentSizeWithAnchor.x = (tmpParentSize.x * anchorMax.x) - (tmpParentSize.x * anchorMin.x);
	tmpParentSizeWithAnchor.y = (tmpParentSize.y * anchorMax.y) - (tmpParentSize.y * anchorMin.y);

	rect.right = tmpParentSizeWithAnchor.x + sizeDelta.x;
	rect.bottom = tmpParentSizeWithAnchor.y + sizeDelta.y;

	if (children.size() > 0)
		for (Transform* child : children)
			((RectTransform*)child)->UpdateRectSize();

}

void RectTransform::UpdateRectPosition()
{
	// anchorMin, anchorMax, pivot���� LocalPosition�� ���Ѵ�.
	Vec2 tmpParentSize = GetParentSize(); // -> 0.0f ~ �θ�ũ�����...

	// �θ��� ũ��� anchor ���� ���� ������ ���Ѵ�.
	//	-> anchoredPosition�� �̸� ���ϸ� ��Ȯ�� LocalPosition�� ������!
	//	-> ���� ��Ʈ������ ��ȯ��Ű�� ��! (�Ƹ���?)
	// ex) anchorMin : 0.0f, 0.0f / anchorMax : 1.0f, 1.0f -> tmpParentCenterWithAnchor : tmpParentSize(0 + 1) / 2
	
	Vec2 tmpParentCenterWithAnchor;
	tmpParentCenterWithAnchor.x = tmpParentSize.x * (anchorMin.x + anchorMax.x) / 2.0f;
	tmpParentCenterWithAnchor.y = tmpParentSize.y * (anchorMin.y + anchorMax.y) / 2.0f;
	
	// Pivot�� �ڽ��� ��Ʈ ũ�⸦ ���� ���� ���� ���Ѵ�
	Vec2 tmpOffset;
	Vec2 tmpMySize = GetMySize();
	tmpOffset.x = tmpMySize.x * pivot.x;
	tmpOffset.y = tmpMySize.y * pivot.y;

	// LocalPosition�� ���ѵ� �����ϰ�, �ڽ��� ��ġ�� ������Ʈ �Ѵ�.
	Vec2 tmpPos = tmpParentCenterWithAnchor - tmpOffset + anchoredPosition;

	SetPosition(tmpPos);

	if (children.size() > 0)
		for (Transform* child : children)
			((RectTransform*)child)->UpdateRectPosition();
}