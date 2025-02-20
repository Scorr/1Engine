#version 430

layout(location = 0) in vec3 mdl_Vertex;
layout(location = 1) in vec3 mdl_TexCoord;

// Outputs to fragment shader
layout(location = 0) out vec4 v2f_position;
layout(location = 1) out vec2 v2f_texcoord0;
layout(location = 2) out vec2 v2f_texcoord1;

// System inputs
//layout(location = 140) uniform float sys_PixelRatio;
layout(binding = 2, std140) uniform sys_cbuffer_PerCamera
{
    mat4 sys_ViewProjectionMatrix;
    vec4 sys_WorldCameraPos;
    vec4 sys_ViewportInfo;
    vec2 sys_ScreenSize;
    vec2 sys_PixelRatio;
};

void main ( void )
{
	vec4 v_screenPos = vec4( mdl_Vertex, 1.0 );

	v2f_position	= vec4( mdl_Vertex, 1.0 );
    v2f_texcoord0	= mdl_TexCoord.xy;
    vec2 render_scale = sys_ViewportInfo.zw / sys_ScreenSize.xy;
    v2f_texcoord1   = (mdl_TexCoord.xy - vec2(0.5, 0.5) * render_scale) / render_scale;

	gl_Position = v_screenPos;
}
