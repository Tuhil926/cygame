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

float a(vec3 p, float phase){
    return sin(1./(1. - (p.x*p.x + p.y*p.y + tan(phase))));
}
float a1(vec3 p, float phase){
    return sin(1./(1. - abs(norm(p) + tan(phase/10.))));
}

vec3 b(vec3 p, float phase, float angle){
    p = p*(1 + sin(angle*100)/50.);
    return vec3(clamp(a1(p, phase), 0., 1.) + clamp(tan((angle - phase)), 0., 1.), a1(p, phase + 0.3), a1(p, phase + 0.6));
    // return vec3(tan(angle - phase), 0., 0.);
}
vec4 c1(vec3 p, float phase){
    float angle = atan(p.y/p.x);
    return clamp(vec4(b(p, phase, angle), 1.)/(norm(p*4.)), 0., 1.);
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
    // color = vec4(a(pos, 0.), a(pos, 2.*3.1415926/3.), a(pos, 4.*3.1415926/3.), 1.);
    // color = vec4(a(pos, 0. + timeSeconds), a(pos, timeSeconds + 3.1415926/3.), a(pos, timeSeconds + 2.*3.1415926/3.), 1.);
    // color = vec4(b(pos, -timeSeconds, angle), 1.);
    // color = vec4(b((pos + vec3(0.5, 0.5, 0.))/2., -timeSeconds*2., angle), 1.);
    color = c(pos , timeSeconds + 50.);
    color += c2(pos + vec3(1., 1., 0.), -timeSeconds);
    color += c2(pos - vec3(1., 1., 0.), -timeSeconds);
    color += c3(pos + vec3(-1., 1., 0.), -timeSeconds);
    color += c3(pos - vec3(-1., 1., 0.), -timeSeconds);

}
