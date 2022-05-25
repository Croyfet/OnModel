#include <OnModel_updating.hpp>

#include <random>
#include <cstdint>
#include <cmath>

#include <CrHeader.hpp>
#include <OnModel_types.hpp>
#include <OnModel_error.hpp>
#include <_OnModel_types.hpp>


namespace cc
{
	namespace onm
	{
		namespace
		{
			constexpr double PC_KB_BOLTZMANN = 1.0;
			constexpr double PC_PI_CIRCUMFERENCE = cst::math::PI_CIRCUMFERENCE;

			static double _balance_energy_2_ISING(HONMODEL OnModel, int64_t x, int64_t y);
			static double _balance_energy_2_XY(HONMODEL OnModel, SPIN_XY after, int64_t x, int64_t y);
			static double _balance_energy_2_HEISENBERG(HONMODEL OnModel, SPIN_HEISENBERG after, int64_t x, int64_t y);
			static double _balance_energy_3_ISING(HONMODEL OnModel, int64_t x, int64_t y, int64_t z);
			static double _balance_energy_3_XY(HONMODEL OnModel, SPIN_XY after, int64_t x, int64_t y, int64_t z);
			static double _balance_energy_3_HEISENBERG(HONMODEL OnModel, SPIN_HEISENBERG after, int64_t x, int64_t y, int64_t z);

		}


		CcResult ONM_Metropolis_SingleSpinFlip(HONMODEL OnModel, int64_t step)
		{
			PRIVATE_HANDLE_ONMODEL* pHandle = reinterpret_cast<PRIVATE_HANDLE_ONMODEL*>(OnModel);

			/* argument error check ***************************/

			if (pHandle == nullptr)return CcResult::FAILED_ARG_HANDLE_NOT_INITIALIZED;
			else
			{
				if (pHandle->spin == nullptr)return CcResult::FAILED_UNEXPECTED;
				if (pHandle->spinaddr == nullptr)return CcResult::FAILED_UNEXPECTED;
			}

			switch (static_cast<uint64_t>(pHandle->config.dimension))
			{
			case static_cast<uint64_t>(DIMENSION::DIMENSION_1):
			case static_cast<uint64_t>(DIMENSION::DIMENSION_2):
			case static_cast<uint64_t>(DIMENSION::DIMENSION_3):
				break;

			default:
				return CcResult::FAILED_ARG_WHY_COME_HERE;
			}

			/**************************************************/


			uint64_t mod = static_cast<uint64_t>(pHandle->config.type) | static_cast<uint64_t>(pHandle->config.dimension);

			switch (mod)
			{
			case static_cast<uint64_t>(DIMENSION::DIMENSION_2) | static_cast<uint64_t>(OnType::ISING):
				ONM_Metropolis_SingleSpinFlip_2_ISING(OnModel, step);
				break;

			case static_cast<uint64_t>(DIMENSION::DIMENSION_2) | static_cast<uint64_t>(OnType::XY):
				ONM_Metropolis_SingleSpinFlip_2_XY(OnModel, step);
				break;

			case static_cast<uint64_t>(DIMENSION::DIMENSION_2) | static_cast<uint64_t>(OnType::HEISENBERG):
				ONM_Metropolis_SingleSpinFlip_2_HEISENBERG(OnModel, step);
				break;

			case static_cast<uint64_t>(DIMENSION::DIMENSION_3) | static_cast<uint64_t>(OnType::ISING):
				ONM_Metropolis_SingleSpinFlip_3_ISING(OnModel, step);
				break;

			case static_cast<uint64_t>(DIMENSION::DIMENSION_3) | static_cast<uint64_t>(OnType::XY):
				ONM_Metropolis_SingleSpinFlip_3_XY(OnModel, step);
				break;

			case static_cast<uint64_t>(DIMENSION::DIMENSION_3) | static_cast<uint64_t>(OnType::HEISENBERG):
				ONM_Metropolis_SingleSpinFlip_3_HEISENBERG(OnModel, step);
				break;

			default:
				return CcResult::FAILED_WHY_COME_HERE;
			}

			return CcResult::SUCCESS;
		}

