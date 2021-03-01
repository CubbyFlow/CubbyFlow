// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_APIC_SOLVER3_HPP
#define CUBBYFLOW_APIC_SOLVER3_HPP

#include <Core/Solver/Hybrid/PIC/PICSolver3.hpp>

namespace CubbyFlow
{
//!
//! \brief 3-D Affine Particle-in-Cell (APIC) implementation
//!
//! This class implements 3-D Affine Particle-in-Cell (APIC) solver from the
//! SIGGRAPH paper, Jiang 2015.
//!
//! \see Jiang, Chenfanfu, et al. "The affine particle-in-cell method."
//!      ACM Transactions on Graphics (TOG) 34.4 (2015): 51.
//!
class APICSolver3 : public PICSolver3
{
 public:
    class Builder;

    //! Default constructor.
    APICSolver3();

    //! Constructs solver with initial grid size.
    APICSolver3(const Vector3UZ& resolution, const Vector3D& gridSpacing,
                const Vector3D& gridOrigin);

    //! Deleted copy constructor.
    APICSolver3(const APICSolver3&) = delete;

    //! Deleted move constructor.
    APICSolver3(APICSolver3&&) noexcept = delete;

    //! Default virtual destructor.
    ~APICSolver3() override = default;

    //! Deleted copy assignment operator.
    APICSolver3& operator=(const APICSolver3&) = delete;

    //! Deleted move assignment operator.
    APICSolver3& operator=(APICSolver3&&) noexcept = delete;

    //! Returns builder fox APICSolver3.
    [[nodiscard]] static Builder GetBuilder();

 protected:
    //! Transfers velocity field from particles to grids.
    void TransferFromParticlesToGrids() override;

    //! Transfers velocity field from grids to particles.
    void TransferFromGridsToParticles() override;

 private:
    Array1<Vector3D> m_cX;
    Array1<Vector3D> m_cY;
    Array1<Vector3D> m_cZ;
};

//! Shared pointer type for the APICSolver3.
using APICSolver3Ptr = std::shared_ptr<APICSolver3>;

//!
//! \brief Front-end to create APICSolver3 objects step by step.
//!
class APICSolver3::Builder final : public GridFluidSolverBuilderBase3<Builder>
{
 public:
    //! Builds APICSolver3.
    [[nodiscard]] APICSolver3 Build() const;

    //! Builds shared pointer of APICSolver3 instance.
    [[nodiscard]] APICSolver3Ptr MakeShared() const;
};
}  // namespace CubbyFlow

#endif