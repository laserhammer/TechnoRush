
cbuffer perModel : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
	float4 color;
};

struct VertexShaderInput
{
	float3 position		: POSITION;
	float4 color		: COLOR;
	float2 uv			: TEXCOORD0;
	float3 normal		: NORMAL;
};

struct VertexToPixel
{
	float4 position		: SV_POSITION;	
	float4 color		: COLOR;
	float2 uv			: TEXCOORD0;
};

VertexToPixel main(VertexShaderInput input)
{
	VertexToPixel output;

	matrix worldViewProj = mul(mul(world, view), projection);
	output.position = mul(float4(input.position, 1.0f), worldViewProj);

	output.color = input.color * float4(color.xyz, 1.0f);

	output.uv = input.uv;

	return output;
}