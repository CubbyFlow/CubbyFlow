// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_PARTICLE_SYSTEM_DATA3_HPP
#define CUBBYFLOW_PARTICLE_SYSTEM_DATA3_HPP

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
struct ParticleSystemData3;
}
}  // namespace CubbyFlow

#endif

namespace CubbyFlow
{
//!
//! \brief      3-D particle system data.
//!
//! This class is the key data structure for storing particle system data. A
//! single particle has position, velocity, and force attributes by default. But
//! it can also have additional custom scalar or vector attributes.
//!
class ParticleSystemData3 : public Serializable
{
 public:
    //! Scalar data chunk.
    using ScalarData = Array1<double>;

    //! Vector data chunk.
    using VectorData = Array1<Vector3D>;

    //! Default constructor.
    ParticleSystemData3();

    //! Constructs particle system data with given number of particles.
    explicit ParticleSystemData3(size_t numberOfParticles);

    //! Copy constructor.
    ParticleSystemData3(const ParticleSystemData3& other);

    //! Default move constructor.
    ParticleSystemData3(ParticleSystemData3&&) noexcept = default;

    //! Default virtual destructor.
    ~ParticleSystemData3() override = default;

    //! Copy assignment operator.
    ParticleSystemData3& operator=(const ParticleSystemData3& other);

    //! Default move assignment operator.
    ParticleSystemData3& operator=(ParticleSystemData3&&) noexcept = default;

    //!
    //! \brief      Resizes the number of particles of the container.
    //!
    //! This function will resize internal containers to store newly given
    //! number of particles including custom data layers. However, this will
    //! invalidate neighbor searcher and neighbor lists. It is users
    //! responsibility to call ParticleSystemData3::BuildNeighborSearcher and
    //! ParticleSystemData3::BuildNeighborLists to refresh those data.
    //!
    //! \param[in]  newNumberOfParticles    New number of particles.
    //!
    void Resize(size_t newNumberOfParticles);

    //! Returns the number of particles.
    size_t GetNumberOfParticles() const;

    //!
    //! \brief      Adds a scalar data layer and returns its index.
    //!
    //! This function adds a new scalar data layer to the system. It can be used
    //! for adding a scalar attribute, such as temperature, to the particles.
    //!
    //! \param[in] initialVal  Initial value of the new scalar data.
    //!
    size_t AddScalarData(double initialVal = 0.0);

    //!
    //! \brief      Adds a vector data layer and returns its index.
    //!
    //! This function adds a new vector data layer to the system. It can be used
    //! for adding a vector attribute, such as vortex, to the particles.
    //!
    //! \param[in] initialVal  Initial value of the new vector data.
    //!
    size_t AddVectorData(const Vector3D& initialVal = Vector3D());

    //! Returns the radius of the particles.
    double GetRadius() const;

    //! Sets the radius of the particles.
    virtual void SetRadius(double newRadius);

    //! Returns the mass of the particles.
    double GetMass() const;

    //! Sets the mass of the particles.
    virtual void SetMass(double newMass);

    //! Returns the position array (immutable).
    ConstArrayView1<Vector3D> Positions() const;

    //! Returns the position array (mutable).
    ArrayView1<Vector3D> Positions();

    //! Returns the velocity array (immutable).
    ConstArrayView1<Vector3D> Velocities() const;

    //! Returns the velocity array (mutable).
    ArrayView1<Vector3D> Velocities();

    //! Returns the force array (immutable).
    ConstArrayView1<Vector3D> Forces() const;

    //! Returns the force array (mutable).
    ArrayView1<Vector3D> Forces();

    //! Returns custom scalar data layer at given index (immutable).
    ConstArrayView1<double> ScalarDataAt(size_t idx) const;

    //! Returns custom scalar data layer at given index (mutable).
    ArrayView1<double> ScalarDataAt(size_t idx);

    //! Returns custom vector data layer at given index (immutable).
    ConstArrayView1<Vector3D> VectorDataAt(size_t idx) const;

    //! Returns custom vector data layer at given index (mutable).
    ArrayView1<Vector3D> VectorDataAt(size_t idx);

