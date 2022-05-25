#pragma once

#include <cstdint>

#include <CrHeader.hpp>
#include <OnModel_types.hpp>


namespace cc
{
	namespace onm
	{
		// <function> ONM_monte_carlo_step
		//     Monte Carlo Update
		// <return> void
		// <argument>
		//     1. [HONMODEL]
		//     2. [int64_t]step
		cc::CcResult ONM_Metropolis_SingleSpinFlip(HONMODEL OnModel, int64_t step);

		// Direct Call
		void ONM_Metropolis_SingleSpinFlip_2_ISING(HONMODEL OnModel, int64_t step);
		void ONM_Metropolis_SingleSpinFlip_2_XY(HONMODEL OnModel, int64_t step);
		void ONM_Metropolis_SingleSpinFlip_2_HEISENBERG(HONMODEL OnModel, int64_t step);
		void ONM_Metropolis_SingleSpinFlip_3_ISING(HONMODEL OnModel, int64_t step);
		void ONM_Metropolis_SingleSpinFlip_3_XY(HONMODEL OnModel, int64_t step);
		void ONM_Metropolis_SingleSpinFlip_3_HEISENBERG(HONMODEL OnModel, int64_t step);

	}
}

