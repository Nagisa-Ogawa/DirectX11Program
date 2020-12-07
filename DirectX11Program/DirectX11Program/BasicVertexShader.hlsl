cbuffer ConstantBuffer
{
	matrix World;         //ワールド変換行列
	matrix View;          //ビュー変換行列
	matrix Projection;    //透視射影変換行列
	matrix WorldViewProjection;	// WVP行列
}

struct VSInput
{
	float4 position : POSITION;		// ローカル座標
	float3 normal : NORMAL;		// 法線データ
};

struct VSOutput
{
	float4 position : SV_POSITION;	// ワールド座標
	float4 color : COLOR0;	// 色データ
};

VSOutput main(VSInput input)
{
	VSOutput output;
	// ワールド座標に変換
	output.position = mul(input.position, WorldViewProjection);
	output.color = float4(1.0f, 0.0f, 0.0f, 0.0f);
	return output;
}
