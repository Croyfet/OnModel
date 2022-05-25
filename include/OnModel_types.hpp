#pragma once

#include <cstdint>

#include <CrHeader.hpp>

namespace cc
{
	namespace onm
	{

		// <handle> ONM Handle
		//     Handle to ONM Object File
		CC_DECLARE_HANDLE(HONMODEL);

		struct _tagMAGNETICFIELD;
		struct _tagMAGNETIZATION;

		using MAGNETICFIELD = struct _tagMAGNETICFIELD;
		using MAGNETIZATION = struct _tagMAGNETIZATION;

		struct _tagMAGNETICFIELD
		{
			double x;
			double y;
			double z;
		};

		struct _tagMAGNETIZATION
		{
			double x;
			double y;
			double z;
		};


		enum class _tagOnType : uint64_t;
		enum class _tagDIMENSION : uint64_t;

		using OnType = enum class _tagOnType : uint64_t;
		CC_ENABLE_BITWISE_OPERATION(OnType)
		using DIMENSION = enum class _tagDIMENSION : uint64_t;
		CC_ENABLE_BITWISE_OPERATION(DIMENSION)


		struct _tagONMODEL_CONFIG;
		// <struct> ONMODEL_CONFIG
		// <member>
		//     1. [uint64_t] type
		//         OT_ISING / OT_XY / OT_HEISENBERG
		//     2. [double] J_ExchangeInteraction
		//     3. [double] T_Temperature
		//     4. [int] demension
		//     5. [int] x
		//     6. [int] y
		//     7. [int] z
		//     8. [MAGNETICFIELD] H_MagneticField
		//         [double]x [double]y [double]z
		using ONMODEL_CONFIG = struct _tagONMODEL_CONFIG;

		struct _tagONMODEL_CONFIG
		{
			OnType type;
			double J_ExchangeInteraction;
			double T_Temperature;
			DIMENSION dimension;
			int64_t x;
			int64_t y;
			int64_t z;
			MAGNETICFIELD H_MagneticField;
		};

		enum class _tagOnType : uint64_t
		{
			ISING =			0x0000000000000001,
			XY =			0x0000000000000002,
			HEISENBERG =	0x0000000000000004,

			INVALID =		0x0000000000000000
		};

		enum class _tagDIMENSION : uint64_t
		{
			DIMENSION_1 =		0x0000000100000000L,
			DIMENSION_2 =		0x0000000200000000L,
			DIMENSION_3 =		0x0000000400000000L,

			INVALID =			0x0000000000000000L
		};


		struct _tagONMPOINT;
		using ONMPOINT = struct _tagONMPOINT;

		struct _tagONMPOINT
		{
			int32_t x;
			int32_t y;
			int32_t z;
		};

		struct _tagSPIN_XY;
		struct _tagSPIN_HEISENBERG;

		using SPIN_ISING = int64_t;
		using SPINADDR_ISING = SPIN_ISING*;

		using SPIN_XY = struct _tagSPIN_XY;
		using SPINADDR_XY = SPIN_XY*;

		using SPIN_HEISENBERG = struct _tagSPIN_HEISENBERG;
		using SPINADDR_HEISENBERG = SPIN_HEISENBERG*;

		struct _tagSPIN_XY
		{
			double x;
			double y;
		};

		struct _tagSPIN_HEISENBERG
		{
			double x;
			double y;
			double z;
		};




		enum class _tagInitType : uint64_t;
		using InitType = enum class _tagInitType : uint64_t;
		enum class _tagInitType : uint64_t
		{
			MT =		0x0000000000000001,
			COLD_P =	0x0000000000000002,
			COLD_N =	0x0000000000000004,
			ENUM =		0x0000000000000008,
			MFIXED =	0x0000000000000010,

			INVALID =	0x0000000000000000
		};

		struct _tagONM_STATUS;
		using ONM_STATUS = struct _tagONM_STATUS;
		struct _tagONM_STATUS
		{
			MAGNETIZATION M_Magnetization;
			double E_Energy;
		};

		enum class _tagInspectSpin : uint64_t;
		using InspectSpin = enum class _tagInspectSpin : uint64_t;
		enum class _tagInspectSpin : uint64_t
		{
			SPIN =			0x000000000001,
			ADDRESS =		0x000000000002,
			SPINADDRESS =	0x000000000004
		};

		enum class _tagInspectStatus : uint64_t;
		using InspectStatus = enum class _tagInspectStatus : uint64_t;
		CC_ENABLE_BITWISE_OPERATION(InspectStatus)
		enum class _tagInspectStatus : uint64_t
		{
			TYPE =					0x0000000000000001,
			EXCHANGEINTERACTION =	0x0000000000000002,
			TEMPERATURE =			0x0000000000000004,
			DIMENSION =				0x0000000000000008,
			LATTICE =				0x0000000000000010,
			MAGNETICFIELD =			0x0000000000000020,
			INITTYPE =				0x0000000000000040,
			UPDATETYPE =			0x0000000000000080,
			UPDATECOUNT =			0x0000000000000100,

			INVALID=				0x0000000000000000
		};

		enum class _tagUpdateMethod : uint64_t;
		using UpdateMethod = enum class _tagUpdateMethod : uint64_t;
		CC_ENABLE_BITWISE_OPERATION(UpdateMethod)
		enum class _tagUpdateMethod : uint64_t
		{
			METROPOLIS =					0x0000000000000001,
			DUAL_SPIN_SWAP_METROPOLIS =		0x0000000000000002,

			INVALID =						0x0000000000000000
		};

	}
}

