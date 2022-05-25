#include <OnModel_datacollection.hpp>

#include <cmath>
#include <complex>

#include <CrHeader.hpp>
#include <OnModel_error.hpp>
#include <_OnModel_types.hpp>


namespace cc
{
	namespace onm
	{
		
		namespace
		{
			static double _energy_2_ISING(HONMODEL OnModel, int64_t x, int64_t y);
			static double _energy_2_XY(HONMODEL OnModel, int64_t x, int64_t y);
			static double _energy_2_HEISENBERG(HONMODEL OnModel, int64_t x, int64_t y);
			static double _energy_3_ISING(HONMODEL OnModel, int64_t x, int64_t y, int64_t z);
			static double _energy_3_XY(HONMODEL OnModel, int64_t x, int64_t y, int64_t z);
			static double _energy_3_HEISENBERG(HONMODEL OnModel, int64_t x, int64_t y, int64_t z);

			static std::complex<double> _cis(double theta);

		}


		CcResult ONM_get_status(HONMODEL OnModel, ONM_STATUS* status)
		{
			PRIVATE_HANDLE_ONMODEL* pHandle = reinterpret_cast<PRIVATE_HANDLE_ONMODEL*>(OnModel);

			/* argument error check ***************************/

			if (pHandle == nullptr)return CcResult::FAILED_ARG_HANDLE_NOT_INITIALIZED;
			else
			{
				if (pHandle->spin == nullptr)return CcResult::FAILED_ARG_UNEXPECTED;
				if (pHandle->spinaddr == nullptr)return CcResult::FAILED_ARG_UNEXPECTED;
			}

			if (status == nullptr)return CcResult::FAILED_ARG_NULLPTR;

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

			uint64_t cmb = static_cast<uint64_t>(pHandle->config.dimension) | static_cast<uint64_t>(pHandle->config.type);

			switch (cmb)
			{
			case static_cast<uint64_t>(DIMENSION::DIMENSION_2) | static_cast<uint64_t>(OnType::ISING):
				ONM_get_status_2_ISING(OnModel, status);
				break;

			case static_cast<uint64_t>(DIMENSION::DIMENSION_2) | static_cast<uint64_t>(OnType::XY):
				ONM_get_status_2_XY(OnModel, status);
				break;

			case static_cast<uint64_t>(DIMENSION::DIMENSION_2) | static_cast<uint64_t>(OnType::HEISENBERG):
				ONM_get_status_2_HEISENBERG(OnModel, status);
				break;

			case static_cast<uint64_t>(DIMENSION::DIMENSION_3) | static_cast<uint64_t>(OnType::ISING):
				ONM_get_status_3_ISING(OnModel, status);
				break;

			case static_cast<uint64_t>(DIMENSION::DIMENSION_3) | static_cast<uint64_t>(OnType::XY):
				ONM_get_status_3_XY(OnModel, status);
				break;

			case static_cast<uint64_t>(DIMENSION::DIMENSION_3) | static_cast<uint64_t>(OnType::HEISENBERG):
				ONM_get_status_3_HEISENBERG(OnModel, status);
				break;

			default:
				return CcResult::FAILED_WHY_COME_HERE;
			}

			return CcResult::SUCCESS;
		}

		void ONM_get_status_2_ISING(HONMODEL OnModel, ONM_STATUS* status)
		{
			PRIVATE_HANDLE_ONMODEL* pHandle = reinterpret_cast<PRIVATE_HANDLE_ONMODEL*>(OnModel);

			int64_t lengthX = pHandle->config.x;
			int64_t lengthY = pHandle->config.y;
			int64_t lattice = lengthX * lengthY;

			SPIN_ISING** si = (SPIN_ISING**)pHandle->spin;

			double e = 0;
			double m = 0;

			for (int64_t i = 0; i < lengthY; i++)
			{
				for (int64_t j = 0; j < lengthX; j++)
				{
					e += _energy_2_ISING(OnModel, j, i);
					m += si[i][j];
				}
			}
			e /= lattice;
			m /= lattice;

			status->E_Energy = e;
			status->M_Magnetization.x = m;
			status->M_Magnetization.y = 0;
			status->M_Magnetization.z = 0;

			return;
		}

