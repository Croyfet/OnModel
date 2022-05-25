#include <OnModel_Debugging.hpp>

#include <iostream>
#include <iomanip>

#include <CrHeader.hpp>
#include <OnModel_types.hpp>
#include <OnModel_error.hpp>
#include <_OnModel_types.hpp>


namespace cc
{
	namespace onm
	{
		CcResult ONM_inspect_spin(HONMODEL OnModel, InspectSpin inspect_spin)
		{
			PRIVATE_HANDLE_ONMODEL* pHandle = reinterpret_cast<PRIVATE_HANDLE_ONMODEL*>(OnModel);

			/* argument error check ***************************/

			if (pHandle == nullptr)return CcResult::FAILED_ARG_NULLPTR;
			else
			{
				if (pHandle->spin == nullptr)return CcResult::FAILED_ARG_UNEXPECTED;
				if (pHandle->spinaddr == nullptr)return CcResult::FAILED_ARG_UNEXPECTED;
			}

			switch (inspect_spin)
			{
			case InspectSpin::SPIN:
			case InspectSpin::ADDRESS:
			case InspectSpin::SPINADDRESS:
				break;

			default:
				return CcResult::FAILED_ARG_WHY_COME_HERE;
			}

			int64_t lengthX = pHandle->config.x;
			int64_t lengthY = pHandle->config.y;
			int64_t lengthZ = pHandle->config.z;

			switch (pHandle->config.type)
			{
			case OnType::ISING:
			case OnType::XY:
			case OnType::HEISENBERG:
				break;
			default:
				return CcResult::FAILED_ARG_WHY_COME_HERE;
			}

			/**************************************************/


			switch (static_cast<uint64_t>(pHandle->config.dimension))
			{
				//case 1: reserved
			case static_cast<uint64_t>(DIMENSION::DIMENSION_2):
			{
				switch (pHandle->config.type)
				{
				case OnType::ISING:
				{
					SPIN_ISING** si = (SPIN_ISING**)pHandle->spin;
					SPINADDR_ISING** lpsi = (SPINADDR_ISING**)pHandle->spinaddr;
					switch (inspect_spin)
					{
					case InspectSpin::SPIN:
					{
						std::cout << "Spin status --------\r\n" << std::endl;
						for (int64_t i = lengthY - 1; i >= 0; i--)
						{
							std::cout << "    ";
							for (int64_t j = 0; j < lengthX; j++)
							{
								std::cout << std::setw(4) << si[i][j];
							}
							std::cout << std::endl;
						}
					}
					break;

					case InspectSpin::ADDRESS:
					{

						std::cout << "Spin Address -------\r\n" << std::endl;
						for (int64_t i = lengthY + 1; i >= 0; i--)
						{
							std::cout << "    ";
							for (int64_t j = 0; j < lengthX + 2; j++)
							{
								std::cout << lpsi[i][j] << " ";
							}
							std::cout << std::endl;
						}

					}
					break;

					case InspectSpin::SPINADDRESS:
					{

						std::cout << "Spin Address -------\r\n" << std::endl;
						for (int64_t i = lengthY + 1; i >= 0; i--)
						{
							std::cout << "    ";
							for (int64_t j = 0; j < lengthX + 2; j++)
							{
								if ((lpsi[i][j]) == nullptr) std::cout << "     ";
								else std::cout << std::setw(4) << *(lpsi[i][j]) << " ";
							}
							std::cout << std::endl;
						}

					}

					break;

					default:
						return CcResult::FAILED_INVALID_FLAG;
					}

				}
				break;

				case OnType::XY:
				{
					SPIN_XY** sx = (SPIN_XY**)pHandle->spin;
					SPINADDR_XY** lpsx = (SPINADDR_XY**)pHandle->spinaddr;
					switch (inspect_spin)
					{
					case InspectSpin::SPIN:
					{
						std::cout << "Spin status --------\r\n" << std::endl;
						for (int64_t i = lengthY - 1; i >= 0; i--)
						{
							std::cout << "          ";
							for (int64_t j = 0; j < lengthX; j++)
							{
								std::cout << std::setw(4) << std::fixed << std::setprecision(2) << sx[i][j].x << "/" << sx[i][j].y << " ";
							}
							std::cout << std::endl;
						}
					}
					break;

					case InspectSpin::ADDRESS:
					{
						std::cout << "Spin Address --------\r\n" << std::endl;
						for (int64_t i = lengthY + 1; i >= 0; i--)
						{
							for (int64_t j = 0; j < lengthX + 2; j++)
							{
								std::cout << lpsx[i][j] << " ";
							}
							std::cout << std::endl;
						}
					}
					break;

					case InspectSpin::SPINADDRESS:
					{
						std::cout << "Spin Address --------\r\n" << std::endl;
						for (int64_t i = lengthY + 1; i >= 0; i--)
						{
							for (int64_t j = 0; j < lengthX + 2; j++)
							{
								std::cout << std::setw(10) << std::fixed << std::setprecision(1) << lpsx[i][j]->x << "/" << lpsx[i][j]->y << " ";
							}
							std::cout << std::endl;
						}
					}

					break;

					default:
						return CcResult::FAILED_INVALID_FLAG;
					}
				}
				break;

				case OnType::HEISENBERG:
				{
					SPIN_HEISENBERG** sh = (SPIN_HEISENBERG**)pHandle->spin;
					SPINADDR_HEISENBERG** lpsh = (SPINADDR_HEISENBERG**)pHandle->spinaddr;

					switch (inspect_spin)
					{
					case InspectSpin::SPIN:
					{
						std::cout << "Spin status --------\r\n" << std::endl;
						for (int64_t i = lengthY - 1; i >= 0; i--)
						{
							std::cout << "             ";
							for (int64_t j = 0; j < lengthX; j++)
							{
								std::cout << std::setw(4) << std::fixed << std::setprecision(2) << sh[i][j].x << "/" << sh[i][j].y << "/" << sh[i][j].z << " ";
							}
							std::cout << std::endl;
						}
					}
					break;

					case InspectSpin::ADDRESS:
					{
						std::cout << "Spin Address --------\r\n" << std::endl;
						for (int64_t i = lengthY + 1; i >= 0; i--)
						{
							for (int64_t j = 0; j < lengthX + 2; j++)
							{
								std::cout << lpsh[i][j] << " ";
							}
							std::cout << std::endl;
						}
					}
					break;

					case InspectSpin::SPINADDRESS:
					{
						std::cout << "Spin Address --------\r\n" << std::endl;
						for (int64_t i = lengthY + 1; i >= 0; i--)
						{
							for (int64_t j = 0; j < lengthX + 2; j++)
							{
								std::cout << std::setw(10) << std::fixed << std::setprecision(1) << lpsh[i][j]->x << "/" << lpsh[i][j]->y << "/" << lpsh[i][j]->z << " ";
							}
							std::cout << std::endl;
						}
					}
					break;

					default:
						return CcResult::FAILED_INVALID_FLAG;
					}
				}
				break;

				default:
					return CcResult::FAILED_INVALID_FLAG;
				}
			}
			break;

			case static_cast<uint64_t>(DIMENSION::DIMENSION_3):
			{
				switch (pHandle->config.type)
				{
				case OnType::ISING:
				{
					SPIN_ISING*** si = (SPIN_ISING***)pHandle->spin;
					SPINADDR_ISING*** lpsi = (SPINADDR_ISING***)pHandle->spinaddr;

					switch (inspect_spin)
					{
					case InspectSpin::SPIN:
					{
						std::cout << "Spin status --------\r\n" << std::endl;

						for (int64_t i = lengthZ - 1; i >= 0; i--)
						{
							for (int64_t j = lengthY - 1; j >= 0; j--)
							{
								for (int64_t k = 0; k < j; k++)std::cout << "    ";
								for (int64_t k = 0; k < lengthX; k++)
								{
									std::cout << std::setw(4) << si[i][j][k];
								}
								std::cout << std::endl;
							}
							std::cout << std::endl;
						}
					}
					break;

					case InspectSpin::ADDRESS:
					{
						std::cout << "Spin status --------\r\n" << std::endl;

						for (int64_t i = lengthZ + 1; i >= 0; i--)
						{
							for (int64_t j = lengthY + 1; j >= 0; j--)
							{
								for (int64_t k = 0; k < j; k++)std::cout << "    ";
								for (int64_t k = 0; k < lengthX + 2; k++)
								{
									std::cout << lpsi[i][j][k] << " ";
								}
								std::cout << std::endl;
							}
							std::cout << std::endl;
						}
					}
					break;

					case InspectSpin::SPINADDRESS:
					{
						std::cout << "Spin status --------\r\n" << std::endl;

						for (int64_t i = lengthZ + 1; i >= 0; i--)
						{
							for (int64_t j = lengthY + 1; j >= 0; j--)
							{
								for (int64_t k = 0; k < j; k++)std::cout << "    ";
								for (int64_t k = 0; k < lengthX + 2; k++)
								{
									std::cout << std::setw(4) << *lpsi[i][j][k];
								}
								std::cout << std::endl;
							}
							std::cout << std::endl;
						}
					}
					break;

					default:
						return CcResult::FAILED_INVALID_FLAG;
					}
				}
				break;

				case OnType::XY:
				{
					SPIN_XY*** sx = (SPIN_XY***)pHandle->spin;
					SPINADDR_XY*** lpsx = (SPINADDR_XY***)pHandle->spinaddr;
					switch (inspect_spin)
					{
					case InspectSpin::SPIN:
					{
						std::cout << "Spin status --------\r\n" << std::endl;

						for (int64_t i = lengthZ - 1; i >= 0; i--)
						{
							for (int64_t j = lengthY - 1; j >= 0; j--)
							{
								for (int64_t k = 0; k < j; k++)std::cout << "    ";
								for (int64_t k = 0; k < lengthX; k++)
								{
									std::cout << std::setw(4) << std::fixed << std::setprecision(1) << sx[i][j][k].x << "/" << std::fixed << std::setprecision(1) << sx[i][j][k].y << " ";
								}
								std::cout << std::endl;
							}
							std::cout << std::endl;
						}
					}
					break;

					case InspectSpin::ADDRESS:
					{
						std::cout << "Spin status --------\r\n" << std::endl;

						for (int64_t i = lengthZ + 1; i >= 0; i--)
						{
							for (int64_t j = lengthY + 1; j >= 0; j--)
							{
								for (int64_t k = 0; k < j; k++)std::cout << "    ";
								for (int64_t k = 0; k < lengthX + 2; k++)
								{
									std::cout << lpsx[i][j][k] << " ";
								}
								std::cout << std::endl;
							}
							std::cout << std::endl;
						}
					}
					break;

					case InspectSpin::SPINADDRESS:
					{
						std::cout << "Spin status --------\r\n" << std::endl;

						for (int64_t i = lengthZ + 1; i >= 0; i--)
						{
							for (int64_t j = lengthY + 1; j >= 0; j--)
							{
								for (int64_t k = 0; k < j; k++)std::cout << "    ";
								for (int64_t k = 0; k < lengthX + 2; k++)
								{
									std::cout << std::setw(4) << std::fixed << std::setprecision(1) << lpsx[i][j][k]->x << "/" << std::fixed << std::setprecision(1) << lpsx[i][j][k]->y << " ";
								}
								std::cout << std::endl;
							}
							std::cout << std::endl;
						}
					}
					break;

					default:
						return CcResult::FAILED_INVALID_FLAG;
					}
				}
				break;

				case OnType::HEISENBERG:
				{
					SPIN_HEISENBERG*** sh = (SPIN_HEISENBERG***)pHandle->spin;
					SPINADDR_HEISENBERG*** lpsh = (SPINADDR_HEISENBERG***)pHandle->spinaddr;
					switch (inspect_spin)
					{
					case InspectSpin::SPIN:
					{
						std::cout << "Spin status --------\r\n" << std::endl;

						for (int64_t i = lengthZ - 1; i >= 0; i--)
						{
							for (int64_t j = lengthY - 1; j >= 0; j--)
							{
								for (int64_t k = 0; k < j; k++)std::cout << "    ";
								for (int64_t k = 0; k < lengthX; k++)
								{
									std::cout << std::setw(4) << std::fixed << std::setprecision(2) << sh[i][j][k].x << "/" << std::fixed << std::setprecision(2) << sh[i][j][k].y << "/" << std::fixed << std::setprecision(2) << sh[i][j][k].z << " ";
								}
								std::cout << std::endl;
							}
							std::cout << std::endl;
						}
					}
					break;

					case InspectSpin::ADDRESS:
					{
						std::cout << "Spin status --------\r\n" << std::endl;

						for (int64_t i = lengthZ + 1; i >= 0; i--)
						{
							for (int64_t j = lengthY + 1; j >= 0; j--)
							{
								for (int64_t k = 0; k < j; k++)std::cout << "    ";
								for (int64_t k = 0; k < lengthX + 2; k++)
								{
									std::cout << lpsh[i][j][k] << " ";
								}
								std::cout << std::endl;
							}
							std::cout << std::endl;
						}
					}
					break;

					case InspectSpin::SPINADDRESS:
					{
						std::cout << "Spin status --------\r\n" << std::endl;

						for (int64_t i = lengthZ + 1; i >= 0; i--)
						{
							for (int64_t j = lengthY + 1; j >= 0; j--)
							{
								for (int64_t k = 0; k < j; k++)std::cout << "    ";
								for (int64_t k = 0; k < lengthX + 2; k++)
								{
									std::cout << std::setw(4) << std::fixed << std::setprecision(1) << lpsh[i][j][k]->x << "/" << std::fixed << std::setprecision(1) << lpsh[i][j][k]->y << "/" << std::fixed << std::setprecision(1) << lpsh[i][j][k]->z << " ";
								}
								std::cout << std::endl;
							}
							std::cout << std::endl;
						}
					}
					break;

					default:
						return CcResult::FAILED_INVALID_FLAG;
					}
				}
				break;

				default:
					return CcResult::FAILED_INVALID_FLAG;
				}
			}
			break;
			default:
				return CcResult::FAILED_INVALID_FLAG;
			}

			return CcResult::SUCCESS;
		}


