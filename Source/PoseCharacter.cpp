#include "PoseCharacter.h"

USING_NS_AX;

const std::string PoseCharacter::ATLAS_PATH = "skeleton.atlas";
const std::string PoseCharacter::JSON_PATH = "skeleton.json";
const std::string PoseCharacter::POSES[4] = { "idle", "walk", "jump", "wave" };

std::string PoseCharacter::getPoseName(const int idx)
{ 
    if (idx < 0 || idx > 3)
        return {};

    return POSES[idx]; 
}

bool PoseCharacter::init()
{
    if (!Node::init())
        return false;

    initWithJsonFile(JSON_PATH, ATLAS_PATH);
    getState()->getData()->setDefaultMix(TRANSITION);

    _animIdx = -1;

    return true;
}

void PoseCharacter::reset()
{
    setTimeScale(1.0f);
    setEmptyAnimation(0, 0.0f);
}

void PoseCharacter::pose(const int idx)
{
    if (idx < 0 || idx > 3 || idx == _animIdx)
        return;

    setAnimation(0, POSES[idx], true);
    _animIdx = idx;
}

void PoseCharacter::freeze()
{
    setTimeScale(0.0f);
}