    //!
    //! \brief      Adds a particle to the data structure.
    //!
    //! This function will add a single particle to the data structure. For
    //! custom data layers, zeros will be assigned for new particles.
    //! However, this will invalidate neighbor searcher and neighbor lists. It
    //! is users responsibility to call
    //! ParticleSystemData3::BuildNeighborSearcher and
    //! ParticleSystemData3::BuildNeighborLists to refresh those data.
    //!
    //! \param[in]  newPosition The new position.
    //! \param[in]  newVelocity The new velocity.
    //! \param[in]  newForce    The new force.
    //!
    void AddParticle(const Vector3D& newPosition,
                     const Vector3D& newVelocity = Vector3D(),
                     const Vector3D& newForce = Vector3D());

    //!
    //! \brief      Adds particles to the data structure.
    //!
    //! This function will add particles to the data structure. For custom data
    //! layers, zeros will be assigned for new particles. However, this will
    //! invalidate neighbor searcher and neighbor lists. It is users
    //! responsibility to call ParticleSystemData3::BuildNeighborSearcher and
    //! ParticleSystemData3::BuildNeighborLists to refresh those data.
    //!
    //! \param[in]  newPositions  The new positions.
    //! \param[in]  newVelocities The new velocities.
    //! \param[in]  newForces     The new forces.
    //!
    void AddParticles(const ConstArrayView1<Vector3D>& newPositions,
                      const ConstArrayView1<Vector3D>& newVelocities =
                          ConstArrayView1<Vector3D>(),
                      const ConstArrayView1<Vector3D>& newForces =
                          ConstArrayView1<Vector3D>());

    //!
    //! \brief      Returns neighbor searcher.
    //!
    //! This function returns currently set neighbor searcher object. By
    //! default, PointParallelHashGridSearcher3 is used.
    //!
    //! \return     Current neighbor searcher.
    //!
    const PointNeighborSearcher3Ptr& GetNeighborSearcher() const;

    //! Sets neighbor searcher.
    void SetNeighborSearcher(
        const PointNeighborSearcher3Ptr& newNeighborSearcher);

    //!
    //! \brief      Returns neighbor lists.
    //!
    //! This function returns neighbor lists which is available after calling
    //! PointParallelHashGridSearcher3::BuildNeighborLists. Each list stores
    //! indices of the neighbors.
    //!
    //! \return     Neighbor lists.
    //!
    const std::vector<std::vector<size_t>>& GetNeighborLists() const;

    //! Builds neighbor searcher with given search radius.
    void BuildNeighborSearcher(double maxSearchRadius);

    //! Builds neighbor lists with given search radius.
    void BuildNeighborLists(double maxSearchRadius);

    //! Serializes this particle system data to the buffer.
    void Serialize(std::vector<uint8_t>* buffer) const override;

    //! Deserializes this particle system data from the buffer.
    void Deserialize(const std::vector<uint8_t>& buffer) override;

    //! Copies from other particle system data.
    void Set(const ParticleSystemData3& other);

 protected:
    void SerializeParticleSystemData(
        flatbuffers::FlatBufferBuilder* builder,
        flatbuffers::Offset<fbs::ParticleSystemData3>* fbsParticleSystemData)
        const;

    void DeserializeParticleSystemData(
        const fbs::ParticleSystemData3* fbsParticleSystemData);

 private:
    double m_radius = 1e-3;
    double m_mass = 1e-3;
    size_t m_numberOfParticles = 0;
    size_t m_positionIdx = 0;
    size_t m_velocityIdx = 0;
    size_t m_forceIdx = 0;

    std::vector<ScalarData> m_scalarDataList;
    std::vector<VectorData> m_vectorDataList;

    PointNeighborSearcher3Ptr m_neighborSearcher;
    std::vector<std::vector<size_t>> m_neighborLists;
};

//! Shared pointer type of ParticleSystemData3.
using ParticleSystemData3Ptr = std::shared_ptr<ParticleSystemData3>;
}  // namespace CubbyFlow

#endif