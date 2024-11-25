//
//  Scene.hpp
//  Raytracing
//
//  Created by Nathan Thurber on 26/2/23.
//

#pragma once

#include "glm.hpp"
#include <vector>

#include "UniformData.hpp"

#include <iostream>
#include "string_cast.hpp"

#include <algorithm>

struct Sphere
{
    glm::vec3 Position{0.0f};
    glm::vec3 Velocity{0.0f};
    //glm::vec3 PreviousPosition{0.0f};
    glm::vec3 Acceleration{0.0f};
    float Mass = 1.0f;
    
    void updatePosition(float dt)
    {
        Velocity = Velocity + Acceleration * dt;
        Position = Position + Velocity * dt;
        
        //glm::vec3 velocity = (Position - PreviousPosition) / dt;
        //PreviousPosition = Position;
        //velocity = velocity + Acceleration * dt;
        //Position = Position + velocity * dt;
        
        Acceleration = {};
    }
    
    void accelerate(glm::vec3 acc)
    {
        Acceleration += acc;
    }
    
    float Radius = 0.0f;

    u_Material material;
};

struct Triangle
{
    glm::vec3 A{0.0f};
    glm::vec3 B{0.0f};
    glm::vec3 C{0.0f};

    glm::vec3 Velocity{0.0f};
    //glm::vec3 PreviousPosition{0.0f};
    glm::vec3 Acceleration{0.0f};
    float Mass = 1.0f;
    
    void updatePosition(float dt)
    {
        //Velocity = Velocity + Acceleration * dt;
        //Position = Position + Velocity * dt;
        
        //glm::vec3 velocity = (Position - PreviousPosition) / dt;
        //PreviousPosition = Position;
        //velocity = velocity + Acceleration * dt;
        //Position = Position + velocity * dt;
        
        Acceleration = {};
    }
    
    void accelerate(glm::vec3 acc)
    {
        Acceleration += acc;
    }
    
};

struct Object
{
    glm::vec3 Velocity{0.0f};
    glm::vec3 Acceleration{0.0f};
    glm::vec3 angularVelocity{0.0f};
    glm::vec3 angularAcceleration{0.0f};
    
    void updatePosition(float dt)
    {
        Velocity = Velocity + Acceleration * dt;
        for (glm::vec3& Vertex : Vertices)
            Vertex = Vertex + Velocity * dt;
        
        CenterOfMass = CenterOfMass + Velocity * dt;
        
        Acceleration = {};
    }
    
    void updateRotations(float dt)
    {
        angularVelocity = angularVelocity + angularAcceleration * dt;
        
        glm::vec3 a = angularVelocity * dt; //angle
        
        glm::mat3 rotationMatrix = {
            glm::cos(a.z)*glm::cos(a.y),    glm::cos(a.z)*glm::sin(a.y)*glm::sin(-a.x) - glm::sin(a.z)*glm::cos(-a.x),    glm::cos(a.z)*glm::sin(a.y)*glm::cos(-a.x) + glm::sin(a.z)*glm::sin(-a.x),
            glm::sin(a.z)*glm::cos(a.y),    glm::sin(a.z)*glm::sin(a.y)*glm::sin(-a.x) + glm::cos(a.z)*glm::cos(-a.x),    glm::sin(a.z)*glm::sin(a.y)*glm::cos(-a.x) - glm::cos(a.z)*glm::sin(-a.x),
            -glm::sin(a.y),                 glm::cos(a.y)*glm::sin(-a.x),                                                glm::cos(a.y)*glm::cos(-a.x)
        };
        
        
        for (glm::vec3& Vertex : Vertices)
            Vertex = rotationMatrix * (Vertex - CenterOfMass) + CenterOfMass;
        
        angularAcceleration = {};
    }
    
    void accelerate(glm::vec3 acc)
    {
        Acceleration += acc;
    }
    
    glm::vec3 getSupportPoint(glm::vec3 direction)
    {
        glm::vec3 supportPoint;
        float maxDot = -MAXFLOAT;
        for (glm::vec3 Vertex : Vertices)
        {
            float dot = glm::dot(direction, Vertex - CenterOfMass);
            if (dot > maxDot)
            {
                maxDot = dot;
                supportPoint = Vertex;
            }
        }
        return supportPoint;
    }
    
    std::vector<glm::vec3> getSupportPoints(glm::vec3 direction)
    {
        std::vector<glm::vec3> supportPoint;
        float dot1 = -MAXFLOAT;
        float dot2 = -MAXFLOAT;
        float dot3 = -MAXFLOAT;
        glm::vec3 supportPoint1{0.0f};
        glm::vec3 supportPoint2{0.0f};
        glm::vec3 supportPoint3{0.0f};
        
        for (glm::vec3 Vertex : Vertices)
        {
            float dot = glm::dot(direction, Vertex - CenterOfMass);
            if (dot > dot1)
            {
                dot1 = dot;
                supportPoint3 = supportPoint2;
                supportPoint2 = supportPoint1;
                supportPoint1 = Vertex;
            }
            else if (dot > dot2)
            {
                dot2 = dot;
                supportPoint3 = supportPoint2;
                supportPoint2 = Vertex;
            }
            else if (dot > dot3)
            {
                dot3 = dot;
                supportPoint3 = Vertex;
            }
        }
        supportPoint.push_back(supportPoint1);
        supportPoint.push_back(supportPoint2);
        supportPoint.push_back(supportPoint3);

        return supportPoint;
    }
    
