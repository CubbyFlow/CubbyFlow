/*************************************************************************
> File Name: ParticleSystemData.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: ParticleSystemData functions for CubbyFlow Python API.
> Created Time: 2018/02/08
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/Particle/ParticleSystemData.h>
#include <API/Python/Utils/pybind11Utils.h>
#include <Core/Particle/ParticleSystemData2.h>
#include <Core/Particle/ParticleSystemData3.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddParticleSystemData2(pybind11::module& m)
{
	pybind11::class_<ParticleSystemData2, ParticleSystemData2Ptr, Serializable>(m, "ParticleSystemData2",
		R"pbdoc(
			This class is the key data structure for storing particle system data. A
			single particle has position, velocity, and force attributes by default. But
			it can also have additional custom scalar or vector attributes.
		)pbdoc")
	.def(pybind11::init<size_t>(),
		R"pbdoc(
			Constructs particle system data with given number of particles.
		)pbdoc",
		pybind11::arg("numberOfParticles") = 0)
	.def("Resize", &ParticleSystemData2::Resize,
		R"pbdoc(
			Resizes the number of particles of the container.

			This function will resize internal containers to store newly given
			number of particles including custom data layers. However, this will
			invalidate neighbor searcher and neighbor lists. It is users
			responsibility to call ParticleSystemData2::GetBuildNeighborSearcher and
			ParticleSystemData2::BuildNeighborLists to refresh those data.

			Parameters
			----------
			- newNumberOfParticles : New number of particles.
		)pbdoc",
		pybind11::arg("newNumberOfParticles"))
	.def_property_readonly("numberOfParticles", &ParticleSystemData2::GetNumberOfParticles,
		R"pbdoc(
			The number of particles.
		)pbdoc")
	.def("AddScalarData", &ParticleSystemData2::AddScalarData,
		R"pbdoc(
			Adds a scalar data layer and returns its index.

			This function adds a new scalar data layer to the system. It can be used
			for adding a scalar attribute, such as temperature, to the particles.

			Parameters
			----------
			- initialVal : Initial value of the new scalar data.
		)pbdoc",
		pybind11::arg("initialVal") = 0.0)
	.def("AddVectorData", [](ParticleSystemData2& instance, pybind11::object object)
	{
		return instance.AddVectorData(ObjectToVector2D(object));
	},
		R"pbdoc(
			Adds a vector data layer and returns its index.

			This function adds a new vector data layer to the system. It can be used
			for adding a vector attribute, such as vortex, to the particles.

			Parameters
			----------
			- initialVal : Initial value of the new vector data.
		)pbdoc",
		pybind11::arg("initialVal") = Vector2D())
	.def_property("radius", &ParticleSystemData2::GetRadius, &ParticleSystemData2::SetRadius,
		R"pbdoc(
			The radius of the particles.
		)pbdoc")
	.def_property("mass", &ParticleSystemData2::GetMass, &ParticleSystemData2::SetMass,
		R"pbdoc(
			The mass of a particle.
		)pbdoc")
	.def_property_readonly("positions", [](ParticleSystemData2& instance)
	{
		return instance.GetPositions();
	},
		R"pbdoc(
			Returns the position array (mutable).
		)pbdoc")
	.def_property_readonly("velocities", [](ParticleSystemData2& instance)
	{
		return instance.GetVelocities();
	},
		R"pbdoc(
			Returns the velocity array (mutable).
		)pbdoc")
	.def_property_readonly("forces", [](ParticleSystemData2& instance)
	{
		return instance.GetForces();
	},
		R"pbdoc(
			Returns the force array (mutable).
		)pbdoc")
	.def("ScalarDataAt", [](ParticleSystemData2& instance, size_t idx)
	{
		return instance.ScalarDataAt(idx);
	},
		R"pbdoc(
			Returns custom scalar data layer at given index (mutable).
		)pbdoc")
	.def("VectorDataAt", [](ParticleSystemData2& instance, size_t idx)
	{
		return instance.VectorDataAt(idx);
	},
		R"pbdoc(
			Returns custom vector data layer at given index (mutable).
		)pbdoc")
	.def("AddParticle", [](ParticleSystemData2& instance, pybind11::object p, pybind11::object v, pybind11::object f)
	{
		instance.AddParticle(ObjectToVector2D(p), ObjectToVector2D(v), ObjectToVector2D(f));
	},
		R"pbdoc(
			Adds a particle to the data structure.

			This function will add a single particle to the data structure. For
			custom data layers, zeros will be assigned for new particles.
			However, this will invalidate neighbor searcher and neighbor lists. It
			is users responsibility to call
			ParticleSystemData2::GetBuildNeighborSearcher and
			ParticleSystemData2::BuildNeighborLists to refresh those data.

			Parameters
			----------
			- newPosition : The new position.
			- newVelocity : The new velocity.
			- newForce    : The new force.
		)pbdoc",
		pybind11::arg("newPosition"),
		pybind11::arg("newVelocity") = Vector2D(),
		pybind11::arg("newForce") = Vector2D())
	.def("AddParticles", [](ParticleSystemData2& instance, pybind11::list ps, pybind11::list vs, pybind11::list fs)
	{
		if (vs.size() > 0 && vs.size() != ps.size())
		{
			throw std::invalid_argument("Wrong input size for velocities list.");
		}
		if (fs.size() > 0 && fs.size() != ps.size())
		{
			throw std::invalid_argument("Wrong input size for velocities list.");
		}

		Array1<Vector2D> positions;
		Array1<Vector2D> velocities;
		Array1<Vector2D> forces;

		for (size_t i = 0; i < ps.size(); ++i)
		{
			positions.Append(ObjectToVector2D(ps[i]));
			
			if (vs.size() > 0)
			{
				velocities.Append(ObjectToVector2D(vs[i]));
			}
			if (fs.size() > 0)
			{
				forces.Append(ObjectToVector2D(fs[i]));
			}
		}

		instance.AddParticles(positions.ConstAccessor(), velocities.ConstAccessor(), forces.ConstAccessor());
	},
		R"pbdoc(
			Adds particles to the data structure.

			This function will add particles to the data structure. For custom data
			layers, zeros will be assigned for new particles. However, this will
			invalidate neighbor searcher and neighbor lists. It is users
			responsibility to call ParticleSystemData2::GetBuildNeighborSearcher and
			ParticleSystemData2::BuildNeighborLists to refresh those data.

			Parameters
			----------
			- newPositions  : The new positions.
			- newVelocities : The new velocities.
			- newForces     : The new forces.
		)pbdoc")
	.def_property("neighborSearcher", &ParticleSystemData2::GetNeighborSearcher, &ParticleSystemData2::SetNeighborSearcher,
		R"pbdoc(
			The neighbor searcher.

			This property returns currently set neighbor searcher object. By
			default, PointParallelHashGridSearcher2 is used.
		)pbdoc")
	.def_property_readonly("neighborLists", &ParticleSystemData2::GetNeighborLists,
		R"pbdoc(
			The neighbor lists.

			This property returns neighbor lists which is available after calling
			PointParallelHashGridSearcher2::GetBuildNeighborLists. Each list stores
			indices of the neighbors.
		)pbdoc")
	.def("Set", [](ParticleSystemData2& instance, const ParticleSystemData2Ptr& other)
	{
		instance.Set(*other);
	},
		R"pbdoc(
			Copies from other particle system data.
		)pbdoc");
}

void AddParticleSystemData3(pybind11::module& m)
{
	pybind11::class_<ParticleSystemData3, ParticleSystemData3Ptr, Serializable>(m, "ParticleSystemData3",
		R"pbdoc(
			This class is the key data structure for storing particle system data. A
			single particle has position, velocity, and force attributes by default. But
			it can also have additional custom scalar or vector attributes.
		)pbdoc")
	.def(pybind11::init<size_t>(),
		R"pbdoc(
			Constructs particle system data with given number of particles.
		)pbdoc",
		pybind11::arg("numberOfParticles") = 0)
	.def("Resize", &ParticleSystemData3::Resize,
		R"pbdoc(
			Resizes the number of particles of the container.

			This function will resize internal containers to store newly given
			number of particles including custom data layers. However, this will
			invalidate neighbor searcher and neighbor lists. It is users
			responsibility to call ParticleSystemData3::GetBuildNeighborSearcher and
			ParticleSystemData3::BuildNeighborLists to refresh those data.

			Parameters
			----------
			- newNumberOfParticles : New number of particles.
		)pbdoc",
		pybind11::arg("newNumberOfParticles"))
	.def_property_readonly("numberOfParticles", &ParticleSystemData3::GetNumberOfParticles,
		R"pbdoc(
			The number of particles.
		)pbdoc")
	.def("AddScalarData", &ParticleSystemData3::AddScalarData,
		R"pbdoc(
			Adds a scalar data layer and returns its index.

			This function adds a new scalar data layer to the system. It can be used
			for adding a scalar attribute, such as temperature, to the particles.

			Parameters
			----------
			- initialVal : Initial value of the new scalar data.
		)pbdoc",
		pybind11::arg("initialVal") = 0.0)
	.def("AddVectorData", [](ParticleSystemData3& instance, pybind11::object object)
	{
		return instance.AddVectorData(ObjectToVector3D(object));
	},
		R"pbdoc(
			Adds a vector data layer and returns its index.

			This function adds a new vector data layer to the system. It can be used
			for adding a vector attribute, such as vortex, to the particles.

			Parameters
			----------
			- initialVal : Initial value of the new vector data.
		)pbdoc",
		pybind11::arg("initialVal") = Vector3D())
	.def_property("radius", &ParticleSystemData3::GetRadius, &ParticleSystemData3::SetRadius,
		R"pbdoc(
			The radius of the particles.
		)pbdoc")
	.def_property("mass", &ParticleSystemData3::GetMass, &ParticleSystemData3::SetMass,
		R"pbdoc(
			The mass of a particle.
		)pbdoc")
	.def_property_readonly("positions", [](ParticleSystemData3& instance)
	{
		return instance.GetPositions();
	},
		R"pbdoc(
			Returns the position array (mutable).
		)pbdoc")
	.def_property_readonly("velocities", [](ParticleSystemData3& instance)
	{
		return instance.GetVelocities();
	},
		R"pbdoc(
			Returns the velocity array (mutable).
		)pbdoc")
	.def_property_readonly("forces", [](ParticleSystemData3& instance)
	{
		return instance.GetForces();
	},
		R"pbdoc(
			Returns the force array (mutable).
		)pbdoc")
	.def("ScalarDataAt", [](ParticleSystemData3& instance, size_t idx)
	{
		return instance.ScalarDataAt(idx);
	},
		R"pbdoc(
			Returns custom scalar data layer at given index (mutable).
		)pbdoc")
	.def("VectorDataAt", [](ParticleSystemData3& instance, size_t idx)
	{
		return instance.VectorDataAt(idx);
	},
		R"pbdoc(
			Returns custom vector data layer at given index (mutable).
		)pbdoc")
	.def("AddParticle", [](ParticleSystemData3& instance, pybind11::object p, pybind11::object v, pybind11::object f)
	{
		instance.AddParticle(ObjectToVector3D(p), ObjectToVector3D(v), ObjectToVector3D(f));
	},
		R"pbdoc(
			Adds a particle to the data structure.

			This function will add a single particle to the data structure. For
			custom data layers, zeros will be assigned for new particles.
			However, this will invalidate neighbor searcher and neighbor lists. It
			is users responsibility to call
			ParticleSystemData3::GetBuildNeighborSearcher and
			ParticleSystemData3::BuildNeighborLists to refresh those data.

			Parameters
			----------
			- newPosition : The new position.
			- newVelocity : The new velocity.
			- newForce    : The new force.
		)pbdoc",
		pybind11::arg("newPosition"),
		pybind11::arg("newVelocity") = Vector3D(),
		pybind11::arg("newForce") = Vector3D())
	.def("AddParticles", [](ParticleSystemData3& instance, pybind11::list ps, pybind11::list vs, pybind11::list fs)
	{
		if (vs.size() > 0 && vs.size() != ps.size())
		{
			throw std::invalid_argument("Wrong input size for velocities list.");
		}
		if (fs.size() > 0 && fs.size() != ps.size())
		{
			throw std::invalid_argument("Wrong input size for velocities list.");
		}

		Array1<Vector3D> positions;
		Array1<Vector3D> velocities;
		Array1<Vector3D> forces;

		for (size_t i = 0; i < ps.size(); ++i)
		{
			positions.Append(ObjectToVector3D(ps[i]));
			
			if (vs.size() > 0)
			{
				velocities.Append(ObjectToVector3D(vs[i]));
			}
			if (fs.size() > 0)
			{
				forces.Append(ObjectToVector3D(fs[i]));
			}
		}

		instance.AddParticles(positions.ConstAccessor(), velocities.ConstAccessor(), forces.ConstAccessor());
	},
		R"pbdoc(
			Adds particles to the data structure.

			This function will add particles to the data structure. For custom data
			layers, zeros will be assigned for new particles. However, this will
			invalidate neighbor searcher and neighbor lists. It is users
			responsibility to call ParticleSystemData3::GetBuildNeighborSearcher and
			ParticleSystemData3::BuildNeighborLists to refresh those data.

			Parameters
			----------
			- newPositions  : The new positions.
			- newVelocities : The new velocities.
			- newForces     : The new forces.
		)pbdoc")
	.def_property("neighborSearcher", &ParticleSystemData3::GetNeighborSearcher, &ParticleSystemData3::SetNeighborSearcher,
		R"pbdoc(
			The neighbor searcher.

			This property returns currently set neighbor searcher object. By
			default, PointParallelHashGridSearcher2 is used.
		)pbdoc")
	.def_property_readonly("neighborLists", &ParticleSystemData3::GetNeighborLists,
		R"pbdoc(
			The neighbor lists.

			This property returns neighbor lists which is available after calling
			PointParallelHashGridSearcher2::buildNeighborLists. Each list stores
			indices of the neighbors.
		)pbdoc")
	.def("Set", [](ParticleSystemData3& instance, const ParticleSystemData3Ptr& other)
	{
		instance.Set(*other);
	},
		R"pbdoc(
			Copies from other particle system data.
		)pbdoc");
}