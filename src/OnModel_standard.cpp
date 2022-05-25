#include <OnModel_standard.hpp>

#include <iostream>
#include <iomanip>
#include <random>
#include <cstdlib>
#include <cmath>
#include <climits>
#include <cstdio>

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

			static CcResult ONM_new_2_ISING(void** spin, void** spin_addr, ONMODEL_CONFIG* oc);
			static CcResult ONM_new_2_XY(void** spin, void** spin_addr, ONMODEL_CONFIG* oc);
			static CcResult ONM_new_2_HEISENBERG(void** spin, void** spin_addr, ONMODEL_CONFIG* oc);
			static CcResult ONM_new_3_ISING(void** spin, void** spin_addr, ONMODEL_CONFIG* oc);
			static CcResult ONM_new_3_XY(void** spin, void** spin_addr, ONMODEL_CONFIG* oc);
			static CcResult ONM_new_3_HEISENBERG(void** spin, void** spin_addr, ONMODEL_CONFIG* oc);
		}

		CcResult ONM_new(HONMODEL* OnModel, ONMODEL_CONFIG OnM_config)
		{
			ONMODEL_CONFIG retconfig = { OnType::INVALID, 0.0, 0.0, DIMENSION::INVALID, 0, 0, 0, 0, 0, 0 };


			/* argument error check ***************************/
			if (*OnModel != nullptr)return CcResult::FAILED_ARG_HANDLE_ALREADY_INITIALIZED;

			int64_t lengthX = OnM_config.x;
			int64_t lengthY = OnM_config.y;
			int64_t lengthZ = OnM_config.z;

			retconfig.H_MagneticField = OnM_config.H_MagneticField;

			OnType type = OnM_config.type;

			switch (type)
			{
			case OnType::ISING:
			case OnType::XY:
			case OnType::HEISENBERG:
				retconfig.type = type;
				break;
			default:
				CcResult::FAILED_ARG_INVALID_FLAG;
			}

			if (OnM_config.T_Temperature == 0)CcResult::FAILED_ARG_NOT_ZERO;
			else retconfig.T_Temperature = OnM_config.T_Temperature;
			if (OnM_config.J_ExchangeInteraction == 0)CcResult::FAILED_ARG_NOT_ZERO;
			else retconfig.J_ExchangeInteraction = OnM_config.J_ExchangeInteraction;

			int64_t flag_dim = static_cast<int64_t>(false);
			DIMENSION dim = OnM_config.dimension;
			switch (dim)
			{
			case DIMENSION::DIMENSION_1:
			case DIMENSION::DIMENSION_2:
			case DIMENSION::DIMENSION_3:
				retconfig.dimension = dim;
				flag_dim = static_cast<int64_t>(true);
				break;

			default:
				CcResult::FAILED_ARG_INVALID_FLAG;
			}

			if(flag_dim)
			{
				int64_t iCountOfAddr = 1;
				int64_t iCountOfSpin = 1;

				switch (dim)
				{
				case DIMENSION::DIMENSION_3:
				{
					if (lengthZ < 1)
					{
						CcResult::FAILED_ARG_ONLY_GT;
						break;
					}
					else
					{
						if (iCountOfAddr >= INT64_MAX / (lengthZ + 2))CcResult::FAILED_ARG_TOO_BIG;
						else
						{
							iCountOfAddr *= (lengthZ + 2);
							iCountOfSpin *= lengthZ;
							retconfig.z = lengthZ;
						}
					}
				}

				case DIMENSION::DIMENSION_2:
				{
					if (lengthY < 1)
					{
						CcResult::FAILED_ARG_ONLY_GT;
						break;
					}
					else
					{
						if (iCountOfAddr >= INT64_MAX / (lengthY + 2))CcResult::FAILED_ARG_TOO_BIG;
						else
						{
							iCountOfAddr *= (lengthY + 2);
							iCountOfSpin *= lengthY;
							retconfig.y = lengthY;
						}
					}
				}

				case DIMENSION::DIMENSION_1:
				{
					if (lengthX < 1)
					{
						CcResult::FAILED_ARG_ONLY_GT;
						break;
					}
					else
					{
						if (iCountOfAddr >= INT64_MAX / (lengthX + 2))CcResult::FAILED_ARG_TOO_BIG;
						else
						{
							iCountOfAddr *= (lengthX + 2);
							iCountOfSpin *= lengthX;
							retconfig.x = lengthX;
						}
					}
				}
					break;

				default:
					CcResult::FAILED_ARG_WHY_COME_HERE;
				}

				int64_t strsize = 0;

				switch (type)
				{
				case OnType::ISING:
					strsize = sizeof(SPIN_ISING);
					break;
				case OnType::XY:
					strsize = sizeof(SPIN_XY);
					break;
				case OnType::HEISENBERG:
					strsize = sizeof(SPIN_HEISENBERG);
					break;
				default:
					CcResult::FAILED_ARG_WHY_COME_HERE;
				}

				if (iCountOfAddr >= INT64_MAX / sizeof(void*))CcResult::FAILED_ARG_TOO_BIG;
				else
				{
					if (iCountOfSpin >= INT64_MAX / strsize)CcResult::FAILED_ARG_TOO_BIG;
					else
					{
						if(iCountOfAddr >= INT64_MAX - iCountOfSpin)CcResult::FAILED_ARG_TOO_BIG;
					}
				}
				
			}

			/**************************************************/

			PRIVATE_HANDLE_ONMODEL* rethandle = nullptr;
			rethandle = (PRIVATE_HANDLE_ONMODEL*)calloc(1, sizeof(PRIVATE_HANDLE_ONMODEL));
			if (rethandle == nullptr)return CcResult::FAILED_ALLOC_ERROR;
			else
			{
				rethandle->config = retconfig;
			}


			CcResult tmp_cres = CcResult::FAILED_INVALID;

			switch (dim)
			{
				//void* tmp;
			//case 1: reserved

			case DIMENSION::DIMENSION_2:
			{
				switch (type)
				{
				case OnType::ISING:
					tmp_cres = ONM_new_2_ISING(&rethandle->spin, &rethandle->spinaddr, &retconfig);
					break;

				case OnType::XY:
					tmp_cres = ONM_new_2_XY(&rethandle->spin, &rethandle->spinaddr, &retconfig);
					break;

				case OnType::HEISENBERG:
					tmp_cres = ONM_new_2_HEISENBERG(&rethandle->spin, &rethandle->spinaddr, &retconfig);
					break;

				default:
					tmp_cres = CcResult::FAILED_WHY_COME_HERE;
				}
			}
			break;

			case DIMENSION::DIMENSION_3:
			{
				switch (type)
				{
				case OnType::ISING:
					tmp_cres = ONM_new_3_ISING(&rethandle->spin, &rethandle->spinaddr, &retconfig);
					break;

				case OnType::XY:
					tmp_cres = ONM_new_3_XY(&rethandle->spin, &rethandle->spinaddr, &retconfig);
					break;

				case OnType::HEISENBERG:
					tmp_cres = ONM_new_3_HEISENBERG(&rethandle->spin, &rethandle->spinaddr, &retconfig);
					break;

				default:
					tmp_cres = CcResult::FAILED_WHY_COME_HERE;
				}
			}
			break;

			default:
				tmp_cres = CcResult::FAILED_ARG_WHY_COME_HERE;
			}

			
			if (tmp_cres != CcResult::SUCCESS)
			{
				free(rethandle);
				rethandle = nullptr;
			}
			else
			{
				*OnModel = reinterpret_cast<HONMODEL>(rethandle);
			}

			return CcResult::SUCCESS;
		}

		namespace
		{

			static CcResult ONM_new_2_ISING(void** spin, void** spin_addr, ONMODEL_CONFIG* oc)
			{
				int64_t lengthX = oc->x;
				int64_t lengthY = oc->y;

				SPIN_ISING** tmp = (SPIN_ISING**)malloc(lengthY * sizeof(SPIN_ISING*));
				if (tmp == nullptr)return CcResult::FAILED_ALLOC_ERROR;
				else
				{
					for (int64_t i = 0; i < lengthY; i++)
					{
						tmp[i] = (SPIN_ISING*)calloc(lengthX, sizeof(SPIN_ISING));
						if (tmp[i] == nullptr)
						{
							for (int64_t j = 0; j < i; j++)free(tmp[j]);
							free(tmp);
							tmp = nullptr;
							return CcResult::FAILED_ALLOC_ERROR;
						}
					}
				}

				
				SPINADDR_ISING** lptmp = (SPINADDR_ISING**)malloc((lengthY + 2) * sizeof(SPINADDR_ISING*));
				if (lptmp == nullptr)
				{
					for (int64_t i = 0; i < lengthY; i++)free(tmp[i]);
					free(tmp);
					tmp = nullptr;
					return CcResult::FAILED_ALLOC_ERROR;
				}
				else
				{
					for (int64_t i = 0; i < lengthY + 2; i++)
					{
						lptmp[i] = (SPINADDR_ISING*)malloc((lengthX + 2) * sizeof(SPINADDR_ISING));
						if (lptmp[i] == nullptr)
						{
							for (int64_t j = 0; j < i; j++)free(lptmp[j]);
							return CcResult::FAILED_ALLOC_ERROR;
						}
					}
				}

				for (int64_t i = 0; i < lengthY; i++)
				{
					for (int64_t j = 0; j < lengthX; j++)
					{
						lptmp[i + 1][j + 1] = &tmp[i][j];
					}
				}

				for (int64_t i = 0; i < lengthX; i++)lptmp[0][i + 1] = &tmp[lengthY - 1][i];
				for (int64_t i = 0; i < lengthX; i++)lptmp[lengthY + 1][i + 1] = &tmp[0][i];
				for (int64_t i = 0; i < lengthY; i++)lptmp[i + 1][0] = &tmp[i][lengthX - 1];
				for (int64_t i = 0; i < lengthY; i++)lptmp[i + 1][lengthX + 1] = &tmp[i][0];
				lptmp[0][0] = &tmp[lengthY - 1][lengthX - 1];
				lptmp[lengthY + 1][lengthX + 1] = &tmp[0][0];
				lptmp[0][lengthX + 1] = &tmp[lengthY - 1][0];
				lptmp[lengthY + 1][0] = &tmp[0][lengthX - 1];

				*spin = tmp;
				*spin_addr = lptmp;

				return CcResult::SUCCESS;
			}

			static CcResult ONM_new_2_XY(void** spin, void** spin_addr, ONMODEL_CONFIG* oc)
			{
				CcResult error = CcResult::FAILED_INVALID;

				int64_t lengthX = oc->x;
				int64_t lengthY = oc->y;

				SPIN_XY** tmp = (SPIN_XY**)malloc(lengthY * sizeof(SPIN_XY*));
				if (tmp == nullptr)return CcResult::FAILED_ALLOC_ERROR;
				else
				{
					for (int64_t i = 0; i < lengthY; i++)
					{
						tmp[i] = (SPIN_XY*)calloc(lengthX, sizeof(SPIN_XY));
						if (tmp[i] == nullptr)
						{
							for (int64_t j = 0; j < i; j++)free(tmp[j]);
							free(tmp);
							tmp = nullptr;
							return CcResult::FAILED_ALLOC_ERROR;
						}
					}
				}


				SPINADDR_XY** lptmp = (SPINADDR_XY**)malloc((lengthY + 2) * sizeof(SPINADDR_XY*));
				if (lptmp == nullptr)
				{
					for (int64_t i = 0; i < lengthY; i++)free(tmp[i]);
					free(tmp);
					tmp = nullptr;
					return CcResult::FAILED_ALLOC_ERROR;
				}
				else
				{
					for (int64_t i = 0; i < lengthY + 2; i++)
					{
						lptmp[i] = (SPINADDR_XY*)malloc((lengthX + 2) * sizeof(SPINADDR_XY));
						if (lptmp[i] == nullptr)
						{
							for (int64_t j = 0; j < i; j++)free(lptmp[j]);
							return CcResult::FAILED_ALLOC_ERROR;
						}
					}
				}

				for (int64_t i = 0; i < lengthY; i++)
				{
					for (int64_t j = 0; j < lengthX; j++)
					{
						lptmp[i + 1][j + 1] = &tmp[i][j];
					}
				}

				for (int64_t i = 0; i < lengthX; i++)lptmp[0][i + 1] = &tmp[lengthY - 1][i];
				for (int64_t i = 0; i < lengthX; i++)lptmp[lengthY + 1][i + 1] = &tmp[0][i];
				for (int64_t i = 0; i < lengthY; i++)lptmp[i + 1][0] = &tmp[i][lengthX - 1];
				for (int64_t i = 0; i < lengthY; i++)lptmp[i + 1][lengthX + 1] = &tmp[i][0];
				lptmp[0][0] = &tmp[lengthY - 1][lengthX - 1];
				lptmp[lengthY + 1][lengthX + 1] = &tmp[0][0];
				lptmp[0][lengthX + 1] = &tmp[lengthY - 1][0];
				lptmp[lengthY + 1][0] = &tmp[0][lengthX - 1];

				*spin = tmp;
				*spin_addr = lptmp;

				return CcResult::SUCCESS;
			}

			static CcResult ONM_new_2_HEISENBERG(void** spin, void** spin_addr, ONMODEL_CONFIG* oc)
			{
				CcResult error = CcResult::FAILED_INVALID;

				int64_t lengthX = oc->x;
				int64_t lengthY = oc->y;

				SPIN_HEISENBERG** tmp = (SPIN_HEISENBERG**)malloc(lengthY * sizeof(SPIN_HEISENBERG*));
				if (tmp == nullptr)return CcResult::FAILED_ALLOC_ERROR;
				else
				{
					for (int64_t i = 0; i < lengthY; i++)
					{
						tmp[i] = (SPIN_HEISENBERG*)calloc(lengthX, sizeof(SPIN_HEISENBERG));
						if (tmp[i] == nullptr)
						{
							for (int64_t j = 0; j < i; j++)free(tmp[j]);
							free(tmp);
							tmp = nullptr;
							return CcResult::FAILED_ALLOC_ERROR;
						}
					}
				}


				SPINADDR_HEISENBERG** lptmp = (SPINADDR_HEISENBERG**)malloc((lengthY + 2) * sizeof(SPINADDR_HEISENBERG*));
				if (lptmp == nullptr)
				{
					for (int64_t i = 0; i < lengthY; i++)free(tmp[i]);
					free(tmp);
					tmp = nullptr;
					return CcResult::FAILED_ALLOC_ERROR;
				}
				else
				{
					for (int64_t i = 0; i < lengthY + 2; i++)
					{
						lptmp[i] = (SPINADDR_HEISENBERG*)malloc((lengthX + 2) * sizeof(SPINADDR_HEISENBERG));
						if (lptmp[i] == nullptr)
						{
							for (int64_t j = 0; j < i; j++)free(lptmp[j]);
							return CcResult::FAILED_ALLOC_ERROR;
						}
					}
				}

				for (int64_t i = 0; i < lengthY; i++)
				{
					for (int64_t j = 0; j < lengthX; j++)
					{
						lptmp[i + 1][j + 1] = &tmp[i][j];
					}
				}

				for (int64_t i = 0; i < lengthX; i++)lptmp[0][i + 1] = &tmp[lengthY - 1][i];
				for (int64_t i = 0; i < lengthX; i++)lptmp[lengthY + 1][i + 1] = &tmp[0][i];
				for (int64_t i = 0; i < lengthY; i++)lptmp[i + 1][0] = &tmp[i][lengthX - 1];
				for (int64_t i = 0; i < lengthY; i++)lptmp[i + 1][lengthX + 1] = &tmp[i][0];
				lptmp[0][0] = &tmp[lengthY - 1][lengthX - 1];
				lptmp[lengthY + 1][lengthX + 1] = &tmp[0][0];
				lptmp[0][lengthX + 1] = &tmp[lengthY - 1][0];
				lptmp[lengthY + 1][0] = &tmp[0][lengthX - 1];

				*spin = tmp;
				*spin_addr = lptmp;

				return CcResult::SUCCESS;
			}

			static CcResult ONM_new_3_ISING(void** spin, void** spin_addr, ONMODEL_CONFIG* oc)
			{
				CcResult error = CcResult::FAILED_INVALID;

				int64_t lengthX = oc->x;
				int64_t lengthY = oc->y;
				int64_t lengthZ = oc->z;

				SPIN_ISING*** tmp = (SPIN_ISING***)malloc(lengthZ * sizeof(SPIN_ISING**));
				if (tmp == nullptr)return CcResult::FAILED_ALLOC_ERROR;
				else
				{
					for (int64_t i = 0; i < lengthZ; i++)
					{
						tmp[i] = (SPIN_ISING**)malloc(lengthY * sizeof(SPIN_ISING*));
						if (tmp[i] == nullptr)
						{
							for (int64_t j = 0; j < i; j++)free(tmp[j]);
							free(tmp);
							return CcResult::FAILED_ALLOC_ERROR;
						}
					}

					for (int64_t i = 0; i < lengthZ; i++)
					{
						for (int64_t j = 0; j < lengthY; j++)
						{
							tmp[i][j] = (SPIN_ISING*)calloc(lengthX, sizeof(SPIN_ISING));
							if (tmp[i][j] == nullptr)
							{
								/*********** from here ************/
								for (int64_t k = 0; k < j; k++)free(tmp[i][k]);
								for (int64_t k = 0; k < i; k++)
								{
									for (int64_t l = 0; l < lengthY; l++)
									{
										free(tmp[k][l]);
									}
								}

								for (int64_t k = 0; k < lengthZ; k++)free(tmp[k]);
								free(tmp);
								return CcResult::FAILED_ALLOC_ERROR;
							}
						}
					}
				}

				SPINADDR_ISING*** lptmp = (SPINADDR_ISING***)malloc((lengthZ + 2) * sizeof(SPINADDR_ISING**));
				if (lptmp == nullptr)return CcResult::FAILED_ALLOC_ERROR;

				for (int64_t i = 0; i < lengthZ + 2; i++)
				{
					lptmp[i] = (SPINADDR_ISING**)malloc((lengthY + 2) * sizeof(SPINADDR_ISING*));
					if (lptmp[i] == nullptr)
					{
						for (int64_t j = 0; j < i; j++)free(lptmp[j]);
						free(lptmp);
						return CcResult::FAILED_ALLOC_ERROR;
					}
				}

				for (int64_t i = 0; i < lengthZ + 2; i++)
				{
					for (int64_t j = 0; j < lengthY + 2; j++)
					{
						lptmp[i][j] = (SPINADDR_ISING*)malloc((lengthX + 2) * sizeof(SPINADDR_ISING));
						if (lptmp[i][j] == nullptr)
						{
							/*********** from here ************/
							for (int64_t k = 0; k < j; k++)free(lptmp[i][k]);
							for (int64_t k = 0; k < i; k++)
							{
								for (int64_t l = 0; l < lengthY + 2; l++)
								{
									free(lptmp[k][l]);
								}
							}

							for (int64_t k = 0; k < lengthZ + 2; k++)free(lptmp[k]);
							free(lptmp);
							return CcResult::FAILED_ALLOC_ERROR;
						}
					}
				}

				// address to pointer

				for (int64_t i = 0; i < lengthZ; i++)
				{
					for (int64_t j = 0; j < lengthY; j++)
					{
						for (int64_t k = 0; k < lengthX; k++)
						{
							lptmp[i + 1][j + 1][k + 1] = &tmp[i][j][k];
						}
					}
				}

				//bottom serface to top surface

				for (int64_t i = 0; i < lengthY; i++)
				{
					for (int64_t j = 0; j < lengthX; j++)
					{
						lptmp[0][i + 1][j + 1] = &tmp[lengthZ - 1][i][j];
					}
				}

				for (int64_t i = 0; i < lengthY; i++)
				{
					for (int64_t j = 0; j < lengthX; j++)
					{
						lptmp[lengthZ + 1][i + 1][j + 1] = &tmp[0][i][j];
					}
				}

				for (int64_t i = 0; i < lengthZ; i++)
				{
					for (int64_t j = 0; j < lengthX; j++)
					{
						lptmp[i + 1][0][j + 1] = &tmp[i][lengthY - 1][j];
					}
				}

				for (int64_t i = 0; i < lengthZ; i++)
				{
					for (int64_t j = 0; j < lengthX; j++)
					{
						lptmp[i + 1][lengthY + 1][j + 1] = &tmp[i][0][j];
					}
				}

				for (int64_t i = 0; i < lengthZ; i++)
				{
					for (int64_t j = 0; j < lengthY; j++)
					{
						lptmp[i + 1][j + 1][0] = &tmp[i][j][lengthX - 1];
					}
				}

				for (int64_t i = 0; i < lengthZ; i++)
				{
					for (int64_t j = 0; j < lengthY; j++)
					{
						lptmp[i + 1][j + 1][lengthX + 1] = &tmp[i][j][0];
					}
				}

				for (int64_t i = 0; i < lengthX; i++)lptmp[0][0][i + 1] = &tmp[lengthZ - 1][lengthY - 1][i];
				for (int64_t i = 0; i < lengthX; i++)lptmp[0][lengthY + 1][i + 1] = &tmp[lengthZ - 1][0][i];
				for (int64_t i = 0; i < lengthX; i++)lptmp[lengthZ + 1][0][i + 1] = &tmp[0][lengthY - 1][i];
				for (int64_t i = 0; i < lengthX; i++)lptmp[lengthZ + 1][lengthY + 1][i + 1] = &tmp[0][0][i];

				for (int64_t i = 0; i < lengthY; i++)lptmp[0][i + 1][0] = &tmp[lengthZ - 1][i][lengthX - 1];
				for (int64_t i = 0; i < lengthY; i++)lptmp[0][i + 1][lengthX + 1] = &tmp[lengthZ - 1][i][0];
				for (int64_t i = 0; i < lengthY; i++)lptmp[lengthZ + 1][i + 1][0] = &tmp[0][i][lengthX - 1];
				for (int64_t i = 0; i < lengthY; i++)lptmp[lengthZ + 1][i + 1][lengthX + 1] = &tmp[0][i][0];

				for (int64_t i = 0; i < lengthZ; i++)lptmp[i + 1][0][0] = &tmp[i][lengthY - 1][lengthX - 1];
				for (int64_t i = 0; i < lengthZ; i++)lptmp[i + 1][0][lengthX + 1] = &tmp[i][lengthY - 1][0];
				for (int64_t i = 0; i < lengthZ; i++)lptmp[i + 1][lengthY + 1][0] = &tmp[i][0][lengthX - 1];
				for (int64_t i = 0; i < lengthZ; i++)lptmp[i + 1][lengthY + 1][lengthX + 1] = &tmp[i][0][0];

				lptmp[0][0][0] = &tmp[lengthZ - 1][lengthY - 1][lengthX - 1];
				lptmp[lengthZ + 1][lengthY + 1][lengthX + 1] = &tmp[0][0][0];
				lptmp[0][0][lengthX + 1] = &tmp[lengthZ - 1][lengthY - 1][0];
				lptmp[0][lengthY + 1][lengthX + 1] = &tmp[lengthZ - 1][0][0];
				lptmp[0][lengthY + 1][0] = &tmp[lengthZ - 1][0][lengthX - 1];
				lptmp[lengthZ + 1][0][lengthX + 1] = &tmp[0][lengthY - 1][0];
				lptmp[lengthZ + 1][lengthY + 1][0] = &tmp[0][0][lengthX - 1];
				lptmp[lengthZ + 1][0][0] = &tmp[0][lengthY - 1][lengthX - 1];

				*spin = tmp;
				*spin_addr = lptmp;

				return CcResult::SUCCESS;
			}

			static CcResult ONM_new_3_XY(void** spin, void** spin_addr, ONMODEL_CONFIG* oc)
			{
				CcResult error = CcResult::FAILED_INVALID;

				int64_t lengthX = oc->x;
				int64_t lengthY = oc->y;
				int64_t lengthZ = oc->z;

				SPIN_XY*** tmp = (SPIN_XY***)malloc(lengthZ * sizeof(SPIN_XY**));
				if (tmp == nullptr)return CcResult::FAILED_ALLOC_ERROR;

				for (int64_t i = 0; i < lengthZ; i++)
				{
					tmp[i] = (SPIN_XY**)malloc(lengthY * sizeof(SPIN_XY*));
					if (tmp[i] == nullptr)
					{
						for (int64_t j = 0; j < i; j++)free(tmp[j]);
						free(tmp);
						return CcResult::FAILED_ALLOC_ERROR;
					}
				}

				for (int64_t i = 0; i < lengthZ; i++)
				{
					for (int64_t j = 0; j < lengthY; j++)
					{
						tmp[i][j] = (SPIN_XY*)calloc(lengthX, sizeof(SPIN_XY));
						if (tmp[i][j] == nullptr)
						{
							/*********** from here ************/
							for (int64_t k = 0; k < j; k++)free(tmp[i][k]);
							for (int64_t k = 0; k < i; k++)
							{
								for (int64_t l = 0; l < lengthY; l++)
								{
									free(tmp[k][l]);
								}
							}

							for (int64_t k = 0; k < lengthZ; k++)free(tmp[k]);
							free(tmp);
							return CcResult::FAILED_ALLOC_ERROR;
						}
					}
				}

				SPINADDR_XY*** lptmp = (SPINADDR_XY***)malloc((lengthZ + 2) * sizeof(SPINADDR_XY**));
				if (lptmp == nullptr)return CcResult::FAILED_ALLOC_ERROR;

				for (int64_t i = 0; i < lengthZ + 2; i++)
				{
					lptmp[i] = (SPINADDR_XY**)malloc((lengthY + 2) * sizeof(SPINADDR_XY*));
					if (lptmp[i] == nullptr)
					{
						for (int64_t j = 0; j < i; j++)free(lptmp[j]);
						free(lptmp);
						return CcResult::FAILED_ALLOC_ERROR;
					}
				}

				for (int64_t i = 0; i < lengthZ + 2; i++)
				{
					for (int64_t j = 0; j < lengthY + 2; j++)
					{
						lptmp[i][j] = (SPINADDR_XY*)malloc((lengthX + 2) * sizeof(SPINADDR_XY));
						if (lptmp[i][j] == nullptr)
						{
							/*********** from here ************/
							for (int64_t k = 0; k < j; k++)free(lptmp[i][k]);
							for (int64_t k = 0; k < i; k++)
							{
								for (int64_t l = 0; l < lengthY + 2; l++)
								{
									free(lptmp[k][l]);
								}
							}

							for (int64_t k = 0; k < lengthZ + 2; k++)free(lptmp[k]);
							free(lptmp);
							return CcResult::FAILED_ALLOC_ERROR;
						}
					}
				}

				// address to pointer

				for (int64_t i = 0; i < lengthZ; i++)
				{
					for (int64_t j = 0; j < lengthY; j++)
					{
						for (int64_t k = 0; k < lengthX; k++)
						{
							lptmp[i + 1][j + 1][k + 1] = &tmp[i][j][k];
						}
					}
				}

				//bottom serface to top surface

				for (int64_t i = 0; i < lengthY; i++)
				{
					for (int64_t j = 0; j < lengthX; j++)
					{
						lptmp[0][i + 1][j + 1] = &tmp[lengthZ - 1][i][j];
					}
				}

				for (int64_t i = 0; i < lengthY; i++)
				{
					for (int64_t j = 0; j < lengthX; j++)
					{
						lptmp[lengthZ + 1][i + 1][j + 1] = &tmp[0][i][j];
					}
				}

				for (int64_t i = 0; i < lengthZ; i++)
				{
					for (int64_t j = 0; j < lengthX; j++)
					{
						lptmp[i + 1][0][j + 1] = &tmp[i][lengthY - 1][j];
					}
				}

				for (int64_t i = 0; i < lengthZ; i++)
				{
					for (int64_t j = 0; j < lengthX; j++)
					{
						lptmp[i + 1][lengthY + 1][j + 1] = &tmp[i][0][j];
					}
				}

				for (int64_t i = 0; i < lengthZ; i++)
				{
					for (int64_t j = 0; j < lengthY; j++)
					{
						lptmp[i + 1][j + 1][0] = &tmp[i][j][lengthX - 1];
					}
				}

				for (int64_t i = 0; i < lengthZ; i++)
				{
					for (int64_t j = 0; j < lengthY; j++)
					{
						lptmp[i + 1][j + 1][lengthX + 1] = &tmp[i][j][0];
					}
				}

				for (int64_t i = 0; i < lengthX; i++)lptmp[0][0][i + 1] = &tmp[lengthZ - 1][lengthY - 1][i];
				for (int64_t i = 0; i < lengthX; i++)lptmp[0][lengthY + 1][i + 1] = &tmp[lengthZ - 1][0][i];
				for (int64_t i = 0; i < lengthX; i++)lptmp[lengthZ + 1][0][i + 1] = &tmp[0][lengthY - 1][i];
				for (int64_t i = 0; i < lengthX; i++)lptmp[lengthZ + 1][lengthY + 1][i + 1] = &tmp[0][0][i];

				for (int64_t i = 0; i < lengthY; i++)lptmp[0][i + 1][0] = &tmp[lengthZ - 1][i][lengthX - 1];
				for (int64_t i = 0; i < lengthY; i++)lptmp[0][i + 1][lengthX + 1] = &tmp[lengthZ - 1][i][0];
				for (int64_t i = 0; i < lengthY; i++)lptmp[lengthZ + 1][i + 1][0] = &tmp[0][i][lengthX - 1];
				for (int64_t i = 0; i < lengthY; i++)lptmp[lengthZ + 1][i + 1][lengthX + 1] = &tmp[0][i][0];

				for (int64_t i = 0; i < lengthZ; i++)lptmp[i + 1][0][0] = &tmp[i][lengthY - 1][lengthX - 1];
				for (int64_t i = 0; i < lengthZ; i++)lptmp[i + 1][0][lengthX + 1] = &tmp[i][lengthY - 1][0];
				for (int64_t i = 0; i < lengthZ; i++)lptmp[i + 1][lengthY + 1][0] = &tmp[i][0][lengthX - 1];
				for (int64_t i = 0; i < lengthZ; i++)lptmp[i + 1][lengthY + 1][lengthX + 1] = &tmp[i][0][0];

				lptmp[0][0][0] = &tmp[lengthZ - 1][lengthY - 1][lengthX - 1];
				lptmp[lengthZ + 1][lengthY + 1][lengthX + 1] = &tmp[0][0][0];
				lptmp[0][0][lengthX + 1] = &tmp[lengthZ - 1][lengthY - 1][0];
				lptmp[0][lengthY + 1][lengthX + 1] = &tmp[lengthZ - 1][0][0];
				lptmp[0][lengthY + 1][0] = &tmp[lengthZ - 1][0][lengthX - 1];
				lptmp[lengthZ + 1][0][lengthX + 1] = &tmp[0][lengthY - 1][0];
				lptmp[lengthZ + 1][lengthY + 1][0] = &tmp[0][0][lengthX - 1];
				lptmp[lengthZ + 1][0][0] = &tmp[0][lengthY - 1][lengthX - 1];

				*spin = tmp;
				*spin_addr = lptmp;

				return CcResult::FAILED_ALLOC_ERROR;
			}

			static CcResult ONM_new_3_HEISENBERG(void** spin, void** spin_addr, ONMODEL_CONFIG* oc)
			{
				CcResult error = CcResult::FAILED_INVALID;

				int64_t lengthX = oc->x;
				int64_t lengthY = oc->y;
				int64_t lengthZ = oc->z;

				SPIN_HEISENBERG*** tmp = (SPIN_HEISENBERG***)malloc(lengthZ * sizeof(SPIN_HEISENBERG**));
				if (tmp == nullptr)return CcResult::FAILED_ALLOC_ERROR;

				for (int64_t i = 0; i < lengthZ; i++)
				{
					tmp[i] = (SPIN_HEISENBERG**)malloc(lengthY * sizeof(SPIN_HEISENBERG*));
					if (tmp[i] == nullptr)
					{
						for (int64_t j = 0; j < i; j++)free(tmp[j]);
						free(tmp);
						return CcResult::FAILED_ALLOC_ERROR;
					}
				}

				for (int64_t i = 0; i < lengthZ; i++)
				{
					for (int64_t j = 0; j < lengthY; j++)
					{
						tmp[i][j] = (SPIN_HEISENBERG*)calloc(lengthX, sizeof(SPIN_HEISENBERG));
						if (tmp[i][j] == nullptr)
						{
							/*********** from here ************/
							for (int64_t k = 0; k < j; k++)free(tmp[i][k]);
							for (int64_t k = 0; k < i; k++)
							{
								for (int64_t l = 0; l < lengthY; l++)
								{
									free(tmp[k][l]);
								}
							}

							for (int64_t k = 0; k < lengthZ; k++)free(tmp[k]);
							free(tmp);
							return CcResult::FAILED_ALLOC_ERROR;
						}
					}
				}

				SPINADDR_HEISENBERG*** lptmp = (SPINADDR_HEISENBERG***)malloc((lengthZ + 2) * sizeof(SPINADDR_HEISENBERG**));
				if (lptmp == nullptr)return CcResult::FAILED_ALLOC_ERROR;

				for (int64_t i = 0; i < lengthZ + 2; i++)
				{
					lptmp[i] = (SPINADDR_HEISENBERG**)malloc((lengthY + 2) * sizeof(SPINADDR_HEISENBERG*));
					if (lptmp[i] == nullptr)
					{
						for (int64_t j = 0; j < i; j++)free(lptmp[j]);
						free(lptmp);
						return CcResult::FAILED_ALLOC_ERROR;
					}
				}

				for (int64_t i = 0; i < lengthZ + 2; i++)
				{
					for (int64_t j = 0; j < lengthY + 2; j++)
					{
						lptmp[i][j] = (SPINADDR_HEISENBERG*)malloc((lengthX + 2) * sizeof(SPINADDR_HEISENBERG));
						if (lptmp[i][j] == nullptr)
						{
							/*********** from here ************/
							for (int64_t k = 0; k < j; k++)free(lptmp[i][k]);
							for (int64_t k = 0; k < i; k++)
							{
								for (int64_t l = 0; l < lengthY + 2; l++)
								{
									free(lptmp[k][l]);
								}
							}

							for (int64_t k = 0; k < lengthZ + 2; k++)free(lptmp[k]);
							free(lptmp);
							return CcResult::FAILED_ALLOC_ERROR;
						}
					}
				}

				// address to pointer

				for (int64_t i = 0; i < lengthZ; i++)
				{
					for (int64_t j = 0; j < lengthY; j++)
					{
						for (int64_t k = 0; k < lengthX; k++)
						{
							lptmp[i + 1][j + 1][k + 1] = &tmp[i][j][k];
						}
					}
				}

				//bottom serface to top surface

				for (int64_t i = 0; i < lengthY; i++)
				{
					for (int64_t j = 0; j < lengthX; j++)
					{
						lptmp[0][i + 1][j + 1] = &tmp[lengthZ - 1][i][j];
					}
				}

				for (int64_t i = 0; i < lengthY; i++)
				{
					for (int64_t j = 0; j < lengthX; j++)
					{
						lptmp[lengthZ + 1][i + 1][j + 1] = &tmp[0][i][j];
					}
				}

				for (int64_t i = 0; i < lengthZ; i++)
				{
					for (int64_t j = 0; j < lengthX; j++)
					{
						lptmp[i + 1][0][j + 1] = &tmp[i][lengthY - 1][j];
					}
				}

				for (int64_t i = 0; i < lengthZ; i++)
				{
					for (int64_t j = 0; j < lengthX; j++)
					{
						lptmp[i + 1][lengthY + 1][j + 1] = &tmp[i][0][j];
					}
				}

				for (int64_t i = 0; i < lengthZ; i++)
				{
					for (int64_t j = 0; j < lengthY; j++)
					{
						lptmp[i + 1][j + 1][0] = &tmp[i][j][lengthX - 1];
					}
				}

				for (int64_t i = 0; i < lengthZ; i++)
				{
					for (int64_t j = 0; j < lengthY; j++)
					{
						lptmp[i + 1][j + 1][lengthX + 1] = &tmp[i][j][0];
					}
				}

				for (int64_t i = 0; i < lengthX; i++)lptmp[0][0][i + 1] = &tmp[lengthZ - 1][lengthY - 1][i];
				for (int64_t i = 0; i < lengthX; i++)lptmp[0][lengthY + 1][i + 1] = &tmp[lengthZ - 1][0][i];
				for (int64_t i = 0; i < lengthX; i++)lptmp[lengthZ + 1][0][i + 1] = &tmp[0][lengthY - 1][i];
				for (int64_t i = 0; i < lengthX; i++)lptmp[lengthZ + 1][lengthY + 1][i + 1] = &tmp[0][0][i];

				for (int64_t i = 0; i < lengthY; i++)lptmp[0][i + 1][0] = &tmp[lengthZ - 1][i][lengthX - 1];
				for (int64_t i = 0; i < lengthY; i++)lptmp[0][i + 1][lengthX + 1] = &tmp[lengthZ - 1][i][0];
				for (int64_t i = 0; i < lengthY; i++)lptmp[lengthZ + 1][i + 1][0] = &tmp[0][i][lengthX - 1];
				for (int64_t i = 0; i < lengthY; i++)lptmp[lengthZ + 1][i + 1][lengthX + 1] = &tmp[0][i][0];

				for (int64_t i = 0; i < lengthZ; i++)lptmp[i + 1][0][0] = &tmp[i][lengthY - 1][lengthX - 1];
				for (int64_t i = 0; i < lengthZ; i++)lptmp[i + 1][0][lengthX + 1] = &tmp[i][lengthY - 1][0];
				for (int64_t i = 0; i < lengthZ; i++)lptmp[i + 1][lengthY + 1][0] = &tmp[i][0][lengthX - 1];
				for (int64_t i = 0; i < lengthZ; i++)lptmp[i + 1][lengthY + 1][lengthX + 1] = &tmp[i][0][0];

				lptmp[0][0][0] = &tmp[lengthZ - 1][lengthY - 1][lengthX - 1];
				lptmp[lengthZ + 1][lengthY + 1][lengthX + 1] = &tmp[0][0][0];
				lptmp[0][0][lengthX + 1] = &tmp[lengthZ - 1][lengthY - 1][0];
				lptmp[0][lengthY + 1][lengthX + 1] = &tmp[lengthZ - 1][0][0];
				lptmp[0][lengthY + 1][0] = &tmp[lengthZ - 1][0][lengthX - 1];
				lptmp[lengthZ + 1][0][lengthX + 1] = &tmp[0][lengthY - 1][0];
				lptmp[lengthZ + 1][lengthY + 1][0] = &tmp[0][0][lengthX - 1];
				lptmp[lengthZ + 1][0][0] = &tmp[0][lengthY - 1][lengthX - 1];

				*spin = tmp;
				*spin_addr = lptmp;

				return CcResult::SUCCESS;
			}

		}


		CcResult ONM_free(HONMODEL OnModel)
		{
			PRIVATE_HANDLE_ONMODEL* pHandle = (PRIVATE_HANDLE_ONMODEL*)OnModel;

			CcResult error = CcResult::FAILED_INVALID;

			/* argument error check ***************************/

			if (OnModel == nullptr)return CcResult::FAILED_ALLOC_ERROR;
			else
			{
				if (pHandle->spin == nullptr)return CcResult::FAILED_ALLOC_ERROR;
				if (pHandle->spinaddr == nullptr)return CcResult::FAILED_ALLOC_ERROR;
			}

			int64_t lengthX = pHandle->config.x;
			int64_t lengthY = pHandle->config.y;
			int64_t lengthZ = pHandle->config.z;

			OnType type = pHandle->config.type;
			switch (type)
			{
			case OnType::ISING:
			case OnType::XY:
			case OnType::HEISENBERG:
				break;
			default:
				return CcResult::FAILED_ARG_WHY_COME_HERE;
			}

			DIMENSION dim = pHandle->config.dimension;

			switch (dim)
			{
			case DIMENSION::DIMENSION_1:
			case DIMENSION::DIMENSION_2:
			case DIMENSION::DIMENSION_3:
				break;

			default:
				return CcResult::FAILED_ARG_INVALID_FLAG;
			}

			/**************************************************/

			

			switch (dim)
			{
				//case 1:
				//	break;

			case DIMENSION::DIMENSION_2:
			{
				switch (type)
				{
				case OnType::ISING:
				{
					SPIN_ISING** si = (SPIN_ISING**)pHandle->spin;
					SPINADDR_ISING** lpsi = (SPINADDR_ISING**)pHandle->spinaddr;

					for (int64_t i = 0; i < lengthY + 2; i++)free(lpsi[i]);
					free(lpsi);

					for (int64_t i = 0; i < lengthY; i++)free(si[i]);
					free(si);
				}
				break;

				case OnType::XY:
				{
					SPIN_XY** sx = (SPIN_XY**)pHandle->spin;
					SPINADDR_XY** lpsx = (SPINADDR_XY**)pHandle->spinaddr;

					for (int64_t i = 0; i < lengthY + 2; i++)free(lpsx[i]);
					free(lpsx);

					for (int64_t i = 0; i < lengthY; i++)free(sx[i]);
					free(sx);
				}
				break;

				case OnType::HEISENBERG:
				{
					SPIN_HEISENBERG** sh = (SPIN_HEISENBERG**)pHandle->spin;
					SPINADDR_HEISENBERG** lpsh = (SPINADDR_HEISENBERG**)pHandle->spinaddr;

					for (int64_t i = 0; i < lengthY + 2; i++)free(lpsh[i]);
					free(lpsh);

					for (int64_t i = 0; i < lengthY; i++)free(sh[i]);
					free(sh);
				}
				break;

				default:
					return CcResult::FAILED_WHY_COME_HERE;
				}
			}
			break;

			case DIMENSION::DIMENSION_3:
			{
				switch (type)
				{
				case OnType::ISING:
				{
					SPIN_ISING*** si = (SPIN_ISING***)pHandle->spin;
					SPINADDR_ISING*** lpsi = (SPINADDR_ISING***)pHandle->spinaddr;

					for (int64_t i = 0; i < lengthZ + 2; i++)
					{
						for (int64_t j = 0; j < lengthY + 2; j++)
						{
							free(lpsi[i][j]);
						}
					}
					for (int64_t i = 0; i < lengthZ + 2; i++)free(lpsi[i]);
					free(lpsi);

					for (int64_t i = 0; i < lengthZ; i++)
					{
						for (int64_t j = 0; j < lengthY; j++)
						{
							free(si[i][j]);
						}
					}
					for (int64_t i = 0; i < lengthZ; i++)free(si[i]);
					free(si);
				}
				break;

				case OnType::XY:
				{
					SPIN_XY*** sx = (SPIN_XY***)pHandle->spin;
					SPINADDR_XY*** lpsx = (SPINADDR_XY***)pHandle->spinaddr;

					for (int64_t i = 0; i < lengthZ + 2; i++)
					{
						for (int64_t j = 0; j < lengthY + 2; j++)
						{
							free(lpsx[i][j]);
						}
					}
					for (int64_t i = 0; i < lengthZ + 2; i++)free(lpsx[i]);
					free(lpsx);

					for (int64_t i = 0; i < lengthZ; i++)
					{
						for (int64_t j = 0; j < lengthY; j++)
						{
							free(sx[i][j]);
						}
					}
					for (int64_t i = 0; i < lengthZ; i++)free(sx[i]);
					free(sx);
				}
				break;

				case OnType::HEISENBERG:
				{
					SPIN_HEISENBERG*** sh = (SPIN_HEISENBERG***)pHandle->spin;
					SPINADDR_HEISENBERG*** lpsh = (SPINADDR_HEISENBERG***)pHandle->spinaddr;

					for (int64_t i = 0; i < lengthZ + 2; i++)
					{
						for (int64_t j = 0; j < lengthY + 2; j++)
						{
							free(lpsh[i][j]);
						}
					}
					for (int64_t i = 0; i < lengthZ + 2; i++)free(lpsh[i]);
					free(lpsh);

					for (int64_t i = 0; i < lengthZ; i++)
					{
						for (int64_t j = 0; j < lengthY; j++)
						{
							free(sh[i][j]);
						}
					}
					for (int64_t i = 0; i < lengthZ; i++)free(sh[i]);
					free(sh);
				}
				break;

				default:
					return CcResult::FAILED_WHY_COME_HERE;
				}
			}
			break;

			default:
				return CcResult::FAILED_WHY_COME_HERE;
			}

			free(pHandle);

			return CcResult::SUCCESS;
		}


		CcResult ONM_init(HONMODEL OnModel, InitType init_type)
		{
			CcResult error = CcResult::FAILED_INVALID;
			PRIVATE_HANDLE_ONMODEL* pHandle = reinterpret_cast<PRIVATE_HANDLE_ONMODEL*>(OnModel);

			/* argument error check ***************************/

			if (OnModel == nullptr)return CcResult::FAILED_HANDLE_NOT_INITIALIZED;
			else
			{
				if (pHandle->spin == nullptr)CcResult::FAILED_ARG_INVALID;
				if (pHandle->spinaddr == nullptr)CcResult::FAILED_ARG_INVALID;
			}

			switch (init_type)
			{
			case InitType::MT:
			case InitType::COLD_P:
			case InitType::COLD_N:
			case InitType::ENUM:
				break;

			default:
				return CcResult::FAILED_ARG_INVALID_FLAG;
			}

			int64_t lengthX = pHandle->config.x;
			int64_t lengthY = pHandle->config.y;
			int64_t lengthZ = pHandle->config.z;

			OnType type = pHandle->config.type;
			switch (type)
			{
			case OnType::ISING:
			case OnType::XY:
			case OnType::HEISENBERG:
				break;
			default:
				return CcResult::FAILED_ARG_INVALID_FLAG;
			}


			/**************************************************/

			DIMENSION dim = pHandle->config.dimension;

			switch (dim)
			{
				//case 1:
			case DIMENSION::DIMENSION_2:
			{
				switch (type)
				{
				case OnType::ISING:
				{
					SPIN_ISING** si = (SPIN_ISING**)pHandle->spin;
					switch (init_type)
					{
					case InitType::MT:
					{
						/* Mersenne Twister */
						std::random_device seed_gen;
						std::mt19937_64 engine(seed_gen());
						std::uniform_int_distribution<> dist(0, 1);

						for (int64_t i = 0; i < lengthY; i++)
						{
							for (int64_t j = 0; j < lengthX; j++)
							{
								int64_t tmpi = dist(engine);
								if (tmpi == 0)tmpi = -1;
								si[i][j] = tmpi;
							}
						}

						pHandle->inittype = InitType::MT;
					}
					break;
					case InitType::COLD_P:
					{
						for (int64_t i = 0; i < lengthY; i++)
						{
							for (int64_t j = 0; j < lengthX; j++)
							{
								si[i][j] = 1;
							}
						}

						pHandle->inittype = InitType::COLD_P;
					}
					break;

					case InitType::COLD_N:
					{
						for (int64_t i = 0; i < lengthY; i++)
						{
							for (int64_t j = 0; j < lengthX; j++)
							{
								si[i][j] = -1;
							}
						}

						pHandle->inittype = InitType::COLD_N;
					}
					break;
					case InitType::ENUM:
					{
						for (int64_t i = 0; i < lengthY; i++)
						{
							for (int64_t j = 0; j < lengthX; j++)
							{
								si[i][j] = i * lengthX + j;
							}
						}

						pHandle->inittype = InitType::ENUM;
					}
					break;

					default:
						return CcResult::FAILED_WHY_COME_HERE;
					}
				}
				break;

				case OnType::XY:
				{
					SPIN_XY** sx = (SPIN_XY**)pHandle->spin;

					switch (init_type)
					{
					case InitType::MT:
					{
						/* Mersenne Twister */
						std::random_device seed_gen;
						std::mt19937_64 engine(seed_gen());
						std::uniform_real_distribution<> dist(0, 1);

						for (int64_t i = 0; i < lengthY; i++)
						{
							for (int64_t j = 0; j < lengthX; j++)
							{
								double tmpd = 2 * PC_PI_CIRCUMFERENCE * dist(engine);
								sx[i][j].x = cos(tmpd);
								sx[i][j].y = sin(tmpd);
							}
						}

						pHandle->inittype = InitType::MT;
					}
					break;

					case InitType::COLD_P:
					{
						for (int64_t i = 0; i < lengthY; i++)
						{
							for (int64_t j = 0; j < lengthX; j++)
							{
								int64_t count = lengthX * i + j;
								sx[i][j].x = 1;
								sx[i][j].y = 1;
							}
						}

						pHandle->inittype = InitType::COLD_P;
					}
					break;

					case InitType::COLD_N:
					{
						for (int64_t i = 0; i < lengthY; i++)
						{
							for (int64_t j = 0; j < lengthX; j++)
							{
								int64_t count = lengthX * i + j;
								sx[i][j].x = -1;
								sx[i][j].y = -1;
							}
						}

						pHandle->inittype = InitType::COLD_N;
					}
					break;

					case InitType::ENUM:
					{
						for (int64_t i = 0; i < lengthY; i++)
						{
							for (int64_t j = 0; j < lengthX; j++)
							{
								int64_t count = lengthX * i + j;
								sx[i][j].x = static_cast<double>(count);
								sx[i][j].y = static_cast<double>(count) + 0.1;
							}
						}

						pHandle->inittype = InitType::ENUM;
					}
					break;

					default:
						return CcResult::FAILED_WHY_COME_HERE;
					}
				}
				break;

				case OnType::HEISENBERG:
				{
					SPIN_HEISENBERG** sh = (SPIN_HEISENBERG**)pHandle->spin;
					switch (init_type)
					{
					case InitType::MT:
					{
						/* Mersenne Twister */
						std::random_device seed_gen_z;
						std::mt19937_64 engine_z(seed_gen_z());
						std::uniform_real_distribution<> dist_z(-1, 1);

						std::random_device seed_gen_arg;
						std::mt19937_64 engine_arg(seed_gen_arg());
						std::uniform_real_distribution<> dist_arg(0, 1);

						for (int64_t i = 0; i < lengthY; i++)
						{
							for (int64_t j = 0; j < lengthX; j++)
							{
								double tmpz = dist_z(engine_z);
								double tmparg = 2 * PC_PI_CIRCUMFERENCE * dist_arg(engine_arg);

								double restlen = pow(1.0 - tmpz * tmpz, 1.0 / 2.0);

								sh[i][j].z = tmpz;
								sh[i][j].x = restlen * cos(tmparg);
								sh[i][j].y = restlen * sin(tmparg);
							}
						}

						pHandle->inittype = InitType::MT;
					}
					break;
					case InitType::COLD_P:
					{
						for (int64_t i = 0; i < lengthY; i++)
						{
							for (int64_t j = 0; j < lengthX; j++)
							{
								sh[i][j].x = 1.0;
								sh[i][j].y = 1.0;
								sh[i][j].z = 1.0;
							}
						}

						pHandle->inittype = InitType::COLD_P;
					}
					break;

					case InitType::COLD_N:
					{
						for (int64_t i = 0; i < lengthY; i++)
						{
							for (int64_t j = 0; j < lengthX; j++)
							{
								sh[i][j].x = -1.0;
								sh[i][j].y = -1.0;
								sh[i][j].z = -1.0;
							}
						}

						pHandle->inittype = InitType::COLD_N;
					}
					break;

					case InitType::ENUM:
					{
						for (int64_t i = 0; i < lengthY; i++)
						{
							for (int64_t j = 0; j < lengthX; j++)
							{
								int64_t count = i * lengthX + j;
								sh[i][j].x = (double)count;
								sh[i][j].y = (double)count + 0.1;
								sh[i][j].z = (double)count + 0.2;
							}
						}

						pHandle->inittype = InitType::ENUM;
					}
					break;

					default:
						return CcResult::FAILED_WHY_COME_HERE;
					}

				}
				break;

				default:
					return CcResult::FAILED_WHY_COME_HERE;
				}
			}
			break;

			case DIMENSION::DIMENSION_3:
			{
				switch (type)
				{
				case OnType::ISING:
				{
					SPIN_ISING*** si = (SPIN_ISING***)pHandle->spin;
					switch (init_type)
					{
					case InitType::MT:
					{
						/* Mersenne Twister */
						std::random_device seed_gen;
						std::mt19937_64 engine(seed_gen());
						std::uniform_int_distribution<> dist(0, 1);

						for (int64_t i = 0; i < lengthZ; i++)
						{
							for (int64_t j = 0; j < lengthY; j++)
							{
								for (int64_t k = 0; k < lengthX; k++)
								{
									int64_t tmpi = dist(engine);
									if (tmpi == 0)tmpi = -1;
									si[i][j][k] = tmpi;
								}
							}
						}

						pHandle->inittype = InitType::MT;
					}
					break;
					case InitType::COLD_P:
					{
						for (int64_t i = 0; i < lengthZ; i++)
						{
							for (int64_t j = 0; j < lengthY; j++)
							{
								for (int64_t k = 0; k < lengthX; k++)
								{
									si[i][j][k] = 1;
								}
							}
						}

						pHandle->inittype = InitType::COLD_P;
					}
					break;

					case InitType::COLD_N:
					{
						for (int64_t i = 0; i < lengthZ; i++)
						{
							for (int64_t j = 0; j < lengthY; j++)
							{
								for (int64_t k = 0; k < lengthX; k++)
								{
									si[i][j][k] = -1;
								}
							}
						}

						pHandle->inittype = InitType::COLD_N;
					}
					break;
					case InitType::ENUM:
					{
						for (int64_t i = 0; i < lengthZ; i++)
						{
							for (int64_t j = 0; j < lengthY; j++)
							{
								for (int64_t k = 0; k < lengthX; k++)
								{
									int64_t count = lengthY * lengthX * i + lengthX * j + k;
									si[i][j][k] = count;
								}
							}
						}

						pHandle->inittype = InitType::ENUM;
					}
					break;

					default:
						return CcResult::FAILED_WHY_COME_HERE;
					}
				}
				break;

				case OnType::XY:
				{
					SPIN_XY*** sx = (SPIN_XY***)pHandle->spin;

					switch (init_type)
					{
					case InitType::MT:
					{
						/* Mersenne Twister */
						std::random_device seed_gen;
						std::mt19937_64 engine(seed_gen());
						std::uniform_real_distribution<> dist(0, 1);

						for (int64_t i = 0; i < lengthZ; i++)
						{
							for (int64_t j = 0; j < lengthY; j++)
							{
								for (int64_t k = 0; k < lengthX; k++)
								{
									double tmpd = 2 * PC_PI_CIRCUMFERENCE * dist(engine);
									sx[i][j][k].x = cos(tmpd);
									sx[i][j][k].y = sin(tmpd);
								}
							}
						}

						pHandle->inittype = InitType::MT;
					}
					break;

					case InitType::COLD_P:
					{
						for (int64_t i = 0; i < lengthZ; i++)
						{
							for (int64_t j = 0; j < lengthY; j++)
							{
								for (int64_t k = 0; k < lengthX; k++)
								{
									sx[i][j][k].x = 1.0;
									sx[i][j][k].y = 1.1;
								}
							}
						}

						pHandle->inittype = InitType::COLD_P;
					}
					break;

					case InitType::COLD_N:
					{
						for (int64_t i = 0; i < lengthZ; i++)
						{
							for (int64_t j = 0; j < lengthY; j++)
							{
								for (int64_t k = 0; k < lengthX; k++)
								{
									sx[i][j][k].x = -1.0;
									sx[i][j][k].y = -1.1;
								}
							}
						}

						pHandle->inittype = InitType::COLD_N;
					}
					break;

					case InitType::ENUM:
					{
						for (int64_t i = 0; i < lengthZ; i++)
						{
							for (int64_t j = 0; j < lengthY; j++)
							{
								for (int64_t k = 0; k < lengthX; k++)
								{
									int64_t count = lengthY * lengthX * i + lengthX * j + k;
									sx[i][j][k].x = static_cast<double>(count);
									sx[i][j][k].y = static_cast<double>(count) + 0.1;
								}
							}
						}

						pHandle->inittype = InitType::ENUM;
					}
					break;

					default:
						return CcResult::FAILED_WHY_COME_HERE;
					}
				}
				break;

				case OnType::HEISENBERG:
				{
					SPIN_HEISENBERG*** sh = (SPIN_HEISENBERG***)pHandle->spin;

					switch (init_type)
					{
					case InitType::MT:
					{
						/* Mersenne Twister */
						std::random_device seed_gen_z;
						std::mt19937_64 engine_z(seed_gen_z());
						std::uniform_real_distribution<> dist_z(-1, 1);

						std::random_device seed_gen_arg;
						std::mt19937_64 engine_arg(seed_gen_arg());
						std::uniform_real_distribution<> dist_arg(0, 1);

						for (int64_t i = 0; i < lengthZ; i++)
						{
							for (int64_t j = 0; j < lengthY; j++)
							{
								for (int64_t k = 0; k < lengthX; k++)
								{
									double tmpz = dist_z(engine_z);
									double tmparg = 2 * PC_PI_CIRCUMFERENCE * dist_arg(engine_arg);

									double restlen = pow(1.0 - tmpz * tmpz, 1.0 / 2.0);

									sh[i][j][k].z = tmpz;
									sh[i][j][k].x = restlen * cos(tmparg);
									sh[i][j][k].y = restlen * sin(tmparg);
								}
							}
						}

						pHandle->inittype = InitType::MT;
					}
					break;

					case InitType::COLD_P:
					{
						for (int64_t i = 0; i < lengthZ; i++)
						{
							for (int64_t j = 0; j < lengthY; j++)
							{
								for (int64_t k = 0; k < lengthX; k++)
								{
									sh[i][j][k].x = 1.0;
									sh[i][j][k].y = 1.1;
									sh[i][j][k].z = 1.2;
								}
							}
						}

						pHandle->inittype = InitType::COLD_P;
					}
					break;

					case InitType::COLD_N:
					{
						for (int64_t i = 0; i < lengthZ; i++)
						{
							for (int64_t j = 0; j < lengthY; j++)
							{
								for (int64_t k = 0; k < lengthX; k++)
								{
									sh[i][j][k].x = -1.0;
									sh[i][j][k].y = -1.1;
									sh[i][j][k].z = -1.2;
								}
							}
						}

						pHandle->inittype = InitType::COLD_N;
					}
					break;
					case InitType::ENUM:
					{
						for (int64_t i = 0; i < lengthZ; i++)
						{
							for (int64_t j = 0; j < lengthY; j++)
							{
								for (int64_t k = 0; k < lengthX; k++)
								{
									int64_t count = lengthY * lengthX * i + lengthX * j + k;
									sh[i][j][k].x = (double)count;
									sh[i][j][k].y = (double)count + 0.1;
									sh[i][j][k].z = (double)count + 0.2;
								}
							}
						}

						pHandle->inittype = InitType::ENUM;
					}
					break;

					default:
						return CcResult::FAILED_WHY_COME_HERE;
					}
				}
				break;
				default:
					return CcResult::FAILED_WHY_COME_HERE;
				}
			}
			break;

			default:
				return CcResult::FAILED_WHY_COME_HERE;
			}

			pHandle->updatetype = UpdateMethod::INVALID;
			pHandle->updatecount = 0;

			return CcResult::SUCCESS;
		}

	}
}
