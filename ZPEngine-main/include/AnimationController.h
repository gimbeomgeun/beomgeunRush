//
// Created by EunwooSong on 2020-12-26.
//

#ifndef ZERO_ENGINE_ANIMATIONCONTROLLER_H
#define ZERO_ENGINE_ANIMATIONCONTROLLER_H

#include <string>
#include <vector>
#include "ZeroWrapper.h"
#include "ZeroComponent.h"
#include <map>
#include <any>

class Sprite2DRenderer;

//-= SpriteAnimation =-
// - 애니메이션을 쉽게 만들수 있게 도와주는 클래스
// - 주로 AnimationController와 함께 사용하며, 직접 사용하고자 한다면 다른 컴포넌트에서 이 클래스의 Update를 실행해야함
// ex)	auto anim = new SpriteAnimation();			//SpriteAnimation 생성, 초기화(AddTextures, SetFps() . . .) 진행
//		anim->Update();								//SprietAnimation 업데이트 진행
//		SpriteRenderer->SetTexture(anim->GetCurrentFrameTexture());	//객체가 가지고 있는 SpriteRenderer의 텍스쳐를 SpriteAnimtion이 출력하는 텍스쳐를 넣음
class SpriteAnimation {
public:
    SpriteAnimation() : fps(0.0f),
                        textureCount(0),
                        isLoop(true),
                        isLoopEnd(false),
                        isEnd(false),
                        currentFrame(0),
                        timeCheck(0)
    {};
    ~SpriteAnimation() {};

    void Update();

	//Animation에 들어갈 텍스쳐 추가
    void AddTextures(std::string root, int textureCount);
    void AddTexture(const char* path);
	
	//Animation에 들어간 텍스쳐 초기화
    void ResetTexture();

	//Animation의 현재 프레임 초기화
    void RestartAnimation();

	//Animation의 속도(fps) 반환 (fps : 초당 몇개의 이미지를 표시할 것인가)
    float GetFps() { return fps; }

	//Animation에 들어간 텍스쳐의 수 반환
    int GetTextureCount() { return textureCount; }

	//Animation의 반복 실행 여부 반환
    bool GetIsAnimationLoop() { return isLoop; }

	//Animation이 마지막 프레임 출력 여부 반환
    bool GetIsAnimationEnd() { return isLoopEnd || isEnd; }

	//Animation이 현재 몇 번째 텍스쳐(프레임)를 출력하는지를 반환
    float GetCurrentFrame() { return currentFrame; }

	//Animation이 현재 출력하고있는 텍스쳐를 반환
    ZeroTexture* GetCurrentFrameTexture() { return textures[(int)currentFrame]; }

	//Animation의 속도 설정
    void SetFps(float fps) { this->fps = fps; }
    
	//Animation의 반복 실행 여부 반환
	void SetIsLoop(bool isLoop) { this->isLoop = isLoop; }

private:
    std::vector<ZeroTexture*> textures;
    float fps;
    int textureCount;
    bool isLoop;
    bool isLoopEnd;
    bool isEnd;
    float currentFrame;
    float timeCheck;
};

// -= AnimOper =-
// - AnimationController에서 Parameter들의 연산을 진행하여 출력할 Animation의 상태를 정할 때 사용됨
// GREATER	:	A > B
// LESS		:	A < B
// EQUALS	:	A = B
// N_EQUALS :	A ≠ B
enum AnimOper {
    GREATER,
    LESS,
    EQUALS,
    N_EQUALS
};

// -= trigger =-
// - Paramerter중 어떤 trigger가 활성화되면 바로 해당하는 Animation 실행
using trigger = bool;

// -= StateNode =- 
// - AnimationController에서 Parameter 연산을 진행할 때 사용됨
template <typename T>
struct StateNode {
    std::string paramName;
    AnimOper operater;
    T targetValue;

    bool CheckOperater(T paramValue) {
        switch (operater)
        {
            case GREATER:
                return paramValue > targetValue;    break;
            case LESS:
                return paramValue < targetValue;    break;
            case EQUALS:
                return paramValue == targetValue;   break;
            case N_EQUALS:
                return paramValue != targetValue;   break;
            default:
                return false;
                break;
        }
    }
};

