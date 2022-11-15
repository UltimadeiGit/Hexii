#pragma once

#include "Hex.h"
#include "HexagonCoordinatePlane.h"

typedef HexagonCoordinatePlane<Hex> HexPlane;
typedef std::shared_ptr<HexPlane> HexPlanePtr;