#pragma once

#include <functional>
#include <vector>

#include "DemandCurve.h"
#include "GeneratorModel.h"

// The rules of the game. The player moves generators, their average is the
// supply, and the supply has to stay close to the demand. Time spent out of
// the tolerance band costs strikes; time spent inside it is banked as uptime.
//
// This class does not depend on Axmol and never touches the UI: the scene
// listens to the callbacks and renders the state.
class BalancerGame
{
public:
    struct Config
    {
        std::vector<float> generatorRates;  // one entry per generator, see GeneratorModel
        float startValue = 0.5f;            // initial generator and demand level
        float tolerance = 0.1f;             // supply may differ from demand by this much
        float gracePeriod = 4.0f;           // seconds out of tolerance that cost one strike
        int strikes = 3;
    };

    explicit BalancerGame(Config config, DemandCurve::Random random = nullptr);

    // Notifications for the view.
    std::function<void(float)> onUpTimeChanged;
    std::function<void(int)> onStrikesChanged;
    // Called on every update: `active` is true while the supply is out of tolerance,
    // and `time` is how long it has been, in seconds.
    std::function<void(bool active, float time)> onGraceChanged;
    std::function<void()> onGameOver;

    void reset();
    void update(float dt);

    // Player actions.
    void increase(size_t generator);
    void decrease(size_t generator);

    [[nodiscard]] size_t getGeneratorCount() const { return _generators.size(); }
    [[nodiscard]] float getGeneratorValue(size_t generator) const { return _generators[generator].getValue(); }
    [[nodiscard]] float getSupply() const { return _supply; }
    [[nodiscard]] float getDemand() const { return _demand.getValue(); }
    [[nodiscard]] float getTolerance() const { return _config.tolerance; }
    [[nodiscard]] float getUpTime() const { return _upTime; }
    [[nodiscard]] int getStrikes() const { return _strikes; }
    [[nodiscard]] bool isOver() const { return _over; }

private:
    Config _config;
    std::vector<GeneratorModel> _generators;
    DemandCurve _demand;

    float _supply = 0.0f;
    int _strikes = 0;
    float _downTime = 0.0f;
    float _upTime = 0.0f;
    bool _over = true;

    void setStrikes(int strikes);
    void setUpTime(float upTime);
};
