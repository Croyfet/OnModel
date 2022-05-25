#pragma once

#include <CrHeader.hpp>
#include <OnModel_types.hpp>

namespace cc
{
	namespace onm
	{
		// <function> ONM_new
		//     Create ONMODEL according to ONMODEL_CONFIG
		// <return> Cresult
		// <argument>
		//     1. [HONMODEL]
		//     2. [ONMODEL_CONFIG]
		CcResult ONM_new(HONMODEL* OnModel, ONMODEL_CONFIG OnM_config);


		// <function> ONM_free
		//     Release ONMODEL created by ONM_new
		// <return> Cresult
		// <argument>
		//     1. [HONMODEL]
		CcResult ONM_free(HONMODEL OnModel);


		// <function> ONM_init
		//     Initialize ONMODEL spin created by ONM_new
		// <return> Cresult
		// <argument>
		//     1. [HONMODEL]
		//     2. [InitType]
		CcResult ONM_init(HONMODEL OnModel, InitType init_type);
	}
}

