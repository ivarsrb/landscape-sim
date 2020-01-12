#version 450
// Per-ciew uniform buffer
layout(set = 0, binding = 1) uniform UniformBufferEnvironemnt {
	// Directional light
	vec3 sunlight_direction; 
	vec3 sunlight_color; 
} u_e;
// In
layout(location = 0) in vec3 i_color;
layout(location = 1) in vec3 i_normal;
// Out
layout(location = 0) out vec4 out_color;

// Calculates and return simple lighting given normalized vectors and colors.
// NOTE: Unlit parts are not shaded in this model.
// C_shaded = F_unlit(n, v) + clamp(dot(l,n)) * C_light * C_surface
vec3 LambertianShading(in vec3 surface_color, in vec3 surface_normal,
						in vec3 light_color, in vec3 vector_to_light) {
	// OPTI: instead of max try clamp(n, 0, 1), it should be very fast
	return max(dot(surface_normal, vector_to_light), 0.0) * light_color * surface_color;
}

void main() {
	const vec3 reverse_light_direction = -u_e.sunlight_direction; 
	// Renormalize normal because this normal is interpolated normal from vertex shader
	// and interpolated normal is not guaranteed to be of a unit length.
	const vec3 normal = normalize(i_normal);
	const vec3 shaded_color = LambertianShading(i_color, normal, u_e.sunlight_color, reverse_light_direction);
	out_color = vec4(shaded_color, 1.0);
}