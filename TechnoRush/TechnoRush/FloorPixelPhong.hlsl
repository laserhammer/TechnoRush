struct PixelShaderInput
{
	float4 position		: SV_POSITION;
	float4 color		: COLOR;
	float2 uv			: TEXCOORD0;
	float3 normal		: NORMAL0;
	float3 worldPos		: POSITION0;
};

Texture2D Texture : register(t0);
SamplerState TrilinearSampler : register(s0);

float4 main(PixelShaderInput input) : SV_TARGET
{
	// Move this to constant buffer
	float3 lightPos = float3(0.0, 8.0, 10.0);
	float3 lightDir = input.worldPos - lightPos;
	float diffusePower = 5.0f;
	float specPower = 5.0f;
	float3 lightSpecColor = float3(1.0f, 1.0f, 1.0f);
		float3 lightDiffuseColor = float3(1.0f, 1.0f, 1.0f);
		float3 ambientLight = float3(1.0f, 1.0f, 1.0f);
		float dis = length(lightDir);
	// And this
	float3 camPos = float3(0.0f, 1.5f, -5.0f);
		float3 outVec = normalize(camPos) - normalize(input.worldPos);

		// Diffuse
		float3 normal = normalize(input.normal);
		float NdotL = dot(normal, -lightDir);
	float intensity = saturate(NdotL);

	float4 diffuse = float4(intensity * lightDiffuseColor * diffusePower / dis, 1.0f);

		// Specular
		float3 H = reflect(lightDir, normal);
		intensity = pow(saturate(dot(outVec, H)), 1);

	float4 specular = float4(intensity * lightSpecColor * specPower / dis, 1.0f);

		float4 texSample = Texture.Sample(TrilinearSampler, input.uv);

		float4 finalColor = specular + (diffuse + float4(ambientLight.xyz, 1.0f)) * texSample;

		return finalColor;
}