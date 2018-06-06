struct INPUT_PIXEL_CUBE {
	float4 projPos : SV_POSITION;
};

float4 main(INPUT_PIXEL_CUBE toScreen) : SV_TARGET {	
	return float4(1,0,0,1);
}