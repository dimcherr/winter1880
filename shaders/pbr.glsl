@ctype mat4 glm::mat4
@ctype mat3 glm::mat3
@ctype vec2 glm::vec2
@ctype vec3 glm::vec3
@ctype vec4 glm::vec4

@vs pbr_vs
layout(binding=0) uniform pbr_vs_params {
    mat4 mvp;
    mat4 matModel;
    mat4 matNormal;
};

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexTexCoord;
layout (location = 3) in vec4 vertexTangent;

out vec3 fragPos;
out vec2 fragTexCoord;
out mat3 TBN;

void main() {
    fragPos = vec3(matModel * vec4(vertexPos, 1.0));
    fragTexCoord = vertexTexCoord;

    vec3 N = normalize(vec3(matNormal * vec4(vertexNormal, 0.0)));
    vec3 T = normalize(vec3(matNormal * vec4(vertexTangent.xyz, 0.0)));
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T) * vertexTangent.w;
    TBN = mat3(T, B, N);

    gl_Position = mvp * vec4(vertexPos, 1.0);
}
@end

@fs pbr_fs
const float PI = 3.14159265358979323846;
const int MAX_DIRECTIONAL_LIGHTS = 2;
const int MAX_SPOT_LIGHTS = 4;
const int MAX_POINT_LIGHTS = 30;

layout(binding=1) uniform pbr_fs_params {
    vec3 viewPos;

    vec3 ambientColor;
    vec3 tint;
    vec3 highlight;
    vec2 tiling;
    vec2 offset;
    float metallicFactor;
    float roughnessFactor;
    vec3 emissiveColor;
    float ambientFactor;
    float time;

    int pointLightCount;
    vec4 pointLightPos[MAX_POINT_LIGHTS];
    vec4 pointLightColor[MAX_POINT_LIGHTS];
};

layout(binding=0) uniform sampler smp;
layout(binding=0) uniform texture2D albedoMap;
layout(binding=1) uniform texture2D normalMap;
layout(binding=2) uniform texture2D ormMap;
layout(binding=3) uniform texture2D cloudTex;

in vec3 fragPos;
in vec2 fragTexCoord;
in mat3 TBN;

out vec4 finalColor;

vec3 SchlickFresnel(float hDotV, vec3 refl) {
    return refl + (1.0 - refl) * pow(1.0 - hDotV, 5.0);
}

float GgxDistribution(float nDotH, float roughness) {
    float a = roughness * roughness * roughness * roughness;
    float d = nDotH * nDotH * (a - 1.0) + 1.0;
    d = PI * d * d;
    return a / max(d, 0.0000001);
}

float GeomSmith(float nDotV, float nDotL, float roughness) {
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    float ik = 1.0 - k;
    float ggx1 = nDotV / (nDotV * ik + k);
    float ggx2 = nDotL / (nDotL * ik + k);
    return ggx1 * ggx2;
}

