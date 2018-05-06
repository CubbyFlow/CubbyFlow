/*************************************************************************
> File Name: ParticleSystemData2.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 2-D particle system data.
> Created Time: 2017/04/28
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PARTICLE_SYSTEM_DATA2_H
#define CUBBYFLOW_PARTICLE_SYSTEM_DATA2_H

#include <Core/Array/Array1.h>
#include <Core/Searcher/PointNeighborSearcher2.h>
#include <Core/Utils/Serialization.h>
#include <Core/Vector/Vector2.h>

#include <memory>
#include <vector>

#ifndef CUBBYFLOW_DOXYGEN

namespace flatbuffers
{
	class FlatBufferBuilder;
	template <typename T> struct Offset;
}

namespace CubbyFlow
{
	namespace fbs
	{
		struct ParticleSystemData2;
	}
}

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

		//! Destructor.
		virtual ~ParticleSystemData2();

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
		size_t AddVectorData(const Vector2D& initialVal = Vector2D());

		//! Returns the radius of the particles.
		double GetRadius() const;

		//! Sets the radius of the particles.
		virtual void SetRadius(double newRadius);

		//! Returns the mass of the particles.
		double GetMass() const;

		//! Sets the mass of the particles.
		virtual void SetMass(double newMass);

		//! Returns the position array (immutable).
		ConstArrayAccessor1<Vector2D> GetPositions() const;

		//! Returns the position array (mutable).
		ArrayAccessor1<Vector2D> GetPositions();

		//! Returns the velocity array (immutable).
		ConstArrayAccessor1<Vector2D> GetVelocities() const;

		//! Returns the velocity array (mutable).
		ArrayAccessor1<Vector2D> GetVelocities();

		//! Returns the force array (immutable).
		ConstArrayAccessor1<Vector2D> GetForces() const;

		//! Returns the force array (mutable).
		ArrayAccessor1<Vector2D> GetForces();

		//! Returns custom scalar data layer at given index (immutable).
		ConstArrayAccessor1<double> ScalarDataAt(size_t idx) const;

		//! Returns custom scalar data layer at given index (mutable).
		ArrayAccessor1<double> ScalarDataAt(size_t idx);

		//! Returns custom vector data layer at given index (immutable).
		ConstArrayAccessor1<Vector2D> VectorDataAt(size_t idx) const;

		//! Returns custom vector data layer at given index (mutable).
		ArrayAccessor1<Vector2D> VectorDataAt(size_t idx);

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
		void AddParticle(
			const Vector2D& newPosition,
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
		void AddParticles(
			const ConstArrayAccessor1<Vector2D>& newPositions,
			const ConstArrayAccessor1<Vector2D>& newVelocities = ConstArrayAccessor1<Vector2D>(),
			const ConstArrayAccessor1<Vector2D>& newForces = ConstArrayAccessor1<Vector2D>());

		//!
		//! \brief      Returns neighbor searcher.
		//!
		//! This function returns currently set neighbor searcher object. By
		//! default, PointParallelHashGridSearcher2 is used.
		//!
		//! \return     Current neighbor searcher.
		//!
		const PointNeighborSearcher2Ptr& GetNeighborSearcher() const;

		//! Sets neighbor searcher.
		void SetNeighborSearcher(const PointNeighborSearcher2Ptr& newNeighborSearcher);

		//!
		//! \brief      Returns neighbor lists.
		//!
		//! This function returns neighbor lists which is available after calling
		//! PointParallelHashGridSearcher2::BuildNeighborLists. Each list stores
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
		void Set(const ParticleSystemData2& other);

		//! Copies from other particle system data.
		ParticleSystemData2& operator=(const ParticleSystemData2& other);

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
		size_t m_positionIdx;
		size_t m_velocityIdx;
		size_t m_forceIdx;

		std::vector<ScalarData> m_scalarDataList;
		std::vector<VectorData> m_vectorDataList;

		PointNeighborSearcher2Ptr m_neighborSearcher;
		std::vector<std::vector<size_t>> m_neighborLists;
	};

	//! Shared pointer type of ParticleSystemData2.
	using ParticleSystemData2Ptr = std::shared_ptr<ParticleSystemData2>;
}

#endif