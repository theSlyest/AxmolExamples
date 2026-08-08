#include "PoseMatchScene.h"

USING_NS_AX;

// on "init" you need to initialize your instance
bool PoseMatchScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
        return false;

    const Size& size = _director->getVisibleSize();
    const Vec2& origin = _director->getVisibleOrigin();

    const auto background = LayerColor::create(Color4B(0x0c, 0x10, 0x18, 255));
    addChild(background);

    const auto ground = LayerColor::create(Color4B(0x24, 0x30, 0x48, 255), size.width, GROUND_HEIGHT);
    ground->setPosition(origin.x, origin.y + GROUND_Y - GROUND_HEIGHT);
    addChild(ground);

    // Score label
    _scoreLbl = Label::createWithTTF("Score: 0", "fonts/arial.ttf", 24.0f);
    _scoreLbl->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _scoreLbl->setPosition(origin.x + UI_OFFSET, origin.y + size.height - UI_OFFSET);
    addChild(_scoreLbl);

    // Time label
    _timeLbl = Label::createWithTTF("Time: 45", "fonts/arial.ttf", 24.0f);
    _timeLbl->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    _timeLbl->setPosition(origin.x + size.width - UI_OFFSET, origin.y + size.height - UI_OFFSET);
    addChild(_timeLbl);

    const float centerX = origin.x + size.width / 2.0f;

    // Banner frame
    const auto frame = DrawNode::create();
    frame->setContentSize(BANNER_SIZE);
    frame->drawRect(Vec2::ZERO, BANNER_SIZE, Color4F(BANNER_COLOR), FRAME_WIDTH);
    frame->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    frame->setPosition(centerX, origin.y + size.height - UI_OFFSET);
    addChild(frame);

    // Pose label
    _poseLbl = Label::createWithTTF("WALK", "fonts/arial.ttf", 36.0f);
    _poseLbl->setHorizontalAlignment(TextHAlignment::CENTER);
    _poseLbl->setTextColor(BANNER_COLOR);
    _poseLbl->setPosition(centerX,  frame->getBoundingBox().getMidY());
    addChild(_poseLbl);

    // Help label
    const auto help = Label::createWithTTF("MATCH THE POSE", "fonts/arial.ttf", 24.0f);
    help->setPosition(centerX, _poseLbl->getPositionY() - BANNER_SIZE.height);
    addChild(help);

    // Prompt
    _promptLbl = Label::createWithTTF("Press the matching key to begin", "fonts/arial.ttf", 22.0f);
    _promptLbl->setPosition(centerX, help->getPositionY() - 48.0f);
    addChild(_promptLbl);

    // Legend
    const auto legend = Label::createWithTTF("1 IDLE         2 WALK         3 JUMP         4 WAVE", "fonts/arial.ttf", 22.0f);
    legend->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    legend->setPosition(centerX, origin.y + UI_OFFSET);
    addChild(legend);

    // Character
    _character = PoseCharacter::create();
    _character->setPosition(centerX, origin.y + GROUND_Y - 6.0f);
    addChild(_character);

    initEndLayer(size);

    const auto keyListener = EventListenerKeyboard::create();
    keyListener->onKeyPressed = [this](const EventKeyboard::KeyCode key, Event *) { onKeyPressed(key); };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

    reset();

    return true;
}

void PoseMatchScene::initEndLayer(const Size& size)
{
    _endLayer = LayerColor::create(Color4B(0, 0, 0, 192));
    _endLayer->setVisible(false);
    addChild(_endLayer, 1);

    const float midX = size.width / 2.0f;
    const float midY = size.height / 2.0f;

    const auto overLbl = Label::createWithTTF("ROUND OVER", "fonts/arial.ttf", 36.0f);
    overLbl->setPosition(midX, midY + 36.0f);
    _endLayer->addChild(overLbl);

    _endScoreLbl = Label::createWithTTF("Score: 0", "fonts/arial.ttf", 24.0f);
    _endScoreLbl->setPosition(midX, midY);
    _endLayer->addChild(_endScoreLbl);

    const auto restartLbl = Label::createWithTTF("Press R to restart", "fonts/arial.ttf", 22.0f);
    restartLbl->setPosition(midX, midY - 36.0f);
    _endLayer->addChild(restartLbl);
}

void PoseMatchScene::reset()
{
    unschedule(TIMER_KEY);
    setScore(0);
    setTime(START_TIME);
    setPose(START_POSE);
    _promptLbl->setVisible(true);
    _character->reset();
    _endLayer->setVisible(false);
}

void PoseMatchScene::gameOver()
{
    unschedule(TIMER_KEY);
    _character->freeze();
    _endScoreLbl->setString(_scoreLbl->getString());
    _endLayer->setVisible(true);
}

void PoseMatchScene::setScore(const int score)
{
    _score = score;
    _scoreLbl->setString("Score: " + std::to_string(score));
}

void PoseMatchScene::setTime(const int time)
{
    _time = time;
    _timeLbl->setString(StringUtils::format("Time: %02d", time));
}

void PoseMatchScene::updateTime()
{
    setTime(_time - 1);
    if (_time == 0)
        gameOver();
}

void PoseMatchScene::nextPose(const int exclude)
{
    int next = random(0, 2);
    if (next >= exclude)
        ++next;
    
    setPose(next);
}

void PoseMatchScene::setPose(const int pose)
{
    _pose = pose;
    std::string name = PoseCharacter::getPoseName(pose);
    for (char &c : name)
        c = static_cast<char>(std::toupper(c));
    
    _poseLbl->setString(name);
}

void PoseMatchScene::onKeyPressed(EventKeyboard::KeyCode key)
{
    switch (key)
    {
    case EventKeyboard::KeyCode::KEY_1:
    case EventKeyboard::KeyCode::KEY_2:
    case EventKeyboard::KeyCode::KEY_3:
    case EventKeyboard::KeyCode::KEY_4: {
        const int input = static_cast<int>(key) - static_cast<int>(EventKeyboard::KeyCode::KEY_1);
        if (_promptLbl->isVisible()) {
            _promptLbl->setVisible(false);
            schedule([this](float dt) { updateTime(); }, 1.0f, TIMER_KEY);
        }

        if (input == _pose)
            setScore(_score + 1);
        
        _character->pose(input);
        nextPose(input);
        break;
    }
    case EventKeyboard::KeyCode::KEY_R:
    case EventKeyboard::KeyCode::KEY_CAPITAL_R:
        reset();
        break;

    default:
        break;
    }
}
