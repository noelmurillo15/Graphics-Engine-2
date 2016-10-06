#pragma pack_matrix(row_major)

cbuffer cbPerObject {
	float4x4 WVP;
	float4x4 World;
};

struct INPUT_GRID_VERTEX {
	float4 pos : POSITION;
	float4 color : COLOR;
};

struct VS_OUTPUT {
	float4 projPos : SV_POSITION;
	float4 colorOut : COLOR;
};


VS_OUTPUT main(INPUT_GRID_VERTEX fromVertexBuffer) {
	VS_OUTPUT output;

	output.projPos = mul(fromVertexBuffer.pos, WVP);

	output.colorOut = fromVertexBuffer.color;

	return output;
}