#pragma once

#include "GridCell.h"

// A cell that can hold a mirror. It shows the mirror it is told to show.
class Mirror : public GridCell
{
public:
    static constexpr float MIRROR_SIZE = 40.0f;
    static const ax::Color4F MIRROR_COLOR;

    bool init() override;

    CREATE_FUNC(Mirror);

    // Show a mirror running from the top left to the bottom right ("\") or from
    // the bottom left to the top right ("/"), or no mirror at all.
    void setMirror(bool present, bool backslash) const;

private:
    static constexpr float WIDTH = 6.0f;

    ax::DrawNode* _mirrorNode = nullptr;
};
