//Based on code from http://msdn.microsoft.com/en-us/library/windows/apps/dn481541.aspx

cbuffer MaterialConstantBuffer : register(b2)
{
	float4 lightColor;
	float4 ka;
	float4 kd;
	float4 ks;
	float4 shininess;
};

struct PixelShaderInput
{
	float4 position		: SV_POSITION;
	float3 outVec		: POSITION0;
	float3 normal		: NORMAL0;
	float3 light		: POSITION1;
	float2 uv			: TEXCOORD0;
};

Texture2D Texture : register(t0);
SamplerState TrilinearSampler : register(s0);

float4 main(PixelShaderInput input) : SV_TARGET
{
	float3 L = normalize(input.light);
	float3 V = normalize(input.outVec);
	float3 R = normalize(reflect(L, input.normal));

	float4 diffuse = ka + (lightColor * kd * max(dot(input.normal, L), 0.0f));
	diffuse = saturate(diffuse);

	float4 specular = ks * pow(max(dot(R, V), 0.0f), shininess.x - 50.0f);
	specular = saturate(specular);

	float4 finalColor = (diffuse + specular) * Texture.Sample(TrilinearSampler, input.uv);

	return finalColor;
}