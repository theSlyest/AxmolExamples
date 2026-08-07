#include "VNSeparator.h"

USING_NS_AX;

/**
 * Factory method (copied from engine CREATE_FUNC macro)
 */
VNSeparator *VNSeparator::create(const float width, const float height, const ax::Color4F &color)
{
    if (auto pRet = new (std::nothrow) VNSeparator(); pRet && pRet->init(width, height, color))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

/**
 * Initializer method
 * @param width Separator width
 * @param height Separator height
 * @param color Separator color
 */
bool VNSeparator::init(const float width, const float height, const ax::Color4F &color)
{
    if (!DrawNode::init())
        return false;

    setContentSize(Vec2(width, height));

    // Center large diamond shape
    const float midHeight = height / 2.0f;
    const float lSide = 0.75f * height;
    const Vec2 lPoints[4] {
        Vec2(0.0f, lSide / 2.0f),
        Vec2(lSide / 2.0f, 0.0f),
        Vec2(lSide, lSide / 2.0f),
        Vec2(lSide / 2.0f, lSide)
    };
    const auto lShape = DrawNode::create();
    lShape->drawPoly(lPoints, 4, true, color, height / 8.0f);
    lShape->setContentSize(Vec2(lSide, lSide));
    lShape->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lShape->setPosition(width / 2.0f, midHeight);
    addChild(lShape);

    // Medium diamond shapes parameters
    const float mSide = height / 3.0f;
    const Vec2 mSize (mSide, mSide);
    const Vec2 mPoints[4] {
        Vec2(0.0f, mSide / 2.0f),
        Vec2(mSide / 2.0f, 0.0f),
        Vec2(mSide, mSide / 2.0f),
        Vec2(mSide / 2.0f, mSide)
    };

    // Medium diamond at the left of the big one
    const auto mShapeLeft = DrawNode::create();
    mShapeLeft->drawSolidPoly(mPoints, 4, color);
    mShapeLeft->setContentSize(mSize);
    mShapeLeft->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    mShapeLeft->setPosition((width - height) / 2.0f, midHeight);
    addChild(mShapeLeft);

    // Medium diamond at the right of the big one
    const auto mShapeRight = DrawNode::create();
    mShapeRight->drawSolidPoly(mPoints, 4, color);
    mShapeRight->setContentSize(mSize);
    mShapeRight->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    mShapeRight->setPosition((width + height) / 2.0f, midHeight);
    addChild(mShapeRight);

    // Small diamond shapes parameters
    const float sSide = height / 4.0f;
    const Vec2 sSize(sSide, sSide);
    const Vec2 sPoints[4] {
        Vec2(0.0f, sSide / 2.0f),
        Vec2(sSide / 2.0f, 0.0f),
        Vec2(sSide, sSide / 2.0f),
        Vec2(sSide / 2.0f, sSide)
    };

    // Small diamond shape at the left end
    const auto sShapeLeft = DrawNode::create();
    sShapeLeft->drawSolidPoly(sPoints, 4, color);
    sShapeLeft->setContentSize(sSize);
    sShapeLeft->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    sShapeLeft->setPosition(0.0f, midHeight);
    addChild(sShapeLeft);

    // Small diamond shape at the right end
    const auto sShapeRight = DrawNode::create();
    sShapeRight->drawSolidPoly(sPoints, 4, color);
    sShapeRight->setContentSize(sSize);
    sShapeRight->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    sShapeRight->setPosition(width, midHeight);
    addChild(sShapeRight);

    // Arm size
    const Size aSize(mShapeLeft->getBoundingBox().getMinX() - sShapeLeft->getBoundingBox().getMaxX(), sSide);

    // Left arm
    const Vec2 leftPoints[4] {
        Vec2(0.0f, aSize.height / 2.0f),
        Vec2(aSize.width - aSize.height / 2.0f, 0.0f),
        Vec2(aSize.width, aSize.height / 2.0f),
        Vec2(aSize.width - aSize.height / 2.0f, aSize.height)
    };
    const auto leftArm = DrawNode::create();
    leftArm->drawSolidPoly(leftPoints, 4, color);
    leftArm->setContentSize(aSize);
    leftArm->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    leftArm->setPosition(mShapeLeft->getBoundingBox().getMinX(), midHeight);
    addChild(leftArm);

    // Right arm
    const Vec2 rightPoints[4] {
        Vec2(0.0f, aSize.height / 2.0f),
        Vec2(aSize.height / 2.0f, 0.0f),
        Vec2(aSize.width, aSize.height / 2.0f),
        Vec2(aSize.height / 2.0f, aSize.height)
    };
    const auto rightArm = DrawNode::create();
    rightArm->drawSolidPoly(rightPoints, 4, color);
    rightArm->setContentSize(aSize);
    rightArm->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    rightArm->setPosition(mShapeRight->getBoundingBox().getMaxX(), midHeight);
    addChild(rightArm);

    return true;
}
