#version 420 core
#extension GL_ARB_explicit_attrib_location : enable


//
// pass1.vert
//

// 変換行列
uniform mat4 ModelMatrix;                                             // モデルマトリックス
uniform mat4 ViewMatrix;                                              // ビューマトリックス
uniform mat4 ProjectionMatrix;                                        // プロジェクションマトリックス

uniform mat4 lightMatrix;

// 頂点属性
layout (location = 0) in vec3 VertexPosition;                                     // ローカル座標系の頂点位置
layout (location = 1) in vec3 VertexNormal;                                     // 頂点の法線ベクトル

// ラスタライザに送る頂点属性
out vec4 p;                                                           // 物体表面上の位置 P
out vec3 n;                                                           // 物体表面上の法線ベクトル N
out vec4 shadowMapCoord;

void main(void)
{
	p = ViewMatrix * ModelMatrix * vec4(VertexPosition, 1.0);	 // フラグメントの位置
	gl_Position = ProjectionMatrix * p;                                              // 頂点位置

	mat3 MVI = transpose(inverse(mat3(ViewMatrix * ModelMatrix)));													  
	n = MVI * VertexNormal;                                       // フラグメントの法線ベクトル N

	shadowMapCoord = lightMatrix * ModelMatrix * vec4(VertexPosition, 1.0) * 0.5 + 0.5;
}
