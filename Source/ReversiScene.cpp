#include "ReversiScene.h"

USING_NS_AX;

// on "init" you need to initialize your instance
bool ReversiScene::init()
{
    if ( !Scene::init() )
        return false;

    const auto& visibleSize = Director::getInstance()->getVisibleSize();

    const auto background = LayerColor::create(Color4B(18, 29, 44, 255));
    addChild(background);

    _board = Board::create();
    const Size& boardSize = _board->getContentSize();
    _board->setPosition((visibleSize.width - boardSize.width)/ 2.0f, (visibleSize.height - boardSize.height)/ 2.0f - 64.0f);
    addChild(_board);

    _info = Info::create();
    _info->setPosition(0.0f, 680.0f);
    _info->setTurn();
    addChild(_info);

    _pieceCounts.assign(2, 0);
    _pieces.assign(8, std::vector<Piece*>(8, nullptr));

    _mouseListener = EventListenerMouse::create();
    _mouseListener->onMouseMove = [this](EventMouse* event) { return onMouseMove(event); };
    _mouseListener->onMouseUp = [this](EventMouse* event) { return onMouseUp(event); };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);

    gameStart();
    
    return true;
}

void ReversiScene::gameStart()
{
    for (auto& row : _pieces) {
        for (int c = 0; c < 8; ++c) {
            if (row[c] != nullptr) {
                removeChild(row[c], true);
                row[c] = nullptr;
            }
        }
    }

    _pieceCounts[0] = 0;
    _pieceCounts[1] = 0;
    _moveCount = 0;
    _isBlackTurn = true;
    _prevLegal = true;

    addPiece(3, 3, true);
    addPiece(4, 4, true);
    addPiece(3, 4, false);
    addPiece(4, 3, false);

    _info->setTurn(true);
    checkLegal();
    _info->setInstruction("Select an orange legal move");
}

void ReversiScene::nextTurn()
{
    for (int c = 0; c < 8; ++c) {
        for (int r = 0; r < 8; ++r) {
            _board->setLegal(r, c, false);
            _board->setHovered(r, c, false);
        }
    }

    // Full grid
    if (_pieceCounts[0] + _pieceCounts[1] == 64) {
        gameOver();
        return;
    }

    _isBlackTurn = !_isBlackTurn;
    _info->setTurn(_isBlackTurn);
    int moves = checkLegal();
    _info->setMoveCount(moves);
    if (moves > 0) {
        _info->setInstruction("Select an orange legal move");
        _prevLegal = true;
    }
    else {
            _info->setInstruction("No possible move! Skipping turn...", true);
        if (_prevLegal) {
            _prevLegal = false;
            scheduleOnce([this](float dt) { nextTurn(); }, 3.0f, "NextTurn");
        }
        else {
            _prevLegal = false;
            scheduleOnce([this](float dt) { gameOver(); }, 3.0f, "GameOver");
        }
    }
}

void ReversiScene::gameOver() const
{
    int result = 0;
    if (_pieceCounts[0] < _pieceCounts[1])
        result = 1;
    else if (_pieceCounts[0] > _pieceCounts[1])
        result = -1;

    _info->setResult(result);
}

void ReversiScene::addPiece(const int row, const int col, const bool isBlack)
{
    if (_pieces[row][col] != nullptr)
        return;

    ++_pieceCounts[isBlack ? 1 : 0];

    const float x = 120.0f + col * SQUARE_SIDE;
    const float y = 56.0f + row * SQUARE_SIDE;
    _pieces[row][col] = Piece::create(isBlack);
    _pieces[row][col]->setPosition(x, y);
    addChild(_pieces[row][col]);
}

std::pair<int, int> ReversiScene::squareFromLocation(const Vec2& location)
{
    if (location.x < 112.0f || location.y < 48.0f)
        return { -1, -1 };

    int row = static_cast<int>((location.y - 48.0f) / SQUARE_SIDE);
    if (row < 0 || row > 7)
        return { -1, -1 };

    int col = (location.x - 112.0f) / SQUARE_SIDE;
    if (col < 0 || col > 7)
        return { -1, -1 };

    AXLOGD("squareFromLocation x:%f, y:%f = row: %d, col: %d", location.x, location.y, row, col);
    return { row, col };
}

bool ReversiScene::onMouseMove(const EventMouse* event)
{
    const std::pair<int, int>& coords = squareFromLocation(event->getLocation());
    if (coords != _hovered) {
        if (coords.first != -1) {
            _board->setHovered(coords.first, coords.second, true);
        }

        if (_hovered.first != -1) {
            _board->setHovered(_hovered.first, _hovered.second, false);
        }

        _hovered = coords;
    }

    return coords.first != -1;
}

