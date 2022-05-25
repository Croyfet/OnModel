#pragma once

#include <CrHeader.h>
#include <OnModel_types.hpp>

namespace cc
{
	namespace onm
	{
		// <function> ONM_insert
		//     Insert to spin
		// <return> Cresult
		// <argument>
		//     1. [HONMODEL]
		//     2. [ONMPOINT]
		//     3. [void*]spin
		//         The pointer to spin state SPIN_ISING / SPIN_XY / SPIN_HEISENBERG
		CcResult ONM_insert(HONMODEL OnModel, ONMPOINT point, void* spin);


		// <function> ONM_inspect_spin
		//     Output spin to console
		// <return> Cresult
		// <argument>
		//     1. [HONMODEL]
		//     2. [uint64_t] InspectTarget
		//         IS_SPIN / IS_ADDR / IS_SPINADDR
		CcResult ONM_inspect_spin(HONMODEL OnModel, InspectSpin inspect_spin);


		// <function> ONM_inspect_config
		//     Output status to console
		// <return> Cresult
		// <argument>
		//     1. [HONMODEL]
		//     2. [uint64_t]
		//         You can combinate the flags OS_**
		//             OS_TYPE / OS_EXCHANGEINTERACTION / OS_
		CcResult ONM_inspect_config(HONMODEL OnModel, InspectStatus status_flag);
	}
}

