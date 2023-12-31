//
// Created by joonl on 2020-12-25.
//

#ifndef ZERO_ENGINE_BUTTON_H
#define ZERO_ENGINE_BUTTON_H

#include <string>
#include <functional>
#include "ZeroComponent.h"
#include "ZeroDefine.h"

// -= Button =-
// - 기본적으로 버튼의 형태를 구현한 컴포넌트
// - UIRenderer 컴포넌트를 추가해서 사용 (필수)
// - 이런 컴포넌트는 만들어서 사용하세요...ㅎㅎ
class Button : public Component {
public:
    Button() :
            isOnCursor(false),
            isOnClick(false),
            isEndCursor(false),
            isEnableAnimation(true),
            boundaryRect(0, 0, 0, 0),
            upScale(1.1f, 1.1f),
            defScale(1.0f, 1.0f),
            upSpeed(5.0f) {}
    ~Button();

    void Start() override;

    void Update() override;

    //버튼의 텍스쳐를 설정하는 함수
    void SetTextureImage(std::string path);

    //버튼의 경계 크기를 가져오는 함수
    Rect GetBoundaryRect() { return boundaryRect; }

    //버튼의 경계 크기를 설정하는 함수
    void SetBoundaryRect(Rect boundaryRect) { this->boundaryRect = boundaryRect; }

    //커서가 버튼 위에 있는지 확인하는 함수
    bool GetIsOnCursor() { return isOnCursor; }

    //커서가 버튼에서 눌렸는지 확인하는 함수
    bool GetIsOnClick() { return isOnClick; }

	//커서가 버튼을 나갔는지 확인하는 함수
    bool GetIsEndCursor() { return isEndCursor; }

	//버튼에 확대 애니메이션을 추가하는지 확인하는 함수
    bool GetIsEnableAnimation() { return isEnableAnimation; }

	//확대 에니메이션의 확대 비율을 반환하는 함수
    Vec2 GetUpScale() { return upScale; }

	//확대 애니메이션의 기본 비율을 반환하는 함수
    Vec2 GetDefScale() { return defScale; }

	//확대 에니메이션의 확대 속도를 반환하는 함수
    float GetUpSpeed() { return upSpeed; }

	//확대 에니메이션의 적용 여부를 설정하는 함수
    void SetIsEnableAnimation(bool enableAnimation) { isEnableAnimation = enableAnimation; }

	//확대 에니메이션의 확대 비율을 설정하는 함수
    void SetUpScale(Vec2 scale) { upScale = scale; }

	//확대 에니메이션의 기본 비율을 설정하는 함수
    void SetDefScale(Vec2 scale) { defScale = scale; }

	//확대 에니메이션의 확대 속도를 설정하는 함수
    void SetUpSpeed(float speed) { upSpeed = speed; }

	//버튼이 클릭되면 발생시킬 함수를 추가하는 함수
	//추가된 모든 함수들을 실행시킨다
    void AddOnClickListener(std::function<void()> onClick) { this->onClick.push_back(std::move(onClick)); }

	//추가된 모든 함수들을 지운다
    void ResetListener() { onClick.clear(); }

	//버튼의 활성화 여부 설정
    void SetActive(bool active) override;

private:
    bool isOnCursor;
    bool isOnClick;
    bool isEndCursor;

    bool isEnableAnimation;

    Rect boundaryRect;

    Vec2 upScale;
    Vec2 defScale;
    float upSpeed;

    std::vector<std::function<void()>> onClick;
};


#endif //ZERO_ENGINE_BUTTON_H
