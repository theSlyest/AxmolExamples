#include "PentagoBlock.h"

#include "2d/Label.h"
#include "2d/Menu.h"
#include "2d/MenuItem.h"
#include "EventDispatcher.h"
#include "PentagoArrow.h"

USING_NS_AX;

const Color4F PentagoBlock::COLOR(Color4B(0x2b, 0x34, 0x42, 0xff));
const std::string PentagoBlock::TURNED_EVENT = "BlockTurned";

bool PentagoBlock::_IsTurning = false;
bool PentagoBlock::_IsEnabled = false;

bool PentagoBlock::init(const bool arrowsUp)
{
    if (!DrawNode::init())
        return false;

    _arrowsUp = arrowsUp;
    setContentSize(Vec2(SIZE, SIZE));

    drawBackground();
    drawSockets();
    initArrows(arrowsUp);

    return true;
}

bool PentagoBlock::onTouch(const Vec2& location, bool isBlack) const
{
    if (!_IsEnabled)
        return false;

    const Vec2& point = convertToNodeSpace(location);

    for (auto& col : _sockets) {
        for (auto socket : col) {
            if (!socket->isEmpty())
                continue;

            if (socket->getBoundingBox().containsPoint(point)) {
                socket->setBlack(isBlack);
                return true;
            }
        }
    }

    return false;
}

void PentagoBlock::reset() const
{
    for (auto& col : _sockets) {
        for (const auto socket : col) {
            socket->setState(SocketState::EMPTY);
        }
    }
}

PentagoBlock *PentagoBlock::create(const bool arrowsUp)
{
    if (auto* pRet = new (std::nothrow) PentagoBlock(); pRet && pRet->init(arrowsUp))
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

void PentagoBlock::drawBackground()
{
    // Draw rectangle with trimmed angles
    constexpr Vec2 points[8] {
        Vec2(RADIUS, 0.0f),
        Vec2(SIZE - RADIUS, 0.0f),
        Vec2(SIZE, RADIUS),
        Vec2(SIZE, SIZE - RADIUS),
        Vec2(SIZE - RADIUS, SIZE),
        Vec2(RADIUS, SIZE),
        Vec2(0.0f, SIZE - RADIUS),
        Vec2(0.0f, RADIUS)
    };
    drawSolidPoly(points, 8, COLOR);

    // Draw circles over the angle
    constexpr int segments = RADIUS * M_PI;
    drawSolidCircle(Vec2(RADIUS, RADIUS), RADIUS, 0.0f, segments, COLOR);
    drawSolidCircle(Vec2(SIZE - RADIUS, RADIUS), RADIUS, 0.0f, segments, COLOR);
    drawSolidCircle(Vec2(SIZE - RADIUS, SIZE - RADIUS), RADIUS, 0.0f, segments, COLOR);
    drawSolidCircle(Vec2(RADIUS, SIZE - RADIUS), RADIUS, 0.0f, segments, COLOR);
}

void PentagoBlock::drawSockets()
{
    _socketNode = DrawNode::create();
    _socketNode->setContentSize(_contentSize);
    addChild(_socketNode);

    const float yOffset = _arrowsUp ? OFFSET - OFFSET_D : OFFSET + OFFSET_D;
    for (int c = 0; c < 3; ++c) {
        const float x = OFFSET + c * SPACING;
        std::vector<PentagoSocket*> col;
        for (int r = 0; r < 3; ++r) {
            auto socket = PentagoSocket::create();
            socket->setPosition(x, yOffset + r * SPACING);
            _socketNode->addChild(socket);
            col.push_back(socket);
        }
        _sockets.push_back(col);
    }
}

void PentagoBlock::initArrows(const bool arrowsUp)
{
    float y;
    PentagoArrow* arrowLeft;
    PentagoArrow* arrowRight;
    if (arrowsUp) {
        y = SIZE - ARROW_OFFSET;
        arrowLeft = PentagoArrow::create(3);
        arrowRight = PentagoArrow::create(0);
    }
    else {
        y = ARROW_OFFSET;
        arrowLeft = PentagoArrow::create(2);
        arrowRight = PentagoArrow::create(1);
    }

    const Vec2& arrowSize = arrowLeft->getContentSize();
    const auto leftLbl = Label::createWithTTF("   ", "fonts/arial.ttf", 48.0f);
    const auto leftItem = MenuItemLabel::create(leftLbl, [this](Object*) { leftArrowCallback(); });
    leftItem->setContentSize(arrowSize);
    leftLbl->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    leftLbl->setPosition(arrowSize / 2.0f);
    leftItem->addChild(arrowLeft, -1);

    const auto rightLbl = Label::createWithTTF("   ", "fonts/arial.ttf", 48.0f);
    const auto rightItem = MenuItemLabel::create(rightLbl, [this](Object*) { rightArrowCallback(); });
    rightItem->setContentSize(arrowSize);
    rightLbl->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    rightLbl->setPosition(arrowSize / 2.0f);
    rightItem->addChild(arrowRight, -1);

    const auto menu = Menu::create(leftItem, rightItem, nullptr);
    menu->alignItemsHorizontallyWithPadding(ARROW_PADDING);
    menu->setContentSize(Vec2(SIZE, arrowSize.y));
    menu->setPosition(SIZE / 2.0f, y);
    addChild(menu);
}

void PentagoBlock::transpose() const
{
    for (int c = 0; c < 3; ++c) {
        for (int r = 0; r < c; ++r) {
            const auto temp = _sockets[c][r]->getState();
            _sockets[c][r]->setState(_sockets[r][c]->getState());
            _sockets[r][c]->setState(temp);
        }
    }
}

void PentagoBlock::rotateRight() const
{
    transpose();
    for (int c = 0; c < 3; ++c) {
            const auto temp = _sockets[c][0]->getState();
            _sockets[c][0]->setState(_sockets[c][2]->getState());
            _sockets[c][2]->setState(temp);
    }
}

void PentagoBlock::rotateLeft() const
{
    transpose();
    for (int r = 0; r < 3; ++r) {
        const auto temp = _sockets[0][r]->getState();
        _sockets[0][r]->setState(_sockets[2][r]->getState());
        _sockets[2][r]->setState(temp);
    }
}

void PentagoBlock::leftArrowCallback() const
{
    if (!_IsEnabled || !_IsTurning)
        return;

    _arrowsUp ? rotateRight() : rotateLeft();
    dispatchTurnedEvent();
}

void PentagoBlock::rightArrowCallback() const
{
    if (!_IsEnabled || !_IsTurning)
        return;

    _arrowsUp ? rotateLeft() : rotateRight();
    dispatchTurnedEvent();
}

void PentagoBlock::dispatchTurnedEvent() const
{
    _eventDispatcher->dispatchCustomEvent(TURNED_EVENT);
}
