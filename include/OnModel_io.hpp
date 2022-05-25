#pragma once

#include <CrHeader.hpp>
#include <OnModel_types.hpp>

namespace cc
{
	namespace onm
	{
		// <function> ONM_save
		//     Save ONM Object File
		// <return> QWORD
		// <argument>
		//     1. [ONMODEL*]
		//         This struct. is must initializad
		//     2. [LPCSTR] path
		//QWORD ONM_load(ONMODEL* OnModel, LPCSTR path);


		// <function> ONM_param
		//     change parameter exchange interaction or Magnetic field or temperature
		// <return> QWORD
		// <argument>
		//     1. [ONMODEL*]
		//     2. [uint64_t]
		//         You can be a combination with PF_ExchangeInteraction / PF_Temperature / PF_MagneticField
		//QWORD ONM_param(ONMODEL* OnModel, uint64_t ParamFlag);
	}
}

