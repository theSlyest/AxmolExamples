#pragma once

#include "2d/DrawNode.h"
#include "2d/Sprite.h"

class SignalScreen : public ax::DrawNode
{
public:
    bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(SignalScreen);

    void setGlitchAmplitude(const float amplitude) { _amplitude = amplitude; } // between 0.0 (normal) and 1.0 (max)

    void update(float dt) override;

    void reset();

private:
    static const std::vector<ax::Color4B> TV_COLORS;
    static const ax::Vec2 SIZE;

    static constexpr float BORDER = 24.0f;
    static constexpr float BORDER_BOTTOM = 44.0f;
    static constexpr float OFFSET = 12.0f;
    static const std::string FRAG_PATH;

    ax::Sprite* _screen = nullptr;
    ax::Vec2 _size;
    ax::backend::ProgramState* _programState = nullptr;

    ax::backend::UniformLocation _locTime;
    ax::backend::UniformLocation _locAmplitude;
    float _totalTime = 0.0f;
    float _amplitude = 0.0f;

    static void drawRoundedRect(ax::DrawNode* node, const ax::Vec2 &start, const ax::Vec2 &dest, float radius, int segments, const ax::Color4F &color);
    static double dist(int x1, int y1, int x2, int y2);
    void drawBackground();
    void initScreen();
    void initShaderProgram();

    ax::Texture2D* createPicture() const;
};
