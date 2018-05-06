/*************************************************************************
> File Name: Octree.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Generic octree data structure.
> Created Time: 2017/10/15
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_OCTREE_H
#define CUBBYFLOW_OCTREE_H

#include <Core/QueryEngine/IntersectionQueryEngine3.h>
#include <Core/QueryEngine/NearestNeighborQueryEngine3.h>

namespace CubbyFlow
{
	//!
	//! \brief      Generic octree data structure.
	//!
	//! This class is a generic octree representation to store arbitrary spatial
	//! data. The octree supports closest neighbor search, overlapping test, and
	//! ray intersection test.
	//!
	//! \tparam     T     Value type.
	//!
	template <typename T>
	class Octree final : public IntersectionQueryEngine3<T>, public NearestNeighborQueryEngine3<T>
	{
	public:
		using ContainerType = std::vector<T>;
		using Iterator = typename ContainerType::iterator;
		using ConstIterator =  typename ContainerType::const_iterator;

		//! Default constructor.
		Octree();

		//! Builds an octree with given list of items, bounding box of the items,
		//! overlapping test function, and max depth of the tree.
		void Build(
			const std::vector<T>& items, const BoundingBox3D& bound,
			const BoxIntersectionTestFunc3<T>& testFunc, size_t maxDepth);

		//! Clears all the contents of this instance.
		void Clear();

		//! Returns the nearest neighbor for given point and distance measure
		//! function.
		NearestNeighborQueryResult3<T> GetNearestNeighbor(
			const Vector3D& pt,
			const NearestNeighborDistanceFunc3<T>& distanceFunc) const override;

		//! Returns true if given \p box intersects with any of the stored items.
		bool IsIntersects(const BoundingBox3D& box,
			const BoxIntersectionTestFunc3<T>& testFunc) const override;

		//! Returns true if given \p ray intersects with any of the stored items.
		bool IsIntersects(const Ray3D& ray,
			const RayIntersectionTestFunc3<T>& testFunc) const override;

		//! Invokes \p visitorFunc for every intersecting items.
		void ForEachIntersectingItem(
			const BoundingBox3D& box, const BoxIntersectionTestFunc3<T>& testFunc,
			const IntersectionVisitorFunc3<T>& visitorFunc) const override;

		//! Invokes \p visitorFunc for every intersecting items.
		void ForEachIntersectingItem(
			const Ray3D& ray, const RayIntersectionTestFunc3<T>& testFunc,
			const IntersectionVisitorFunc3<T>& visitorFunc) const override;

		//! Returns the closest intersection for given \p ray.
		ClosestIntersectionQueryResult3<T> GetClosestIntersection(
			const Ray3D& ray,
			const GetRayIntersectionFunc3<T>& testFunc) const override;

		//! Returns the begin iterator of the item.
		Iterator begin();

		//! Returns the end iterator of the item.
		Iterator end();

		//! Returns the immutable begin iterator of the item.
		ConstIterator begin() const;

		//! Returns the immutable end iterator of the item.
		ConstIterator end() const;

		//! Returns the number of items.
		size_t GetNumberOfItems() const;

		//! Returns the item at \p i.
		const T& GetItem(size_t i) const;

		//! Returns the number of octree nodes.
		size_t GetNumberOfNodes() const;

		//! Returns the list of the items for given node index.
		const std::vector<size_t>& GetItemsAtNode(size_t nodeIdx) const;

		//!
		//! \brief      Returns a child's index for given node.
		//!
		//! For a given node, its children is stored continuously, such that if the
		//! node's first child's index is i, then i + 1, i + 2, ... , i + 7 are the
		//! indices for its children. The order of octant is x-major.
		//!
		//! \param[in]  nodeIdx The node index.
		//! \param[in]  childIdx The child index (0 to 7).
		//!
		//! \return     Index of the selected child.
		//!
		size_t GetChildIndex(size_t nodeIdx, size_t childIdx) const;

		//! Returns the bounding box of this octree.
		const BoundingBox3D& GetBoundingBox() const;

		//! Returns the maximum depth of the tree.
		size_t GetMaxDepth() const;

	private:
		struct Node
		{
			size_t firstChild = std::numeric_limits<size_t>::max();
			std::vector<size_t> items;

			bool IsLeaf() const;
		};

		size_t m_maxDepth = 1;
		BoundingBox3D m_bbox;
		std::vector<T> m_items;
		std::vector<Node> m_nodes;

		void Build(size_t nodeIdx, size_t currentDepth,
			const BoundingBox3D& currentBound,
			const BoxIntersectionTestFunc3<T>& overlapsFunc);

		bool IsIntersects(const BoundingBox3D& box,
			const BoxIntersectionTestFunc3<T>& testFunc, size_t nodeIdx,
			const BoundingBox3D& currentBound) const;

		bool IsIntersects(const Ray3D& ray,
			const RayIntersectionTestFunc3<T>& testFunc, size_t nodeIdx,
			const BoundingBox3D& currentBound) const;

		void ForEachIntersectingItem(const BoundingBox3D& box,
			const BoxIntersectionTestFunc3<T>& testFunc,
			const IntersectionVisitorFunc3<T>& visitorFunc,
			size_t nodeIdx,
			const BoundingBox3D& currentBound) const;

		void ForEachIntersectingItem(const Ray3D& ray,
			const RayIntersectionTestFunc3<T>& testFunc,
			const IntersectionVisitorFunc3<T>& visitorFunc,
			size_t nodeIdx,
			const BoundingBox3D& currentBound) const;

		ClosestIntersectionQueryResult3<T> GetClosestIntersection(
			const Ray3D& ray, const GetRayIntersectionFunc3<T>& testFunc,
			size_t nodeIdx, const BoundingBox3D& currentBound,
			ClosestIntersectionQueryResult3<T> best) const;
	};
}

#include <Core/Geometry/Octree-Impl.h>

#endif