		void ONM_get_status_2_XY(HONMODEL OnModel, ONM_STATUS* status)
		{
			PRIVATE_HANDLE_ONMODEL* pHandle = reinterpret_cast<PRIVATE_HANDLE_ONMODEL*>(OnModel);

			int64_t lengthX = pHandle->config.x;
			int64_t lengthY = pHandle->config.y;
			int64_t lattice = lengthX * lengthY;

			SPIN_XY** sx = (SPIN_XY**)pHandle->spin;

			double e = 0;
			double mx = 0;
			double my = 0;

			for (int64_t i = 0; i < lengthY; i++)
			{
				for (int64_t j = 0; j < lengthX; j++)
				{
					e += _energy_2_XY(OnModel, j, i);
					mx += sx[i][j].x;
					my += sx[i][j].y;
				}
			}
			e /= lattice;
			mx /= lattice;
			my /= lattice;

			status->E_Energy = e;
			status->M_Magnetization.x = mx;
			status->M_Magnetization.y = my;
			status->M_Magnetization.z = 0;

			return;
		}

		void ONM_get_status_2_HEISENBERG(HONMODEL OnModel, ONM_STATUS* status)
		{
			PRIVATE_HANDLE_ONMODEL* pHandle = reinterpret_cast<PRIVATE_HANDLE_ONMODEL*>(OnModel);

			int64_t lengthX = pHandle->config.x;
			int64_t lengthY = pHandle->config.y;
			int64_t lattice = lengthX * lengthY;

			SPIN_HEISENBERG** sh = (SPIN_HEISENBERG**)pHandle->spin;

			double e = 0;
			double mx = 0;
			double my = 0;
			double mz = 0;

			for (int64_t i = 0; i < lengthY; i++)
			{
				for (int64_t j = 0; j < lengthX; j++)
				{
					e += _energy_2_HEISENBERG(OnModel, j, i);
					mx += sh[i][j].x;
					my += sh[i][j].y;
					mz += sh[i][j].z;
				}
			}
			e /= lattice;
			mx /= lattice;
			my /= lattice;
			mz /= lattice;

			status->E_Energy = e;
			status->M_Magnetization.x = mx;
			status->M_Magnetization.y = my;
			status->M_Magnetization.z = mz;

			return;
		}

		void ONM_get_status_3_ISING(HONMODEL OnModel, ONM_STATUS* status)
		{
			PRIVATE_HANDLE_ONMODEL* pHandle = reinterpret_cast<PRIVATE_HANDLE_ONMODEL*>(OnModel);

			int64_t lengthX = pHandle->config.x;
			int64_t lengthY = pHandle->config.y;
			int64_t lengthZ = pHandle->config.z;
			int64_t lattice = lengthX * lengthY * lengthZ;

			SPIN_ISING*** si = (SPIN_ISING***)pHandle->spin;

			double e = 0;
			double m = 0;

			for (int64_t i = 0; i < lengthZ; i++)
			{
				for (int64_t j = 0; j < lengthY; j++)
				{
					for (int64_t k = 0; k < lengthX; k++)
					{
						e += _energy_3_ISING(OnModel, k, j, i);
						m += si[i][j][k];
					}
				}
			}
			e /= lattice;
			m /= lattice;

			status->E_Energy = e;
			status->M_Magnetization.x = m;
			status->M_Magnetization.y = 0;
			status->M_Magnetization.z = 0;

			return;
		}

