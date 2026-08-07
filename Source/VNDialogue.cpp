#include "VNDialogue.h"

#include "Director.h"
#include "VNButton.h"
#include "VNSeparator.h"
#include "2d/Label.h"

USING_NS_AX;

Color4F VNDialogue::COLOR(0.15f, 0.15f, 0.15f, 1.0f);
Color4F VNDialogue::SEP_COLOR(0.65f, 0.65f, 0.65f, 1.0f);
Vec2 VNDialogue::PADDING(32.0f, 16.0f);

/**
 * Factory method (copied from engine CREATE_FUNC macro)
 */
VNDialogue *VNDialogue::create(const std::string &title, const std::string &body, const ccMenuCallback &callback)
{
    if (auto pRet = new (std::nothrow) VNDialogue(); pRet && pRet->init(title, body, callback))
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
 * Initializer
 */
bool VNDialogue::init(const std::string &title, const std::string &body, const ccMenuCallback &callback)
{
    if (!DrawNode::init())
        return false;

    const auto& size = _director->getVisibleSize();
    setContentSize(Size(size.width - 2.0f * MARGIN, HEIGHT));

    drawBackground();

    // Title label in the top left corner
    const auto titleLbl = Label::createWithTTF(title, "fonts/arial.ttf", TITLE_FONT);
    titleLbl->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    titleLbl->setPosition(PADDING.x, _contentSize.height - PADDING.y);
    addChild(titleLbl);

    // Separator below the title
    const auto separator = VNSeparator::create(_contentSize.width - 2.0f * PADDING.x, 24.0f, SEP_COLOR);
    separator->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    separator->setPosition(PADDING.x, titleLbl->getBoundingBox().getMinY() - PADDING.y / 2.0f);
    addChild(separator);

    // Button in the bottom left corner
    const auto button = VNButton::create("Next", callback);
    button->setPosition(_contentSize.width - PADDING.x - button->getContentSize().width / 2.0f,
                        PADDING.y + button->getContentSize().height / 2.0f);
    addChild(button);

    // Body text below the separator
    const auto bodyLbl = Label::createWithTTF(body, "fonts/arial.ttf", BODY_FONT);
    bodyLbl->setLineHeight(BODY_FONT * BODY_LINE);
    bodyLbl->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    bodyLbl->setPosition(PADDING.x, titleLbl->getBoundingBox().getMinY() - 2.0f * PADDING.y);
    addChild(bodyLbl);

    return true;
}

/**
 * Draw the background rounded rectangle
 */
void VNDialogue::drawBackground()
{
    // First draw a rectangle with trimmed corners
    const Vec2 points[8] {
        Vec2(RADIUS, 0.0f),
        Vec2(_contentSize.width - RADIUS, 0.0f),
        Vec2(_contentSize.width, RADIUS),
        Vec2(_contentSize.width, _contentSize.height - RADIUS),
        Vec2(_contentSize.width - RADIUS, _contentSize.height),
        Vec2(RADIUS, _contentSize.height),
        Vec2(0.0f, _contentSize.height - RADIUS),
        Vec2(0.0f, RADIUS)
    };
    drawSolidPoly(points, 8, COLOR);

    // Then draw 4 circles over the corners
    constexpr int segments = M_PI * RADIUS;
    drawSolidCircle(Vec2(RADIUS, RADIUS), RADIUS, 0.0f, segments, COLOR);
    drawSolidCircle(Vec2(_contentSize.width - RADIUS, RADIUS), RADIUS, 0.0f, segments, COLOR);
    drawSolidCircle(Vec2(_contentSize.width - RADIUS, _contentSize.height - RADIUS), RADIUS, 0.0f, segments, COLOR);
    drawSolidCircle(Vec2(RADIUS, _contentSize.height - RADIUS), RADIUS, 0.0f, segments, COLOR);
}
