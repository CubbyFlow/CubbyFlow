// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_POINT_SIMPLE_LIST_SEARCHER_HPP
#define CUBBYFLOW_POINT_SIMPLE_LIST_SEARCHER_HPP

#include <Core/Searcher/PointNeighborSearcher.hpp>

namespace CubbyFlow
{
//!
//! \brief Simple ad-hoc N-D point searcher.
//!
//! This class implements N-D point searcher simply by looking up every point in
//! the list. Thus, this class is not ideal for searches involing large number
//! of points, but only for small set of items.
//!
template <size_t N>
class PointSimpleListSearcher final : public PointNeighborSearcher<N>
{
 public:
    CUBBYFLOW_NEIGHBOR_SEARCHER_TYPE_NAME(PointSimpleListSearcher, N)

    class Builder;

    using typename PointNeighborSearcher<N>::ForEachNearbyPointFunc;

    //! Default constructor.
    PointSimpleListSearcher() = default;

    //! Copy constructor.
    PointSimpleListSearcher(const PointSimpleListSearcher& other);

    //!
    //! \brief      Builds internal structure for given points list.
    //!
    //! For this class, this function simply copies the given point list to the
    //! internal list.
    //!
    //! \param[in]  points The points to search.
    //!
    void Build(const ConstArrayView1<Vector<double, N>>& points) override;

    //!
    //! Invokes the callback function for each nearby point around the origin
    //! within given radius.
    //!
    //! \param[in]  origin   The origin position.
    //! \param[in]  radius   The search radius.
    //! \param[in]  callback The callback function.
    //!
    void ForEachNearbyPoint(
        const Vector<double, N>& origin, double radius,
        const ForEachNearbyPointFunc& callback) const override;

    //!
    //! Returns true if there are any nearby points for given origin within
    //! radius.
    //!
    //! \param[in]  origin The origin.
    //! \param[in]  radius The radius.
    //!
    //! \return     True if has nearby point, false otherwise.
    //!
    bool HasNearbyPoint(const Vector<double, N>& origin,
                        double radius) const override;

    //!
    //! \brief      Creates a new instance of the object with same properties
    //!             than original.
    //!
    //! \return     Copy of this object.
    //!
    std::shared_ptr<PointNeighborSearcher<N>> Clone() const override;

    //! Assignment operator.
    PointSimpleListSearcher& operator=(const PointSimpleListSearcher& other);

    //! Copy from the other instance.
    void Set(const PointSimpleListSearcher& other);

    //! Serializes the neighbor searcher into the buffer.
    void Serialize(std::vector<uint8_t>* buffer) const override;

    //! Deserializes the neighbor searcher from the buffer.
    void Deserialize(const std::vector<uint8_t>& buffer) override;

    //! Returns builder fox PointSimpleListSearcher.
    static Builder GetBuilder();

 private:
    template <size_t M = N>
    static std::enable_if_t<M == 2, void> Serialize(
        const PointSimpleListSearcher<2>& searcher,
        std::vector<uint8_t>* buffer);

    template <size_t M = N>
    static std::enable_if_t<M == 3, void> Serialize(
        const PointSimpleListSearcher<3>& searcher,
        std::vector<uint8_t>* buffer);

    template <size_t M = N>
    static std::enable_if_t<M == 2, void> Deserialize(
        const std::vector<uint8_t>& buffer,
        PointSimpleListSearcher<2>& searcher);

    template <size_t M = N>
    static std::enable_if_t<M == 3, void> Deserialize(
        const std::vector<uint8_t>& buffer,
        PointSimpleListSearcher<3>& searcher);

    Array1<Vector<double, N>> m_points;
};

//! 2-D PointSimpleListSearcher type.
using PointSimpleListSearcher2 = PointSimpleListSearcher<2>;

//! 3-D PointSimpleListSearcher type.
using PointSimpleListSearcher3 = PointSimpleListSearcher<3>;

//! Shared pointer for the PointSimpleListSearcher2 type.
using PointSimpleListSearcher2Ptr = std::shared_ptr<PointSimpleListSearcher<2>>;

//! Shared pointer for the PointSimpleListSearcher3 type.
using PointSimpleListSearcher3Ptr = std::shared_ptr<PointSimpleListSearcher<3>>;

//!
//! \brief Front-end to create PointSimpleListSearcher objects step by step.
//!
template <size_t N>
class PointSimpleListSearcher<N>::Builder final
    : public PointNeighborSearcherBuilder<N>
{
 public:
    //! Builds PointSimpleListSearcher instance.
    PointSimpleListSearcher<N> Build() const;

    //! Builds shared pointer of PointSimpleListSearcher instance.
    std::shared_ptr<PointSimpleListSearcher<N>> MakeShared() const;

    //! Returns shared pointer of PointNeighborSearcher type.
    std::shared_ptr<PointNeighborSearcher<N>> BuildPointNeighborSearcher()
        const override;
};
}  // namespace CubbyFlow

#endif