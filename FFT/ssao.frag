R"zzz(#version 410 core
layout(location = 0) out vec4 fragment_color;
in vec2 tex_coord;
uniform sampler2D depSten;
uniform sampler2D vs_Ray;
uniform sampler2D noise;

uniform float aspect;
uniform mat4 projection;
uniform float zNear;
uniform float zFar;

vec4 toTextureSpace(vec4 r){
	r.x = (r.x + 1.0) / 2.0;
	//Aspect ration correction
	r.y = ((r.y + 1.0) / 2.0) * aspect;
	r.z = (r.z + 1.0) / 2.0;
	return r;
}

float linearize_depth(float d)
{
    return zNear * zFar / (zFar + d * (zNear - zFar));
}

void main() {
	float ao = 0.0;
	vec4 start = vec4(texture(vs_Ray, tex_coord).xyz, 1.0);

	for(float i = 0; i < 12; i++){
		for(float j = 0; j < 12; j++){
			vec4 check = start + vec4(texture(noise,vec2(i / 12.0,j /12.0)).xyz, 0.0) / (i/4.0 + 1.0);
			check = projection * check;
			check = toTextureSpace(check / check.w);
			fragment_color = check;
			if(check.z <= texture(depSten, vec2(check.x,check.y)).x 
				|| abs(linearize_depth(check.z) - linearize_depth(texture(depSten, vec2(check.x,check.y)).x)) > 3.0) ao += 1.0/72.0;
		}
	}
	//Some minor readjustments to reduce the noise
	if(ao > 0.8) ao = 1.0;
	ao = min(ao, 1.0);

	fragment_color = vec4(ao,ao,ao,1.0);
}
)zzz"
