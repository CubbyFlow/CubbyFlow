/*************************************************************************
> File Name: Parallel.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Parallel functions for CubbyFlow.
> Created Time: 2017/10/13
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/Utils/Parallel.h>

#if defined(CUBBYFLOW_TASKING_TBB)
#include <tbb/task_arena.h>
#include <tbb/task_scheduler_init.h>
#elif defined(CUBBYFLOW_TASKING_OPENMP)
#include <omp.h>
#endif

#include <memory>
#include <thread>

static unsigned int MAX_NUMBER_OF_THREADS = std::thread::hardware_concurrency();

namespace CubbyFlow
{
	void SetMaxNumberOfThreads(unsigned int numThreads)
	{
#if defined(CUBBYFLOW_TASKING_TBB)
		static std::unique_ptr<tbb::task_scheduler_init> tbbInit;

		if (!tbbInit.get())
		{
			tbbInit.reset(new tbb::task_scheduler_init(numThreads));
		}
		else
		{
			tbbInit->terminate();
			tbbInit->initialize(numThreads);
		}
#elif defined(CUBBYFLOW_TASKING_OPENMP)
		omp_set_num_threads(numThreads);
#endif
		MAX_NUMBER_OF_THREADS = std::max(numThreads, 1u);
	}

	unsigned int GetMaxNumberOfThreads()
	{
		return MAX_NUMBER_OF_THREADS;
	}
}