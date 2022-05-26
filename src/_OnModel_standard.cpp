#include <_OnModel_standard.hpp>

#include <CrHeader.h>
#include <_OnModel_types.hpp>

namespace cc
{
	namespace onm
	{
		
		CcResult INSPECT_HONMODEL_CONFIG(HONMODEL OnModel)
		{
			if (OnModel == nullptr)return CcResult::FAILED_ARG_HANDLE_NOT_INITIALIZED;
			else
			{
				PRIVATE_HANDLE_ONMODEL* pOnModel = (PRIVATE_HANDLE_ONMODEL*)OnModel;

				switch (pOnModel->config.type)
				{
				case OnType::ISING:
				case OnType::XY:
				case OnType::HEISENBERG:
					break;

				default:
					return CcResult::FAILED_ARG_INVALID_FLAG;
				}

				switch (static_cast<uint64_t>(pOnModel->config.dimension))
				{
				case static_cast<uint64_t>(DIMENSION::DIMENSION_1):
				case static_cast<uint64_t>(DIMENSION::DIMENSION_2):
				case static_cast<uint64_t>(DIMENSION::DIMENSION_3):
					break;

				default:
					return CcResult::FAILED_ARG_INVALID_FLAG;
				}


			}


			return CcResult::SUCCESS;
		}

		CcResult INSPECT_HONMODEL(HONMODEL OnModel)
		{
			if (OnModel == nullptr)return CcResult::FAILED_ARG_HANDLE_NOT_INITIALIZED;
			else
			{
				PRIVATE_HANDLE_ONMODEL* pOnModel = (PRIVATE_HANDLE_ONMODEL*)OnModel;
			}

			return CcResult::SUCCESS;
		}

		CcResult INSPECT_HONMODEL_LITE(HONMODEL OnModel)
		{
			if (OnModel == nullptr)return CcResult::FAILED_ARG_HANDLE_NOT_INITIALIZED;
			else
			{
				PRIVATE_HANDLE_ONMODEL* pOnModel = (PRIVATE_HANDLE_ONMODEL*)OnModel;

				if (pOnModel->spin == nullptr)return CcResult::FAILED_ARG_HANDLE_NOT_INITIALIZED;
				if (pOnModel->spinaddr == nullptr)return CcResult::FAILED_ARG_HANDLE_NOT_INITIALIZED;
			}

			return CcResult::SUCCESS;
		}
	}
}


