// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Particle/ParticleSystemData3.hpp>
#include <Core/Searcher/PointNeighborSearcher3.hpp>
#include <Core/Searcher/PointParallelHashGridSearcher3.hpp>
#include <Core/Utils/Factory.hpp>
#include <Core/Utils/FlatbuffersHelper.hpp>
#include <Core/Utils/Logging.hpp>
#include <Core/Utils/Parallel.hpp>
#include <Core/Utils/Timer.hpp>
#include <Core/Vector/Vector3.hpp>

#include <Flatbuffers/generated/ParticleSystemData3_generated.h>

namespace CubbyFlow
{
static const size_t DEFAULT_HASH_GRID_RESOLUTION = 64;

ParticleSystemData3::ParticleSystemData3() : ParticleSystemData3{ 0 }
{
    // Do nothing
}

ParticleSystemData3::ParticleSystemData3(size_t NumberOfParticles)
{
    m_positionIdx = AddVectorData();
    m_velocityIdx = AddVectorData();
    m_forceIdx = AddVectorData();

    // Use PointParallelHashGridSearcher3 by default
    m_neighborSearcher = std::make_shared<PointParallelHashGridSearcher3>(
        DEFAULT_HASH_GRID_RESOLUTION, DEFAULT_HASH_GRID_RESOLUTION,
        DEFAULT_HASH_GRID_RESOLUTION, 2.0 * m_radius);

    Resize(NumberOfParticles);
}

ParticleSystemData3::ParticleSystemData3(const ParticleSystemData3& other)
{
    Set(other);
}

ParticleSystemData3& ParticleSystemData3::operator=(
    const ParticleSystemData3& other)
{
    Set(other);
    return *this;
}

void ParticleSystemData3::Resize(size_t newNumberOfParticles)
{
    m_numberOfParticles = newNumberOfParticles;

    for (auto& attr : m_scalarDataList)
    {
        attr.Resize(newNumberOfParticles, 0.0);
    }

    for (auto& attr : m_vectorDataList)
    {
        attr.Resize(newNumberOfParticles, Vector3D{});
    }
}

size_t ParticleSystemData3::GetNumberOfParticles() const
{
    return m_numberOfParticles;
}

size_t ParticleSystemData3::AddScalarData(double initialVal)
{
    const size_t attrIdx = m_scalarDataList.size();
    m_scalarDataList.emplace_back(GetNumberOfParticles(), initialVal);
    return attrIdx;
}

size_t ParticleSystemData3::AddVectorData(const Vector3D& initialVal)
{
    const size_t attrIdx = m_vectorDataList.size();
    m_vectorDataList.emplace_back(GetNumberOfParticles(), initialVal);
    return attrIdx;
}

double ParticleSystemData3::GetRadius() const
{
    return m_radius;
}

void ParticleSystemData3::SetRadius(double newRadius)
{
    m_radius = std::max(newRadius, 0.0);
}

double ParticleSystemData3::GetMass() const
{
    return m_mass;
}

void ParticleSystemData3::SetMass(double newMass)
{
    m_mass = std::max(newMass, 0.0);
}

ConstArrayAccessor1<Vector3D> ParticleSystemData3::GetPositions() const
{
    return VectorDataAt(m_positionIdx);
}

ArrayAccessor1<Vector3D> ParticleSystemData3::GetPositions()
{
    return VectorDataAt(m_positionIdx);
}

ConstArrayAccessor1<Vector3D> ParticleSystemData3::GetVelocities() const
{
    return VectorDataAt(m_velocityIdx);
}

ArrayAccessor1<Vector3D> ParticleSystemData3::GetVelocities()
{
    return VectorDataAt(m_velocityIdx);
}

ConstArrayAccessor1<Vector3D> ParticleSystemData3::GetForces() const
{
    return VectorDataAt(m_forceIdx);
}

ArrayAccessor1<Vector3D> ParticleSystemData3::GetForces()
{
    return VectorDataAt(m_forceIdx);
}

ConstArrayAccessor1<double> ParticleSystemData3::ScalarDataAt(size_t idx) const
{
    return m_scalarDataList[idx].ConstAccessor();
}

ArrayAccessor1<double> ParticleSystemData3::ScalarDataAt(size_t idx)
{
    return m_scalarDataList[idx].Accessor();
}

ConstArrayAccessor1<Vector3D> ParticleSystemData3::VectorDataAt(
    size_t idx) const
{
    return m_vectorDataList[idx].ConstAccessor();
}

ArrayAccessor1<Vector3D> ParticleSystemData3::VectorDataAt(size_t idx)
{
    return m_vectorDataList[idx].Accessor();
}

void ParticleSystemData3::AddParticle(const Vector3D& newPosition,
                                      const Vector3D& newVelocity,
                                      const Vector3D& newForce)
{
    const Array1<Vector3D> newPositions = { newPosition };
    const Array1<Vector3D> newVelocities = { newVelocity };
    const Array1<Vector3D> newForces = { newForce };

    AddParticles(newPositions.ConstAccessor(), newVelocities.ConstAccessor(),
                 newForces.ConstAccessor());
}

void ParticleSystemData3::AddParticles(
    const ConstArrayAccessor1<Vector3D>& newPositions,
    const ConstArrayAccessor1<Vector3D>& newVelocities,
    const ConstArrayAccessor1<Vector3D>& newForces)
{
    if (newVelocities.size() > 0 && newVelocities.size() != newPositions.size())
    {
        throw std::invalid_argument{
            "newVelocities.size() > 0 && newVelocities.size() != "
            "newPositions.size()"
        };
    }

    if (newForces.size() > 0 && newForces.size() != newPositions.size())
    {
        throw std::invalid_argument{
            "newForces.size() > 0 && newForces.size() != newPositions.size()"
        };
    }

    size_t oldNumberOfParticles = GetNumberOfParticles();
    const size_t newNumberOfParticles =
        oldNumberOfParticles + newPositions.size();

    Resize(newNumberOfParticles);

    auto pos = GetPositions();
    auto vel = GetVelocities();
    auto frc = GetForces();

    ParallelFor(ZERO_SIZE, newPositions.size(), [&](size_t i) {
        pos[i + oldNumberOfParticles] = newPositions[i];
    });

    if (newVelocities.size() > 0)
    {
        ParallelFor(ZERO_SIZE, newPositions.size(), [&](size_t i) {
            vel[i + oldNumberOfParticles] = newVelocities[i];
        });
    }

    if (newForces.size() > 0)
    {
        ParallelFor(ZERO_SIZE, newPositions.size(), [&](size_t i) {
            frc[i + oldNumberOfParticles] = newForces[i];
        });
    }
}

const PointNeighborSearcher3Ptr& ParticleSystemData3::GetNeighborSearcher()
    const
{
    return m_neighborSearcher;
}

void ParticleSystemData3::SetNeighborSearcher(
    const PointNeighborSearcher3Ptr& newNeighborSearcher)
{
    m_neighborSearcher = newNeighborSearcher;
}

const std::vector<std::vector<size_t>>& ParticleSystemData3::GetNeighborLists()
    const
{
    return m_neighborLists;
}

void ParticleSystemData3::BuildNeighborSearcher(double maxSearchRadius)
{
    const Timer timer;

    // Use PointParallelHashGridSearcher3 by default
    m_neighborSearcher = std::make_shared<PointParallelHashGridSearcher3>(
        DEFAULT_HASH_GRID_RESOLUTION, DEFAULT_HASH_GRID_RESOLUTION,
        DEFAULT_HASH_GRID_RESOLUTION, 2.0 * maxSearchRadius);

    m_neighborSearcher->Build(GetPositions());

    CUBBYFLOW_INFO << "Building neighbor searcher took: "
                   << timer.DurationInSeconds() << " seconds";
}

void ParticleSystemData3::BuildNeighborLists(double maxSearchRadius)
{
    const Timer timer;

    m_neighborLists.resize(GetNumberOfParticles());

    auto points = GetPositions();

    for (size_t i = 0; i < GetNumberOfParticles(); ++i)
    {
        Vector3D origin = points[i];
        m_neighborLists[i].clear();

        m_neighborSearcher->ForEachNearbyPoint(
            origin, maxSearchRadius, [&](size_t j, const Vector3D&) {
                if (i != j)
                {
                    m_neighborLists[i].push_back(j);
                }
            });
    }

    CUBBYFLOW_INFO << "Building neighbor list took: "
                   << timer.DurationInSeconds() << " seconds";
}

void ParticleSystemData3::Serialize(std::vector<uint8_t>* buffer) const
{
    flatbuffers::FlatBufferBuilder builder{ 1024 };
    flatbuffers::Offset<fbs::ParticleSystemData3> fbsParticleSystemData;

    SerializeParticleSystemData(&builder, &fbsParticleSystemData);

    builder.Finish(fbsParticleSystemData);

    uint8_t* buf = builder.GetBufferPointer();
    const size_t size = builder.GetSize();

    buffer->resize(size);
    memcpy(buffer->data(), buf, size);
}

void ParticleSystemData3::Deserialize(const std::vector<uint8_t>& buffer)
{
    const fbs::ParticleSystemData3* fbsParticleSystemData =
        fbs::GetParticleSystemData3(buffer.data());
    DeserializeParticleSystemData(fbsParticleSystemData);
}

void ParticleSystemData3::Set(const ParticleSystemData3& other)
{
    m_radius = other.m_radius;
    m_mass = other.m_mass;
    m_positionIdx = other.m_positionIdx;
    m_velocityIdx = other.m_velocityIdx;
    m_forceIdx = other.m_forceIdx;
    m_numberOfParticles = other.m_numberOfParticles;

    for (const auto& attr : other.m_scalarDataList)
    {
        m_scalarDataList.emplace_back(attr);
    }

    for (const auto& attr : other.m_vectorDataList)
    {
        m_vectorDataList.emplace_back(attr);
    }

    m_neighborSearcher = other.m_neighborSearcher->Clone();
    m_neighborLists = other.m_neighborLists;
}

void ParticleSystemData3::SerializeParticleSystemData(
    flatbuffers::FlatBufferBuilder* builder,
    flatbuffers::Offset<fbs::ParticleSystemData3>* fbsParticleSystemData) const
{
    // Copy data
    std::vector<flatbuffers::Offset<fbs::ScalarParticleData3>> scalarDataList;
    for (const auto& scalarData : m_scalarDataList)
    {
        flatbuffers::Offset<fbs::ScalarParticleData3> fbsScalarData =
            fbs::CreateScalarParticleData3(
                *builder,
                builder->CreateVector(scalarData.data(), scalarData.size()));
        scalarDataList.push_back(fbsScalarData);
    }
    const flatbuffers::Offset<
        flatbuffers::Vector<flatbuffers::Offset<fbs::ScalarParticleData3>>>
        fbsScalarDataList = builder->CreateVector(scalarDataList);

    std::vector<flatbuffers::Offset<fbs::VectorParticleData3>> vectorDataList;
    for (const auto& vectorData : m_vectorDataList)
    {
        std::vector<fbs::Vector3D> newVectorData;
        for (const auto& v : vectorData)
        {
            newVectorData.push_back(CubbyFlowToFlatbuffers(v));
        }

        flatbuffers::Offset<fbs::VectorParticleData3> fbsVectorData =
            CreateVectorParticleData3(
                *builder, builder->CreateVectorOfStructs(newVectorData.data(),
                                                         newVectorData.size()));
        vectorDataList.push_back(fbsVectorData);
    }
    const flatbuffers::Offset<
        flatbuffers::Vector<flatbuffers::Offset<fbs::VectorParticleData3>>>
        fbsVectorDataList = builder->CreateVector(vectorDataList);

    // Copy neighbor searcher
    const flatbuffers::Offset<flatbuffers::String> neighborSearcherType =
        builder->CreateString(m_neighborSearcher->TypeName());
    std::vector<uint8_t> neighborSearcherSerialized;
    m_neighborSearcher->Serialize(&neighborSearcherSerialized);
    const flatbuffers::Offset<fbs::PointNeighborSearcherSerialized3>
        fbsNeighborSearcher = fbs::CreatePointNeighborSearcherSerialized3(
            *builder, neighborSearcherType,
            builder->CreateVector(neighborSearcherSerialized.data(),
                                  neighborSearcherSerialized.size()));

    // Copy neighbor lists
    std::vector<flatbuffers::Offset<fbs::ParticleNeighborList3>> neighborLists;
    for (const auto& neighbors : m_neighborLists)
    {
        std::vector<uint64_t> neighbors64(neighbors.begin(), neighbors.end());
        flatbuffers::Offset<fbs::ParticleNeighborList3> fbsNeighborList =
            fbs::CreateParticleNeighborList3(
                *builder,
                builder->CreateVector(neighbors64.data(), neighbors64.size()));
        neighborLists.push_back(fbsNeighborList);
    }

    const flatbuffers::Offset<
        flatbuffers::Vector<flatbuffers::Offset<fbs::ParticleNeighborList3>>>
        fbsNeighborLists = builder->CreateVector(neighborLists);

    // Copy the searcher
    *fbsParticleSystemData = fbs::CreateParticleSystemData3(
        *builder, m_radius, m_mass, m_positionIdx, m_velocityIdx, m_forceIdx,
        fbsScalarDataList, fbsVectorDataList, fbsNeighborSearcher,
        fbsNeighborLists);
}

void ParticleSystemData3::DeserializeParticleSystemData(
    const fbs::ParticleSystemData3* fbsParticleSystemData)
{
    m_scalarDataList.clear();
    m_vectorDataList.clear();

    // Copy scalars
    m_radius = fbsParticleSystemData->radius();
    m_mass = fbsParticleSystemData->mass();
    m_positionIdx = static_cast<size_t>(fbsParticleSystemData->positionIdx());
    m_velocityIdx = static_cast<size_t>(fbsParticleSystemData->velocityIdx());
    m_forceIdx = static_cast<size_t>(fbsParticleSystemData->forceIdx());

    // Copy data
    const flatbuffers::Vector<flatbuffers::Offset<fbs::ScalarParticleData3>>*
        fbsScalarDataList = fbsParticleSystemData->scalarDataList();
    for (const auto fbsScalarData : (*fbsScalarDataList))
    {
        const flatbuffers::Vector<double>* data = fbsScalarData->data();

        m_scalarDataList.emplace_back(data->size());

        Array<double, 1>& newData = *(m_scalarDataList.rbegin());

        for (uint32_t i = 0; i < data->size(); ++i)
        {
            newData[i] = data->Get(i);
        }
    }

    const flatbuffers::Vector<flatbuffers::Offset<fbs::VectorParticleData3>>*
        fbsVectorDataList = fbsParticleSystemData->vectorDataList();
    for (const auto fbsVectorData : (*fbsVectorDataList))
    {
        const flatbuffers::Vector<const fbs::Vector3D*>* data =
            fbsVectorData->data();

        m_vectorDataList.emplace_back(data->size());

        Array<Vector<double, 3>, 1>& newData = *(m_vectorDataList.rbegin());

        for (uint32_t i = 0; i < data->size(); ++i)
        {
            newData[i] = FlatbuffersToCubbyFlow(*data->Get(i));
        }
    }

    m_numberOfParticles = m_vectorDataList[0].size();

    // Copy neighbor searcher
    const fbs::PointNeighborSearcherSerialized3* fbsNeighborSearcher =
        fbsParticleSystemData->neighborSearcher();
    m_neighborSearcher = Factory::BuildPointNeighborSearcher3(
        fbsNeighborSearcher->type()->c_str());
    const std::vector<uint8_t> neighborSearcherSerialized{
        fbsNeighborSearcher->data()->begin(), fbsNeighborSearcher->data()->end()
    };
    m_neighborSearcher->Deserialize(neighborSearcherSerialized);

    // Copy neighbor list
    const flatbuffers::Vector<flatbuffers::Offset<fbs::ParticleNeighborList3>>*
        fbsNeighborLists = fbsParticleSystemData->neighborLists();
    m_neighborLists.resize(fbsNeighborLists->size());

    for (uint32_t i = 0; i < fbsNeighborLists->size(); ++i)
    {
        const flatbuffers::Vector<
            flatbuffers::Offset<fbs::ParticleNeighborList3>>::return_type
            fbsNeighborList = fbsNeighborLists->Get(i);
        m_neighborLists[i].resize(fbsNeighborList->data()->size());
        std::transform(fbsNeighborList->data()->begin(),
                       fbsNeighborList->data()->end(),
                       m_neighborLists[i].begin(),
                       [](uint64_t val) { return static_cast<size_t>(val); });
    }
}
}  // namespace CubbyFlow