void ReversiScene::switchPieceColor(int row, int col)
{
    _pieces[row][col]->setBlack(_isBlackTurn);
    if (_isBlackTurn) {
        --_pieceCounts[0];
        ++_pieceCounts[1];
    }
    else {
        ++_pieceCounts[0];
        --_pieceCounts[1];
    }
}

void ReversiScene::switchColorFrom(int row, int col) 
{
    int shift = 1;
    bool goLeft = true;
    bool goRight = true;
    bool goUp = true;
    bool goDown = true;
    while (goLeft || goRight || goUp || goDown) {
        if (goLeft) {
            int c = col - shift;
            if (c < 0 || _pieces[row][c] == nullptr) {
                goLeft = false;
            }
            else if (_pieces[row][c]->isBlack() == _isBlackTurn) {
                if (shift > 1) {
                    for (int i = col - 1; i > c; --i) {
                        switchPieceColor(row, i);
                    }
                }
                goLeft = false;
            }
        }
        if (goRight) {
            int c = col + shift;
            if (c > 7 || _pieces[row][c] == nullptr) {
                goRight = false;
            }
            else if (_pieces[row][c]->isBlack() == _isBlackTurn) {
                if (shift > 1) {
                    for (int i = col + 1; i < c; ++i) {
                        switchPieceColor(row, i);
                    }
                }
                goRight = false;
            }
        }
        if (goDown) {
            int r = row - shift;
            if (r < 0 || _pieces[r][col] == nullptr) {
                goDown = false;
            }
            else if (_pieces[r][col]->isBlack() == _isBlackTurn) {
                if (shift > 1) {
                    for (int i = row - 1; i > r; --i) {
                        switchPieceColor(i, col);
                    }
                }
                goDown = false;
            }
        }
        if (goUp) {
            int r = row + shift;
            if (r > 7 || _pieces[r][col] == nullptr) {
                goUp = false;
            }
            else if (_pieces[r][col]->isBlack() == _isBlackTurn) {
                if (shift > 1) {
                    for (int i = row + 1; i < r; ++i) {
                        switchPieceColor(i, col);
                    }
                }
                goUp = false;
            }
        }
        ++shift;
    }
    _info->setPieceCount(_pieceCounts[0], false);
    _info->setPieceCount(_pieceCounts[1], true);
}

bool ReversiScene::onMouseUp(EventMouse* event)
{
    const std::pair<int, int>& coords = squareFromLocation(event->getLocation());
    if (coords.first != -1) {
        if (_board->isLegal(coords.first, coords.second)) {
            addPiece(coords.first, coords.second, _isBlackTurn);
            switchColorFrom(coords.first, coords.second);
            nextTurn();
        }
        return true;
    }
    return false;
}

bool ReversiScene::keepChecking(int row, int col, bool isFirst) const
{
    if (row < 0 || row > 7)
        return false;

    if (col < 0 || col > 7)
        return false;

    if (_board->isLegal(row, col))
        return false;

    if (_pieces[row][col] == nullptr && isFirst)
        return false;

    if (_pieces[row][col] != nullptr && _pieces[row][col]->isBlack() == _isBlackTurn) 
        return false;
    
    if (_pieces[row][col] == nullptr) {
        _board->setLegal(row, col, true);
        return false;
    }

    return true;
}

int ReversiScene::checkLegal()
{
    int result = 0;
    for (int c = 0; c < 8; ++c) {
        for (int r = 0; r < 8; ++r) {
            if (_pieces[r][c] != nullptr && _pieces[r][c]->isBlack() == _isBlackTurn) {
                int shift = 1;
                bool goLeft = true;
                bool goRight = true;
                bool goUp = true;
                bool goDown = true;
                while (goLeft || goRight || goUp || goDown) {
                    if (goLeft) {
                        int col = c - shift;
                        goLeft = keepChecking(r, col, shift == 1);
                        if (!goLeft && col > -1 && _board->isLegal(r, col))
                            result++;
                    }
                    if (goRight) {
                        int col = c + shift;
                        goRight = keepChecking(r, col, shift == 1);
                        if (!goRight && col < 8 && _board->isLegal(r, col))
                            result++;
                    }
                    if (goDown) {
                        int row = r - shift;
                        goDown = keepChecking(row, c, shift == 1);
                        if (!goDown && row > -1 && _board->isLegal(row, c))
                            result++;
                    }
                    if (goUp) {
                        int row = r + shift;
                        goUp = keepChecking(row, c, shift == 1);
                        if (!goUp && row < 8 && _board->isLegal(row, c))
                            result++;
                    }
                    ++shift;
                }
            }
        }
    }
    return result;
}