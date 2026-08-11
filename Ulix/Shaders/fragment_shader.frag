#version 450

layout(location = 0) in vec2 fragment_position;
layout(location = 1) in vec4 fragment_color;
layout(location = 2) in float fragment_corner_radius;
layout(location = 3) in vec2 fragment_half_size;
layout(location = 4) in float fragment_border_width;
layout(location = 5) in vec4 fragment_border_color;
layout(location = 6) in vec2 fragment_center;

layout(location = 0) out vec4 out_color;


float rounded_box_sdf(vec2 position, vec2 bounds, float radius) {
    vec2 delta = abs(position) - bounds + radius;
    return length(max(delta, 0.0)) + min(max(delta.x, delta.y), 0.0) - radius;
}

void main() {
    vec2 local_position = fragment_position - fragment_center;
    float dist = rounded_box_sdf(local_position, fragment_half_size, fragment_corner_radius);
    float fw = fwidth(dist);

    float inner = dist + fragment_border_width;
    float fill_alpha = 1.0 - smoothstep(-fw, fw, dist);
    float border_alpha = smoothstep(-fw, fw, inner) - smoothstep(-fw, fw, dist);
    float border_color_alpha = fragment_border_color.a * border_alpha;

    vec3 color = mix(fragment_color.rgb, fragment_border_color.rgb, border_color_alpha);
    out_color = vec4(color, fragment_color.a * clamp(fill_alpha + border_alpha, 0.0, 1.0));
}
