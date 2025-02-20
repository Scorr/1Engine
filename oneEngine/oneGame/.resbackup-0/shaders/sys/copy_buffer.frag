// sys/copy_buffer
// Used to copy a buffer from one buffer to another if blit cannot be used.
// There is no projection calculations that occur. Input vertex coordinates are the device's coordinates.
// If you muck with this shader, you might as well release the cross-platform yandere phone virus.
#version 330

// Inputs from vertex shader
in vec4 v2f_position;
in vec2 v2f_texcoord0;

// Samplers
uniform sampler2D textureSampler0;

uniform vec4 sys_SinTime;

void main ( void )  
{
	vec4 diffuseColor = texture( textureSampler0, v2f_texcoord0 );
	gl_FragColor = diffuseColor;
}