		void ONM_get_status_3_XY(HONMODEL OnModel, ONM_STATUS* status)
		{
			PRIVATE_HANDLE_ONMODEL* pHandle = reinterpret_cast<PRIVATE_HANDLE_ONMODEL*>(OnModel);

			int64_t lengthX = pHandle->config.x;
			int64_t lengthY = pHandle->config.y;
			int64_t lengthZ = pHandle->config.z;
			int64_t lattice = lengthX * lengthY * lengthZ;

			SPIN_XY*** sx = (SPIN_XY***)pHandle->spin;

			double e = 0;
			double mx = 0;
			double my = 0;

			for (int64_t i = 0; i < lengthZ; i++)
			{
				for (int64_t j = 0; j < lengthY; j++)
				{
					for (int64_t k = 0; k < lengthX; k++)
					{
						e += _energy_3_XY(OnModel, k, j, i);
						mx += sx[i][j][k].x;
						my += sx[i][j][k].y;
					}
				}
			}
			e /= lattice;
			mx /= lattice;
			my /= lattice;

			status->E_Energy = e;
			status->M_Magnetization.x = mx;
			status->M_Magnetization.y = my;
			status->M_Magnetization.z = 0;

			return;
		}

		void ONM_get_status_3_HEISENBERG(HONMODEL OnModel, ONM_STATUS* status)
		{
			PRIVATE_HANDLE_ONMODEL* pHandle = reinterpret_cast<PRIVATE_HANDLE_ONMODEL*>(OnModel);

			int64_t lengthX = pHandle->config.x;
			int64_t lengthY = pHandle->config.y;
			int64_t lengthZ = pHandle->config.z;
			int64_t lattice = lengthX * lengthY * lengthZ;

			SPIN_HEISENBERG*** sh = (SPIN_HEISENBERG***)pHandle->spin;

			double e = 0;
			double mx = 0;
			double my = 0;
			double mz = 0;

			for (int64_t i = 0; i < lengthZ; i++)
			{
				for (int64_t j = 0; j < lengthY; j++)
				{
					for (int64_t k = 0; k < lengthX; k++)
					{
						e += _energy_3_HEISENBERG(OnModel, k, j, i);
						mx += sh[i][j][k].x;
						my += sh[i][j][k].y;
						mz += sh[i][j][k].z;
					}
				}
			}
			e /= lattice;
			mx /= lattice;
			my /= lattice;
			mz /= lattice;

			status->E_Energy = e;
			status->M_Magnetization.x = mx;
			status->M_Magnetization.y = my;
			status->M_Magnetization.z = mz;

			return;
		}
		

		namespace
		{
			static double _energy_2_ISING(HONMODEL OnModel, int64_t x, int64_t y)
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

				double ans = 0.5 * (-J) * (double)center * (top + left + right + bottom) - h * center;

				return ans;
			}

			static double _energy_2_XY(HONMODEL OnModel, int64_t x, int64_t y)
			{
				PRIVATE_HANDLE_ONMODEL* pHandle = reinterpret_cast<PRIVATE_HANDLE_ONMODEL*>(OnModel);

				double ans = 0;

				double J = pHandle->config.J_ExchangeInteraction;
				MAGNETICFIELD mf = pHandle->config.H_MagneticField;

				SPINADDR_XY** lpsx = (SPINADDR_XY**)pHandle->spinaddr;

				SPIN_XY old = *(lpsx[y + 1][x + 1]);

				double before_top = old.x * lpsx[y + 2][x + 1]->x + old.y * lpsx[y + 2][x + 1]->y;
				double before_left = old.x * lpsx[y + 1][x]->x + old.y * lpsx[y + 1][x]->y;
				double before_right = old.x * lpsx[y + 1][x + 2]->x + old.y * lpsx[y + 1][x + 2]->y;
				double before_bottom = old.x * lpsx[y][x + 1]->x + old.y * lpsx[y][x + 1]->y;
				double before_term_m = old.x * mf.x + old.y * mf.y;

				ans = 0.5 * (-J) * (before_top + before_left + before_right + before_bottom) - before_term_m;

				return ans;
			}

