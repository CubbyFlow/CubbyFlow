// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_GRID_EMITTER_SET3_HPP
#define CUBBYFLOW_GRID_EMITTER_SET3_HPP

#include <Core/Emitter/GridEmitter3.hpp>

#include <vector>

namespace CubbyFlow
{
//!
//! \brief 3-D grid-based emitter set.
//!
class GridEmitterSet3 final : public GridEmitter3
{
 public:
    class Builder;

    //! Constructs an emitter.
    GridEmitterSet3() = default;

    //! Constructs an emitter with sub-emitters.
    explicit GridEmitterSet3(const std::vector<GridEmitter3Ptr>& emitters);

    //! Default copy constructor.
    GridEmitterSet3(const GridEmitterSet3&) = default;

    //! Default move constructor.
    GridEmitterSet3(GridEmitterSet3&&) noexcept = default;

    //! Default virtual destructor.
    ~GridEmitterSet3() override = default;

    //! Default copy assignment operator.
    GridEmitterSet3& operator=(const GridEmitterSet3&) = default;

    //! Default move assignment operator.
    GridEmitterSet3& operator=(GridEmitterSet3&&) noexcept = default;

    //! Adds sub-emitter.
    void AddEmitter(const GridEmitter3Ptr& emitter);

    //! Returns builder fox GridEmitterSet3.
    [[nodiscard]] static Builder GetBuilder();

 private:
    void OnUpdate(double currentTimeInSeconds,
                  double timeIntervalInSeconds) override;

    std::vector<GridEmitter3Ptr> m_emitters;
};

//! Shared pointer type for the GridEmitterSet3.
using GridEmitterSet3Ptr = std::shared_ptr<GridEmitterSet3>;

//!
//! \brief Front-end to create GridEmitterSet3 objects step by step.
//!
class GridEmitterSet3::Builder final
{
 public:
    //! Returns builder with list of sub-emitters.
    [[nodiscard]] Builder& WithEmitters(
        const std::vector<GridEmitter3Ptr>& emitters);

    //! Builds GridEmitterSet3.
    [[nodiscard]] GridEmitterSet3 Build() const;

    //! Builds shared pointer of GridEmitterSet3 instance.
    [[nodiscard]] GridEmitterSet3Ptr MakeShared() const;

 private:
    std::vector<GridEmitter3Ptr> m_emitters;
};
}  // namespace CubbyFlow

#endif