		void ONM_Metropolis_SingleSpinFlip_2_ISING(HONMODEL OnModel, int64_t step)
		{
			PRIVATE_HANDLE_ONMODEL* pHandle = reinterpret_cast<PRIVATE_HANDLE_ONMODEL*>(OnModel);

			int64_t lengthX = pHandle->config.x;
			int64_t lengthY = pHandle->config.y;

			double temperature = pHandle->config.T_Temperature;

			std::random_device seed;
			std::mt19937_64 engine(seed());
			std::uniform_real_distribution<> dist(0, 1);

			SPIN_ISING** si = (SPIN_ISING**)pHandle->spin;

			for (int64_t i = 0; i < step; i++)
			{
				for (int64_t j = 0; j < lengthY; j++)
				{
					for (int64_t k = 0; k < lengthX; k++)
					{
						double comp = _balance_energy_2_ISING(OnModel, k, j);
						if (comp < 0.0)si[j][k] *= (-1);
						else
						{
							double rate = dist(engine);
							double bale = exp(-comp / PC_KB_BOLTZMANN / temperature);

							if (bale > rate)si[j][k] *= (-1);
						}
					}
				}
			}

			UpdateMethod utype = pHandle->updatetype;
			if (utype != UpdateMethod::METROPOLIS)
			{
				pHandle->updatecount = 0;
				pHandle->updatetype = UpdateMethod::METROPOLIS;
			}

			pHandle->updatecount += step;

			return;
		}

		void ONM_Metropolis_SingleSpinFlip_2_XY(HONMODEL OnModel, int64_t step)
		{
			PRIVATE_HANDLE_ONMODEL* pHandle = reinterpret_cast<PRIVATE_HANDLE_ONMODEL*>(OnModel);

			int64_t lengthX = pHandle->config.x;
			int64_t lengthY = pHandle->config.y;

			double temperature = pHandle->config.T_Temperature;

			std::random_device seed;
			std::mt19937_64 engine(seed());
			std::uniform_real_distribution<> dist(0, 1);

			std::uniform_real_distribution<> aft(0, 1);

			SPIN_XY** sx = (SPIN_XY**)pHandle->spin;

			for (int64_t i = 0; i < step; i++)
			{
				for (int64_t j = 0; j < lengthY; j++)
				{
					for (int64_t k = 0; k < lengthX; k++)
					{
						SPIN_XY latest = { 0 };

						double rn = 2 * PC_PI_CIRCUMFERENCE * dist(engine);
						latest.x = cos(rn);
						latest.y = sin(rn);

						double comp = _balance_energy_2_XY(OnModel, latest, k, j);
						if (comp < 0.0)sx[j][k] = latest;
						else
						{
							double rate = dist(engine);
							double bale = exp(-comp / PC_KB_BOLTZMANN / temperature);

							if (bale > rate)sx[j][k] = latest;
						}
					}
				}
			}

			UpdateMethod utype = pHandle->updatetype;
			if (utype != UpdateMethod::METROPOLIS)
			{
				pHandle->updatecount = 0;
				pHandle->updatetype = UpdateMethod::METROPOLIS;
			}

			pHandle->updatecount += step;

			return;
		}

