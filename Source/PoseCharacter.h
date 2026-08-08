#pragma once

#include "spine/spine-axmol.h"

class PoseCharacter : public spine::SkeletonAnimation
{
public:
    CREATE_FUNC(PoseCharacter);

    static std::string getPoseName(int idx);

    bool init() override;

    void reset();

    void pose(int idx);

    void freeze();

private:
    static constexpr float TRANSITION = 0.2f;
    static const std::string ATLAS_PATH;
    static const std::string JSON_PATH;
    static const std::string POSES[4];

    int _animIdx = 0;
};
