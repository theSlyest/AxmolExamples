#pragma once

#include "axmol.h"
#include "Disk.h"
#include "Peg.h"

class DiskTowerScene : public ax::Scene
{
public:
    static ax::Scene* createScene();

    bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(DiskTowerScene);

private:
    static constexpr float BaseHeight = 128.0f;
    static constexpr float TopHeight = 460.0f;

    int _moves = 0;
    int _pegLifted = 0; // Index of the peg from which a disk is lifted, -1 if no disk is lifted

    std::vector<Peg*> _pegs;
    std::vector<Disk*> _disks;

    ax::Label* _movesLabel = nullptr;
    ax::Label* _finalMovesLabel = nullptr;
    ax::EventListenerKeyboard* _keyboardListener = nullptr;
    ax::EventListenerMouse* _mouseListener = nullptr;
    
    RoundedRect* _panel = nullptr;

    void initMoves(const ax::Size& visibleSize, const ax::Vec2& origin);
    void initBase(const ax::Size& visibleSize, const ax::Vec2& origin);
    void initPegs(const ax::Size& visibleSize, const ax::Vec2& origin);
    void initDisks();
    void initKeyboardListener();
    void initMouseListener();
    void initPanel(const ax::Size& visibleSize, const ax::Vec2& origin);

    bool onMouseDown(ax::Event *event);

    void setMoves(int moves);

    void restart();
    void resetDisks();

    bool addToPeg(int pegIndex, unsigned char diskSize);
    void positionDiskOnPeg(int pegIndex, unsigned char diskSize) const;

    bool isGameWon() const;
    void showWinMessage() const;
};
