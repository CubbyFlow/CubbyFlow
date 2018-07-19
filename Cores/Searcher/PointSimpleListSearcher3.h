/*************************************************************************
> File Name: PointSimpleListSearcher3.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Simple ad-hoc 3-D point searcher.
> Created Time: 2017/05/25
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_POINT_SIMPLE_LIST_SEARCHER3_H
#define CUBBYFLOW_POINT_SIMPLE_LIST_SEARCHER3_H

#include <Searcher/PointNeighborSearcher3.h>

namespace CubbyFlow
{
	//!
	//! \brief Simple ad-hoc 3-D point searcher.
	//!
	//! This class implements 3-D point searcher simply by looking up every point in
	//! the list. Thus, this class is not ideal for searches involving large number
	//! of points, but only for small set of items.
	//!
	class PointSimpleListSearcher3 final : public PointNeighborSearcher3
	{
	public:
		CUBBYFLOW_NEIGHBOR_SEARCHER3_TYPE_NAME(PointSimpleListSearcher3)

		class Builder;

		//! Default constructor.
		PointSimpleListSearcher3();

		//! Copy constructor.
		PointSimpleListSearcher3(const PointSimpleListSearcher3& other);

		//!
		//! \brief      Builds internal structure for given points list.
		//!
		//! For this class, this function simply copies the given point list to the
		//! internal list.
		//!
		//! \param[in]  points The points to search.
		//!
		void Build(const ConstArrayAccessor1<Vector3D>& points) override;

		//!
		//! Invokes the callback function for each nearby point around the origin
		//! within given radius.
		//!
		//! \param[in]  origin   The origin position.
		//! \param[in]  radius   The search radius.
		//! \param[in]  callback The callback function.
		//!
		void ForEachNearbyPoint(
			const Vector3D& origin,
			double radius,
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
		bool HasNearbyPoint(const Vector3D& origin, double radius) const override;

		//!
		//! \brief      Creates a new instance of the object with same properties
		//!             than original.
		//!
		//! \return     Copy of this object.
		//!
		PointNeighborSearcher3Ptr Clone() const override;

		//! Assignment operator.
		PointSimpleListSearcher3& operator=(const PointSimpleListSearcher3& other);

		//! Copy from the other instance.
		void Set(const PointSimpleListSearcher3& other);

		//! Serializes the neighbor searcher into the buffer.
		void Serialize(std::vector<uint8_t>* buffer) const override;

		//! Deserializes the neighbor searcher from the buffer.
		void Deserialize(const std::vector<uint8_t>& buffer) override;

		//! Returns builder fox PointSimpleListSearcher3.
		static Builder GetBuilder();

	private:
		std::vector<Vector3D> m_points;
	};

	//! Shared pointer for the PointSimpleListSearcher3 type.
	using PointSimpleListSearcher3Ptr = std::shared_ptr<PointSimpleListSearcher3>;

	//!
	//! \brief Front-end to create PointSimpleListSearcher3 objects step by step.
	//!
	class PointSimpleListSearcher3::Builder final : public PointNeighborSearcherBuilder3
	{
	public:
		//! Builds PointSimpleListSearcher3 instance.
		PointSimpleListSearcher3 Build() const;

		//! Builds shared pointer of PointSimpleListSearcher3 instance.
		PointSimpleListSearcher3Ptr MakeShared() const;

		//! Returns shared pointer of PointNeighborSearcher3 type.
		PointNeighborSearcher3Ptr BuildPointNeighborSearcher() const override;
	};
}

#endif