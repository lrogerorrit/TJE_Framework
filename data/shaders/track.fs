
varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;

uniform vec4 u_color;


uniform sampler2D u_rail_texture;
uniform sampler2D u_tie_texture;

uniform bool isTie;

void main()
{
	vec2 uv = v_uv;
	gl_FragColor = u_color * texture2D( isTie?u_tie_texture:u_rail_texture, uv );
}