		void ONM_Metropolis_SingleSpinFlip_2_HEISENBERG(HONMODEL OnModel, int64_t step)
		{
			PRIVATE_HANDLE_ONMODEL* pHandle = reinterpret_cast<PRIVATE_HANDLE_ONMODEL*>(OnModel);

			int64_t lengthX = pHandle->config.x;
			int64_t lengthY = pHandle->config.y;

			double temperature = pHandle->config.T_Temperature;

			std::random_device seed;
			std::mt19937_64 engine(seed());
			std::uniform_real_distribution<> dist(0, 1);

			std::uniform_real_distribution<> aft_z(-1, 1);
			std::uniform_real_distribution<> aft_arg(0, 1);

			SPIN_HEISENBERG** sh = (SPIN_HEISENBERG**)pHandle->spin;

			for (int64_t i = 0; i < step; i++)
			{
				for (int64_t j = 0; j < lengthY; j++)
				{
					for (int64_t k = 0; k < lengthX; k++)
					{
						SPIN_HEISENBERG latest = { 0 };

						double zlen = aft_z(engine);
						double least = pow(1 - zlen * zlen, 1.0 / 2.0);
						double rn = 2 * PC_PI_CIRCUMFERENCE * aft_arg(engine);

						latest.z = zlen;
						latest.x = least * cos(rn);
						latest.y = least * sin(rn);

						double comp = _balance_energy_2_HEISENBERG(OnModel, latest, k, j);
						if (comp < 0.0)sh[j][k] = latest;
						else
						{
							double rate = dist(engine);
							double bale = exp(-comp / PC_KB_BOLTZMANN / temperature);

							if (bale > rate)sh[j][k] = latest;
						}
					}
				}
			}

			UpdateMethod utype = pHandle->updatetype;
			if (utype != UpdateMethod::METROPOLIS)
			{
				pHandle->updatecount = 0;
				pHandle->updatetype = UpdateMethod::METROPOLIS;
			}

			pHandle->updatecount += step;

			return;
		}

		void ONM_Metropolis_SingleSpinFlip_3_ISING(HONMODEL OnModel, int64_t step)
		{
			PRIVATE_HANDLE_ONMODEL* pHandle = reinterpret_cast<PRIVATE_HANDLE_ONMODEL*>(OnModel);

			int64_t lengthX = pHandle->config.x;
			int64_t lengthY = pHandle->config.y;
			int64_t lengthZ = pHandle->config.z;

			double temperature = pHandle->config.T_Temperature;

			std::random_device seed;
			std::mt19937_64 engine(seed());
			std::uniform_real_distribution<> dist(0, 1);

			SPIN_ISING*** si = (SPIN_ISING***)pHandle->spin;

			for (int64_t i = 0; i < step; i++)
			{
				for (int64_t j = 0; j < lengthZ; j++)
				{
					for (int64_t k = 0; k < lengthY; k++)
					{
						for (int64_t l = 0; l < lengthX; l++)
						{
							double comp = _balance_energy_3_ISING(OnModel, l, k, j);
							if (comp < 0.0)si[j][k][l] *= (-1);
							else
							{
								double rate = dist(engine);
								double bale = exp(-comp / PC_KB_BOLTZMANN / temperature);

								if (bale > rate)si[j][k][l] *= (-1);
							}
						}
					}
				}
			}

			UpdateMethod utype = pHandle->updatetype;
			if (utype != UpdateMethod::METROPOLIS)
			{
				pHandle->updatecount = 0;
				pHandle->updatetype = UpdateMethod::METROPOLIS;
			}

			pHandle->updatecount += step;

			return;
		}

		void ONM_Metropolis_SingleSpinFlip_3_XY(HONMODEL OnModel, int64_t step)
		{
			PRIVATE_HANDLE_ONMODEL* pHandle = reinterpret_cast<PRIVATE_HANDLE_ONMODEL*>(OnModel);

			int64_t lengthX = pHandle->config.x;
			int64_t lengthY = pHandle->config.y;
			int64_t lengthZ = pHandle->config.z;

			double temperature = pHandle->config.T_Temperature;

			std::random_device seed;
			std::mt19937_64 engine(seed());
			std::uniform_real_distribution<> dist(0, 1);

			std::uniform_real_distribution<> aft(0, 1);

			SPIN_XY*** sx = (SPIN_XY***)pHandle->spin;

			for (int64_t i = 0; i < step; i++)
			{
				for (int64_t j = 0; j < lengthZ; j++)
				{
					for (int64_t k = 0; k < lengthY; k++)
					{
						for (int64_t l = 0; l < lengthX; l++)
						{
							SPIN_XY latest = { 0 };

							double rn = 2 * PC_PI_CIRCUMFERENCE * dist(engine);
							latest.x = cos(rn);
							latest.y = sin(rn);

							double comp = _balance_energy_3_XY(OnModel, latest, l, k, j);
							if (comp < 0.0)sx[j][k][l] = latest;
							else
							{
								double rate = dist(engine);
								double bale = exp(-comp / PC_KB_BOLTZMANN / temperature);

								if (bale > rate)sx[j][k][l] = latest;
							}
						}
					}
				}
			}

			UpdateMethod utype = pHandle->updatetype;
			if (utype != UpdateMethod::METROPOLIS)
			{
				pHandle->updatecount = 0;
				pHandle->updatetype = UpdateMethod::METROPOLIS;
			}

			pHandle->updatecount += step;

			return;
		}
		
