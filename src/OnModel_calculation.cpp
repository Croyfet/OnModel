#include <OnModel_calculation.hpp>

#include <cmath>

#include <CrHeader.hpp>
#include <OnModel_error.hpp>

namespace cc
{
	namespace ONM
	{


		CcResult ONM_calc_CorrelationLength(double* cl, double length, double pi_sf, double wn_sf)
		{
			double ret = 0.0;

			/* argument error check */


			if (cl == nullptr)return CcResult::FAILED_ARG_NULLPTR;

			if (pi_sf <= 0)return CcResult::FAILED_ARG_TOO_SMALL;

			if (pi_sf <= 0)return CcResult::FAILED_ARG_TOO_SMALL;
			if (wn_sf <= 0)return CcResult::FAILED_ARG_TOO_SMALL;

			/***********************************************/

			double denom = std::sin(cc::cst::math::PI_CIRCUMFERENCE / length);

			if (denom == 0)
			{
				*cl = 0;
				return CcResult::FAILED_ARG_TOO_SMALL;
			}
			else
			{
				ret = std::sqrt(pi_sf / wn_sf - 1) / denom / 2;
				*cl = ret;
			}

			return CcResult::SUCCESS;
		}




	}
}






