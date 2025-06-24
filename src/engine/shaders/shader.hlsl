struct VS_Input {
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD;
    float4 color : COLOR;
};

struct PS_Input {
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD;
    float4 color : COLOR;
};

cbuffer cb : register(b0) {
    matrix world;
    matrix view;
    matrix proj;
};

PS_Input VS_Main(VS_Input input) {
    PS_Input output;
    output.position = mul(mul(mul(float4(-input.position, 1.0), world), view), proj);
    output.normal = mul(input.normal, (float3x3) world);
    output.tex = input.tex;
    output.color = input.color;
    return output;
}

// --- Pixel Shader ---
float4 PS_Main(PS_Input input) : SV_TARGET {
    float3 lightDir = normalize(float3(0.5, -1.0, 0.5));
    float diffuse = max(0.2, dot(-lightDir, -normalize(input.normal)));
    return input.color * diffuse;
}