		void ONM_Metropolis_SingleSpinFlip_3_HEISENBERG(HONMODEL OnModel, int64_t step)
		{
			PRIVATE_HANDLE_ONMODEL* pHandle = reinterpret_cast<PRIVATE_HANDLE_ONMODEL*>(OnModel);

			int64_t lengthX = pHandle->config.x;
			int64_t lengthY = pHandle->config.y;
			int64_t lengthZ = pHandle->config.z;

			double temperature = pHandle->config.T_Temperature;

			std::random_device seed;
			std::mt19937_64 engine(seed());
			std::uniform_real_distribution<> dist(0, 1);

			std::uniform_real_distribution<> aft_z(-1, 1);
			std::uniform_real_distribution<> aft_arg(0, 1);

			SPIN_HEISENBERG*** sh = (SPIN_HEISENBERG***)pHandle->spin;

			for (int64_t i = 0; i < step; i++)
			{
				for (int64_t j = 0; j < lengthZ; j++)
				{
					for (int64_t k = 0; k < lengthY; k++)
					{
						for (int64_t l = 0; l < lengthX; l++)
						{
							SPIN_HEISENBERG latest = { 0 };

							double zlen = aft_z(engine);
							double least = pow(1 - zlen * zlen, 1.0 / 2.0);
							double rn = 2 * PC_PI_CIRCUMFERENCE * aft_arg(engine);

							latest.z = zlen;
							latest.x = least * cos(rn);
							latest.y = least * sin(rn);

							double comp = _balance_energy_3_HEISENBERG(OnModel, latest, l, k, j);
							if (comp < 0.0)sh[j][k][l] = latest;
							else
							{
								double rate = dist(engine);
								double bale = exp(-comp / PC_KB_BOLTZMANN / temperature);

								if (bale > rate)sh[j][k][l] = latest;
							}
						}
					}
				}
			}

			UpdateMethod utype = pHandle->updatetype;
			if (utype != UpdateMethod::METROPOLIS)
			{
				pHandle->updatecount = 0;
				pHandle->updatetype = UpdateMethod::METROPOLIS;
			}

			pHandle->updatecount += step;

			return;
		}
		

		namespace
		{

