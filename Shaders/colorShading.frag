#version 130

in vec2 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;

// fragment shader operates on each pixel in a polygon	

out vec4 color;

uniform float time;
uniform vec2 mousePosition;
uniform sampler2D playerTexture;

//layout(origin_upper_left, pixel_center_integer) in vec4 gl_FragCoord;

void main ()
{

	vec4 texture_color = texture(playerTexture, fragmentUV);

	color = vec4(fragmentColor.r * (cos(fragmentPosition.x * 2.0 + time) + 1.0) * 0.5,
				 fragmentColor.g * (cos(fragmentPosition.y * 4.0 + time) + 1.0) * 0.5,
				 fragmentColor.b * (cos(fragmentPosition.x + time * 0.5) + 1.0) * 0.5, fragmentColor.a) *texture_color;
}
