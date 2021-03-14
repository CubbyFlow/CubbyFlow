// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_ITERATIVE_LEVEL_SET_SOLVER2_HPP
#define CUBBYFLOW_ITERATIVE_LEVEL_SET_SOLVER2_HPP

#include <Core/Solver/LevelSet/LevelSetSolver2.hpp>

namespace CubbyFlow
{
//!
//! \brief Abstract base class for 2-D PDE-based iterative level set solver.
//!
//! This class provides infrastructure for 2-D PDE-based iterative level set
//! solver. Internally, the class implements upwind-style wave propagation and
//! the inheriting classes must provide a way to compute the derivatives for
//! given grid points.
//!
//! \see Osher, Stanley, and Ronald Fedkiw. Level set methods and dynamic
//!     implicit surfaces. Vol. 153. Springer Science & Business Media, 2006.
//!
class IterativeLevelSetSolver2 : public LevelSetSolver2
{
 public:
    //! Default constructor.
    IterativeLevelSetSolver2() = default;

    //! Deleted copy constructor.
    IterativeLevelSetSolver2(const IterativeLevelSetSolver2&) = delete;

    //! Deleted move constructor.
    IterativeLevelSetSolver2(IterativeLevelSetSolver2&&) noexcept = delete;

    //! Default virtual destructor.
    ~IterativeLevelSetSolver2() override = default;

    //! Deleted copy assignment operator.
    IterativeLevelSetSolver2& operator=(const IterativeLevelSetSolver2&) =
        delete;

    //! Deleted move assignment operator.
    IterativeLevelSetSolver2& operator=(IterativeLevelSetSolver2&&) noexcept =
        delete;

    //!
    //! Reinitializes given scalar field to signed-distance field.
    //!
    //! \param inputSDF Input signed-distance field which can be distorted.
    //! \param maxDistance Max range of reinitialization.
    //! \param outputSDF Output signed-distance field.
    //!
    void Reinitialize(const ScalarGrid2& inputSDF, double maxDistance,
                      ScalarGrid2* outputSDF) override;

    //!
    //! Extrapolates given scalar field from negative to positive SDF region.
    //!
    //! \param input Input scalar field to be extrapolated.
    //! \param sdf Reference signed-distance field.
    //! \param maxDistance Max range of extrapolation.
    //! \param output Output scalar field.
    //!
    void Extrapolate(const ScalarGrid2& input, const ScalarField2& sdf,
                     double maxDistance, ScalarGrid2* output) override;

    //!
    //! Extrapolates given collocated vector field from negative to positive SDF
    //! region.
    //!
    //! \param input Input collocated vector field to be extrapolated.
    //! \param sdf Reference signed-distance field.
    //! \param maxDistance Max range of extrapolation.
    //! \param output Output collocated vector field.
    //!
    void Extrapolate(const CollocatedVectorGrid2& input,
                     const ScalarField2& sdf, double maxDistance,
                     CollocatedVectorGrid2* output) override;

    //!
    //! Extrapolates given face-centered vector field from negative to positive
    //! SDF region.
    //!
    //! \param input Input face-centered field to be extrapolated.
    //! \param sdf Reference signed-distance field.
    //! \param maxDistance Max range of extrapolation.
    //! \param output Output face-centered vector field.
    //!
    void Extrapolate(const FaceCenteredGrid2& input, const ScalarField2& sdf,
                     double maxDistance, FaceCenteredGrid2* output) override;

    //! Returns the maximum CFL limit.
    [[nodiscard]] double GetMaxCFL() const;

    //!
    //! \brief Sets the maximum CFL limit.
    //!
    //! This function sets the maximum CFL limit for the internal upwind-style
    //! PDE calculation. The negative input will be clamped to 0.
    //!
    void SetMaxCFL(double newMaxCfl);

 protected:
    //! Computes the derivatives for given grid point.
    virtual void GetDerivatives(ConstArrayView2<double> grid,
                                const Vector2D& gridSpacing, size_t i, size_t j,
                                std::array<double, 2>* dx,
                                std::array<double, 2>* dy) const = 0;

 private:
    void Extrapolate(const ConstArrayView2<double>& input,
                     const ConstArrayView2<double>& sdf,
                     const Vector2D& gridSpacing, double maxDistance,
                     ArrayView2<double> output);

    [[nodiscard]] static unsigned int DistanceToNumberOfIterations(
        double distance, double dtau);

    [[nodiscard]] static double Sign(const ConstArrayView2<double>& sdf,
                                     const Vector2D& gridSpacing, size_t i,
                                     size_t j);

    [[nodiscard]] double PseudoTimeStep(const ConstArrayView2<double>& sdf,
                                        const Vector2D& gridSpacing) const;

    double m_maxCFL = 0.5;
};

using IterativeLevelSetSolver2Ptr = std::shared_ptr<IterativeLevelSetSolver2>;
}  // namespace CubbyFlow

#endif