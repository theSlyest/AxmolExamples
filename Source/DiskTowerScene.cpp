#include "DiskTowerScene.h"
#include "RoundedRect.h"

USING_NS_AX;

Scene* DiskTowerScene::createScene()
{
    return DiskTowerScene::create();
}

// on "init" you need to initialize your instance
bool DiskTowerScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto bg = LayerColor::create(Color4B(20, 24, 31, 255));
    addChild(bg);

    auto title = Label::createWithTTF("DISK TOWER", "fonts/arial.ttf", 24.0f);
    title->setTextColor(Color4B(255, 194, 74, 255));
    title->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    title->setPosition(Vec2(origin.x + 24.0f, origin.y + visibleSize.height - 24.0f));
    this->addChild(title, 1);

    auto help = Label::createWithTTF("Click a peg to lift its top disk, then a peg to drop it. R to restart", "fonts/arial.ttf", 18.0f);
    help->setTextColor(Color4B(255, 255, 255, 255));
    help->setPosition(Vec2(origin.x + visibleSize.width / 2.0f, origin.y + 32.0f));
    this->addChild(help, 1);

    initMoves(visibleSize, origin);
    initBase(visibleSize, origin);
    initPegs(visibleSize, origin);
    initPanel(visibleSize, origin);
    initDisks();
    initKeyboardListener();
    initMouseListener();

    resetDisks();

    return true;
}

void DiskTowerScene::initMoves(const Size& visibleSize, const Vec2& origin)
{
    _moves = 0;
    _movesLabel = Label::createWithTTF("Moves: 0", "fonts/arial.ttf", 24.0f);
    _movesLabel->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    _movesLabel->setPosition(Vec2(origin.x + visibleSize.width - 24.0f, origin.y + visibleSize.height - 24.0f));
    addChild(_movesLabel, 1);
}

void DiskTowerScene::initBase(const Size &visibleSize, const Vec2 &origin)
{
    auto base = RoundedRect::create(visibleSize.width - 128.0f, 32.0f, 8.0f, Color4F(0.42f, 0.31f, 0.18f, 1.0f));
    base->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    base->setPosition(Vec2(origin.x + 64.0f, origin.y + BaseHeight));
    addChild(base);
}

void DiskTowerScene::initPegs(const Size &visibleSize, const Vec2 &origin)
{
    float pegSpacing = visibleSize.width / 4.0f;
    for (int i = 0; i < 3; ++i)
    {
        auto peg = Peg::create();
        peg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        peg->setPosition(Vec2(origin.x + (i + 1) * pegSpacing, origin.y + 128.0f));
        addChild(peg);
        _pegs.push_back(peg);
    }
}

void DiskTowerScene::initDisks()
{
    for (int i = 0; i < 5; ++i) {
        auto disk = Disk::create(i + 1);
        disk->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        addChild(disk);
        _disks.push_back(disk);
    }
}

void DiskTowerScene::initKeyboardListener()
{
    _keyboardListener = EventListenerKeyboard::create();
    _keyboardListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event) {
        if (keyCode == EventKeyboard::KeyCode::KEY_R) {
            restart();
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_keyboardListener, this);
}

void DiskTowerScene::initMouseListener()
{
    _mouseListener = EventListenerMouse::create();
    _mouseListener->onMouseDown = [this](Event* event) { return onMouseDown(event); };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);
    _pegLifted = -1; // No peg lifted initially
}

