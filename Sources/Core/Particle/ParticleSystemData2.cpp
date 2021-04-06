// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Particle/ParticleSystemData2.hpp>
#include <Core/Searcher/PointNeighborSearcher.hpp>
#include <Core/Searcher/PointParallelHashGridSearcher2.hpp>
#include <Core/Utils/Factory.hpp>
#include <Core/Utils/FlatbuffersHelper.hpp>
#include <Core/Utils/Logging.hpp>
#include <Core/Utils/Parallel.hpp>
#include <Core/Utils/Timer.hpp>

#include <Flatbuffers/generated/ParticleSystemData2_generated.h>

namespace CubbyFlow
{
static const size_t DEFAULT_HASH_GRID_RESOLUTION = 64;

ParticleSystemData2::ParticleSystemData2() : ParticleSystemData2{ 0 }
{
    // Do nothing
}

ParticleSystemData2::ParticleSystemData2(size_t numberOfParticles)
{
    m_positionIdx = AddVectorData();
    m_velocityIdx = AddVectorData();
    m_forceIdx = AddVectorData();

    // Use PointParallelHashGridSearcher2 by default
    m_neighborSearcher = std::make_shared<PointParallelHashGridSearcher2>(
        DEFAULT_HASH_GRID_RESOLUTION, DEFAULT_HASH_GRID_RESOLUTION,
        2.0 * m_radius);

    Resize(numberOfParticles);
}

ParticleSystemData2::ParticleSystemData2(const ParticleSystemData2& other)
{
    Set(other);
}

ParticleSystemData2& ParticleSystemData2::operator=(
    const ParticleSystemData2& other)
{
    Set(other);
    return *this;
}

void ParticleSystemData2::Resize(size_t newNumberOfParticles)
{
    m_numberOfParticles = newNumberOfParticles;

    for (auto& attr : m_scalarDataList)
    {
        attr.Resize(newNumberOfParticles, 0.0);
    }

    for (auto& attr : m_vectorDataList)
    {
        attr.Resize(newNumberOfParticles, Vector2D{});
    }
}

size_t ParticleSystemData2::GetNumberOfParticles() const
{
    return m_numberOfParticles;
}

size_t ParticleSystemData2::AddScalarData(double initialVal)
{
    const size_t attrIdx = m_scalarDataList.size();
    m_scalarDataList.emplace_back(GetNumberOfParticles(), initialVal);
    return attrIdx;
}

size_t ParticleSystemData2::AddVectorData(const Vector2D& initialVal)
{
    const size_t attrIdx = m_vectorDataList.size();
    m_vectorDataList.emplace_back(GetNumberOfParticles(), initialVal);
    return attrIdx;
}

double ParticleSystemData2::GetRadius() const
{
    return m_radius;
}

void ParticleSystemData2::SetRadius(double newRadius)
{
    m_radius = std::max(newRadius, 0.0);
}

double ParticleSystemData2::GetMass() const
{
    return m_mass;
}

void ParticleSystemData2::SetMass(double newMass)
{
    m_mass = std::max(newMass, 0.0);
}

ConstArrayView1<Vector2D> ParticleSystemData2::Positions() const
{
    return VectorDataAt(m_positionIdx);
}

ArrayView1<Vector2D> ParticleSystemData2::Positions()
{
    return VectorDataAt(m_positionIdx);
}

ConstArrayView1<Vector2D> ParticleSystemData2::Velocities() const
{
    return VectorDataAt(m_velocityIdx);
}

ArrayView1<Vector2D> ParticleSystemData2::Velocities()
{
    return VectorDataAt(m_velocityIdx);
}

ConstArrayView1<Vector2D> ParticleSystemData2::Forces() const
{
    return VectorDataAt(m_forceIdx);
}

ArrayView1<Vector2D> ParticleSystemData2::Forces()
{
    return VectorDataAt(m_forceIdx);
}

ConstArrayView1<double> ParticleSystemData2::ScalarDataAt(size_t idx) const
{
    return ConstArrayView1<double>(m_scalarDataList[idx]);
}

ArrayView1<double> ParticleSystemData2::ScalarDataAt(size_t idx)
{
    return ArrayView1<double>(m_scalarDataList[idx]);
}

ConstArrayView1<Vector2D> ParticleSystemData2::VectorDataAt(size_t idx) const
{
    return ConstArrayView1<Vector2D>(m_vectorDataList[idx]);
}

ArrayView1<Vector2D> ParticleSystemData2::VectorDataAt(size_t idx)
{
    return ArrayView1<Vector2D>(m_vectorDataList[idx]);
}

void ParticleSystemData2::AddParticle(const Vector2D& newPosition,
                                      const Vector2D& newVelocity,
                                      const Vector2D& newForce)
{
    const Array1<Vector2D> newPositions = { newPosition };
    const Array1<Vector2D> newVelocities = { newVelocity };
    const Array1<Vector2D> newForces = { newForce };

    AddParticles(newPositions, newVelocities, newForces);
}

void ParticleSystemData2::AddParticles(
    const ConstArrayView1<Vector2D>& newPositions,
    const ConstArrayView1<Vector2D>& newVelocities,
    const ConstArrayView1<Vector2D>& newForces)
{
    if (newVelocities.Length() > 0 &&
        newVelocities.Length() != newPositions.Length())
    {
        throw std::invalid_argument{
            "newVelocities.Length() > 0 && newVelocities.Length() != "
            "newPositions.Length()"
        };
    }

    if (newForces.Length() > 0 && newForces.Length() != newPositions.Length())
    {
        throw std::invalid_argument{
            "newForces.Length() > 0 && newForces.Length() != "
            "newPositions.Length()"
        };
    }

    size_t oldNumberOfParticles = GetNumberOfParticles();
    const size_t newNumberOfParticles =
        oldNumberOfParticles + newPositions.Length();

    Resize(newNumberOfParticles);

    auto pos = Positions();
    auto vel = Velocities();
    auto frc = Forces();

    ParallelFor(ZERO_SIZE, newPositions.Length(), [&](size_t i) {
        pos[i + oldNumberOfParticles] = newPositions[i];
    });

    if (newVelocities.Length() > 0)
    {
        ParallelFor(ZERO_SIZE, newPositions.Length(), [&](size_t i) {
            vel[i + oldNumberOfParticles] = newVelocities[i];
        });
    }

    if (newForces.Length() > 0)
    {
        ParallelFor(ZERO_SIZE, newPositions.Length(), [&](size_t i) {
            frc[i + oldNumberOfParticles] = newForces[i];
        });
    }
}

const PointNeighborSearcher2Ptr& ParticleSystemData2::GetNeighborSearcher()
    const
{
    return m_neighborSearcher;
}

void ParticleSystemData2::SetNeighborSearcher(
    const PointNeighborSearcher2Ptr& newNeighborSearcher)
{
    m_neighborSearcher = newNeighborSearcher;
}

const std::vector<std::vector<size_t>>& ParticleSystemData2::GetNeighborLists()
    const
{
    return m_neighborLists;
}

void ParticleSystemData2::BuildNeighborSearcher(double maxSearchRadius)
{
    const Timer timer;

    // Use PointParallelHashGridSearcher2 by default
    m_neighborSearcher = std::make_shared<PointParallelHashGridSearcher2>(
        DEFAULT_HASH_GRID_RESOLUTION, DEFAULT_HASH_GRID_RESOLUTION,
        2.0 * maxSearchRadius);

    m_neighborSearcher->Build(Positions());

    CUBBYFLOW_INFO << "Building neighbor searcher took: "
                   << timer.DurationInSeconds() << " seconds";
}

void ParticleSystemData2::BuildNeighborLists(double maxSearchRadius)
{
    const Timer timer;

    m_neighborLists.resize(GetNumberOfParticles());

    auto points = Positions();

    for (size_t i = 0; i < GetNumberOfParticles(); ++i)
    {
        Vector2D origin = points[i];
        m_neighborLists[i].clear();

        m_neighborSearcher->ForEachNearbyPoint(
            origin, maxSearchRadius, [&](size_t j, const Vector2D&) {
                if (i != j)
                {
                    m_neighborLists[i].push_back(j);
                }
            });
    }

    CUBBYFLOW_INFO << "Building neighbor list took: "
                   << timer.DurationInSeconds() << " seconds";
}

void ParticleSystemData2::Serialize(std::vector<uint8_t>* buffer) const
{
    flatbuffers::FlatBufferBuilder builder{ 1024 };
    flatbuffers::Offset<fbs::ParticleSystemData2> fbsParticleSystemData;

    SerializeParticleSystemData(&builder, &fbsParticleSystemData);

    builder.Finish(fbsParticleSystemData);

    uint8_t* buf = builder.GetBufferPointer();
    const size_t size = builder.GetSize();

    buffer->resize(size);
    memcpy(buffer->data(), buf, size);
}

void ParticleSystemData2::Deserialize(const std::vector<uint8_t>& buffer)
{
    const fbs::ParticleSystemData2* fbsParticleSystemData =
        fbs::GetParticleSystemData2(buffer.data());
    DeserializeParticleSystemData(fbsParticleSystemData);
}

void ParticleSystemData2::Set(const ParticleSystemData2& other)
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

void ParticleSystemData2::SerializeParticleSystemData(
    flatbuffers::FlatBufferBuilder* builder,
    flatbuffers::Offset<fbs::ParticleSystemData2>* fbsParticleSystemData) const
{
    // Copy data
    std::vector<flatbuffers::Offset<fbs::ScalarParticleData2>> scalarDataList;
    for (const auto& scalarData : m_scalarDataList)
    {
        flatbuffers::Offset<fbs::ScalarParticleData2> fbsScalarData =
            fbs::CreateScalarParticleData2(
                *builder,
                builder->CreateVector(scalarData.data(), scalarData.Length()));
        scalarDataList.push_back(fbsScalarData);
    }
    const flatbuffers::Offset<
        flatbuffers::Vector<flatbuffers::Offset<fbs::ScalarParticleData2>>>
        fbsScalarDataList = builder->CreateVector(scalarDataList);

    std::vector<flatbuffers::Offset<fbs::VectorParticleData2>> vectorDataList;
    for (const auto& vectorData : m_vectorDataList)
    {
        std::vector<fbs::Vector2D> newVectorData;
        for (const auto& v : vectorData)
        {
            newVectorData.push_back(CubbyFlowToFlatbuffers(v));
        }

        flatbuffers::Offset<fbs::VectorParticleData2> fbsVectorData =
            CreateVectorParticleData2(
                *builder, builder->CreateVectorOfStructs(newVectorData.data(),
                                                         newVectorData.size()));
        vectorDataList.push_back(fbsVectorData);
    }
    const flatbuffers::Offset<
        flatbuffers::Vector<flatbuffers::Offset<fbs::VectorParticleData2>>>
        fbsVectorDataList = builder->CreateVector(vectorDataList);

    // Copy neighbor searcher
    const flatbuffers::Offset<flatbuffers::String> neighborSearcherType =
        builder->CreateString(m_neighborSearcher->TypeName());
    std::vector<uint8_t> neighborSearcherSerialized;
    m_neighborSearcher->Serialize(&neighborSearcherSerialized);
    const flatbuffers::Offset<fbs::PointNeighborSearcherSerialized2>
        fbsNeighborSearcher = fbs::CreatePointNeighborSearcherSerialized2(
            *builder, neighborSearcherType,
            builder->CreateVector(neighborSearcherSerialized.data(),
                                  neighborSearcherSerialized.size()));

    // Copy neighbor lists
    std::vector<flatbuffers::Offset<fbs::ParticleNeighborList2>> neighborLists;
    for (const auto& neighbors : m_neighborLists)
    {
        std::vector<uint64_t> neighbors64(neighbors.begin(), neighbors.end());
        flatbuffers::Offset<fbs::ParticleNeighborList2> fbsNeighborList =
            fbs::CreateParticleNeighborList2(
                *builder,
                builder->CreateVector(neighbors64.data(), neighbors64.size()));
        neighborLists.push_back(fbsNeighborList);
    }

    const flatbuffers::Offset<
        flatbuffers::Vector<flatbuffers::Offset<fbs::ParticleNeighborList2>>>
        fbsNeighborLists = builder->CreateVector(neighborLists);

    // Copy the searcher
    *fbsParticleSystemData = fbs::CreateParticleSystemData2(
        *builder, m_radius, m_mass, m_positionIdx, m_velocityIdx, m_forceIdx,
        fbsScalarDataList, fbsVectorDataList, fbsNeighborSearcher,
        fbsNeighborLists);
}

void ParticleSystemData2::DeserializeParticleSystemData(
    const fbs::ParticleSystemData2* fbsParticleSystemData)
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
    const flatbuffers::Vector<flatbuffers::Offset<fbs::ScalarParticleData2>>*
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

