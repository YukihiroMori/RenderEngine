#ifndef INCLUDE_SMITH_BRDF
#define INCLUDE_SMITH_BRDF

#define PI 3.14159265358979323f
#define INV_PI 0.31830988618379067239521257108191f

float GGX(float NoV, float roughness){
	float k = roughness / 2;
	return NoV / (NoV * (1.0f - k) + k);
}

float geometryForLut(float roughness, float NoL){
	return GGX(NoL, roughness * roughness);
}

float GeometoryFacotr(float NoV, float roughness){
	return GGX(NoV , roughness);
}

float visibilityForLut(float roughness, float NoV)
{
    return GGX(NoV, roughness * roughness);
}

float fresnelForLut(float VoH){
	return pow(1.0 - VoH, 5.0);
}



vec2 sumLut(vec2 current, float G, float V, float F, float VoH, float NoL, float NoH, float NoV){
	G = G * V;
	float G_Vis = G * VoH / (NoH * NoV);
	current.x += (1.0 - F) * G_Vis;
	current.y += F * G_Vis;

	return current;
}

vec3 importanceSamplingGGX(vec2 Xi, float roughness, vec3 N){
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

	return vec3(TangentX * H.x , TangentY * H.y , N * H.z);
}

float specularD(float roughness, float NoH){
	float r2 = roughness * roughness;
	float NoH2 = NoH * NoH;
	float a = 1.0 / (PI * r2 * pow(NoH, 4));
	float b = exp((NoH2 - 1) / r2 * NoH2);
	return a * b;
}

vec4 sumSpecular(vec3 hdrPixel, float NoL, vec4 result){
	result.xyz += (hdrPixel * NoL);
	result.w += NoL;
	return result;
}

vec3 impotanceSampleDiffuse(vec2 Xi, vec3 N){
	float CosTheta = 1.0 - Xi.y;
	float SinTheta = sqrt(1.0 - CosTheta * CosTheta);
	float Phi = 2 * PI * Xi.x;

	vec3 H;
	H.x = SinTheta * cos(Phi);
	H.y = SinTheta * sin(Phi);
	H.z = CosTheta;

	vec3 UpVector = abs(N.z) < 0.999 ? vec3(0, 0, 1) : vec3(1, 0, 0);
	vec3 TangentX = normalize( cross( Upvector , N) );
	vec3 TangentY = cross( N , TangentX );

	return vec3(TangentX * H.x , TangentY * H.y , N * H.z);
}

vec4 sumDiffuse(vec3 diffuseSample, float NoV, vec4 result){
	result.xyz += diffuseSample;
	result.w++;
	return result;
}


#endif