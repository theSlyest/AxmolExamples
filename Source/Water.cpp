#include "Water.h"

USING_NS_AX;

const Color4B Water::LineColor(42, 169, 192, 255); // #2aa9c0

bool Water::init()
{
    if (constexpr auto size = static_cast<float>(GRID_SIZE);
        !Sprite::initWithTexture(createSurface(), Rect(0.0f, 0.0f, size, size)) )
    {
        return false;
    }

    const auto& mesh = createMesh();
    setPolygonInfo(mesh);

    scheduleUpdate();

    return true;
}

Texture2D* Water::createSurface()
{
    const Color4B bgColor(0, 0, 0, 0); // Transparent

    constexpr int bufferSize = GRID_SIZE * GRID_SIZE * 4;
    auto* buffer = static_cast<GLubyte*>(malloc(bufferSize));

    for (int y = 0; y < GRID_SIZE; ++y) {
        for (int x = 0; x < GRID_SIZE; ++x) {
            const int index = (y * GRID_SIZE + x) * 4;

            if (x % CELL_SIZE == 0 || y % CELL_SIZE == 0 || x == GRID_SIZE - 1 || y == GRID_SIZE - 1) { // Line
                buffer[index]     = LineColor.r;
                buffer[index + 1] = LineColor.g;
                buffer[index + 2] = LineColor.b;
                buffer[index + 3] = LineColor.a;
            }
            else {
                buffer[index]     = bgColor.r;
                buffer[index + 1] = bgColor.g;
                buffer[index + 2] = bgColor.b;
                buffer[index + 3] = bgColor.a;
            }
        }
    }

    const auto texture = new Texture2D();
    texture->initWithData(buffer, bufferSize, backend::PixelFormat::RGBA8, GRID_SIZE,
        static_cast<float>(GRID_SIZE));
    free(buffer);

    const auto texParams = Texture2D::TexParams(
        backend::SamplerFilter::LINEAR, backend::SamplerFilter::LINEAR, 
        backend::SamplerAddressMode::CLAMP_TO_EDGE, backend::SamplerAddressMode::CLAMP_TO_EDGE);
    texture->setTexParameters(texParams);
    texture->autorelease();

    return texture;
}

PolygonInfo Water::createMesh()
{
    PolygonInfo polygonInfo;

    // Vertices
    polygonInfo.triangles.vertCount = GRID_LINES * GRID_LINES; 
    polygonInfo.triangles.verts = new V3F_C4B_T2F[polygonInfo.triangles.vertCount];

    int quads = (GRID_LINES - 1) * (GRID_LINES - 1); // Number of quads (grid cells)
    polygonInfo.triangles.indexCount = quads * 6; // 2 triangles per quad
    polygonInfo.triangles.indices = new unsigned short[polygonInfo.triangles.indexCount];

    // Populating vertices
    for (int y = 0; y < GRID_LINES; ++y) {
        for (int x = 0; x < GRID_LINES; ++x) {
            int index = y * GRID_LINES + x;
            V3F_C4B_T2F& vertex = polygonInfo.triangles.verts[index];
            vertex.vertices = Vec3(float(x * CELL_SIZE), float(y * CELL_SIZE), 0.0f); // Position
            vertex.texCoords = Tex2F(float(x) / (GRID_LINES - 1), float(y) / (GRID_LINES - 1)); // UV coords
            vertex.colors = LineColor; // #2aa9c0
        }
    }

    // populating indices (connecting them into triangles)
    int indexOffset = 0;
    for (int y = 0; y < GRID_LINES - 1; ++y) {
        for (int x = 0; x < GRID_LINES - 1; ++x) {
            // 1st triangle
            polygonInfo.triangles.indices[indexOffset++] = y * GRID_LINES + x;
            polygonInfo.triangles.indices[indexOffset++] = y * GRID_LINES + x + 1;
            polygonInfo.triangles.indices[indexOffset++] = (y + 1) * GRID_LINES + x;
            // 2nd triangle
            polygonInfo.triangles.indices[indexOffset++] = y * GRID_LINES + x + 1;
            polygonInfo.triangles.indices[indexOffset++] = (y + 1) * GRID_LINES + x + 1;
            polygonInfo.triangles.indices[indexOffset++] = (y + 1) * GRID_LINES + x;
        }
    }

    // Size
    polygonInfo.setRect(Rect(0.0f, 0.0f, GRID_SIZE, GRID_SIZE));

    return polygonInfo;
}

bool Water::onTouch(Touch* touch, Event* event)
{
    if (getBoundingBox().containsPoint(touch->getLocation())) {
        _ripples.emplace_back(convertTouchToNodeSpace(touch));
        return true;
    }

    return false;
}

void Water::updateRipples(float dt)
{
    for (auto it = _ripples.begin(); it != _ripples.end(); ) {
        it->elapsed += dt;
        it->radius += RIPPLE_SPEED * dt;

        float lifeRatio = it->elapsed / RIPPLE_LIFESPAN;
        if (lifeRatio >= 1.0f) {
            it = _ripples.erase(it);
        }
        else {
            it->amplitude = RIPPLE_AMPLITUDE * (1.0f - lifeRatio);
            ++it;
        }
    }
}

void Water::updateMesh()
{
    auto& triangles = getPolygonInfo().triangles;
    for (int y = 0; y < GRID_LINES; ++y) {
        for (int x = 0; x < GRID_LINES; ++x) {
            int index = y * GRID_LINES + x;
            V3F_C4B_T2F& vertex = triangles.verts[index];

            // Baseline 2D point
            const auto baseX = static_cast<float>(x * CELL_SIZE);
            const auto baseY = static_cast<float>(y * CELL_SIZE);
            float z = 0.0f;

            for(const auto& ripple : _ripples) {
                const float dx = baseX - ripple.origin.x;
                const float dy = baseY - ripple.origin.y;
                const float distance = sqrtf(dx * dx + dy * dy); // Pythagorus

                if (const float distanceFromWaveFront = fabsf(distance - ripple.radius);
                    distanceFromWaveFront < 120.0f && distance > 0.0f) { // Only deform within ripple boundary
                    const float waveValue = cosf((distance - ripple.radius) * 0.1f); // Frequency modifier
                    const float envelope = expf(-powf(distanceFromWaveFront / 40.0f, 2.0f)); // Attenuation factor to narrow the ripple width
                    z = waveValue * ripple.amplitude * envelope;
                }
            }

            vertex.vertices.z = z;
        }
    }
}

void Water::update(float dt)
{
    updateRipples(dt);
    updateMesh();
}

void Water::flatten()
{
    unscheduleUpdate();
    _ripples.clear();

    auto& triangles = getPolygonInfo().triangles;
    for (int y = 0; y < GRID_LINES; ++y) {
        for (int x = 0; x < GRID_LINES; ++x) {
            int index = y * GRID_LINES + x;
            V3F_C4B_T2F& vertex = triangles.verts[index];
            vertex.vertices.z = 0.0f;
        }
    }

    scheduleUpdate();
}