//
// Created by EunwooSong on 2020-12-06.
//

#include "stdafx.h"
#include "ZeroComponentManager.h"

ZeroComponentManager::~ZeroComponentManager() {

}

void ZeroComponentManager::Start() {
    for (auto const& pair : m_ComponentArrays) {
        auto const& components = pair.second;

        for (auto compoIter : components->GetComponentArray()) {
            if (compoIter)
                if (!compoIter->GetIsStarted())
                    if (compoIter->GetActive())
                        compoIter->Start();
        }
    }
}

void ZeroComponentManager::Update() {
    for (auto const& pair : m_ComponentArrays) {
        auto const& components = pair.second;

        auto const& componentArray = components->GetComponentArray();

        for (auto compoIter : componentArray) {
            if (compoIter)
                if (compoIter->GetIsStarted())
                    if (compoIter->GetActive())
                        compoIter->Update();
        }
    }
}

void ZeroComponentManager::LateUpdate() {
    for (auto const& pair : m_ComponentArrays) {
        auto const& components = pair.second;

        auto const& componentArray = components->GetComponentArray();

        for (auto compoIter : componentArray) {
            if (compoIter)
                if (compoIter->GetIsStarted())
                    if (compoIter->GetActive())
                        compoIter->LateUpdate();
        }
    }
}

void ZeroComponentManager::Render() {
    for (auto const& pair : m_ComponentArrays) {
        auto const& components = pair.second;

        auto const& componentArray = components->GetComponentArray();

        for (auto compoIter : componentArray) {
            if (compoIter)
                if (compoIter->GetIsStarted())
                    if (compoIter->GetActive())
                        compoIter->Render();
        }
    }
}

void ZeroComponentManager::EndScene() {
    for (auto const& pair : m_ComponentArrays) {
        auto const& components = pair.second;

        auto const& componentArray = components->GetComponentArray();

        for (auto compoIter : componentArray) {
            if (compoIter)
                if (compoIter->GetIsStarted())
                    if (compoIter->GetActive())
                        compoIter->EndScene();
        }
    }
}

void ZeroComponentManager::FixedUpdate() {
    for (auto const& pair : m_ComponentArrays) {
        auto const& components = pair.second;

        auto const& componentArray = components->GetComponentArray();

        for (auto compoIter : componentArray) {
            if (compoIter)
                if (compoIter->GetIsStarted())
                    if (compoIter->GetActive())
                        compoIter->FixedUpdate();
        }
    }
}

void ZeroComponentManager::Release() {
    for (auto const& pair : m_ComponentArrays) {
        auto components = pair.second;

        components.reset();
    }

    m_ComponentIDs.clear();
    m_ComponentArrays.clear();
}
