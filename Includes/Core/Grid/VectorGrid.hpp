// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_VECTOR_GRID_HPP
#define CUBBYFLOW_VECTOR_GRID_HPP

#include <Core/Array/ArrayView.hpp>
#include <Core/Field/VectorField.hpp>
#include <Core/Grid/Grid.hpp>

namespace CubbyFlow
{
//! Abstract base class for N-D vector grid structure.
template <size_t N>
class VectorGrid : public VectorField<N>, public Grid<N>
{
 public:
    //! Read-write array view type.
    using VectorDataView = ArrayView<Vector<double, N>, N>;

    //! Read-only array view type.
    using ConstVectorDataView = ArrayView<const Vector<double, N>, N>;

    using Grid<N>::Resolution;
    using Grid<N>::GridSpacing;
    using Grid<N>::Origin;

    //! Constructs an empty grid.
    VectorGrid() = default;

    //! Default virtual destructor.
    ~VectorGrid() override = default;

    //! Copy constructor.
    VectorGrid(const VectorGrid& other);

    //! Move constructor.
    VectorGrid(VectorGrid&& other) noexcept;

    //! Copy assignment operator.
    VectorGrid& operator=(const VectorGrid& other);

    //! Move assignment operator.
    VectorGrid& operator=(VectorGrid&& other) noexcept;

    //! Clears the contents of the grid.
    void Clear();

    //! Resizes the grid using given parameters.
    void Resize(const Vector<size_t, N>& resolution,
                const Vector<double, N>& gridSpacing =
                    Vector<double, N>::MakeConstant(1.0),
                const Vector<double, N>& origin = Vector<double, N>{},
                const Vector<double, N>& initialValue = Vector<double, N>{});

    //! Resizes the grid using given parameters.
    void Resize(const Vector<double, N>& gridSpacing,
                const Vector<double, N>& origin);

    //! Fills the grid with given value.
    virtual void Fill(const Vector<double, N>& value,
                      ExecutionPolicy policy = ExecutionPolicy::Parallel) = 0;

    //! Fills the grid with given position-to-value mapping function.
    virtual void Fill(
        const std::function<Vector<double, N>(const Vector<double, N>&)>& func,
        ExecutionPolicy policy = ExecutionPolicy::Parallel) = 0;

    //! Returns the copy of the grid instance.
    [[nodiscard]] virtual std::shared_ptr<VectorGrid<N>> Clone() const = 0;

    //! Serializes the grid instance to the output buffer.
    void Serialize(std::vector<uint8_t>* buffer) const override;

    //! Deserializes the input buffer to the grid instance.
    void Deserialize(const std::vector<uint8_t>& buffer) override;

 protected:
    using Grid<N>::SetSizeParameters;
    using Grid<N>::GetData;
    using Grid<N>::SetData;

    //!
    //! \brief Invoked when the resizing happens.
    //!
    //! This callback function is called when the grid gets resized. The
    //! overriding class should allocate the internal storage based on its
    //! data layout scheme.
    //!
    virtual void OnResize(const Vector<size_t, N>& resolution,
                          const Vector<double, N>& gridSpacing,
                          const Vector<double, N>& origin,
                          const Vector<double, N>& initialValue) = 0;
};

//! 2-D VectorGrid type.
using VectorGrid2 = VectorGrid<2>;

//! 3-D VectorGrid type.
using VectorGrid3 = VectorGrid<3>;

//! Shared pointer for the VectorGrid2 type.
using VectorGrid2Ptr = std::shared_ptr<VectorGrid2>;

//! Shared pointer for the VectorGrid3 type.
using VectorGrid3Ptr = std::shared_ptr<VectorGrid3>;

//! Abstract base class for N-D vector grid builder.
template <size_t N>
class VectorGridBuilder
{
 public:
    //! Creates a builder.
    VectorGridBuilder() = default;

    //! Default virtual destructor.
    virtual ~VectorGridBuilder() = default;

    //! Deleted copy constructor.
    VectorGridBuilder(const VectorGridBuilder& other) = delete;

    //! Deleted move constructor.
    VectorGridBuilder(VectorGridBuilder&& other) noexcept = delete;

    //! Deleted copy assignment operator.
    VectorGridBuilder& operator=(const VectorGridBuilder& other) = delete;

    //! Deleted move assignment operator.
    VectorGridBuilder& operator=(VectorGridBuilder&& other) noexcept = delete;

    //! Returns N-D vector grid with given parameters.
    [[nodiscard]] virtual std::shared_ptr<VectorGrid<N>> Build(
        const Vector<size_t, N>& resolution,
        const Vector<double, N>& gridSpacing,
        const Vector<double, N>& gridOrigin,
        const Vector<double, N>& initialVal) const = 0;
};

//! 2-D VectorGridBuilder type.
using VectorGridBuilder2 = VectorGridBuilder<2>;

//! 3-D VectorGridBuilder type.
using VectorGridBuilder3 = VectorGridBuilder<3>;

//! Shared pointer for the VectorGridBuilder2 type.
using VectorGridBuilder2Ptr = std::shared_ptr<VectorGridBuilder2>;

//! Shared pointer for the VectorGridBuilder3 type.
using VectorGridBuilder3Ptr = std::shared_ptr<VectorGridBuilder3>;
}  // namespace CubbyFlow

#endif