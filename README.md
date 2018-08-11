# UBrotEngineX

UBrotEngineX is a game engine written in modern C++ using DirectX 11.

## Table of Contents

- [Dependencies](#dependencies)
- [Features](#features)
- [Design Questions](#design-questions)
- [Tutorials](#tutorials)
- [Example Application](#example-application)
- [License](#license)

## Dependencies

- DirectX 11
- Mircosoft ComPtr
- Metaprogramming Library
- rapidJSON
## Features

### In Progress

### Planned

- [ ] use [Boost Hana](https://www.boost.org/doc/libs/1_61_0/libs/hana/doc/html/index.html) as MPL
- [ ] Github CI (style checks, maybe building)
- [ ] FPS counter
- [ ] Logger
- [ ] Smart tiling system
- [ ] Thread-Manager (multithreading)
- [ ] Refactor ```Direct3D::ResetViewport()```

### Done

- [x] basic entity-component system (**DOD**)
- [x] basic asset management
- [x] basic rendering system

## Design Questions

- Use include guards or ```#pragma once```?
- Use ```std::filesystem``` (C++17)?
- Refactor namespaces?
- Packing (which) functionalities as libraries (LIB, DLL)?
- Usage of ```static_assert()``` and ```assert()```
- Basic styleguide
	- ```auto``` where possible
	- ```const```, ```noexcept``` and ```constexpr``` where possible
	- type traits?
	- ```std::variant```, ```std::optional``` and ```std::future```
	

## Tutorials

## Example Application

## License