			static double _balance_energy_2_ISING(HONMODEL OnModel, int64_t x, int64_t y)
			{
				PRIVATE_HANDLE_ONMODEL* pHandle = reinterpret_cast<PRIVATE_HANDLE_ONMODEL*>(OnModel);

				double J = pHandle->config.J_ExchangeInteraction;
				double h = pHandle->config.H_MagneticField.x;

				SPINADDR_ISING** lpsi = (SPINADDR_ISING**)pHandle->spinaddr;

				int64_t center = *(lpsi[y + 1][x + 1]);
				int64_t top = *(lpsi[y + 2][x + 1]);
				int64_t left = *(lpsi[y + 1][x]);
				int64_t right = *(lpsi[y + 1][x + 2]);
				int64_t bottom = *(lpsi[y][x + 1]);

				double ans = 2.0 * (J * (double)center * (top + left + right + bottom) + h * center);

				return ans;
			}
			static double _balance_energy_2_XY(HONMODEL OnModel, SPIN_XY after, int64_t x, int64_t y)
			{
				PRIVATE_HANDLE_ONMODEL* pHandle = reinterpret_cast<PRIVATE_HANDLE_ONMODEL*>(OnModel);

				double ans = 0;

				double J = pHandle->config.J_ExchangeInteraction;
				MAGNETICFIELD mf = pHandle->config.H_MagneticField;

				SPINADDR_XY** lpsx = (SPINADDR_XY**)pHandle->spinaddr;

				SPIN_XY old = *(lpsx[y + 1][x + 1]);

				SPIN_XY latest = after;

				double before_top = old.x * lpsx[y + 2][x + 1]->x + old.y * lpsx[y + 2][x + 1]->y;
				double before_left = old.x * lpsx[y + 1][x]->x + old.y * lpsx[y + 1][x]->y;
				double before_right = old.x * lpsx[y + 1][x + 2]->x + old.y * lpsx[y + 1][x + 2]->y;
				double before_bottom = old.x * lpsx[y][x + 1]->x + old.y * lpsx[y][x + 1]->y;
				double before_term_m = old.x * mf.x + old.y * mf.y;

				double before_ans = ((-J) * (before_top + before_left + before_right + before_bottom) - before_term_m);

				double after_top = latest.x * lpsx[y + 2][x + 1]->x + latest.y * lpsx[y + 2][x + 1]->y;
				double after_left = latest.x * lpsx[y + 1][x]->x + latest.y * lpsx[y + 1][x]->y;
				double after_right = latest.x * lpsx[y + 1][x + 2]->x + latest.y * lpsx[y + 1][x + 2]->y;
				double after_bottom = latest.x * lpsx[y][x + 1]->x + latest.y * lpsx[y][x + 1]->y;
				double after_term_m = latest.x * mf.x + latest.y * mf.y;

				double after_ans = ((-J) * (after_top + after_left + after_right + after_bottom) - after_term_m);

				ans = after_ans - before_ans;

				return ans;
			}
			static double _balance_energy_2_HEISENBERG(HONMODEL OnModel, SPIN_HEISENBERG after, int64_t x, int64_t y)
			{
				PRIVATE_HANDLE_ONMODEL* pHandle = reinterpret_cast<PRIVATE_HANDLE_ONMODEL*>(OnModel);

				double ans = 0;

				double J = pHandle->config.J_ExchangeInteraction;
				MAGNETICFIELD mf = pHandle->config.H_MagneticField;

				SPINADDR_HEISENBERG** lpsh = (SPINADDR_HEISENBERG**)pHandle->spinaddr;

				SPIN_HEISENBERG old = *(lpsh[y + 1][x + 1]);

				SPIN_HEISENBERG latest = after;

				double before_top = old.x * lpsh[y + 2][x + 1]->x + old.y * lpsh[y + 2][x + 1]->y + old.z * lpsh[y + 2][x + 1]->z;
				double before_left = old.x * lpsh[y + 1][x]->x + old.y * lpsh[y + 1][x]->y + old.z * lpsh[y + 1][x]->z;
				double before_right = old.x * lpsh[y + 1][x + 2]->x + old.y * lpsh[y + 1][x + 2]->y + old.z * lpsh[y + 1][x + 2]->z;
				double before_bottom = old.x * lpsh[y][x + 1]->x + old.y * lpsh[y][x + 1]->y + old.z * lpsh[y][x + 1]->z;
				double before_term_m = old.x * mf.x + old.y * mf.y + old.z * mf.z;

				double before_ans = ((-J) * (before_top + before_left + before_right + before_bottom) - before_term_m);

				double after_top = latest.x * lpsh[y + 2][x + 1]->x + latest.y * lpsh[y + 2][x + 1]->y;
				double after_left = latest.x * lpsh[y + 1][x]->x + latest.y * lpsh[y + 1][x]->y + latest.z * lpsh[y + 1][x]->z;
				double after_right = latest.x * lpsh[y + 1][x + 2]->x + latest.y * lpsh[y + 1][x + 2]->y + latest.z * lpsh[y + 1][x + 2]->z;
				double after_bottom = latest.x * lpsh[y][x + 1]->x + latest.y * lpsh[y][x + 1]->y + latest.z * lpsh[y][x + 1]->z;
				double after_term_m = latest.x * mf.x + latest.y * mf.y + latest.z * mf.z;

				double after_ans = ((-J) * (after_top + after_left + after_right + after_bottom) - after_term_m);

				ans = after_ans - before_ans;

				return ans;
			}
			static double _balance_energy_3_ISING(HONMODEL OnModel, int64_t x, int64_t y, int64_t z)
			{
				PRIVATE_HANDLE_ONMODEL* pHandle = reinterpret_cast<PRIVATE_HANDLE_ONMODEL*>(OnModel);

				double J = pHandle->config.J_ExchangeInteraction;
				double h = pHandle->config.H_MagneticField.x;

				SPINADDR_ISING*** lpsi = (SPINADDR_ISING***)pHandle->spinaddr;

				int64_t center = *(lpsi[z + 1][y + 1][x + 1]);
				int64_t top = *(lpsi[z + 2][y + 1][x + 1]);
				int64_t bottom = *(lpsi[z][y + 1][x + 1]);
				int64_t north = *(lpsi[z + 1][y + 2][x + 1]);
				int64_t south = *(lpsi[z + 1][y][x + 1]);
				int64_t east = *(lpsi[z + 1][y + 1][x + 2]);
				int64_t west = *(lpsi[z + 1][y + 1][x]);

				double ans = 2.0 * (J * (double)center * (top + north + south + east + west + bottom) + h * center);

				return ans;
			}
			static double _balance_energy_3_XY(HONMODEL OnModel, SPIN_XY after, int64_t x, int64_t y, int64_t z)
			{
				PRIVATE_HANDLE_ONMODEL* pHandle = reinterpret_cast<PRIVATE_HANDLE_ONMODEL*>(OnModel);

				double ans = 0;

				double J = pHandle->config.J_ExchangeInteraction;
				MAGNETICFIELD mf = pHandle->config.H_MagneticField;

				SPINADDR_XY*** lpsx = (SPINADDR_XY***)pHandle->spinaddr;

				SPIN_XY old = *(lpsx[z + 1][y + 1][x + 1]);

				SPIN_XY latest = after;

				double before_top = old.x * lpsx[z + 2][y + 1][x + 1]->x + old.y * lpsx[z + 2][y + 1][x + 1]->y;
				double before_bottom = old.x * lpsx[z][y + 1][x + 1]->x + old.y * lpsx[z][y + 1][x + 1]->y;
				double before_north = old.x * lpsx[z + 1][y + 2][x + 1]->x + old.y * lpsx[z + 1][y + 2][x + 1]->y;
				double before_south = old.x * lpsx[z + 1][y][x + 1]->x + old.y * lpsx[z + 1][y][x + 1]->y;
				double before_east = old.x * lpsx[z + 1][y + 1][x + 2]->x + old.y * lpsx[z + 1][y + 1][x + 2]->y;
				double before_west = old.x * lpsx[z + 1][y + 1][x]->x + old.y * lpsx[z + 1][y + 1][x]->y;

				double before_term_m = old.x * mf.x + old.y * mf.y;

				double before_ans = ((-J) * (before_top + before_bottom + before_north + before_south + before_east + before_west) - before_term_m);

				double after_top = latest.x * lpsx[z + 2][y + 1][x + 1]->x + latest.y * lpsx[z + 2][y + 1][x + 1]->y;
				double after_bottom = latest.x * lpsx[z][y + 1][x + 1]->x + latest.y * lpsx[z][y + 1][x + 1]->y;
				double after_north = latest.x * lpsx[z + 1][y + 2][x + 1]->x + latest.y * lpsx[z + 1][y + 2][x + 1]->y;
				double after_south = latest.x * lpsx[z + 1][y][x + 1]->x + latest.y * lpsx[z + 1][y][x + 1]->y;
				double after_east = latest.x * lpsx[z + 1][y + 1][x + 2]->x + latest.y * lpsx[z + 1][y + 1][x + 2]->y;
				double after_west = latest.x * lpsx[z + 1][y + 1][x]->x + latest.y * lpsx[z + 1][y + 1][x]->y;

				double after_term_m = latest.x * mf.x + latest.y * mf.y;

				double after_ans = ((-J) * (after_top + after_bottom + after_north + after_south + after_east + after_west) - after_term_m);


				ans = after_ans - before_ans;

				return ans;
			}
			static double _balance_energy_3_HEISENBERG(HONMODEL OnModel, SPIN_HEISENBERG after, int64_t x, int64_t y, int64_t z)
			{
				PRIVATE_HANDLE_ONMODEL* pHandle = reinterpret_cast<PRIVATE_HANDLE_ONMODEL*>(OnModel);

				double ans = 0;

				double J = pHandle->config.J_ExchangeInteraction;
				MAGNETICFIELD mf = pHandle->config.H_MagneticField;

				SPINADDR_HEISENBERG*** lpsx = (SPINADDR_HEISENBERG***)pHandle->spinaddr;

				SPIN_HEISENBERG old = *(lpsx[z + 1][y + 1][x + 1]);

				SPIN_HEISENBERG latest = after;

				double before_top = old.x * lpsx[z + 2][y + 1][x + 1]->x + old.y * lpsx[z + 2][y + 1][x + 1]->y + old.z * lpsx[z + 2][y + 1][x + 1]->z;
				double before_bottom = old.x * lpsx[z][y + 1][x + 1]->x + old.y * lpsx[z][y + 1][x + 1]->y + old.z * lpsx[z][y + 1][x + 1]->z;
				double before_north = old.x * lpsx[z + 1][y + 2][x + 1]->x + old.y * lpsx[z + 1][y + 2][x + 1]->y + old.z * lpsx[z + 1][y + 2][x + 1]->z;
				double before_south = old.x * lpsx[z + 1][y][x + 1]->x + old.y * lpsx[z + 1][y][x + 1]->y + old.z * lpsx[z + 1][y][x + 1]->z;
				double before_east = old.x * lpsx[z + 1][y + 1][x + 2]->x + old.y * lpsx[z + 1][y + 1][x + 2]->y + old.z * lpsx[z + 1][y + 1][x + 2]->z;
				double before_west = old.x * lpsx[z + 1][y + 1][x]->x + old.y * lpsx[z + 1][y + 1][x]->y + old.z * lpsx[z + 1][y + 1][x]->z;

				double before_term_m = old.x * mf.x + old.y * mf.y + old.z * mf.z;

				double before_ans = ((-J) * (before_top + before_bottom + before_north + before_south + before_east + before_west) - before_term_m);

				double after_top = latest.x * lpsx[z + 2][y + 1][x + 1]->x + latest.y * lpsx[z + 2][y + 1][x + 1]->y + latest.z * lpsx[z + 2][y + 1][x + 1]->z;
				double after_bottom = latest.x * lpsx[z][y + 1][x + 1]->x + latest.y * lpsx[z][y + 1][x + 1]->y + latest.z * lpsx[z][y + 1][x + 1]->z;
				double after_north = latest.x * lpsx[z + 1][y + 2][x + 1]->x + latest.y * lpsx[z + 1][y + 2][x + 1]->y + latest.z * lpsx[z + 1][y + 2][x + 1]->z;
				double after_south = latest.x * lpsx[z + 1][y][x + 1]->x + latest.y * lpsx[z + 1][y][x + 1]->y + latest.z * lpsx[z + 1][y][x + 1]->z;
				double after_east = latest.x * lpsx[z + 1][y + 1][x + 2]->x + latest.y * lpsx[z + 1][y + 1][x + 2]->y + latest.z * lpsx[z + 1][y + 1][x + 2]->z;
				double after_west = latest.x * lpsx[z + 1][y + 1][x]->x + latest.y * lpsx[z + 1][y + 1][x]->y + latest.z * lpsx[z + 1][y + 1][x]->z;

				double after_term_m = latest.x * mf.x + latest.y * mf.y + latest.z * mf.z;

				double after_ans = ((-J) * (after_top + after_bottom + after_north + after_south + after_east + after_west) - after_term_m);


				ans = after_ans - before_ans;

				return ans;
			}

		}

	}
}
