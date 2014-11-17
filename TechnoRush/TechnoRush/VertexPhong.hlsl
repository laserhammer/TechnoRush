//Based on code from http://msdn.microsoft.com/en-us/library/windows/apps/dn481541.aspx

cbuffer perModel : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
};

cbuffer LightConstantBuffer : register(b1)
{
	float4 lightPos;
};

struct VertexShaderInput
{
	float3 position		:POSITION;	
	float2 uv			:TEXCOORD0;
	float3 normal		:NORMAL;
};

struct VertexToPixel
{
	float4 position		: SV_POSITION;	
	float3 outVec		: POSITION0;
	float3 outNormal	: NORMAL0;
	float3 outLightVec	: POSITION1;
	float2 uv			: TEXCOORD0;
};

VertexToPixel main(VertexShaderInput input)
{
	matrix mvMatrix = mul(world, view);
	matrix mvpMatrix = mul(mvMatrix, projection);

	VertexToPixel output;

	float4 pos = float4(input.position, 1.0f);
	float4 normal = float4(input.normal, 1.0f);
	float4 light = float4(lightPos.xyz, 1.0f);

	//Temporary specularity hack, pass in camera info later
	float4 eye = float4(0.0f, 2.5f, -5.0f, 1.0f);

	output.position = mul(pos, mvpMatrix);
	output.outNormal = mul(normal, mvMatrix).xyz;
	output.outVec = -(eye - mul(pos, mvMatrix)).xyz;
	output.outLightVec = mul(light, mvMatrix).xyz;
	output.uv = input.uv;

	return output;
}