void DiskTowerScene::initPanel(const Size &visibleSize, const Vec2 &origin)
{
    _panel = RoundedRect::create(visibleSize.width - 256.0f, visibleSize.height - 192.0f, 16.0f, Color4F(0.42f, 0.51f, 0.18f, 1.0f));
    _panel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _panel->setPosition(Vec2(origin.x + visibleSize.width / 2.0f, origin.y + visibleSize.height / 2.0f));
    _panel->setVisible(false);
    addChild(_panel, 1);

    Size panelSize = _panel->getContentSize();

    auto solved = Label::createWithTTF("SOLVED", "fonts/arial.ttf", 48.0f);
    solved->setPosition(Vec2(panelSize.width / 2.0f, panelSize.height / 2.0f + 64.0f));
    _panel->addChild(solved);

    _finalMovesLabel = Label::createWithTTF("Moves: 0", "fonts/arial.ttf", 36.0f);
    _finalMovesLabel->setPosition(Vec2(panelSize.width / 2.0f, panelSize.height / 2.0f));
    _panel->addChild(_finalMovesLabel);

    auto restart = Label::createWithTTF("Press R to restart", "fonts/arial.ttf", 18.0f);
    restart->setPosition(Vec2(panelSize.width / 2.0f, panelSize.height / 2.0f - 64.0f));
    _panel->addChild(restart);
}

bool DiskTowerScene::onMouseDown(Event * event)
{
    const auto* e = dynamic_cast<EventMouse*>(event);
    if (e->getMouseButton() != EventMouse::MouseButton::BUTTON_LEFT)
        return false;

    const Vec2& clickPos = e->getLocation();
    for (int i = 0; i < _pegs.size(); ++i) {
        if (const auto& peg = _pegs[i]; peg->getBoundingBox().containsPoint(clickPos)) {
            if (_pegLifted == -1) {
                if (peg->getTopDisk() == 0) {
                    // No disk to lift from this peg
                    return true;
                }

                _disks[peg->getTopDisk() - 1]->setPosition(Vec2(peg->getPositionX(), TopHeight)); // Lift the top disk from this peg
                _pegLifted = i; // Store the index of the lifted peg
            }
            else {
                const unsigned char liftedDiskIndex = _pegs[_pegLifted]->getTopDisk() - 1;
                const auto liftedDisk = _disks[liftedDiskIndex];
                if (i == _pegLifted) {
                    positionDiskOnPeg(i, liftedDisk->getSize()); // Return the disk to its original peg
                    _pegLifted = -1; // No peg lifted
                }
                else if (addToPeg(i, liftedDisk->getSize())) {
                    _pegs[_pegLifted]->removeTopDisk(); // Remove the disk from the original peg
                    _pegLifted = -1; // No peg lifted
                    setMoves(_moves + 1);                    

                    if (isGameWon()) {
                        showWinMessage();
                    }
                }
            }
            return true;
        }
    }
    return false;
}

void DiskTowerScene::setMoves(const int moves)
{
    _moves = moves;
    _movesLabel->setString("Moves: " + std::to_string(_moves));
}

void DiskTowerScene::restart()
{
    _panel->setVisible(false);
    _mouseListener->setEnabled(true);
    setMoves(0);
    resetDisks();
}

void DiskTowerScene::resetDisks()
{
    for (const auto& peg : _pegs) {
        peg->clearDisks();
    }

    for (int i = _disks.size(); i > 0; --i) {
        addToPeg(0, _disks[i - 1]->getSize());
    }
}

bool DiskTowerScene::addToPeg(int pegIndex, unsigned char diskSize)
{
    if (pegIndex < 0 || pegIndex >= _pegs.size()) {
        return false;
    }

    auto& peg = _pegs[pegIndex];
    if (!peg->isEmpty() && diskSize >= peg->getTopDisk()) {
        return false;
    }

    peg->addDisk(diskSize);
    positionDiskOnPeg(pegIndex, diskSize);
    return true;
}

void DiskTowerScene::positionDiskOnPeg(const int pegIndex, const unsigned char diskSize) const
{
    auto& peg = _pegs[pegIndex];
    _disks[diskSize - 1]->setPosition(Vec2(peg->getPositionX(), BaseHeight + (peg->countDisks() - 1) * Disk::Height));
}

bool DiskTowerScene::isGameWon() const
{
    return _pegs.back()->countDisks() == _disks.size();
}

void DiskTowerScene::showWinMessage() const
{
    _finalMovesLabel->setString("Moves: " + std::to_string(_moves));
    _panel->setVisible(true);
    _mouseListener->setEnabled(false);
}