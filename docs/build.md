# Build [Croyfet OnModel]


## Product

- include/
- lib/ (Compile yourself)

Other directories doesn't need to use.

## Ready
1. Downloads this repository

	```
	git clone git@github.com:Croyfet/OnModel.git
	```

2. Move to the directory

	```
	mv OnModel
	```

3. Get submodule
	```
	git submodule init
	git submodule update
	```

4. Build


## Build

### Windows

#### Static Library

Open `msbuild/lib/lib.sln` with Visual Studio and select `build` from menu.


### Linux

`g++` and `make` are required to build in Linux.

#### Static Library

```
make
```

---
[Back to Home](../readme.md)
