#include "Obstacle.h"

USING_NS_AX;

Obstacle* Obstacle::create(const int lane)
{
    if (auto* pRet = new (std::nothrow) Obstacle(); pRet && pRet->init(lane))
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

bool Obstacle::init(const int lane)
{
    const int width = RandomHelper::random_int(MIN_WIDTH, MAX_WIDTH);

    if (!BaseBox::init(BoxType::Obstacle, width, LANE_HEIGHT))
        return false;

    _lane = lane;
    setDiscretePosition(lane > 6 ? GRID_WIDTH : -width, lane + 1);
    _goLeft = (lane > 6);
    const int period = getPeriodFromLane(lane);
    schedule([this](float dt) { move(); }, period * 0.2f, "move");

    return true;
}

int Obstacle::getPeriodFromLane(const int lane)
{
    switch (lane) {
        case 0:
        case 1:
        case 12:
        case 13:
            return 4;
            break;

        case 2:
        case 3:
        case 10:
        case 11:
            return 3;
            break;

        case 4:
        case 5:
        case 8:
        case 9:
            return 2;
            break;

        case 6:
        case 7:
        default:
            return 1;
            break;
    }
}

void Obstacle::move()
{
    setPositionX(_goLeft ? _position.x - BASE_LENGTH : _position.x + BASE_LENGTH); 
    if (getBoundingBox().intersectsRect(ax::Rect(_director->getVisibleOrigin(), _director->getVisibleSize())))
        _director->getEventDispatcher()->dispatchCustomEvent("obstacleMove", this);
    else
        _director->getEventDispatcher()->dispatchCustomEvent("obstacleOut", this);
}

int Obstacle::getLane() const { return _lane; }
