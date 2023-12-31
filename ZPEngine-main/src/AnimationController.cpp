//
// Created by EunwooSong on 2020-12-26.
//

#include "stdafx.h"
#include "AnimationController.h"
#include "ZeroGameObject.h"
#include "Sprite2DRenderer.h"
#include "ZeroSystem.h"

//SpriteAnimation
void SpriteAnimation::Update() {
    currentFrame += ZERO_TIME_MGR->GetDeltaTime() * fps;

    if (isLoopEnd)
        isLoopEnd = false;

    if ((int)currentFrame >= textureCount) {
        if (isLoop) {
            isLoopEnd = true;
            currentFrame = 0;
        }
        else {
            currentFrame = textureCount - 1;
            isEnd = true;
        }
    }
}
void SpriteAnimation::AddTextures(std::string root, int textureCount) {
    this->textureCount = textureCount;
    for (int i = 0; i < textureCount; i++) {
        std::string tmp = root + "/" + std::to_string(int(i + 1)) + ".png";
        textures.push_back(ZERO_RESOURCES_MGR->LoadTexture(tmp));
    }
}
void SpriteAnimation::AddTexture(const char* path) {
    textures.push_back(ZERO_RESOURCES_MGR->LoadTexture(path));
    textureCount++;
}
void SpriteAnimation::ResetTexture() {
    textures.resize(0);
    textureCount = 0;
}
void SpriteAnimation::RestartAnimation() {
    currentFrame = 0.0f;
    isEnd = false;
    isLoopEnd = false;
}

AnimationController::AnimationController()
{
    entry_node = anim_node.begin();
}

AnimationController::~AnimationController()
{
    for (auto iter : anim_node) {
        AnimationNode* node = iter.second;

        for (auto gIter : node->stateGroup)
            SAFE_DELETE(gIter.second);

        SAFE_DELETE(node->currentAnim);
        SAFE_DELETE(node);
    }
}

void AnimationController::Start()
{
    Component::Start();
    targetRenderer = GetOwner()->GetComponent<Sprite2DRenderer>();
    current_node = entry_node;
    current_node->second->currentAnim->RestartAnimation();
}

void AnimationController::Update()
{
    Component::Update();
    current_node->second->currentAnim->Update();
}

void AnimationController::LateUpdate()
{
    Component::LateUpdate();
    CheckAnimationState();
}

void AnimationController::Render()
{
    Component::Render();
    targetRenderer->SetTexture(current_node->second->currentAnim->GetCurrentFrameTexture());
}

void AnimationController::CheckAnimationState()
{
    AnimationNode* node = current_node->second;

    if (!node->stateGroup.empty()) {
        for (std::pair<std::string, StateGroup*> iter : node->stateGroup) {
            std::string nextAnim = iter.first;
            StateGroup* group = iter.second;

            bool isCanChange = !group->isHasExitTime;
            if(!isCanChange) isCanChange = node->currentAnim->GetIsAnimationEnd();

            if (isCanChange) {
                bool skipThisNode = false;

                //Check state type int
                for (StateNode<int> state_int : group->stateType_int) {
                    if (!state_int.CheckOperater(GetParameterInt(state_int.paramName))) {
                        skipThisNode = true;
                        break;
                    }
                }
                if (skipThisNode) continue;

                //Check state type float
                for (StateNode<float> state_float : group->stateType_float) {
                    if (!state_float.CheckOperater(GetParameterFloat(state_float.paramName))) {
                        skipThisNode = true;
                        break;
                    }
                }
                if (skipThisNode) continue;

                //Check state type bool
                for (StateNode<bool> state_bool : group->stateType_bool) {
                    if (!state_bool.CheckOperater(GetParameterBool(state_bool.paramName))) {
                        skipThisNode = true;
                        break;
                    }
                }
                if (skipThisNode) continue;

                //Change current node
                ChangeAnimationNode(nextAnim);
                break;
            }
        }
    }
}

SpriteAnimation* AnimationController::GetCurrentAnimation()
{
    return current_node->second->currentAnim;
}

