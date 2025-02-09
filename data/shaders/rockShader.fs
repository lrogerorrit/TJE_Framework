varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;

uniform vec4 u_color;
uniform sampler2D u_texture;
uniform float u_time;

void main()
{
	vec2 uv = v_uv;
	vec3 N= normalize(v_normal);
	gl_FragColor = u_color * texture2D( u_texture, uv )*max(dot(vec3(0,1,0),N),0.1);
}
