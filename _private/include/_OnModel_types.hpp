#pragma once

#include <cstdint>
#include <OnModel_types.hpp>

namespace cc
{
	namespace onm
	{

		using PRIVATE_HANDLE_ONMODEL = struct _tagPRIVATE_HANDLE_ONMODEL;

		struct _tagPRIVATE_HANDLE_ONMODEL
		{
			ONMODEL_CONFIG config;
			void* spin;
			void* spinaddr;
			InitType inittype;
			UpdateMethod updatetype;
			uint64_t updatecount;
		};


	}
}

