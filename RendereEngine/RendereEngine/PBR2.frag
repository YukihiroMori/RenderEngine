#version 420 core
#extension GL_ARB_explicit_attrib_location : enable


#include "utility.glsl"
#include "PBRUtility.glsl"

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 lightMatrix;

uniform sampler2D unit[6];
uniform sampler2D shadowMap;

uniform vec4 ambient;

uniform float roughness;
uniform float f0;

uniform uint NumSamples;

in vec2 tc;	

out vec4 fc;

vec3 importanceSampleGGX(vec2 Xi, float roughness, vec3 N){
	float a = roughness * roughness;

	float Phi = 2 * PI * Xi.x;
	float CosTheta = sqrt((1 - Xi.y) / (1 + (a*a - 1) * Xi.y));
	float SinTheta = sqrt(1 - CosTheta * CosTheta);

	vec3 H;
	H.x = SinTheta * cos(Phi);
	H.y = SinTheta * sin(Phi);
	H.z = CosTheta;

	vec3 UpVector = abs(N.z) < 0.999 ? vec3(0, 0, 1) : vec3(1, 0, 0);
	vec3 TangentX = normalize(cross(UpVector, N));
	vec3 TangentY = cross(N , TangentX);

	return vec3(TangentX * H.x + TangentY * H.y + N * H.z);
}


vec3 importanceSampleDiffuse(vec2 Xi, vec3 N){
	float CosTheta = sqrt(1.0 - Xi.y);
	float SinTheta = sqrt(1.0 - CosTheta * CosTheta);
	float Phi = 2 * PI * Xi.x;

	vec3 H;
	H.x = SinTheta * cos(Phi);
	H.y = SinTheta * sin(Phi);
	H.z = CosTheta;

	vec3 UpVector = abs(N.z) < 0.99999 ? vec3(0, 0, 1) : vec3(1, 0, 0);
	vec3 TangentX = normalize( cross( UpVector , N) );
	vec3 TangentY = cross( N , TangentX );

	return vec3(TangentX * H.x + TangentY * H.y + N * H.z);
}

// beckmann distribution function
float distribution(float NoH, float roughness){
	float a = roughness * roughness;
	float r1 = 1.0 / (4.0 * a * pow(NoH, 4.0));
	float r2 = (NoH * NoH - 1.0) / (a * NoH * NoH);
	return r1 * exp(r2);
 }

// geometric attenuation
float geometryCookTorrance(float NoH, float NoL, float NoV, float VoH){
	float NH2 = 2.0 * NoH;
	float g1 = (NH2 * NoV) / VoH;
	float g2 = (NH2 * NoL) / VoH;
	return min(1.0, min(g1, g2));
}

// Schlick approximation
float fresnel(float f0, float VoH){
	return f0 + (1.0 - f0) * pow(1.0 - VoH, 5.0);
}

float visibility(float NoL , float NoV){
	return 1.0 / PI * NoL * NoV;
}

vec3 brdfCookTorrance(vec3 N, vec3 V, float roughness){
	vec3 resultColor = vec3(0.0f);

	float k = 0.2;

	for(uint i = 0; i < NumSamples; i ++){
		vec2 Xi = hammersley(i , NumSamples);
		vec3 H = importanceSampleGGX(Xi, roughness, N);
		vec3 L = reflect(-V, H);

		float NoL = saturate(dot(N, L));

		if(NoL > 0.0){
			float NoV = saturate(dot(N, V));
			float NoH = saturate(dot(N, H));
			float VoH = saturate(dot(V , H));

			float D = distribution(NoH, roughness);
			float F = fresnel(f0, VoH);
			float G = geometryCookTorrance(NoH, NoL, NoV, VoH);
			float Vis = visibility(NoL, NoV);

			float brdfFactor = D * F * G * Vis;

			if(isnan(brdfFactor)){
				return resultColor;
			}	

			resultColor += textureLod(unit[5],  panorama(L) , 3).rgb * NoL * (k + brdfFactor * (1.0 - k));
		}
	}

	resultColor /= float(NumSamples);

	return resultColor;
}

vec3 brdfLambert(vec3 N, vec3 V){
	vec3 resultColor = vec3(0.0f);

	for(uint i = 0; i < NumSamples; i ++){
		vec2 Xi = hammersley(i , NumSamples);
		vec3 H = importanceSampleDiffuse(Xi , N);
		vec3 L = reflect(-V, H);

		float NoL = saturate(dot(N, L));
		float NoV = saturate(dot(N, V));
		float NoH = saturate(dot(N, H));

		if(NoH > 0.0){
			resultColor += texture(unit[0] , tc) * textureLod(unit[5], panorama(H) , 5).rgb;
		}
	}

	resultColor *= 1.0 / float(NumSamples);

	return resultColor;
}



void main(void){
	vec4 albedo = texture(unit[0] , tc);
	vec4 fresnel = texture(unit[1] , tc);
	vec4 position = texture(unit[2] , tc);
	vec3 normal = texture(unit[3] , tc).xyz;
	vec3 view = -normalize(position.xyz);

	fc.xyz =  brdfLambert(normal, view);
	fc.xyz += brdfCookTorrance(normal, view, roughness);

	fc *= Exposure;
	
	fc = mix(ambient, fc , albedo.a);

}