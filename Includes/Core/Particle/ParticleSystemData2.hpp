// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_PARTICLE_SYSTEM_DATA2_HPP
#define CUBBYFLOW_PARTICLE_SYSTEM_DATA2_HPP

#include <Core/Array/Array.hpp>
#include <Core/Matrix/Matrix.hpp>
#include <Core/Searcher/PointNeighborSearcher.hpp>
#include <Core/Utils/Serialization.hpp>

#include <memory>
#include <vector>

#ifndef CUBBYFLOW_DOXYGEN

namespace flatbuffers
{
class FlatBufferBuilder;
template <typename T>
struct Offset;
}  // namespace flatbuffers

namespace CubbyFlow
{
namespace fbs
{
struct ParticleSystemData2;
}
}  // namespace CubbyFlow

#endif

namespace CubbyFlow
{
//!
//! \brief      2-D particle system data.
//!
//! This class is the key data structure for storing particle system data. A
//! single particle has position, velocity, and force attributes by default. But
//! it can also have additional custom scalar or vector attributes.
//!
class ParticleSystemData2 : public Serializable
{
 public:
    //! Scalar data chunk.
    using ScalarData = Array1<double>;

    //! Vector data chunk.
    using VectorData = Array1<Vector2D>;

    //! Default constructor.
    ParticleSystemData2();

    //! Constructs particle system data with given number of particles.
    explicit ParticleSystemData2(size_t numberOfParticles);

    //! Copy constructor.
    ParticleSystemData2(const ParticleSystemData2& other);

    //! Default move constructor.
    ParticleSystemData2(ParticleSystemData2&&) noexcept = default;

    //! Default virtual destructor.
    ~ParticleSystemData2() override = default;

    //! Copy assignment operator.
    ParticleSystemData2& operator=(const ParticleSystemData2& other);

    //! Default move assignment operator.
    ParticleSystemData2& operator=(ParticleSystemData2&&) noexcept = default;

    //!
    //! \brief      Resizes the number of particles of the container.
    //!
    //! This function will resize internal containers to store newly given
    //! number of particles including custom data layers. However, this will
    //! invalidate neighbor searcher and neighbor lists. It is users
    //! responsibility to call ParticleSystemData2::BuildNeighborSearcher and
    //! ParticleSystemData2::BuildNeighborLists to refresh those data.
    //!
    //! \param[in]  newNumberOfParticles    New number of particles.
    //!
    void Resize(size_t newNumberOfParticles);

    //! Returns the number of particles.
    [[nodiscard]] size_t GetNumberOfParticles() const;

    //!
    //! \brief      Adds a scalar data layer and returns its index.
    //!
    //! This function adds a new scalar data layer to the system. It can be used
    //! for adding a scalar attribute, such as temperature, to the particles.
    //!
    //! \param[in] initialVal  Initial value of the new scalar data.
    //!
    [[nodiscard]] size_t AddScalarData(double initialVal = 0.0);

    //!
    //! \brief      Adds a vector data layer and returns its index.
    //!
    //! This function adds a new vector data layer to the system. It can be used
    //! for adding a vector attribute, such as vortex, to the particles.
    //!
    //! \param[in] initialVal  Initial value of the new vector data.
    //!
    [[nodiscard]] size_t AddVectorData(const Vector2D& initialVal = Vector2D());

    //! Returns the radius of the particles.
    [[nodiscard]] double GetRadius() const;

    //! Sets the radius of the particles.
    virtual void SetRadius(double newRadius);

    //! Returns the mass of the particles.
    [[nodiscard]] double GetMass() const;

    //! Sets the mass of the particles.
    virtual void SetMass(double newMass);

    //! Returns the position array (immutable).
    [[nodiscard]] ConstArrayView1<Vector2D> Positions() const;

    //! Returns the position array (mutable).
    [[nodiscard]] ArrayView1<Vector2D> Positions();

    //! Returns the velocity array (immutable).
    [[nodiscard]] ConstArrayView1<Vector2D> Velocities() const;

    //! Returns the velocity array (mutable).
    [[nodiscard]] ArrayView1<Vector2D> Velocities();

    //! Returns the force array (immutable).
    [[nodiscard]] ConstArrayView1<Vector2D> Forces() const;

    //! Returns the force array (mutable).
    [[nodiscard]] ArrayView1<Vector2D> Forces();

    //! Returns custom scalar data layer at given index (immutable).
    [[nodiscard]] ConstArrayView1<double> ScalarDataAt(size_t idx) const;