    const flatbuffers::Vector<flatbuffers::Offset<fbs::VectorParticleData2>>*
        fbsVectorDataList = fbsParticleSystemData->vectorDataList();
    for (const auto fbsVectorData : (*fbsVectorDataList))
    {
        const flatbuffers::Vector<const fbs::Vector2D*>* data =
            fbsVectorData->data();

        m_vectorDataList.emplace_back(data->size());

        Array<Vector<double, 2>, 1>& newData = *(m_vectorDataList.rbegin());

        for (uint32_t i = 0; i < data->size(); ++i)
        {
            newData[i] = FlatbuffersToCubbyFlow(*data->Get(i));
        }
    }

    m_numberOfParticles = m_vectorDataList[0].Length();

    // Copy neighbor searcher
    const fbs::PointNeighborSearcherSerialized2* fbsNeighborSearcher =
        fbsParticleSystemData->neighborSearcher();
    m_neighborSearcher = Factory::BuildPointNeighborSearcher2(
        fbsNeighborSearcher->type()->c_str());
    const std::vector<uint8_t> neighborSearcherSerialized{
        fbsNeighborSearcher->data()->begin(), fbsNeighborSearcher->data()->end()
    };
    m_neighborSearcher->Deserialize(neighborSearcherSerialized);

    // Copy neighbor list
    const flatbuffers::Vector<flatbuffers::Offset<fbs::ParticleNeighborList2>>*
        fbsNeighborLists = fbsParticleSystemData->neighborLists();
    m_neighborLists.resize(fbsNeighborLists->size());

    for (uint32_t i = 0; i < fbsNeighborLists->size(); ++i)
    {
        const flatbuffers::Vector<
            flatbuffers::Offset<fbs::ParticleNeighborList2>>::return_type
            fbsNeighborList = fbsNeighborLists->Get(i);
        m_neighborLists[i].resize(fbsNeighborList->data()->size());
        std::transform(fbsNeighborList->data()->begin(),
                       fbsNeighborList->data()->end(),
                       m_neighborLists[i].begin(),
                       [](uint64_t val) { return static_cast<size_t>(val); });
    }
}
}  // namespace CubbyFlow