			static double _energy_2_HEISENBERG(HONMODEL OnModel, int64_t x, int64_t y)
			{
				PRIVATE_HANDLE_ONMODEL* pHandle = reinterpret_cast<PRIVATE_HANDLE_ONMODEL*>(OnModel);

				double ans = 0;

				double J = pHandle->config.J_ExchangeInteraction;
				MAGNETICFIELD mf = pHandle->config.H_MagneticField;

				SPINADDR_HEISENBERG** lpsh = (SPINADDR_HEISENBERG**)pHandle->spinaddr;

				SPIN_HEISENBERG old = *(lpsh[y + 1][x + 1]);

				double before_top = old.x * lpsh[y + 2][x + 1]->x + old.y * lpsh[y + 2][x + 1]->y + old.z * lpsh[y + 2][x + 1]->z;
				double before_left = old.x * lpsh[y + 1][x]->x + old.y * lpsh[y + 1][x]->y + old.z * lpsh[y + 1][x]->z;
				double before_right = old.x * lpsh[y + 1][x + 2]->x + old.y * lpsh[y + 1][x + 2]->y + old.z * lpsh[y + 1][x + 2]->z;
				double before_bottom = old.x * lpsh[y][x + 1]->x + old.y * lpsh[y][x + 1]->y + old.z * lpsh[y][x + 1]->z;
				double before_term_m = old.x * mf.x + old.y * mf.y + old.z * mf.z;

				ans = 0.5 * (-J) * (before_top + before_left + before_right + before_bottom) - before_term_m;

				return ans;
			}

			static double _energy_3_ISING(HONMODEL OnModel, int64_t x, int64_t y, int64_t z)
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

				double ans = 0.5 * J * (double)center * (top + north + south + east + west + bottom) - h * center;

				return ans;
			}

			static double _energy_3_XY(HONMODEL OnModel, int64_t x, int64_t y, int64_t z)
			{
				PRIVATE_HANDLE_ONMODEL* pHandle = reinterpret_cast<PRIVATE_HANDLE_ONMODEL*>(OnModel);

				double ans = 0;

				double J = pHandle->config.J_ExchangeInteraction;
				MAGNETICFIELD mf = pHandle->config.H_MagneticField;

				SPINADDR_XY*** lpsx = (SPINADDR_XY***)pHandle->spinaddr;

				SPIN_XY old = *(lpsx[z + 1][y + 1][x + 1]);

				double before_top = old.x * lpsx[z + 2][y + 1][x + 1]->x + old.y * lpsx[z + 2][y + 1][x + 1]->y;
				double before_bottom = old.x * lpsx[z][y + 1][x + 1]->x + old.y * lpsx[z][y + 1][x + 1]->y;
				double before_north = old.x * lpsx[z + 1][y + 2][x + 1]->x + old.y * lpsx[z + 1][y + 2][x + 1]->y;
				double before_south = old.x * lpsx[z + 1][y][x + 1]->x + old.y * lpsx[z + 1][y][x + 1]->y;
				double before_east = old.x * lpsx[z + 1][y + 1][x + 2]->x + old.y * lpsx[z + 1][y + 1][x + 2]->y;
				double before_west = old.x * lpsx[z + 1][y + 1][x]->x + old.y * lpsx[z + 1][y + 1][x]->y;

				double before_term_m = old.x * mf.x + old.y * mf.y;

				ans = 0.5 * (-J) * (before_top + before_bottom + before_north + before_south + before_east + before_west) - before_term_m;

				return ans;
			}

