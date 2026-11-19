#include "BomberGrid.h"

USING_NS_AX;

BomberGrid* BomberGrid::create(const GridModel& model)
{
    if (auto* pRet = new (std::nothrow) BomberGrid(); pRet && pRet->init(model))
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

bool BomberGrid::init(const GridModel& model)
{
    if (!Node::init())
        return false;

    _model = &model;
    setContentSize(Size(GridModel::COLS * GridTile::SIZE, GridModel::ROWS * GridTile::SIZE));

    for (int col = 0; col < GridModel::COLS; ++col) {
        std::vector<GridTile*> column;
        for (int row = 0; row < GridModel::ROWS; ++row) {
            const auto tile = GridTile::create();
            tile->setPosition(GridModel::coordsToPosition({ col, row }));
            addChild(tile);
            column.push_back(tile);
        }
        _tiles.push_back(column);
    }

    for (int col = 0; col < GridModel::COLS; ++col)
        for (int row = 0; row < GridModel::ROWS; ++row)
            refresh(col, row);

    return true;
}

void BomberGrid::refresh(const int col, const int row)
{
    const TileState state = _model->getState(col, row);
    _tiles[col][row]->show(state, _model->isOnFire(col, row));

    const GridCoords coords(col, row);
    const auto bomb = _bombs.find(coords);
    if (state == TileState::BOMB && bomb == _bombs.end()) {
        const auto node = BomberBomb::create();
        node->setPosition(GridModel::coordsToPosition(coords));
        addChild(node);
        _bombs[coords] = node;
    }
    else if (state != TileState::BOMB && bomb != _bombs.end()) {
        bomb->second->removeFromParent();
        _bombs.erase(bomb);
    }
}
