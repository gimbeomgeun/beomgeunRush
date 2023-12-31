//
// Created by EunwooSong on 2020-12-24.
//

#ifndef ZERO_ENGINE_UIRENDERER_H
#define ZERO_ENGINE_UIRENDERER_H

#include "Sprite2DRenderer.h"

class RectTransform;

// -= UIRenderer =-
// - UI좌표계에 이미지를 출력하는 컴포넌트
// - RectTransform이 필수적으로 사용된다.
// - Sprite2DRenderer와 기능이 같다
class UIRenderer : public Sprite2DRenderer {
public:
    UIRenderer();
    void Start() override;
    void Render() override;

    void SetIsRayTarget(bool isRayTarget) { this->isRayTarget = isRayTarget; }
    bool GetIsRayTarget() { return isRayTarget; }

    // 이미지의 사이즈에 맞춰서 Rect를 설정합니다.
    void SetNagativeSize();

private:
    RectTransform* rectTransform;
    bool isRayTarget;
};


#endif //ZERO_ENGINE_UIRENDERER_H