			static double _energy_3_HEISENBERG(HONMODEL OnModel, int64_t x, int64_t y, int64_t z)
			{
				PRIVATE_HANDLE_ONMODEL* pHandle = reinterpret_cast<PRIVATE_HANDLE_ONMODEL*>(OnModel);

				double ans = 0;

				double J = pHandle->config.J_ExchangeInteraction;
				MAGNETICFIELD mf = pHandle->config.H_MagneticField;

				SPINADDR_HEISENBERG*** lpsx = (SPINADDR_HEISENBERG***)pHandle->spinaddr;

				SPIN_HEISENBERG old = *(lpsx[z + 1][y + 1][x + 1]);

				double before_top = old.x * lpsx[z + 2][y + 1][x + 1]->x + old.y * lpsx[z + 2][y + 1][x + 1]->y + old.z * lpsx[z + 2][y + 1][x + 1]->z;
				double before_bottom = old.x * lpsx[z][y + 1][x + 1]->x + old.y * lpsx[z][y + 1][x + 1]->y + old.z * lpsx[z][y + 1][x + 1]->z;
				double before_north = old.x * lpsx[z + 1][y + 2][x + 1]->x + old.y * lpsx[z + 1][y + 2][x + 1]->y + old.z * lpsx[z + 1][y + 2][x + 1]->z;
				double before_south = old.x * lpsx[z + 1][y][x + 1]->x + old.y * lpsx[z + 1][y][x + 1]->y + old.z * lpsx[z + 1][y][x + 1]->z;
				double before_east = old.x * lpsx[z + 1][y + 1][x + 2]->x + old.y * lpsx[z + 1][y + 1][x + 2]->y + old.z * lpsx[z + 1][y + 1][x + 2]->z;
				double before_west = old.x * lpsx[z + 1][y + 1][x]->x + old.y * lpsx[z + 1][y + 1][x]->y + old.z * lpsx[z + 1][y + 1][x]->z;

				double before_term_m = old.x * mf.x + old.y * mf.y + old.z * mf.z;

				ans = 0.5 * (-J) * (before_top + before_bottom + before_north + before_south + before_east + before_west) - before_term_m;

				return ans;
			}

		}



		CcResult ONM_get_StructureFactor(HONMODEL OnModel, double sf[3], double wavenum[])
		{
			PRIVATE_HANDLE_ONMODEL* pHandle = reinterpret_cast<PRIVATE_HANDLE_ONMODEL*>(OnModel);

			constexpr double low_threshold = -1024;
			constexpr double high_threshold = 1024;

			double retd[3] = { 0.0, 0.0, 0.0 };

			/* argument error check ***************************/

			if (OnModel == nullptr)return CcResult::FAILED_ARG_HANDLE_NOT_INITIALIZED;
			else
			{
				if (pHandle->spin == nullptr)return CcResult::FAILED_ARG_UNEXPECTED;
				if (pHandle->spinaddr == nullptr)return CcResult::FAILED_ARG_UNEXPECTED;

				if (pHandle->config.type != OnType::HEISENBERG)return CcResult::FAILED_ARG_INVALID_FLAG;
			}

			if (sf == nullptr)return CcResult::FAILED_ARG_NULLPTR;

			if (wavenum == nullptr)return CcResult::FAILED_ARG_NULLPTR;
			else
			{
				switch (static_cast<uint64_t>(pHandle->config.dimension))
				{
				case static_cast<uint64_t>(DIMENSION::DIMENSION_2):
				{
					if ((wavenum[0] < low_threshold) || (wavenum[0] > high_threshold))return CcResult::FAILED_ARG_RANGE_OUT;
					if ((wavenum[1] < low_threshold) || (wavenum[1] > high_threshold))return CcResult::FAILED_ARG_RANGE_OUT;
				}
				break;
				case static_cast<uint64_t>(DIMENSION::DIMENSION_3):
				{
					if ((wavenum[0] < low_threshold) || (wavenum[0] > high_threshold))return CcResult::FAILED_ARG_RANGE_OUT;
					if ((wavenum[1] < low_threshold) || (wavenum[1] > high_threshold))return CcResult::FAILED_ARG_RANGE_OUT;
					if ((wavenum[2] < low_threshold) || (wavenum[2] > high_threshold))return CcResult::FAILED_ARG_RANGE_OUT;
				}
				break;

				default:
					return CcResult::FAILED_WHY_COME_HERE;
				}
			}


			/**************************************************/

			int64_t lengthX = 0;
			int64_t lengthY = 0;
			int64_t lengthZ = 0;
			int64_t density = 0;

			switch (static_cast<uint64_t>(pHandle->config.dimension))
			{
			case static_cast<uint64_t>(DIMENSION::DIMENSION_2):
			{
				lengthX = pHandle->config.x;
				lengthY = pHandle->config.y;
				lengthZ = 0;

				density = lengthX * lengthY;
			}
				break;
			case static_cast<uint64_t>(DIMENSION::DIMENSION_3):
			{
				lengthX = pHandle->config.x;
				lengthY = pHandle->config.y;
				lengthZ = pHandle->config.z;

				density = lengthX * lengthY * lengthZ;
			}
				break;

			default:
				return CcResult::FAILED_WHY_COME_HERE;
			}


			switch (static_cast<uint64_t>(pHandle->config.dimension))
			{
			case static_cast<uint64_t>(DIMENSION::DIMENSION_2):
			{
				SPIN_HEISENBERG** sh = (SPIN_HEISENBERG**)pHandle->spin;
				double wvnm[2] = { 0.0, 0.0 };
				wvnm[0] = wavenum[0];
				wvnm[1] = wavenum[1];
				std::complex<double> cav[3] = { (0.0, 0.0), (0.0, 0.0), (0.0, 0.0)};

				for (int64_t i = 0; i < lengthY; i++)
				{
					for (int64_t j = 0; j < lengthX; j++)
					{
						std::complex<double> c = _cis(i * wavenum[1] + j * wavenum[0]);

						cav[0].real(sh[i][j].x * c.real() + cav[0].real());
						cav[0].imag(sh[i][j].x * c.imag() + cav[0].imag());
						cav[1].real(sh[i][j].y * c.real() + cav[1].real());
						cav[1].imag(sh[i][j].y * c.imag() + cav[1].imag());
						cav[2].real(sh[i][j].z * c.real() + cav[2].real());
						cav[2].imag(sh[i][j].z * c.imag() + cav[2].imag());
					}
				}

				retd[0] = cav[0].real() * cav[0].real() + cav[0].imag() * cav[0].imag();
				retd[1] = cav[1].real() * cav[1].real() + cav[1].imag() * cav[1].imag();
				retd[2] = cav[2].real() * cav[2].real() + cav[2].imag() * cav[2].imag();
				retd[0] /= density;
				retd[1] /= density;
				retd[2] /= density;
			}
				break;

			case static_cast<uint64_t>(DIMENSION::DIMENSION_3):
			{
				SPIN_HEISENBERG*** sh = (SPIN_HEISENBERG***)pHandle->spin;
				double wvnm[3] = { 0.0, 0.0, 0.0 };
				wvnm[0] = wavenum[0];
				wvnm[1] = wavenum[1];
				wvnm[2] = wavenum[2];
				std::complex<double> cav[3] = { (0.0, 0.0), (0.0, 0.0), (0.0, 0.0) };

				for (int64_t i = 0; i < lengthZ; i++)
				{
					for (int64_t j = 0; j < lengthY; j++)
					{
						for (int64_t k = 0; k < lengthX; k++)
						{
							std::complex<double> c = _cis(i * wavenum[2] + j * wavenum[1] + k * wavenum[0]);

							cav[0].real(sh[i][j][k].x * c.real() + cav[0].real());
							cav[0].imag(sh[i][j][k].x * c.imag() + cav[0].imag());
							cav[1].real(sh[i][j][k].y * c.real() + cav[1].real());
							cav[1].imag(sh[i][j][k].y * c.imag() + cav[1].imag());
							cav[2].real(sh[i][j][k].z * c.real() + cav[2].real());
							cav[2].imag(sh[i][j][k].z * c.imag() + cav[2].imag());
						}
					}
				}

				retd[0] = cav[0].real() * cav[0].real() + cav[0].imag() * cav[0].imag();
				retd[1] = cav[1].real() * cav[1].real() + cav[1].imag() * cav[1].imag();
				retd[2] = cav[2].real() * cav[2].real() + cav[2].imag() * cav[2].imag();
				retd[0] /= density;
				retd[1] /= density;
				retd[2] /= density;
			}
				break;

			default:
				return CcResult::FAILED_WHY_COME_HERE;
			}


			double* tmpn = sf;
			*tmpn = retd[0];
			tmpn++;
			*tmpn = retd[1];
			tmpn++;
			*tmpn = retd[2];

			return CcResult::SUCCESS;
		}

		namespace
		{
			static std::complex<double> _cis(double theta)
			{
				std::complex<double> retcplx = { 0.0, 0.0 };

				retcplx.real(cos(theta));
				retcplx.imag(sin(theta));

				return retcplx;
			}
		}

	}
}
