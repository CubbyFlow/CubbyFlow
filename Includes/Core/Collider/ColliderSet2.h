/*************************************************************************
> File Name: ColliderSet2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Collection of 2-D colliders.
> Created Time: 2017/05/09
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_COLLIDER_SET2_H
#define CUBBYFLOW_COLLIDER_SET2_H

#include <Core/Collider/Collider2.h>

#include <vector>

namespace CubbyFlow
{
	//! Collection of 2-D colliders
	class ColliderSet2 final : public Collider2
	{
	public:
		class Builder;

		//! Default constructor.
		ColliderSet2();

		//! Constructs with other colliders.
		explicit ColliderSet2(const std::vector<Collider2Ptr>& others);

		//! Returns the velocity of the collider at given \p point.
		Vector2D VelocityAt(const Vector2D& point) const override;

		//! Adds a collider to the set.
		void AddCollider(const Collider2Ptr& collider);

		//! Returns number of colliders.
		size_t NumberOfColliders() const;

		//! Returns collider at index \p i.
		Collider2Ptr Collider(size_t i) const;

		//! Returns builder fox ColliderSet2.
		static Builder GetBuilder();

	private:
		std::vector<Collider2Ptr> m_colliders;
	};

	//! Shared pointer for the ColliderSet2 type.
	typedef std::shared_ptr<ColliderSet2> ColliderSet2Ptr;

	//!
	//! \brief Front-end to create ColliderSet2 objects step by step.
	//!
	class ColliderSet2::Builder final
	{
	public:
		//! Returns builder with other colliders.
		Builder& WithColliders(const std::vector<Collider2Ptr>& others);

		//! Builds ColliderSet2.
		ColliderSet2 Build() const;

		//! Builds shared pointer of ColliderSet2 instance.
		ColliderSet2Ptr MakeShared() const;

	private:
		std::vector<Collider2Ptr> m_colliders;
	};
}

#endif