// -= StateGroup =-
// - AnimationController에서 Parameter 연산을 진행할 때 사용됨
// - Animation에서 다른 Animation으로 옮길 떄 여러 연산을 진행하기 위함
struct StateGroup {
    bool isHasExitTime;

    std::vector<StateNode<int>>   stateType_int;
    std::vector<StateNode<float>> stateType_float;
    std::vector<StateNode<bool>>  stateType_bool;
};

// -= AnimationNode =-
// - AnimationController에서 넣은 Animation을 관리할 때 사용됨
struct AnimationNode {
    SpriteAnimation* currentAnim;

    std::map<std::string, StateGroup*> stateGroup;
};

// -= AnimationController =-
// - 여러 Animation들을 상태에 따라 쉽게 관리하기 위한 컴포넌트
// - SpriteAnimation과 Parameter, State들을 설정해주면 외부에서 Parameter값들만 넘겨주면 자동으로 Animation들을 관리한다.
//
// - Animation 생성 순서
//  1. SpriteAnimation을 생성한다
//  2. Parameter을 설정한다.
//  3. AnimationNode를 추가한다.
//  4. AddState를 통해 state를 등록한다.
//  5. SetEntryAnimationNode()을 통해 시작 에니메이션을 설정한다
class AnimationController :
        public Component
{
public:
    AnimationController();
    ~AnimationController();

    void Start() override;
    void Update() override;
    void LateUpdate() override;
    void Render() override;

protected:
    //Animation들의 상태를 확인하는 함수
    void CheckAnimationState();

public:
    //현재 출력중인 Animation을 가져오는 함수
    SpriteAnimation* GetCurrentAnimation();

    //현재 출력중인 Animation의 이름을 가져오는함수
    std::string GetCurrentAnimationName();

    //T형 Parameter를 등록하는 함수
    //int, float, bool, trigger를 사용할 수 있다.
    //ex) AnimationController::AddParameter<int>("param_B");
    template<typename T>
    void AddParameter(std::string paramKey);

    //T형 Parameter를 초기값을 가지고 등록하는 함수
    //int, float, bool, trigger를 사용할 수 있다.
    //ex) AnimationController::AddParameter<int>("param_B", 0);
    template<typename T>
    void AddParameter(std::string paramKey, T value);

    // 애니메이션을 AnimationController에 추가하는 함수
    void AddAnimationNode(std::string nodeName, SpriteAnimation* p_anim);

    // 애니메이션이 끝나면 다음 에니메이션으로 넘어갈지 설정하는 함수
    // 기본적으로 true가 들어간다.
    void SetHasExitTime(std::string targetNode, std::string nextNode, bool isHasExitTime);

	// Animation의 상태(관계)를 설정하는 함수
	// ex) anim_A실행 중, param_B가 0보다 크거나 같으면 anim_B 실행
	// AnimationController::AddState("anim_A", "param_B", GREATER, 0, "anim_B");
	// AnimationController::AddState("anim_A", "param_B", EQUALS, 0, "anim_B");
	template<typename T>
    void AddState(std::string targetAnim, std::string paramName, AnimOper oper, T value, std::string nextAnim);

	// 트리거 발동시 실행할 Animation을 설정하는 함수
	// ex) trigger_A 발동되면 anim_A 실행
	// AnimationController::AddTrigger("trigger_A", "anim_A");
    void AddTrigger(std::string paramKey, std::string targetAnim);

    // 처음 시작 Animation 설정하는 함수
    void SetEntryAnimationNode(std::string targetAnim);

    // int형 Parameter를 설정하는 함수
    void SetInt     (std::string paramKey, int      value);
    // float형 Parameter를 설정하는 함수
    void SetFloat   (std::string paramKey, float    value);
    // bool형 Parameter를 설정하는 함수
    void SetBool    (std::string paramKey, bool     value);
    
    // trigger형 Parameter를 활성화하는 함수
    void SetTrigger (std::string paramKey);
    // int형 Parameter의 값을 가져오는 함수
    int   GetParameterInt         (std::string paramKey);
    // float형 Parameter의 값을 가져오는 함수
    float GetParameterFloat       (std::string paramKey);
    // bool형 Parameter의 값을 가져오는 함수
    bool  GetParameterBool        (std::string paramKey);

private:
    void ChangeAnimationNode(std::string nextAnim);

    std::map<std::string, AnimationNode*>::iterator     entry_node;
    std::map<std::string, AnimationNode*>::iterator     current_node;
    std::map<std::string, AnimationNode*>               anim_node;
    std::map<std::string, StateNode<trigger>>           stateType_Trigger;

    std::map<std::string, int>      param_int;
    std::map<std::string, float>    param_float;
    std::map<std::string, bool>     param_bool;
    std::map<std::string, trigger>  param_trigger;

    Sprite2DRenderer* targetRenderer;
};

