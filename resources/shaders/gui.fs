#version 430

in vec2 vs_color;
in vec2 texture_coord;
out vec4 fragment_color;

uniform sampler2D icon_texture;
void main()
{
	//fragment_color = texture(icon_texture, texture_coord);
  	//fragment_color = vec4(vs_color, 1.0, 1.0);
	fragment_color = vec4(1.0, 1.0, 1.0, 1.0);
}