    glm::vec3 getAverageSupportPoint(glm::vec3 direction)
    {
        glm::vec3 avgPoint{0.0f};
        std::vector<glm::vec3> avg;
        float maxDot = -MAXFLOAT;
        for (glm::vec3 Vertex : Vertices)
        {
            float dot = glm::dot(direction, Vertex - CenterOfMass);
            if (dot > maxDot)
                maxDot = dot;
        }
        
        for (glm::vec3 Vertex : Vertices)
        {
            float dot = glm::dot(direction, Vertex - CenterOfMass);
            if (dot == maxDot)
            {
                avg.push_back(Vertex);
            }
        }
        
        for (glm::vec3 Vertex : avg)
        {
            avgPoint.x = avgPoint.x + Vertex.x;
            avgPoint.y = avgPoint.y + Vertex.y;
            avgPoint.z = avgPoint.z + Vertex.z;
        }
        
        avgPoint.x = avgPoint.x / avg.size();
        avgPoint.y = avgPoint.y / avg.size();
        avgPoint.z = avgPoint.z / avg.size();

        //std::cout << glm::to_string(avgPoint) << std::endl;
        
        return avgPoint;
    }
    
    float getMaxDot(glm::vec3 direction)
    {
        float maxDot = -MAXFLOAT;
        for (glm::vec3 Vertex : Vertices)
        {
            float dot = glm::dot(direction, Vertex - CenterOfMass);
            if (dot > maxDot)
                maxDot = dot;
        }
        return maxDot;
    }
    
    float Mass = 1.0f;
    glm::vec3 CenterOfMass{0.0f};
    float boundingRadius = 1.0f/0.0f; //distance to furthest vertex for optimization with collisions
    glm::mat3 inertiaTensor; /*
                            [inertia in x axis, 0.0f, 0.0f,
                            0.0f, inertia in y axis, 0.0f,
                            0.0f, 0.0f, inertia in z axis]
                            */
    
    
    std::vector<glm::vec3> Vertices;
    std::vector<glm::ivec3> Indices;
    u_Material material;
};

class Scene
{
public:
    //Scene(std::vector<Sphere> spheres);
    
    //void AddSphere(Sphere sphere);
    //void RemoveSphere(int objectIndex);
    
    //void AddTriangle(Triangle triangle);
    
    void AddObject(Object object);
    //void RemoveObject(int objectIndex);
    
    const glm::vec3 GetSpherePosition(int objectIndex) const { return Spheres[objectIndex].Position; }
    const float GetSphereRadius(int objectIndex) const { return Spheres[objectIndex].Radius; }
    //const u_Material GetSphereMaterial(int objectIndex) const { return Spheres[objectIndex].material; }
    const uint32_t GetSphereCount() const { return Spheres.size(); }
    //
    //const glm::vec3 GetTriangleCornerA(int objectIndex, int triangleIndex) const { return Triangles[objectIndex].A; }
    //const glm::vec3 GetTriangleCornerB(int objectIndex) const { return Triangles[objectIndex].B; }
    //const glm::vec3 GetTriangleCornerC(int objectIndex) const { return Triangles[objectIndex].C; }
    //const u_Material GetTriangleMaterial(int objectIndex) const { return Triangles[objectIndex].material; }
    //const uint32_t GetTriangleCount() const { return Triangles.size(); }
    
    const uint32_t GetObjectCount() const { return Objects.size(); }
    const uint32_t GetObjectVertexCount(int objectIndex) const { return Objects[objectIndex].Vertices.size(); }
    const uint32_t GetObjectIndexCount(int objectIndex) const { return Objects[objectIndex].Indices.size(); }
    const u_Material GetObjectMaterial(int objectIndex) const { return Objects[objectIndex].material; }
    const glm::vec3 GetObjectVertex(int objectIndex, int vertexIndex) const { return Objects[objectIndex].Vertices[vertexIndex]; }
    const glm::vec3 GetObjectindex(int objectIndex, int triangleIndex) const { return Objects[objectIndex].Indices[triangleIndex]; }

    
    void OnUpdate(float dt);
    
private:
    std::vector<Sphere> Spheres;
    
    std::vector<Triangle> Triangles;
    
    std::vector<Object> Objects;
    
    glm::vec3 gravity = {0.0f, 0.0f, 0.0f};
    
    void updatePositions(float dt);
    void applyGravity();
    void applyConstraint();
    void solveCollisions();
};
