#version 330

// Inputs
in vec4 v_color;
in vec2 v_texCoord;

// Output
layout(location = 0) out vec4 FragColor;

// Globals
uniform sampler2D u_tex0;

layout(std140) uniform fs_ub {
    float u_time;
    float u_amplitude; // 0.0 - 1.0
} _10;

// Pseudo random function
float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

void main() {
    vec2 uv = v_texCoord;

    if (_10.u_amplitude <= 0.0) {
        FragColor = texture(u_tex0, uv) * v_color;
        return;
    }

    // Jitter
    float sliceY = floor(uv.y * 50.0);
    float sliceNoise = random(vec2(sliceY, floor(_10.u_time * 15.0)));

    if (sliceNoise < 0.3 * _10.u_amplitude) {
        float offset = (random(vec2(_10.u_time, sliceY)) - 0.5) * 0.08 * _10.u_amplitude;
        uv.x += offset;
    }

    // RGB Split
    float splitAmount = 0.025 * _10.u_amplitude;
    float r = texture(u_tex0, vec2(uv.x + splitAmount, uv.y)).r;
    float g = texture(u_tex0, uv).g;
    float b = texture(u_tex0, vec2(uv.x + splitAmount, uv.y)).b;
    float a = texture(u_tex0, uv).a;

    vec4 color = vec4(r, g, b, a);

    // Scanlines
    float scanline = sin(uv.y * 800.0 + _10.u_time * 10.0) * 0.25 * _10.u_amplitude;
    color.rgb -= scanline;

    // Noise
    float noise = (random(uv + _10.u_time) - 0.5) * 0.75 * _10.u_amplitude;
    color.rgb += noise;

    FragColor = color * v_color;
}