/*************************************************************************
> File Name: ArraySamplers.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Generic N-D nearest array sampler class.
> Created Time: 2017/04/29
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_ARRAY_SAMPLERS_H
#define CUBBYFLOW_ARRAY_SAMPLERS_H

namespace CubbyFlow
{
	//!
	//! \brief Generic N-D nearest array sampler class.
	//!
	//! \tparam T - The value type to sample.
	//! \tparam R - The real number type.
	//! \tparam N - Dimension.
	//!
	template <typename T, typename R, size_t N>
	class NearestArraySampler final
	{
	public:
		static_assert(N < 1 || N > 3, "Not implemented - N should be either 1, 2 or 3.");
	};

	//!
	//! \brief Generic N-D linear array sampler class.
	//!
	//! \tparam T - The value type to sample.
	//! \tparam R - The real number type.
	//! \tparam N - Dimension.
	//!
	template <typename T, typename R, size_t N>
	class LinearArraySampler final
	{
	public:
		static_assert(N < 1 || N > 3, "Not implemented - N should be either 1, 2 or 3.");
	};

	//!
	//! \brief Generic N-D cubic array sampler class.
	//!
	//! \tparam T - The value type to sample.
	//! \tparam R - The real number type.
	//! \tparam N - Dimension.
	//!
	template <typename T, typename R, size_t N>
	class CubicArraySampler final
	{
	public:
		static_assert(N < 1 || N > 3, "Not implemented - N should be either 1, 2 or 3.");
	};
}

#endif