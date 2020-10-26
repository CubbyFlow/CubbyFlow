// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_GRID_EMITTER_SET2_HPP
#define CUBBYFLOW_GRID_EMITTER_SET2_HPP

#include <Core/Emitter/GridEmitter2.hpp>

#include <vector>

namespace CubbyFlow
{
//!
//! \brief 2-D grid-based emitter set.
//!
class GridEmitterSet2 final : public GridEmitter2
{
 public:
    class Builder;

    //! Default constructor.
    GridEmitterSet2() = default;

    //! Constructs an emitter with sub-emitters.
    explicit GridEmitterSet2(const std::vector<GridEmitter2Ptr>& emitters);

    //! Default copy constructor.
    GridEmitterSet2(const GridEmitterSet2&) = default;

    //! Default move constructor.
    GridEmitterSet2(GridEmitterSet2&&) noexcept = default;

    //! Default virtual destructor.
    ~GridEmitterSet2() override = default;

    //! Default copy assignment operator.
    GridEmitterSet2& operator=(const GridEmitterSet2&) = default;

    //! Default move assignment operator.
    GridEmitterSet2& operator=(GridEmitterSet2&&) noexcept = default;

    //! Adds sub-emitter.
    void AddEmitter(const GridEmitter2Ptr& emitter);

    //! Returns builder fox GridEmitterSet2.
    [[nodiscard]] static Builder GetBuilder();

 private:
    void OnUpdate(double currentTimeInSeconds,
                  double timeIntervalInSeconds) override;

    std::vector<GridEmitter2Ptr> m_emitters;
};

//! Shared pointer type for the GridEmitterSet2.
using GridEmitterSet2Ptr = std::shared_ptr<GridEmitterSet2>;

//!
//! \brief Front-end to create GridEmitterSet2 objects step by step.
//!
class GridEmitterSet2::Builder final
{
 public:
    //! Returns builder with list of sub-emitters.
    [[nodiscard]] Builder& WithEmitters(
        const std::vector<GridEmitter2Ptr>& emitters);

    //! Builds GridEmitterSet2.
    [[nodiscard]] GridEmitterSet2 Build() const;

    //! Builds shared pointer of GridEmitterSet2 instance.
    [[nodiscard]] GridEmitterSet2Ptr MakeShared() const;

 private:
    std::vector<GridEmitter2Ptr> m_emitters;
};
}  // namespace CubbyFlow

#endif