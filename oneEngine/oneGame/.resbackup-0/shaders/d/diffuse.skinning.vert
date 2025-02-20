#version 140

in vec3 mdl_Vertex;
in vec3 mdl_TexCoord;
in vec4 mdl_Color;
in vec3 mdl_Normal;
in vec3 mdl_Tangents;
in vec3 mdl_Binormals;

in vec4 mdl_BoneWeights;
in uvec4 mdl_BoneIndices;

// Outputs to fragment shader
out mat3 v2f_tangent;
out vec4 v2f_colors;
out vec4 v2f_position;
out vec2 v2f_texcoord0;
out float v2f_fogdensity;
out vec3 v2f_screenpos;

// System inputs
uniform mat4 sys_ModelMatrix;
uniform mat4 sys_ModelRotationMatrix;
uniform mat4 sys_ModelViewProjectionMatrix;

// Buffer sampler
//uniform samplerBuffer textureMatrices;
// Skinning
layout(std140) uniform sys_SkinningDataMajor
{
	mat4 majorSkinning [256];
};
/*layout(std140) uniform sys_SkinningDataMinor
{
	mat4 minorSkinning [256];
};*/

// Lighting and Shadows
layout(std140) uniform sys_LightingInfo
{
	vec4 sys_LightColor[8];
	vec4 sys_LightPosition[8];
	vec4 sys_LightProperties[8];
	vec4 sys_LightShadowInfo[8];
	mat4 sys_LightMatrix[8];	// first 4 are cascade lights
};
out vec4 v2f_lightcoord[8];

// Fog
layout(std140) uniform sys_Fog
{
	vec4	sys_FogColor;
	float 	sys_FogEnd;
	float 	sys_FogScale;
};

void main ( void )
{
	vec4 v_localPos = vec4( mdl_Vertex, 1.0 );
	// Vertex skinning
	vec3 v_finalNorm= vec3( 0,0,0 );
	vec4 v_finalPos = vec4( 0,0,0,0 );
	mat4 v_tempMatrix;
	// Build matrix for bone0
	v_tempMatrix = majorSkinning[int(mdl_BoneIndices.x)];
		v_tempMatrix = transpose(v_tempMatrix);
	v_finalPos += v_tempMatrix * v_localPos * mdl_BoneWeights.x;
	v_finalNorm += mat3(v_tempMatrix) * mdl_Normal * mdl_BoneWeights.x;
	// Build matrix for bone1
	v_tempMatrix = majorSkinning[int(mdl_BoneIndices.y)];
		v_tempMatrix = transpose(v_tempMatrix);
	v_finalPos += v_tempMatrix * v_localPos * mdl_BoneWeights.y;
	v_finalNorm += mat3(v_tempMatrix) * mdl_Normal * mdl_BoneWeights.y;
	// Build matrix for bone2
	v_tempMatrix = majorSkinning[int(mdl_BoneIndices.z)];
		v_tempMatrix = transpose(v_tempMatrix);
	v_finalPos += v_tempMatrix * v_localPos * mdl_BoneWeights.z;
	v_finalNorm += mat3(v_tempMatrix) * mdl_Normal * mdl_BoneWeights.z;
	// Build matrix for bone3
	v_tempMatrix = majorSkinning[int(mdl_BoneIndices.w)];
		v_tempMatrix = transpose(v_tempMatrix);
	v_finalPos += v_tempMatrix * v_localPos * mdl_BoneWeights.w;
	v_finalNorm += mat3(v_tempMatrix) * mdl_Normal * mdl_BoneWeights.w;
	
	// Set the final result
	v_localPos = v_finalPos;
	// End vertex skinning
	vec4 v_screenPos = sys_ModelViewProjectionMatrix * v_localPos;

	//v2f_normals		= sys_ModelRotationMatrix*vec4( normalize(v_finalNorm), 1.0 );
	vec4 v_normal	= sys_ModelRotationMatrix*vec4( normalize(v_finalNorm), 1.0 );
	vec4 v_tangent	= sys_ModelRotationMatrix*vec4( normalize(mdl_Tangents), 1.0 );
	vec4 v_binormal	= sys_ModelRotationMatrix*vec4( normalize(mdl_Binormals), 1.0 );
	if ( dot( cross( v_normal.xyz, v_tangent.xyz ), v_binormal.xyz ) < 0 ) {
		v_tangent = -v_tangent;
	}
	v2f_tangent		= mat3( v_tangent.xyz, v_binormal.xyz, v_normal.xyz );
	
	v2f_colors		= mdl_Color;
	v2f_position	= sys_ModelMatrix*v_localPos;
	v2f_texcoord0	= mdl_TexCoord.xy;
	v2f_fogdensity = clamp( (sys_FogEnd - v_screenPos.z) * sys_FogScale, 0, 1 );

	v2f_screenpos	= vec3( v_screenPos.x,v_screenPos.y,v_screenPos.w );
	v2f_lightcoord[0] = (v2f_position * sys_LightMatrix[0]);
	v2f_lightcoord[1] = (v2f_position * sys_LightMatrix[1]);
	v2f_lightcoord[2] = (v2f_position * sys_LightMatrix[2]);
	v2f_lightcoord[3] = (v2f_position * sys_LightMatrix[3]);
	
	gl_Position = v_screenPos;
}









/*
 //Fluxxor cloth
 
 uniform vec4 sys_Time;

void main ( void )
{
	vec4 v_localPos = gl_Vertex;
	
	v2f_normals		= sys_ModelRotationMatrix*vec4( gl_Normal, 1.0 );
	v2f_colors		= gl_Color;
	//v2f_emissive	= gl_SecondaryColor.rgb;
	v2f_position	= sys_ModelMatrix*gl_Vertex;
	v2f_texcoord0	= gl_MultiTexCoord0.xy;
	

	v_localPos.xyz += v2f_normals.xyz*0.075;
	v_localPos.xyz += vec3( sin( v2f_normals.x*2.2 ), cos( v2f_normals.y*3.2 ), sin( v2f_normals.z*1.7 ) ) * 0.025;
	v_localPos.xyz += vec3(
		sin( (v2f_normals.y*2.4+sys_Time.y+v_localPos.z*3.4)*1.4 )*0.3, 
		cos( (v2f_normals.z*2.4+sys_Time.y+v_localPos.x*3.4)*1.5 )*0.3, 
		sin( (v2f_normals.x*2.4+sys_Time.y+v_localPos.y*3.4)*1.6 ) ) * 0.05;
	
	vec4 v_screenPos = gl_ModelViewProjectionMatrix * v_localPos;
	v2f_fogdensity  = max( 0.0, (gl_Fog.end - v_screenPos.z) * gl_Fog.scale );
	
	gl_Position = v_screenPos;
}*/