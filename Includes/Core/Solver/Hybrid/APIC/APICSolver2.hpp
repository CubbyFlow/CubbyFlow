// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_APIC_SOLVER2_HPP
#define CUBBYFLOW_APIC_SOLVER2_HPP

#include <Core/Solver/Hybrid/PIC/PICSolver2.hpp>

namespace CubbyFlow
{
//!
//! \brief 2-D Affine Particle-in-Cell (APIC) implementation
//!
//! This class implements 2-D Affine Particle-in-Cell (APIC) solver from the
//! SIGGRAPH paper, Jiang 2015.
//!
//! \see Jiang, Chenfanfu, et al. "The affine particle-in-cell method."
//!      ACM Transactions on Graphics (TOG) 34.4 (2015): 51.
//!
class APICSolver2 : public PICSolver2
{
 public:
    class Builder;

    //! Default constructor.
    APICSolver2();

    //! Constructs solver with initial grid size.
    APICSolver2(const Vector2UZ& resolution, const Vector2D& gridSpacing,
                const Vector2D& gridOrigin);

    //! Deleted copy constructor.
    APICSolver2(const APICSolver2&) = delete;

    //! Deleted move constructor.
    APICSolver2(APICSolver2&&) noexcept = delete;

    //! Default virtual destructor.
    ~APICSolver2() override = default;

    //! Deleted copy assignment operator.
    APICSolver2& operator=(const APICSolver2&) = delete;

    //! Deleted move assignment operator.
    APICSolver2& operator=(APICSolver2&&) noexcept = delete;

    //! Returns builder fox APICSolver2.
    [[nodiscard]] static Builder GetBuilder();

 protected:
    //! Transfers velocity field from particles to grids.
    void TransferFromParticlesToGrids() override;

    //! Transfers velocity field from grids to particles.
    void TransferFromGridsToParticles() override;

 private:
    Array1<Vector2D> m_cX;
    Array1<Vector2D> m_cY;
};

//! Shared pointer type for the APICSolver2.
using APICSolver2Ptr = std::shared_ptr<APICSolver2>;

//!
//! \brief Front-end to create APICSolver2 objects step by step.
//!
class APICSolver2::Builder final : public GridFluidSolverBuilderBase2<Builder>
{
 public:
    //! Builds APICSolver2.
    [[nodiscard]] APICSolver2 Build() const;

    //! Builds shared pointer of APICSolver2 instance.
    [[nodiscard]] APICSolver2Ptr MakeShared() const;
};
}  // namespace CubbyFlow

#endif