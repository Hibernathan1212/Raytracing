//
//  UniformData.hpp
//  Raytracing
//
//  Created by Nathan Thurber on 13/11/22.
//

#pragma once

struct u_CameraData {
    alignas(16) glm::vec3 CameraOrigin = glm::vec3(0.0f, 0.0f, 0.0f);
    alignas(4) float ViewportWidth = 1600;
    alignas(4) float ViewportHeight = 900;
    alignas(16) glm::mat4 Projection{ 1.0f };
    alignas(16) glm::mat4 View{ 1.0f };
    alignas(16) glm::mat4 InverseProjection{ 1.0f };
    alignas(16) glm::mat4 InverseView{ 1.0f };
};

struct u_Material
{
    alignas(16) glm::vec3 Albedo{1.0f};
    alignas(4)  float Roughness = 1.0f;
    alignas(16) glm::vec3 Emission = glm::vec3(0.0f, 0.0f, 0.0f) * 0.0f; //Emission color * Emission power
    alignas(4)  float Restitution = 1.0f; //from 0 to 1, 1 being no energy loss in collision and 0 being all energy lost

};

//struct u_Sphere
//{
//    glm::vec3 Position{0.0f};
//    float Radius = 0.0f;
//
//    u_Material material;
//};
//
//struct u_Triangle
//{
//    alignas(16) glm::vec3 A{0.0f};
//    alignas(16) glm::vec3 B{0.0f};
//    alignas(16) glm::vec3 C{0.0f};
//
//    u_Material material;
//};

struct u_Object
{
    alignas(16) glm::vec4 Vertices[128];
    alignas(16) glm::ivec4 Indices[126];
    u_Material material;
    uint32_t numberOfIndices;
};

struct UniformData {
    u_CameraData cameraData;
    u_Object Objects[10];
    uint32_t numberOfObjects;
};