std::string AnimationController::GetCurrentAnimationName() {
    return current_node->first;
}

void AnimationController::AddAnimationNode(std::string nodeName, SpriteAnimation* p_anim)
{
    if (anim_node.find(nodeName) != anim_node.end()) {
        ZERO_LOGGER::Error("Duplicated animation identifier");
        return;
    }

    AnimationNode* iter = new AnimationNode();
    iter->currentAnim = p_anim;

    anim_node.insert(std::pair<std::string, AnimationNode*>(nodeName, iter));
}

void AnimationController::SetHasExitTime(std::string targetNode, std::string nextNode, bool isHasExitTime)
{
    AnimationNode* node = anim_node[targetNode];

    if (node->stateGroup.find(nextNode) == node->stateGroup.end()) {
        StateGroup* iter = new StateGroup();
        iter->isHasExitTime = isHasExitTime;
        node->stateGroup.insert(std::pair<std::string, StateGroup*>(nextNode, iter));
    }
    else {
        StateGroup* iter = node->stateGroup[nextNode];
        iter->isHasExitTime = isHasExitTime;
    }
}

void AnimationController::AddTrigger(std::string paramKey, std::string targetAnim)
{
    if (anim_node.find(targetAnim) == anim_node.end()) {
        ZERO_LOGGER::Error("Nonexistent Animation identifier");
    }

    StateNode<trigger> tmp;

    tmp.paramName = paramKey;
    tmp.operater = EQUALS;
    tmp.targetValue = (trigger)true;

    stateType_Trigger.insert(std::pair<std::string, StateNode<trigger>>(targetAnim, tmp));
}

void AnimationController::SetEntryAnimationNode(std::string targetAnim)
{
    if (anim_node.find(targetAnim) == anim_node.end()) {
        ZERO_LOGGER::Error("Nonexistent Animation identifier");
    }

    entry_node = anim_node.find(targetAnim);
}

void AnimationController::SetInt(std::string paramKey, int value)
{
    if (param_int.find(paramKey) == param_int.end()) {
        ZERO_LOGGER::Error("Nonexistent animation parameter identifier (int)");
        return;
    }

    param_int[paramKey] = value;
}

void AnimationController::SetFloat(std::string paramKey, float value)
{
    if (param_float.find(paramKey) == param_float.end()) {
        ZERO_LOGGER::Error("Nonexistent animation parameter identifier (float)");
        return;
    }

    param_float[paramKey] = value;
}

void AnimationController::SetBool(std::string paramKey, bool value)
{
    if (param_bool.find(paramKey) == param_bool.end()) {
        ZERO_LOGGER::Error("Nonexistent animation parameter identifier (bool)");
        return;
    }

    param_bool[paramKey] = value;
}

void AnimationController::SetTrigger(std::string paramKey)
{
    if (param_trigger.find(paramKey) == param_trigger.end()) {
        ZERO_LOGGER::Error("Nonexistent animation parameter identifier (trigger)");
        return;
    }

    param_trigger[paramKey] = true;
}

int AnimationController::GetParameterInt(std::string paramKey)
{
    if (param_int.find(paramKey) == param_int.end()) {
        ZERO_LOGGER::Error("Nonexistent animation parameter identifier (int)");
        return 0;
    }

    return param_int[paramKey];
}

float AnimationController::GetParameterFloat(std::string paramKey)
{
    if (param_float.find(paramKey) == param_float.end()) {
        ZERO_LOGGER::Error("Nonexistent animation parameter identifier (float)");
        return 0.0f;
    }

    return param_float[paramKey] ;
}

bool AnimationController::GetParameterBool(std::string paramKey)
{
    if (param_bool.find(paramKey) == param_bool.end()) {
        ZERO_LOGGER::Error("Nonexistent animation parameter identifier (bool)");
        return false;
    }

    return param_bool[paramKey];
}

void AnimationController::ChangeAnimationNode(std::string nextAnim)
{
    current_node = anim_node.find(nextAnim);
    current_node->second->currentAnim->RestartAnimation();
}
