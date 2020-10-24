#pragma once
#include "Model.hpp"
class Water : public Model
{
	
public:
	Water(ID3D11Device* device);
	VOID UpdateWater(ID3D11DeviceContext* context);
};

