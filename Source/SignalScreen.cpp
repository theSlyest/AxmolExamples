#include "SignalScreen.h"

#include "2d/ClippingNode.h"
#include "glad/gl.h"
#include "renderer/Shaders.h"
#include "renderer/backend/ProgramManager.h"

USING_NS_AX;

const std::vector<Color4B> SignalScreen::TV_COLORS {
    Color4B(255, 0, 0, 255),
    Color4B(255, 128, 0, 255),
    Color4B(255, 255, 0, 255),
    Color4B(0, 255, 0, 255),
    Color4B(0, 255, 255, 255),
    Color4B(0, 0, 255, 255)
};

const Vec2 SignalScreen::SIZE(880.0f, 660.0f);
const std::string SignalScreen::FRAG_PATH = "shaders/tv_glitch.frag";

bool SignalScreen::init()
{
    if (!DrawNode::init())
        return false;

    setContentSize(SIZE);

    drawBackground();
    initScreen();

    return true;
}

void SignalScreen::update(const float dt)
{
    _totalTime += dt;
    if (const auto programState = _screen->getProgramState(); programState != nullptr) {
        programState->setUniform(_locTime, &_totalTime, sizeof(_totalTime));
        programState->setUniform(_locAmplitude, &_amplitude, sizeof(_amplitude));
    }
}

void SignalScreen::reset()
{
    _amplitude = 0.0f;
    _totalTime = 0.0f;
    update(0.0f);
}

void SignalScreen::drawRoundedRect(DrawNode *node, const Vec2 &start, const Vec2 &dest, const float radius, const int segments, const Color4F &color)
{
    const Vec2 points[8]{
        Vec2(start.x, start.y + radius),
        Vec2(start.x + radius, start.y),
        Vec2(dest.x - radius, start.y),
        Vec2(dest.x, start.y + radius),
        Vec2(dest.x, dest.y - radius),
        Vec2(dest.x - radius, dest.y),
        Vec2(start.x + radius, dest.y),
        Vec2(start.x, dest.y - radius)};

    node->drawSolidPoly(points, 8, color);

    node->drawSolidCircle(Vec2(start.x + radius, start.y + radius), radius, 0.0f, segments, color);
    node->drawSolidCircle(Vec2(dest.x - radius, start.y + radius), radius, 0.0f, segments, color);
    node->drawSolidCircle(Vec2(dest.x - radius, dest.y - radius), radius, 0.0f, segments, color);
    node->drawSolidCircle(Vec2(start.x + radius, dest.y - radius), radius, 0.0f, segments, color);
}

double SignalScreen::dist(int x1, int y1, int x2, int y2)
{
    const int xx = x1 - x2;
    const int yy = y1 - y2;
    return sqrt(xx * xx + yy * yy);
}

void SignalScreen::drawBackground()
{
    drawRoundedRect(this, Vec2::ZERO, Vec2(SIZE), 56.0f, 192, Color4F(0.13f, 0.15f, 0.2f, 1.0f));
    drawRoundedRect(this, Vec2(BORDER, BORDER_BOTTOM), Vec2(SIZE.width - BORDER, SIZE.height - BORDER), 36.0f, 160, Color4F(0.08f, 0.1f, 0.15f, 1.0f));
    drawCircle(Vec2(SIZE.width - BORDER_BOTTOM, BORDER_BOTTOM * 0.5f), BORDER_BOTTOM * 0.3f, 0.0f, BORDER_BOTTOM, false, Color4F(0.0f, 1.0f, 1.0f, 1.0f), 1.5f);
}

void SignalScreen::initScreen()
{
    _size = Vec2(SIZE.width - 2.0f * (BORDER + OFFSET), SIZE.height - BORDER - BORDER_BOTTOM - 2.0f * OFFSET);
    _size.width = std::floor(_size.width / 6) * 6;

    _screen = Sprite::createWithTexture(createPicture(), ax::Rect(0.0f, 0.0f, _size.x, _size.y));
    _screen->setPosition(_size / 2.0f);

    const auto stencil = DrawNode::create();
    stencil->setContentSize(_size);
    drawRoundedRect(stencil, Vec2::ZERO, Vec2(_size), 28.0f, 96, Color4F::BLACK);

    const auto clipper = ClippingNode::create(stencil);
    clipper->addChild(_screen);
    clipper->setPosition(BORDER + OFFSET, BORDER_BOTTOM + OFFSET);
    addChild(clipper);

    initShaderProgram();
}

void SignalScreen::initShaderProgram()
{
    const auto program = backend::ProgramManager::getInstance()->loadProgram(positionTextureColor_vert, FRAG_PATH, VertexLayoutType::Sprite);
    const auto programState = new backend::ProgramState(program);
    _locTime = programState->getUniformLocation("u_time");
    _locAmplitude = programState->getUniformLocation("u_amplitude");
    _screen->setProgramState(programState);

    program->release();
    programState->release();
}

Texture2D *SignalScreen::createPicture() const
{
    // Columns
    const int width = static_cast<int>(_size.width);
    const int height = static_cast<int>(_size.height);
    const int bufferSize = width * height * 4;
    const auto buffer = static_cast<GLubyte *>(malloc(bufferSize));

    const int midX = width / 2;
    const int midY = height / 2;
    const int ringRadiusIn = 0.75f * midY - 8;
    const int ringRadiusOut = ringRadiusIn + 16;
    const int circleRadius = height / 6;
    const int colWidth = width / 6;
    for (int x = 0; x < width; ++x)
    {
        const Color4B &color = TV_COLORS[x / colWidth];
        for (int y = 0; y < height; ++y)
        {
            const int idx = (y * width + x) * 4;
            if (const double fromCenter = dist(x, y, midX, midY);
                fromCenter <= circleRadius                                     // Circle
                || (fromCenter <= ringRadiusOut && fromCenter >= ringRadiusIn) // Ring
                || (fromCenter < ringRadiusIn && y > midY - 16 && y < midY + 16))
            { // Horizontal line
                buffer[idx] = 255;
                buffer[idx + 1] = 255;
                buffer[idx + 2] = 255;
                buffer[idx + 3] = 255;
            }
            else
            {
                buffer[idx] = color.r;
                buffer[idx + 1] = color.g;
                buffer[idx + 2] = color.b;
                buffer[idx + 3] = color.a;
            }
        }
    }

    const auto texture = new Texture2D();
    texture->initWithData(buffer, bufferSize, backend::PixelFormat::RGBA8, width, height);
    free(buffer);

    const auto texParams = Texture2D::TexParams(
        backend::SamplerFilter::LINEAR, backend::SamplerFilter::LINEAR,
        backend::SamplerAddressMode::CLAMP_TO_EDGE, backend::SamplerAddressMode::CLAMP_TO_EDGE);
    texture->setTexParameters(texParams);
    texture->autorelease();

    return texture;
}
