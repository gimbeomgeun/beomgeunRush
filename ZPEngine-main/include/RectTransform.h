#pragma once
#include "ZeroDefine.h"
#include "Transform.h"

namespace ZeroRect {
	typedef enum RectPreset {
		RECT_PIVOT_LEFT			= 1 << 1,
		RECT_PIVOT_RIGHT		= 1 << 2,
		RECT_PIVOT_TOP			= 1 << 3,
		RECT_PIVOT_BOTTOM		= 1 << 4,
		RECT_PIVOT_CENTER		= 1 << 5,
		RECT_ANCHOR_LEFT		= 1 << 6,
		RECT_ANCHOR_RIGHT		= 1 << 7,
		RECT_ANCHOR_VERTICAL	= 1 << 8,
		RECT_ANCHOR_BOTTOM		= 1 << 9,
		RECT_ANCHOR_TOP			= 1 << 10,
		RECT_ANCHOR_HORIZONTAL	= 1 << 11,
		RECT_ANCHOR_CENTER		= 1 << 12,
		RECT_ANCHOR_MAX			= 1 << 13
	} RectPreset;

	typedef enum Axis {
		HORIZONTAL = 0,
		VERTICAL = 1
	} Axis;

	typedef enum Edge {
		LEFT = 1,
		RIGHT = 2,
		TOP = 3,
		BOTTOM = 4
	} Edge;
}

// RectTransform
// - GameObject�� ��ġ, ������ �����ϴ� �Լ�
// - �⺻������ UI�� �����ϴ� ��ü�� ���ԵǾ� �ִ� ������Ʈ�̴�.
// - Rect�� ��ǥ��� �»�ܿ��� ���ϴ����� ����ȴ�...
class Camera2D;

class RectTransform : public Transform {
public:
	RectTransform();
	~RectTransform();

	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void Render() override;

	//Setter
	void SetRect(float x, float y, float width, float height) { rect.left = x, rect.top = y, rect.right = width, rect.bottom = height; };
	void SetAnchorMin(float x, float y) { this->anchorMin.x = x; this->anchorMin.y = y; };
	void SetAnchorMin(Vec2& anchorMin) { this->anchorMin.x = anchorMin.x; this->anchorMin.y = anchorMin.y; };
	void SetAnchorMax(float x, float y) { this->anchorMax.x = x; this->anchorMax.y = y; };
	void SetAnchorMax(Vec2& anchorMax) { this->anchorMax.x = anchorMax.x; this->anchorMax.y = anchorMax.y; };
	void SetOffsetMin(Vec2& value) {
		Vec2 offset = value - (anchoredPosition) - Vec2::Scale(sizeDelta, pivot);
		sizeDelta -= offset;
		anchoredPosition += Vec2::Scale(offset, Vec2::one - pivot);
	}
	void SetOffsetMax(Vec2& value) {
		Vec2 offset = value - (anchoredPosition + Vec2::Scale(sizeDelta, Vec2::one - pivot));
		sizeDelta += offset;
		anchoredPosition += Vec2::Scale(offset, pivot);
	}
	void SetSizeDelta(float x, float y) { sizeDelta.x = x; sizeDelta.y = y; }
	void SetSizeDelta(Vec2& sizeDelta) { this->sizeDelta = sizeDelta; }
	void SetPivot(float x, float y) { this->pivot.x = x; this->pivot.y = y; }
	void SetPivot(Vec2& pivot) { this->pivot = pivot; }
	void SetAnchoredPostion(Vec2& pos) {
		anchoredPosition = pos;
	}
	void SetAnchoredPostion(float x, float y) {
		anchoredPosition.x = x;
		anchoredPosition.y = y;
	}

	//Getter
	Rect& GetRect() { return rect; }
	Vec2& GetAnchorMin() { return anchorMin; }
	Vec2& GetAnchorMax() { return anchorMax; }
	Vec2& GetOffsetMin() { return anchoredPosition - Vec2::Scale(sizeDelta, pivot); }
	Vec2& GetOffsetMax() { return anchoredPosition + Vec2::Scale(sizeDelta, Vec2::one - pivot); }
	Vec2& GetPivot() { return pivot; }
	Vec2& GetAnchoredPosition() { return anchoredPosition; }

	// Pivot�� Anchor�� ���������� �� ���� ������
	void SetRectPreset(ZeroRect::RectPreset pivot, ZeroRect::RectPreset anchor);
	void SetPivotPreset(ZeroRect::RectPreset preset);
	void SetAnchorPreset(ZeroRect::RectPreset preset);
	
	// ���� �Լ�
	void SetSizeWithCurrentAnchors(ZeroRect::Axis axis, float size);
	void SetInsetAndSizeFromParentEdge(ZeroRect::Edge edge, float inset, float size);
	Vec2 GetParentSize();
	Vec2 GetMySize();	// �ڽ��� width, height ��ȯ
	void UpdateRectSize();		// �������� ��Ʈ�� ũ�⸦ ������Ʈ �Ѵ�.
	void UpdateRectPosition();	// �������� ��Ʈ�� ��Ȯ�� ��ġ�� ������Ʈ �Ѵ�.

protected:
	Rect rect; //left -> posX, right -> posY... ������Ʈ�� �߽������� ���� ���� �������� �󸶳� ������ �ִ��� Ȯ��
	
	//anchorMin/Max
	Vec2 anchorMin; //*
	Vec2 anchorMax; //*

	//offsetMin/Max
	Vec2 offsetMin; // Left, Top -> AnchorMin �������� �󸶳� ������ �ִ���
	Vec2 offsetMax;	// Right, Bottom -> AnchorMax �������� �󸶳� ������ �ִ���
	
	Vec2 sizeDelta; //*
	Vec2 pivot; //������Ʈ�� �߽��� *
	Vec2 anchoredPosition; //*
};