template<typename T>
inline void AnimationController::AddParameter(std::string paramKey)
{
    const char* tmpName = typeid(T).name();

    if(strcmp("int", tmpName) == 0)
        param_int.insert(std::pair<std::string, int>(paramKey, (int)0));
    else if(strcmp("float", tmpName) == 0)
        param_float.insert(std::pair<std::string, float>(paramKey, (float).0f));
    else if(strcmp("bool", tmpName) == 0)
        param_bool.insert(std::pair<std::string, bool>(paramKey, (bool)false));
    else if(strcmp("trigger", tmpName) == 0)
        param_trigger.insert(std::pair<std::string, trigger>(paramKey, (trigger)false));
    else
        std::cout << "[ ERROR ] Animation Parameter Type Error " << std::endl;
}

template<typename T>
inline void AnimationController::AddParameter(std::string paramKey, T value)
{
    const char* tmpName = typeid(T).name();

    if (strcmp("int", tmpName) == 0)
        param_int.insert(std::pair<std::string, int>(paramKey, (int)value));
    else if (strcmp("float", tmpName) == 0)
        param_float.insert(std::pair<std::string, float>(paramKey, (float)value));
    else if (strcmp("bool", tmpName) == 0)
        param_bool.insert(std::pair<std::string, bool>(paramKey, (bool)value));
    else if (strcmp("trigger", tmpName) == 0)
        param_trigger.insert(std::pair<std::string, trigger>(paramKey, (trigger)value));
    else
        std::cout << "[ ERROR ] Animation Parameter Type Error " << std::endl;
}

template<typename T>
inline void AnimationController::AddState(std::string targetAnim, std::string paramName, AnimOper oper, T value, std::string nextAnim)
{
    bool isError = false;
    if (anim_node.find(targetAnim) == anim_node.end() || anim_node.find(nextAnim) == anim_node.end()) {
        std::cout << "[ ERROR ] Nonexistent Animation identifier " << std::endl;
        return;
    }

    AnimationNode* iter = anim_node[targetAnim];

    const char* tmpName = typeid(T).name();

    if (iter->stateGroup.find(nextAnim) == iter->stateGroup.end())
        SetHasExitTime(targetAnim, nextAnim, true);

    if (strcmp("int", tmpName) == 0) {
        StateNode<int> state;
        state.paramName = paramName;
        state.operater = oper;
        state.targetValue = value;
        iter->stateGroup[nextAnim]->stateType_int.push_back(state);
    }
    else if (strcmp("float", tmpName) == 0) {
        StateNode<float> state;
        state.paramName = paramName;
        state.operater = oper;
        state.targetValue = value;
        iter->stateGroup[nextAnim]->stateType_float.push_back(state);
    }
    else if (strcmp("bool", tmpName) == 0) {
        StateNode<bool> state;
        state.paramName = paramName;
        state.operater = oper;
        state.targetValue = value;
        iter->stateGroup[nextAnim]->stateType_bool.push_back(state);
    }
    else
        std::cout << "[ ERROR ] Animation State Type Error" << std::endl;
}


#endif //ZERO_ENGINE_ANIMATIONCONTROLLER_H