vec3 ComputePBR() {
    vec2 uv = vec2(fragTexCoord.x * tiling.x + offset.x, fragTexCoord.y * tiling.y + offset.y);
    vec3 albedo = texture(sampler2D(albedoMap, smp), uv).rgb;
    albedo = tint * albedo;

    vec4 orm = texture(sampler2D(ormMap, smp), uv);

    float ao = clamp(orm.r, 0.0, 1.0);
    float roughness = clamp(orm.g * roughnessFactor, 0.04, 1.0);
    float metallic = clamp(orm.b * metallicFactor, 0.04, 1.0);

    vec3 N = texture(sampler2D(normalMap, smp), uv).rgb;
    N = N * 2.0 - 1.0;
    N = normalize(TBN * N);

    vec3 V = normalize(viewPos - fragPos);

    // return N;//vec3(metallic,metallic,metallic);
    // If  dia-electric use base reflectivity of 0.04 otherwise ut is a metal use albedo as base reflectivity
    vec3 baseRefl = mix(vec3(0.04), albedo, metallic);
    vec3 lightAccum = vec3(0.0);  // Acumulate lighting lum

    for (int i = 0; i < pointLightCount; i++) {
        vec3 L = normalize(pointLightPos[i].xyz - fragPos);      // Compute light vector
        vec3 H = normalize(V + L);                                  // Compute halfway bisecting vector
        float dist = length(pointLightPos[i].xyz - fragPos);     // Compute distance to light

        float lx = max(abs(L.x), 0.0001);
        float lz = max(abs(L.z), 0.0001);
        float lr = atan(lz, lx);
        float lalpha = (lr + PI) / (2.0 * PI);
        float sampledLAlpha = texture(sampler2D(cloudTex, smp), vec2(lr * 0.1, time)).r;
        //sampledLAlpha = clamp(sampledLAlpha, 0.5, 1.0);

        //dist = max(1.f, dist);

        //dist = mix(0.f, dist, sampledLAlpha);
        float multip = sampledLAlpha + sin(lalpha + time) * 10.0;
        //multip = sin(lalpha) * 0.1;

        //dist = min(dist, 3.f);
        //float attenuation = (multip * 0.000001 + pointLightPos[i].a) / (dist * dist * dist);                   // Compute attenuation
        //float maxL = 7.f + sampledLAlpha * 2.0 + sin(time * 3.0) * abs(sin(lr + time * 10.0)) * 0.1 + multip * 0.0000001;
        float magic = sin(time * 3.0) * abs(sin(lr + sin(L.y) + time * 10.0)) * 0.3;
        float maxL = 2.f + magic + multip * 0.0000001;
        //dist = clamp(dist, 0.f, maxL);
        //attenuation = mix(100.f, 0.f, dist / maxL);

        float attenuation = pointLightPos[i].a / (dist * dist * 0.27);                   // Compute attenuation
        attenuation = pow(attenuation, maxL);
        attenuation = clamp(attenuation, 0.0, 1.0) * 10.0;
        //float attenuation = sampledLAlpha;
        //if (dist > 7.f) {
            //attenuation = 0.f;
        //}
        vec3 radiance = mix(vec3(0.9, 0.3, 0.0) * pointLightColor[i].a * attenuation, pointLightColor[i].rgb * pointLightColor[i].a * attenuation, clamp(attenuation, 0.0, 1.0)) * pointLightColor[i].rgb; // Compute input radiance, light energy comming in
        //float attenuation = 1.0 / (dist * dist * 0.23);                   // Compute attenuation
        //vec3 radiance = pointLightColor[i].rgb * 1.0 * attenuation; // Compute input radiance, light energy comming in

        // Cook-Torrance BRDF distribution function
        float nDotV = max(dot(N, V), 0.0000001);
        float nDotL = max(dot(N, L), 0.0000001);
        float hDotV = max(dot(H, V), 0.0);
        float nDotH = max(dot(N, H), 0.0);
        float D = GgxDistribution(nDotH, roughness);    // Larger the more micro-facets aligned to H
        float G = GeomSmith(nDotV, nDotL, roughness);   // Smaller the more micro-facets shadow
        vec3 F = SchlickFresnel(hDotV, baseRefl);       // Fresnel proportion of specular reflectance

        vec3 spec = (D * G * F) / (4.0 * nDotV * nDotL);
        
        // Difuse and spec light can't be above 1.0
        // kD = 1.0 - kS  diffuse component is equal 1.0 - spec comonent
        vec3 kD = vec3(1.0) - F;
        
        // Mult kD by the inverse of metallnes, only non-metals should have diffuse light
        kD *= 1.0 - metallic;
        lightAccum += (kD * albedo / PI + spec) * radiance * nDotL; // Angle of light has impact on result
    }
    
    vec3 ambientFinal = (ambientColor + albedo) * ambientFactor * 0.5;
    
    return ambientFinal + lightAccum * ao + emissiveColor * (1.f - orm.g);
    //return ambientFinal + lightAccum * ao;
    //return lightAccum * ao;
}

void main() {
    vec3 color = ComputePBR();

     // HDR tonemapping
    color = pow(color, color + vec3(1.0));
    
    // Gamma correction
    color = pow(color, vec3(1.0 / 2.2));

    finalColor = vec4(color, 1.0) + vec4(highlight, 1.0);
    //finalColor = vec4(color, 1.0);
    //vec2 uv = vec2(fragTexCoord.x * tiling.x + offset.x, fragTexCoord.y * tiling.y + offset.y);
    //vec3 albedo = texture(sampler2D(albedoMap, smp), uv).rgb;
    //vec3 N = texture(sampler2D(normalMap, smp), uv).rgb;
    //N = N * 2.0 - 1.0;
    //N = normalize(TBN * N);
    //vec3 V = normalize(viewPos - fragPos);
    //vec4 orm = texture(sampler2D(ormMap, smp), uv);

    //float ao = clamp(orm.r, 0.0, 1.0);
    //float roughness = clamp(orm.g * roughnessFactor, 0.04, 1.0);
    //float metallic = clamp(orm.b * metallicFactor, 0.04, 1.0);

    //finalColor = vec4(roughness, roughness, roughness, 1.0);
}

@end

@program pbr pbr_vs pbr_fs