		CcResult ONM_inspect_config(HONMODEL OnModel, InspectStatus status_flag)
		{
			PRIVATE_HANDLE_ONMODEL* pHandle = reinterpret_cast<PRIVATE_HANDLE_ONMODEL*>(OnModel);

			/* argument error check ***************************/

			if (pHandle == nullptr)return CcResult::FAILED_ARG_NULLPTR;
			else
			{
				if (pHandle->spin == nullptr)return CcResult::FAILED_ARG_UNEXPECTED;
				if (pHandle->spinaddr == nullptr)return CcResult::FAILED_ARG_UNEXPECTED;
			}

			int64_t lengthX = pHandle->config.x;
			int64_t lengthY = pHandle->config.y;
			int64_t lengthZ = pHandle->config.z;

			switch (pHandle->config.type)
			{
			case OnType::ISING:
			case OnType::XY:
			case OnType::HEISENBERG:
				break;
			default:
				return CcResult::FAILED_INVALID_FLAG;
			}

			/**************************************************/

			uint64_t spin_flag = static_cast<uint64_t>(status_flag);
			uint64_t target_flag = 0;

			std::cout << "- Configuration --------------------------" << std::endl;

			target_flag = static_cast<uint64_t>(InspectStatus::TYPE);
			if (target_flag == (spin_flag & target_flag))
			{
				switch (pHandle->config.type)
				{
				case OnType::ISING:
					std::cout << "           Model : Ising model" << std::endl;
					break;
				case OnType::XY:
					std::cout << "           Model : XY model" << std::endl;
					break;
				case OnType::HEISENBERG:
					std::cout << "           Model : Heisenberg model" << std::endl;
					break;

				default:
					std::cout << "           Model : INVALID DATA" << std::endl;
				}
			}

			target_flag = static_cast<uint64_t>(InspectStatus::EXCHANGEINTERACTION);
			if (target_flag == (spin_flag & target_flag))std::cout << std::fixed << "               J : " << pHandle->config.J_ExchangeInteraction << std::endl;

			target_flag = static_cast<uint64_t>(InspectStatus::TEMPERATURE);
			if (target_flag == (spin_flag & target_flag))std::cout << std::fixed << "               T : " << pHandle->config.T_Temperature << std::endl;

			target_flag = static_cast<uint64_t>(InspectStatus::DIMENSION);
			if (target_flag == (spin_flag & target_flag))
			{
				int64_t dim_num = 0;
				switch (static_cast<uint64_t>(pHandle->config.dimension))
				{
				case static_cast<uint64_t>(DIMENSION::DIMENSION_1):
					dim_num = 1;
					break;
				case static_cast<uint64_t>(DIMENSION::DIMENSION_2):
					dim_num = 2;
					break;
				case static_cast<uint64_t>(DIMENSION::DIMENSION_3):
					dim_num = 3;
					break;
				}

				std::cout << "       Dimension : " << dim_num << std::endl;
			}

			target_flag = static_cast<uint64_t>(InspectStatus::LATTICE);
			if (target_flag == (spin_flag & target_flag))
			{
				switch (static_cast<uint64_t>(pHandle->config.dimension))
				{
				case static_cast<uint64_t>(DIMENSION::DIMENSION_2):
					std::cout << "         LATTICE : " << pHandle->config.x << " x " << pHandle->config.y << std::endl;
					break;
				case static_cast<uint64_t>(DIMENSION::DIMENSION_3):
					std::cout << "         LATTICE : " << pHandle->config.x << " x " << pHandle->config.y << " x " << pHandle->config.z << std::endl;
					break;
				default:
					std::cout << "         LATTICE : ERROR" << std::endl;
				}
			}

			target_flag = static_cast<uint64_t>(InspectStatus::MAGNETICFIELD);
			if (target_flag == (spin_flag & target_flag))
			{
				switch (pHandle->config.type)
				{
				case OnType::ISING:
					std::cout << std::fixed << "  Magnetic field : " << pHandle->config.H_MagneticField.x << "" << std::endl;
					break;
				case OnType::XY:
					std::cout << std::fixed << "  Magnetic field : (" << pHandle->config.H_MagneticField.x << ", " << pHandle->config.H_MagneticField.y << ")" << std::endl;
					break;
				case OnType::HEISENBERG:
					std::cout << std::fixed << "  Magnetic field : (" << pHandle->config.H_MagneticField.x << ", " << pHandle->config.H_MagneticField.y << ", " << pHandle->config.H_MagneticField.z << ")" << std::endl;
					break;
				default:
					break;
				}
			}

			target_flag = static_cast<uint64_t>(InspectStatus::TYPE);
			if (target_flag == (spin_flag & target_flag))
			{
				switch (pHandle->inittype)
				{
				case InitType::MT:
					std::cout << " Initialize Type : Mersenne Twister (std::19937)" << std::endl;
					break;
				case InitType::COLD_P:
					std::cout << " Initialize Type : Fill \"1\"" << std::endl;
					break;
				case InitType::COLD_N:
					std::cout << " Initialize Type : Fill \"-1\"" << std::endl;
					break;
				case InitType::ENUM:
					std::cout << " Initialize Type : Enumeration" << std::endl;
					break;
				default:
					std::cout << " Initialize Type : ERROR" << std::endl;
				}
			}

			target_flag = static_cast<uint64_t>(InspectStatus::UPDATETYPE);
			if (target_flag == (spin_flag & target_flag))
			{
				switch (pHandle->updatetype)
				{
				case UpdateMethod::METROPOLIS:
					std::cout << "     Update Type : Default Metropolis Update" << std::endl;
					break;
				/*case UPDATE_METHOD_INSERT:
					std::cout << "     Update Type : Inserted something" << std::endl;
					break;*/
				case UpdateMethod::DUAL_SPIN_SWAP_METROPOLIS:
					std::cout << "     Update Type : Dual spin swap Metropolis Update" << std::endl;
					break;

				default:
					std::cout << "     Update Type : ERROR" << std::endl;
				}
			}

			target_flag = static_cast<uint64_t>(InspectStatus::UPDATECOUNT);
			if (target_flag == (spin_flag & target_flag))std::cout << std::fixed << "    Update count : " << pHandle->updatecount << std::endl;

			target_flag = static_cast<uint64_t>(InspectStatus::INITTYPE);
			if (target_flag == (spin_flag & target_flag))
			{
				switch (pHandle->inittype)
				{
				case InitType::MT:
					std::cout << std::fixed << "    Update count : Mersenne Twister" << std::endl;
					break;
				case InitType::COLD_P:
					std::cout << std::fixed << "    Update count : Fill All (+1)" << std::endl;
					break;
				case InitType::COLD_N:
					std::cout << std::fixed << "    Update count : Fill All (-1)" << std::endl;
					break;
				case InitType::ENUM:
					std::cout << std::fixed << "    Update count : Enumeration" << std::endl;
					break;
				case InitType::MFIXED:
					std::cout << std::fixed << "    Update count : Magnetization Fixed" << std::endl;
					break;
				}
			}

			std::cout << "------------------------------------------" << std::endl;

			return CcResult::SUCCESS;
		}

	}
}
