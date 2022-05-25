#pragma once

#include <CrHeader.hpp>
#include <OnModel_types.hpp>

namespace cc
{
	namespace onm
	{

		// <function> ONM_get_status
		//     Get status Magnetization and energy
		// <return> void
		// <argument>
		//     1. [HONMODEL]
		//     2. [ONM_STATUS]status
		CcResult ONM_get_status(HONMODEL OnModel, ONM_STATUS* status);

		void ONM_get_status_2_ISING(HONMODEL OnModel, ONM_STATUS* status);
		void ONM_get_status_2_XY(HONMODEL OnModel, ONM_STATUS* status);
		void ONM_get_status_2_HEISENBERG(HONMODEL OnModel, ONM_STATUS* status);
		void ONM_get_status_3_ISING(HONMODEL OnModel, ONM_STATUS* status);
		void ONM_get_status_3_XY(HONMODEL OnModel, ONM_STATUS* status);
		void ONM_get_status_3_HEISENBERG(HONMODEL OnModel, ONM_STATUS* status);

		// [Only for Antiferro Heisenberg]
		// <function> ONM_get_verticalmz
		//     Get vertical fagnetic field
		// <return> QWORD
		// <argument>
		//     1. [HONMODEL]
		//     2. [double*[3]]
		//     3. [double*[]]
		//         This array size is 2 or 3 that depends on the dimension of model.
		CcResult ONM_get_StructureFactor(HONMODEL OnModel, double norm[3], double wavenum[]);




	}
}

