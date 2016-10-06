#pragma pack_matrix(row_major)

cbuffer OBJECT {
	float4x4 WVP;
	float4x4 World;
};

struct INPUT_CUBE_VERTEX {
	float3 pos : POSITION;
	float2 uv : COLOR;
	float3 norm : NORMAL;
	float3 tan : TANGENT;
};

struct VS_OUTPUT {
	float4 projPos : SV_POSITION;
};


VS_OUTPUT main(INPUT_CUBE_VERTEX fromVertexBuffer) {
	VS_OUTPUT output;

	output.projPos = mul(float4(fromVertexBuffer.pos, 1), WVP);

	return output;
}