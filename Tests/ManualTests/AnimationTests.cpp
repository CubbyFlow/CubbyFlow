#include "pch.hpp"

#include <ManualTests.hpp>

#include <Core/Animation/Animation.hpp>
#include <Core/Array/Array1.hpp>

using namespace CubbyFlow;

class SineAnimation : public Animation
{
 public:
    double x = 0.0;

 protected:
    void OnUpdate(const Frame& frame) override
    {
        x = std::sin(10.0 * frame.TimeInSeconds());
    }
};

class SineWithDecayAnimation final : public Animation
{
 public:
    double x = 0.0;

 protected:
    void OnUpdate(const Frame& frame) override
    {
        double decay = exp(-frame.TimeInSeconds());
        x = std::sin(10.0 * frame.TimeInSeconds()) * decay;
    }
};

CUBBYFLOW_TESTS(Animation);

CUBBYFLOW_BEGIN_TEST_F(Animation, OnUpdateSine)
{
    Array1<double> t(240);
    Array1<double> data(240);

    SineAnimation sineAnim;

    char fileName[256];
    snprintf(fileName, sizeof(fileName), "data.#line2,0000,x.npy");
    SaveData(t.ConstAccessor(), 0, fileName);
    snprintf(fileName, sizeof(fileName), "data.#line2,0000,y.npy");
    SaveData(data.ConstAccessor(), 0, fileName);

    for (Frame frame; frame.index < 240; frame.Advance())
    {
        sineAnim.Update(frame);

        t[frame.index] = frame.TimeInSeconds();
        data[frame.index] = sineAnim.x;

        snprintf(fileName, sizeof(fileName), "data.#line2,%04d,x.npy",
                 frame.index);
        SaveData(t.ConstAccessor(), frame.index, fileName);
        snprintf(fileName, sizeof(fileName), "data.#line2,%04d,y.npy",
                 frame.index);
        SaveData(data.ConstAccessor(), frame.index, fileName);
    }

    SaveData(t.ConstAccessor(), "data.#line2,x.npy");
    SaveData(data.ConstAccessor(), "data.#line2,y.npy");
}
CUBBYFLOW_END_TEST_F

CUBBYFLOW_BEGIN_TEST_F(Animation, OnUpdateSineWithDecay)
{
    Array1<double> t(240);
    Array1<double> data(240);

    SineWithDecayAnimation sineWithDecayAnim;

    char fileName[256];
    snprintf(fileName, sizeof(fileName), "data.#line2,0000,x.npy");
    SaveData(t.ConstAccessor(), 0, fileName);
    snprintf(fileName, sizeof(fileName), "data.#line2,0000,y.npy");
    SaveData(data.ConstAccessor(), 0, fileName);

    for (Frame frame; frame.index < 240; frame.Advance())
    {
        sineWithDecayAnim.Update(frame);

        t[frame.index] = frame.TimeInSeconds();
        data[frame.index] = sineWithDecayAnim.x;

        snprintf(fileName, sizeof(fileName), "data.#line2,%04d,x.npy",
                 frame.index);
        SaveData(t.ConstAccessor(), frame.index, fileName);
        snprintf(fileName, sizeof(fileName), "data.#line2,%04d,y.npy",
                 frame.index);
        SaveData(data.ConstAccessor(), frame.index, fileName);
    }

    SaveData(t.ConstAccessor(), "data.#line2,x.npy");
    SaveData(data.ConstAccessor(), "data.#line2,y.npy");
}
CUBBYFLOW_END_TEST_F