#include <cstdio>

#include <CrHeader.hpp>
#include <OnModel.hpp>

int main()
{
	using namespace cc::onm;
	using cc::CcResult;

	HONMODEL on = nullptr;
	ONMODEL_CONFIG oc = { OnType::HEISENBERG, -1.0, 0.01, DIMENSION::DIMENSION_2, 32, 24, 0, 0, 0, 0 };

	CcResult a = ONM_new(&on, oc);

	CcResult b = ONM_init(on, InitType::MT);

	InspectStatus tmpis = InspectStatus::LATTICE | InspectStatus::DIMENSION | InspectStatus::EXCHANGEINTERACTION | InspectStatus::TEMPERATURE | InspectStatus::UPDATETYPE | InspectStatus::INITTYPE;
	CcResult c = ONM_inspect_config(on, tmpis);

	{
		ONM_STATUS os;
		ONM_get_status(on, &os);
		printf("Energy : %lf Magnetization : %lf\r\n", os.E_Energy, os.M_Magnetization.z);
	}


	printf("\r\n");

	CcResult d = ONM_Metropolis_SingleSpinFlip(on, 1000);
	c = ONM_inspect_config(on, tmpis);

	{
		ONM_STATUS os;
		ONM_get_status(on, &os);
		printf("Energy : %lf Magnetization : %lf\r\n", os.E_Energy, os.M_Magnetization.z);
	}

	ONM_free(on);

	return 0;
}

