# Usage [Croyfet OnModel]

## Requirement

### Include directory

- include/ (namespace cc::onm)
- external/CrHeader/include (namespace cc)

### Library directory
- lib/

### Library

#### Windows
- OnModel.lib

#### Linux
- libOnModel.a

## code

See sample for details. [OnModel/Sample](https://github.com/Croyfet/OnModel/tree/main/sample)

1. Initialize `HONMODEL` handle by `ONMODEL_CONFIG` struct.

	This sample is below condition.
	
	- Ising model
	- ExchangeInteraction : -1.0 (Antiferromagnetics)
	- Temperature
	- Dimension : 2D square lattice
	- lattice size : 16 x 16
	- External magneticfield : no

	```C++
	cc::onm::HONMODEL hOnm = nullptr;

	cc::onm::ONMODEL_CONFIG config;
	config.OnType = OnModel::ISING;
	config.J_ExchangeInteraction = -1.0;
	config.T_Temperature = 0.01;
	config.dimension = DIMENSION::DIMENSION_2;
	config.x = config.y = 16;
	config.H_MagneticField.z = 0;

	cc::onm::ONM_new(&hOnm, config);
	```
	`cc::onm::HONMODEL` handle must free by `cc::onm::ONM_free` function after simulation.

2. Thermalization (Monte Carlo step)

	Use Metropolis update method 1000 times.

	```C++
	cc::onm::ONM_Metropolis_SingleSpinFlip(hOnm, 1000);
	```

	You can call this function many times.
	```C++
	cc::onm::ONM_Metropolis_SingleSpinFlip(hOnm, 10000); //Thermalization
	//sampling here
	cc::onm::ONM_Metropolis_SingleSpinFlip(hOnm, 1000);
	//sampling here
	cc::onm::ONM_Metropolis_SingleSpinFlip(hOnm, 1000);
	...
	```

3. Sampling

	Example of energy and magnetization.

	```C++
	cc::onm::ONM_STATUS os;
	cc::onm::ONM_get_status(hOnm, &os);
	std::printf("Energy : %lf Magnetization : %lf\r\n", os.E_Energy, os.M_Magnetization.z);
	```

4. Free

	```C++
	cc::onm::ONM_free(hOnm);
	hOnm = nullptr;
	```

---
[Back to Home](../readme.md)
