#version 330
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_explicit_uniform_location : require

// System outputs
layout(location = 0) out vec4 FragDiffuse;
layout(location = 1) out vec4 FragNormals;
layout(location = 2) out vec4 FragLighting;
layout(location = 3) out vec4 FragGlowmap;

// System Inputs

layout(std140) uniform sys_cbuffer_PerObject
{
    mat4 sys_ModelTRS;
    mat4 sys_ModelRS;
    mat4 sys_ModelViewProjectionMatrix;
    mat4 sys_ModelViewProjectionMatrixInverse;
};
layout(std140) uniform sys_cbuffer_PerObjectExt
{
    vec4    sys_DiffuseColor;
    vec4    sys_SpecularColor;
    // Emissive RGB - Emissive color.
    // Emissive A - Alpha cutoff.
    vec4    sys_EmissiveColor;
    vec4    sys_LightingOverrides;

    vec4    sys_TextureScale;
    vec4    sys_TextureOffset;
};
layout(std140) uniform sys_cbuffer_PerCamera
{
    mat4 sys_ViewProjectionMatrix;
    vec4 sys_WorldCameraPos;
    vec4 sys_ViewportInfo;
    vec2 sys_ScreenSize;
    vec2 sys_PixelRatio;
};
layout(std140) uniform sys_cbuffer_PerFrame
{
    // Time inputs
    vec4    sys_SinTime;
    vec4    sys_CosTime;
    vec4    sys_Time;

    // Fog
	vec4	sys_FogColor;
	vec4	sys_AtmoColor;
	float 	sys_FogEnd;
	float 	sys_FogScale;
};

// Samplers
layout(location = 20) uniform sampler2D textureSampler0; // Diffuse RGB, Specular/Discard A
layout(location = 21) uniform sampler2D textureSampler1; // Normals RGB, Height A
layout(location = 22) uniform sampler2D textureSampler2; // Ambient Occlusion R, Metallic G, Smoothness B, Discard A
layout(location = 23) uniform sampler2D textureSampler3; // Overlay RGB, Blend Style A (0 for MUL-BIAS, 1 for LERP)

// Vertex Outputs
in vec4 v2f_colors;
in vec4 v2f_position;
in vec3 v2f_texcoord;
in vec4 v2f_normals;
in vec4 v2f_binorms;
in vec4 v2f_tangents;

vec4 mainDiffuse ( vec4 colorDiffuse, vec4 colorOverlay, vec4 surfaceProperties )
{
    // pixelDiffuse - Generate via pulling from the palette.
    // rgb  surface color

    // Start with a diffuse pushed to zero by metallicness
    vec4 result = colorDiffuse * sys_DiffuseColor * (1.0 - surfaceProperties.g);
    // MUL-BIAS
    result.rgb *= mix( min(colorOverlay.rgb * 2.0, 1.0), vec3(1.0,1.0,1.0), colorOverlay.a );
    // MIX
    result.rgb += (colorOverlay.rgb - result.rgb) * colorOverlay.a;

    return vec4( result.rgb, 1.0 );
}

vec4 mainGlowmap ( vec4 colorOverlay )
{
    // pixelGlow
	// rgb	surface glow
	// a	rim lighting strength

    return vec4(
        max(colorOverlay.rgb - vec3(0.5,0.5,0.5), 0.0) * 2.0 * colorOverlay.a,
        sys_LightingOverrides.g );
}
vec4 mainLighting ( vec4 colorDiffuse, vec4 surfaceProperties )
{
    // pixelLightProperty
    // rgb  specular color
	// a	smoothness

    return vec4(
		colorDiffuse.rgb * surfaceProperties.g * 2.0,
		surfaceProperties.b );
}

vec4 mainNormals ( vec4 colorNormals, vec4 colorSurface )
{
    // pixelNormal
    // rgb	surface normal
    // a	ambient occlusion

    //vec4 baseNormal = vec4( v2f_normals.xyz, 1.0 );
    //return vec4(baseNormal.xyz, colorSurface.r);
    // TODO: Use normal map properly
    //baseNormal.xyz = colorNormals.xyz * 2.0 - 1.0;
    //vec4 transformedNormal = sys_ModelRS * vec4(baseNormal.xyz, 1.0);

    vec3 baseNormal = colorNormals.xyz * 2.0 - 1.0;
    vec3 transformedNormal = mat3( v2f_tangents.xyz, v2f_binorms.xyz, v2f_normals.xyz ) * baseNormal;
    return vec4(transformedNormal.xyz, colorSurface.r);
}

void main ( void )
{
    vec4 colorDiffuse = texture( textureSampler0, v2f_texcoord.xy );
    if ( colorDiffuse.a < sys_EmissiveColor.a ) discard;
    vec4 colorSurface = texture( textureSampler2, v2f_texcoord.xy );
    if ( colorSurface.a < 0.5 ) discard;
    vec4 colorNormals = texture( textureSampler1, v2f_texcoord.xy );
    vec4 colorOverlay = texture( textureSampler3, v2f_texcoord.xy );

    FragDiffuse  = mainDiffuse(colorDiffuse, colorOverlay, colorSurface);
    FragNormals  = mainNormals(colorNormals, colorSurface);
    FragLighting = mainLighting(colorDiffuse, colorSurface);
    FragGlowmap  = mainGlowmap(colorOverlay);
}
