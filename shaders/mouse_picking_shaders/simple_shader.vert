#version 450

//! @note Position and Color used in the Vertex class.
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Color;
layout(location = 2) in vec3 Normals;
layout(location = 3) in vec2 Uv;


layout(location = 0) out vec3 fragColor;

// we set our descriptor set to binding = 0
// meaning that our descriptor set references to GlobalUbo
// layout(set = 0, binding = 0) uniform GlobalLightUbo {
//     mat4 Model; // light model
//     vec3 Direction; // light dir
// } light_ubo;

layout(set = 0, binding = 0) uniform GlobalCameraUbo{
    mat4 Projection;
    mat4 View;
    mat4 Model;
    vec3 Lightatlas::Transform;
    // vec4 Color;
} camera_ubo;

layout(push_constant) uniform Push {
    // mat4 atlas::Transform; // proj * view
    mat4 Projection;
    mat4 View;
    mat4 Model;
    vec3 Lightatlas::Transform;
    vec4 Color;
} push;

// layout(push_constant) uniform Properties {
//     vec4 Color;
// } properties;

// const vec3 dir_to_light = normalize(vec3(1.0, -3.0, -1.0));
// vec3 light_atlas::Transform = vec3(1.0, -3.0, -1.0);
// const vec3 dir_to_light = normalize(vec3(1.0, -3.0, -1.0));
// vec3 dir_to_light = normalize(push.Lightatlas::Transform);
// const vec3 dir_to_light = normalize();

/*
vec3 dir_to_light = normalize(camera_ubo.Lightatlas::Transform);
const float AMBIENT = 0.2;
void main(){
    gl_Position = (camera_ubo.Projection * camera_ubo.View * camera_ubo.Model) * vec4(Position, 1.0);

    vec3 normalize_world_space = normalize(mat3(camera_ubo.Model) * Normals);
    float LightIntensity = AMBIENT + max(dot(normalize_world_space, dir_to_light), 0);

    // fragColor = LightIntensity * Color;
    // fragColor = LightIntensity * push.Color.xyz;
    fragColor = LightIntensity * Color;
}
*/



vec3 dir_to_light = normalize(push.Lightatlas::Transform);
const float AMBIENT = 0.2;
void main(){
    gl_Position = (push.Projection * push.View * push.Model) * vec4(Position, 1.0);

    vec3 normalize_world_space = normalize(mat3(push.Model) * Normals);
    float LightIntensity = AMBIENT + max(dot(normalize_world_space, dir_to_light), 0);

    // fragColor = LightIntensity * Color;
    fragColor = LightIntensity * Color * push.Color.xyz;
}
