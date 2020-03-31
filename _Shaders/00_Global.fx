cbuffer CB_PerFrame
{
    matrix View;
    matrix ViewInverse;
    matrix Projection;
    matrix VP;

    float Time;
};

cbuffer CB_World
{
    matrix World;
};

Texture2D DiffuseMap;
Texture2D SpecularMap;
Texture2D NormalMap;

TextureCube SkyCubeMap;
///////////////////////////////////////////////////////////////////////////////

float4 WorldPosition(float4 position)
{
    return mul(position, World);
}

float4 ViewProjection(float4 position)
{
    position = mul(position, View);
    return mul(position, Projection);
}

float3 WorldNormal(float3 normal)
{
    return mul(normal, (float3x3) World);
}

///////////////////////////////////////////////////////////////////////////////

struct MeshOutput
{
    float4 Position : SV_Position0;
    float4 wvpPosition : Position1;
    float3 oPosition : Position2;
    float3 wPosition : Position3;
    
    float2 Uv : Uv0;
    float3 Normal : Normal0;
};

struct VertexMesh
{
    float4 Position : Position0;
    float2 Uv : Uv0;
    float3 Normal : Normal;
};

MeshOutput VS_Mesh(VertexMesh input)
{
    MeshOutput output;
    
    output.oPosition = input.Position.xyz;
    output.Position = WorldPosition(input.Position);
    output.wPosition = output.Position.xyz;
    
    output.Position = ViewProjection(output.Position);
    output.wvpPosition = output.Position;
    
    output.Normal = WorldNormal(input.Normal);
    output.Uv = input.Uv;
    
    return output;
}

struct Vertex
{
    float Position : POSITION0;
};

struct VertexNormal
{
    float4 Position : POSITION0;
    float3 Normal : NORMAL0;
};

struct VertexColorNormal
{
    float4 Position : POSITION0;
    float4 Color : COLOR0;
    float3 Normal : NORMAL0;
};

struct VertexTexture
{
    float4 Position : POSITION0;
    float2 Uv : UV0;
};

struct VertexTextureNormal
{
    float4 Position : POSITION0;
    float2 Uv : UV0;
    float3 Normal : NORMAL0;
};

///////////////////////////////////////////////////////////////////////////////

RasterizerState FillModeWireFrame
{
    FillMode = WireFrame;
};

RasterizerState FrontCounterClockwise_True
{
    FrontCounterClockwise = true;
};

DepthStencilState DepthEnable_False
{
    DepthEnable = false;
};

SamplerState LinearSampler
{
    Filter = MIN_MAG_MIP_LINEAR;

    AddressU = Wrap;
    AddressV = Wrap;
};

///////////////////////////////////////////////////////////////////////////////


#define P_VP(name, vs, ps) \
pass name \
{ \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}

#define P_RS_VP(name, rs, vs, ps) \
pass name \
{ \
    SetRasterizerState(rs); \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}

#define P_RS_DSS_VP(name, rs,dss, vs, ps) \
pass name \
{ \
    SetRasterizerState(rs); \
    SetDepthStencilState(dss,0); \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}