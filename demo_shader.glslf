#version 410 core
in vec3 theColor;
in vec3 theNormal;
in vec3 pos;
in vec2 textureCoord;
out vec4 color;
uniform vec3 lightDirection;
uniform vec3 cameraLocation;
uniform sampler2D text;
uniform int isTexture;
uniform float specMultiplier;
uniform float timeSeconds;
float norm(vec3 c){
    return sqrt(c.x*c.x + c.y*c.y);
}
float a1(vec3 p, float phase){
    return sin(1./(1. - abs(norm(p) + tan(phase/10.))));
}
vec3 b(vec3 p, float phase, float angle){
    p = p*(1 + sin(angle*100)/50.);
    return vec3(clamp(a1(p, phase), 0., 1.) + clamp(tan((angle - phase)), 0., 1.), a1(p, phase + 0.3), a1(p, phase + 0.6));
}
vec4 c(vec3 p, float phase){
    float angle = atan(p.y/p.x);
    vec3 p1 = p*(1 + sin(angle*100)/50.);
    vec3 b_ret = vec3(a1(p1, phase), a1(p1, phase + 0.1), a1(p, phase + 0.2));
    float val = clamp(.1/tan(phase - angle), 0., 1.);
    return clamp(vec4(b_ret, 1.)/(norm(p*4.)) + vec4(val, val*val/2., val*val*val/2., 1.), 0., 1.);
}
vec4 c2(vec3 p, float phase){
    float angle = -atan(p.y/p.x);
    vec3 p1 = p*(1 + sin(angle*100)/50.);
    vec3 b_ret = vec3(a1(p1, phase), a1(p1, phase + 0.1), a1(p, phase + 0.2));
    float val = clamp(.1/tan(angle - phase), 0., 1.);
    return clamp(vec4(b_ret, 1.)/(norm(p*4.)) + vec4(val, val*val/2., val*val*val/2., 1.), 0., 1.);
}
vec4 c3(vec3 p, float phase){
    float angle = atan(p.y/p.x);
    vec3 p1 = p*(1 + sin(angle*100)/50.);
    vec3 b_ret = vec3(a1(p1, phase), a1(p1, phase + 0.1), a1(p, phase + 0.2));
    float val = clamp(.1/tan(angle - phase), 0., 1.);
    return clamp(vec4(b_ret, 1.)/(norm(p*4.)) + vec4(val, val*val/2., val*val*val/2., 1.), 0., 1.);
}
void main() {
    vec3 pos1 = pos*2.;
    color = c(pos1 , timeSeconds + 50.);
    color += c2(pos1 + vec3(1., 1., 0.), -timeSeconds);
    color += c2(pos1 - vec3(1., 1., 0.), -timeSeconds);
    color += c3(pos1 + vec3(-1., 1., 0.), -timeSeconds);
    color += c3(pos1 - vec3(-1., 1., 0.), -timeSeconds);
}