    //! Returns custom scalar data layer at given index (mutable).
    [[nodiscard]] ArrayView1<double> ScalarDataAt(size_t idx);

    //! Returns custom vector data layer at given index (immutable).
    [[nodiscard]] ConstArrayView1<Vector2D> VectorDataAt(size_t idx) const;

    //! Returns custom vector data layer at given index (mutable).
    [[nodiscard]] ArrayView1<Vector2D> VectorDataAt(size_t idx);

    //!
    //! \brief      Adds a particle to the data structure.
    //!
    //! This function will add a single particle to the data structure. For
    //! custom data layers, zeros will be assigned for new particles.
    //! However, this will invalidate neighbor searcher and neighbor lists. It
    //! is users responsibility to call
    //! ParticleSystemData2::BuildNeighborSearcher and
    //! ParticleSystemData2::BuildNeighborLists to refresh those data.
    //!
    //! \param[in]  newPosition The new position.
    //! \param[in]  newVelocity The new velocity.
    //! \param[in]  newForce    The new force.
    //!
    void AddParticle(const Vector2D& newPosition,
                     const Vector2D& newVelocity = Vector2D(),
                     const Vector2D& newForce = Vector2D());

    //!
    //! \brief      Adds particles to the data structure.
    //!
    //! This function will add particles to the data structure. For custom data
    //! layers, zeros will be assigned for new particles. However, this will
    //! invalidate neighbor searcher and neighbor lists. It is users
    //! responsibility to call ParticleSystemData2::BuildNeighborSearcher and
    //! ParticleSystemData2::BuildNeighborLists to refresh those data.
    //!
    //! \param[in]  newPositions  The new positions.
    //! \param[in]  newVelocities The new velocities.
    //! \param[in]  newForces     The new forces.
    //!
    void AddParticles(const ConstArrayView1<Vector2D>& newPositions,
                      const ConstArrayView1<Vector2D>& newVelocities =
                          ConstArrayView1<Vector2D>(),
                      const ConstArrayView1<Vector2D>& newForces =
                          ConstArrayView1<Vector2D>());

    //!
    //! \brief      Returns neighbor searcher.
    //!
    //! This function returns currently set neighbor searcher object. By
    //! default, PointParallelHashGridSearcher2 is used.
    //!
    //! \return     Current neighbor searcher.
    //!
    [[nodiscard]] const PointNeighborSearcher2Ptr& GetNeighborSearcher() const;

    //! Sets neighbor searcher.
    void SetNeighborSearcher(
        const PointNeighborSearcher2Ptr& newNeighborSearcher);

    //!
    //! \brief      Returns neighbor lists.
    //!
    //! This function returns neighbor lists which is available after calling
    //! PointParallelHashGridSearcher2::BuildNeighborLists. Each list stores
    //! indices of the neighbors.
    //!
    //! \return     Neighbor lists.
    //!
    [[nodiscard]] const std::vector<std::vector<size_t>>& GetNeighborLists()
        const;

    //! Builds neighbor searcher with given search radius.
    void BuildNeighborSearcher(double maxSearchRadius);

    //! Builds neighbor lists with given search radius.
    void BuildNeighborLists(double maxSearchRadius);

    //! Serializes this particle system data to the buffer.
    void Serialize(std::vector<uint8_t>* buffer) const override;

    //! Deserializes this particle system data from the buffer.
    void Deserialize(const std::vector<uint8_t>& buffer) override;

    //! Copies from other particle system data.
    void Set(const ParticleSystemData2& other);

 protected:
    void SerializeParticleSystemData(
        flatbuffers::FlatBufferBuilder* builder,
        flatbuffers::Offset<fbs::ParticleSystemData2>* fbsParticleSystemData)
        const;

    void DeserializeParticleSystemData(
        const fbs::ParticleSystemData2* fbsParticleSystemData);

 private:
    double m_radius = 1e-3;
    double m_mass = 1e-3;
    size_t m_numberOfParticles = 0;
    size_t m_positionIdx = 0;
    size_t m_velocityIdx = 0;
    size_t m_forceIdx = 0;

    std::vector<ScalarData> m_scalarDataList;
    std::vector<VectorData> m_vectorDataList;

    PointNeighborSearcher2Ptr m_neighborSearcher;
    std::vector<std::vector<size_t>> m_neighborLists;
};

//! Shared pointer type of ParticleSystemData2.
using ParticleSystemData2Ptr = std::shared_ptr<ParticleSystemData2>;
}  // namespace CubbyFlow

#endif