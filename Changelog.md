### 05/08/2023

* Thematic overhaul to a bee theme

* Renamed Hex -> Hexii

* Added "Hexagon" base class for general hexagons. (Changes ahead of the district skill trees to come)

* Additionally, HexiiPlane (formerly 'HexPlane') has separated into a base class HexagonPlane and subclass HexiiPlane, where HexagonPlane handles the geometry and relative positioning, while HexiiPlane handles the gameplay aspects. (Changes ahead of the district skill trees to come)

* Split off upgrades into BeeGroup, each of which has its own UpgradePath. A hexii can have multiple BeeGroups. (Changes ahead of the district upgrades to come)

### 05/12/2022 

+ Added BigInt.hpp for some b i g numbers
~ Fixed BigInt.hpp linker errors

### 04/12/2022 Commit

+ Implemented implicit hexagon collision detection by way of a world coordinate to axial coordinate algorithm.
- Removed all traces of shared pointers because cocos uses its own autorelease pool for basically everything.

# 03/12/2022 Progression

