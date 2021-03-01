// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_ITERATIVE_LEVEL_SET_SOLVER3_HPP
#define CUBBYFLOW_ITERATIVE_LEVEL_SET_SOLVER3_HPP

#include <Core/Solver/LevelSet/LevelSetSolver3.hpp>

namespace CubbyFlow
{
//!
//! \brief Abstract base class for 3-D PDE-based iterative level set solver.
//!
//! This class provides infrastructure for 3-D PDE-based iterative level set
//! solver. Internally, the class implements upwind-style wave propagation and
//! the inheriting classes must provide a way to compute the derivatives for
//! given grid points.
//!
//! \see Osher, Stanley, and Ronald Fedkiw. Level set methods and dynamic
//!     implicit surfaces. Vol. 153. Springer Science & Business Media, 2006.
//!
class IterativeLevelSetSolver3 : public LevelSetSolver3
{
 public:
    //! Default constructor.
    IterativeLevelSetSolver3() = default;

    //! Deleted copy constructor.
    IterativeLevelSetSolver3(const IterativeLevelSetSolver3&) = delete;

    //! Deleted move constructor.
    IterativeLevelSetSolver3(IterativeLevelSetSolver3&&) noexcept = delete;

    //! Default virtual destructor.
    ~IterativeLevelSetSolver3() override = default;

    //! Deleted copy assignment operator.
    IterativeLevelSetSolver3& operator=(const IterativeLevelSetSolver3&) =
        delete;

    //! Deleted move assignment operator.
    IterativeLevelSetSolver3& operator=(IterativeLevelSetSolver3&&) noexcept =
        delete;

    //!
    //! Reinitializes given scalar field to signed-distance field.
    //!
    //! \param inputSDF Input signed-distance field which can be distorted.
    //! \param maxDistance Max range of reinitialization.
    //! \param outputSDF Output signed-distance field.
    //!
    void Reinitialize(const ScalarGrid3& inputSDF, double maxDistance,
                      ScalarGrid3* outputSDF) override;

    //!
    //! Extrapolates given scalar field from negative to positive SDF region.
    //!
    //! \param input Input scalar field to be extrapolated.
    //! \param sdf Reference signed-distance field.
    //! \param maxDistance Max range of extrapolation.
    //! \param output Output scalar field.
    //!
    void Extrapolate(const ScalarGrid3& input, const ScalarField3& sdf,
                     double maxDistance, ScalarGrid3* output) override;

    //!
    //! Extrapolates given collocated vector field from negative to positive SDF
    //! region.
    //!
    //! \param input Input collocated vector field to be extrapolated.
    //! \param sdf Reference signed-distance field.
    //! \param maxDistance Max range of extrapolation.
    //! \param output Output collocated vector field.
    //!
    void Extrapolate(const CollocatedVectorGrid3& input,
                     const ScalarField3& sdf, double maxDistance,
                     CollocatedVectorGrid3* output) override;

    //!
    //! Extrapolates given face-centered vector field from negative to positive
    //! SDF region.
    //!
    //! \param input Input face-centered field to be extrapolated.
    //! \param sdf Reference signed-distance field.
    //! \param maxDistance Max range of extrapolation.
    //! \param output Output face-centered vector field.
    //!
    void Extrapolate(const FaceCenteredGrid3& input, const ScalarField3& sdf,
                     double maxDistance, FaceCenteredGrid3* output) override;

    //! Returns the maximum CFL limit.
    [[nodiscard]] double GetMaxCFL() const;

    //!
    //! \brief Sets the maximum CFL limit.
    //!
    //! This function sets the maximum CFL limit for the internal upwind-style
    //! PDE calculation. The negative input will be clamped to 0.
    //!
    void SetMaxCFL(double newMaxCFL);

 protected:
    //! Computes the derivatives for given grid point.
    virtual void GetDerivatives(ConstArrayView3<double> grid,
                                const Vector3D& gridSpacing, size_t i, size_t j,
                                size_t k, std::array<double, 2>* dx,
                                std::array<double, 2>* dy,
                                std::array<double, 2>* dz) const = 0;

 private:
    void Extrapolate(const ConstArrayView3<double>& input,
                     const ConstArrayView3<double>& sdf,
                     const Vector3D& gridSpacing, double maxDistance,
                     ArrayView3<double> output);

    [[nodiscard]] static unsigned int DistanceToNumberOfIterations(
        double distance, double dtau);

    [[nodiscard]] static double Sign(const ConstArrayView3<double>& sdf,
                                     const Vector3D& gridSpacing, size_t i,
                                     size_t j, size_t k);

    [[nodiscard]] double PseudoTimeStep(const ConstArrayView3<double>& sdf,
                                        const Vector3D& gridSpacing) const;

    double m_maxCFL = 0.5;
};

using IterativeLevelSetSolver3Ptr = std::shared_ptr<IterativeLevelSetSolver3>;
}  // namespace CubbyFlow

#endif