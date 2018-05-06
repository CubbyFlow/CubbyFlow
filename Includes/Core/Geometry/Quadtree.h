/*************************************************************************
> File Name: Quadtree.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Generic quadtree data structure.
> Created Time: 2017/10/15
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_QUADTREE_H
#define CUBBYFLOW_QUADTREE_H

#include <Core/QueryEngine/IntersectionQueryEngine2.h>
#include <Core/QueryEngine/NearestNeighborQueryEngine2.h>

namespace CubbyFlow
{
	//!
	//! \brief      Generic quadtree data structure.
	//!
	//! This class is a generic quadtree representation to store arbitrary spatial
	//! data. The quadtree supports closest neighbor search, overlapping test, and
	//! ray intersection test.
	//!
	//! \tparam     T     Value type.
	//!
	template <typename T>
	class Quadtree final : public IntersectionQueryEngine2<T>, public NearestNeighborQueryEngine2<T>
	{
	public:
		using ContainerType = std::vector<T>;
		using Iterator = typename ContainerType::iterator;
		using ConstIterator = typename ContainerType::const_iterator;

		//! Default constructor.
		Quadtree();

		//! Builds an quadtree with given list of items, bounding box of the items,
		//! overlapping test function, and max depth of the tree.
		void Build(const std::vector<T>& items, const BoundingBox2D& bound,
			const BoxIntersectionTestFunc2<T>& testFunc, size_t maxDepth);

		//! Clears all the contents of this instance.
		void Clear();

		//! Returns the nearest neighbor for given point and distance measure
		//! function.
		NearestNeighborQueryResult2<T> GetNearestNeighbor(
			const Vector2D& pt,
			const NearestNeighborDistanceFunc2<T>& distanceFunc) const override;

		//! Returns true if given \p box intersects with any of the stored items.
		bool IsIntersects(const BoundingBox2D& box,
			const BoxIntersectionTestFunc2<T>& testFunc) const override;

		//! Returns true if given \p ray intersects with any of the stored items.
		bool IsIntersects(const Ray2D& ray,
			const RayIntersectionTestFunc2<T>& testFunc) const override;

		//! Invokes \p visitorFunc for every intersecting items.
		void ForEachIntersectingItem(
			const BoundingBox2D& box, const BoxIntersectionTestFunc2<T>& testFunc,
			const IntersectionVisitorFunc2<T>& visitorFunc) const override;

		//! Invokes \p visitorFunc for every intersecting items.
		void ForEachIntersectingItem(
			const Ray2D& ray, const RayIntersectionTestFunc2<T>& testFunc,
			const IntersectionVisitorFunc2<T>& visitorFunc) const override;

		//! Returns the closest intersection for given \p ray.
		ClosestIntersectionQueryResult2<T> GetClosestIntersection(
			const Ray2D& ray,
			const GetRayIntersectionFunc2<T>& testFunc) const override;

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

		//! Returns the number of quadtree nodes.
		size_t GetNumberOfNodes() const;

		//! Returns the list of the items for given noide index.
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

		//! Returns the bounding box of this quadtree.
		const BoundingBox2D& GetBoundingBox() const;

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
		BoundingBox2D m_bbox;
		std::vector<T> m_items;
		std::vector<Node> m_nodes;

		void Build(size_t nodeIdx, size_t currentDepth,
			const BoundingBox2D& currentBound,
			const BoxIntersectionTestFunc2<T>& overlapsFunc);

		bool IsIntersects(const BoundingBox2D& box,
			const BoxIntersectionTestFunc2<T>& testFunc, size_t nodeIdx,
			const BoundingBox2D& currentBound) const;

		bool IsIntersects(const Ray2D& ray,
			const RayIntersectionTestFunc2<T>& testFunc, size_t nodeIdx,
			const BoundingBox2D& currentBound) const;

		void ForEachIntersectingItem(const BoundingBox2D& box,
			const BoxIntersectionTestFunc2<T>& testFunc,
			const IntersectionVisitorFunc2<T>& visitorFunc,
			size_t nodeIdx,
			const BoundingBox2D& currentBound) const;

		void ForEachIntersectingItem(const Ray2D& ray,
			const RayIntersectionTestFunc2<T>& testFunc,
			const IntersectionVisitorFunc2<T>& visitorFunc,
			size_t nodeIdx,
			const BoundingBox2D& currentBound) const;

		ClosestIntersectionQueryResult2<T> GetClosestIntersection(
			const Ray2D& ray, const GetRayIntersectionFunc2<T>& testFunc,
			size_t nodeIdx, const BoundingBox2D& currentBound,
			ClosestIntersectionQueryResult2<T> best) const;
	};
}

#include <Core/Geometry/Quadtree-Impl.h>

#endif