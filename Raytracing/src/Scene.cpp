//
//  Scene.cpp
//  Raytracing
//
//  Created by Nathan Thurber on 26/2/23.
//

#include "Scene.hpp"
#include <iostream>

void Scene::AddObject(Object object)
{
    Objects.push_back(object);
}

//void Scene::RemoveObject(int objectIndex)
//{
//    Objects.erase(Objects.begin() + objectIndex);
//}

void Scene::OnUpdate(float dt)
{
    const int substeps = 1;
    
    float sub_dt = dt / substeps;
    
    for (int i = 0; i < substeps; i++)
    {
        applyGravity();
        //applyConstraint();
        solveCollisions();
        updatePositions(sub_dt);
    }
}

void Scene::updatePositions(float dt)
{
    for (Object& object : Objects)
    {
        object.updatePosition(dt);
        object.updateRotations(dt);
    }
}

void Scene::applyGravity()
{
    for (Object& object : Objects)
    {
        object.accelerate(gravity);
    }
}

//void Scene::applyConstraint()
//{
//    const glm::vec3 position {0.0f, 0.0f, 0.0f};
//    const float radius = 15.0f;
//
//    for (Sphere& sphere : Spheres)
//    {
//        const glm::vec3 to_sphere = sphere.Position - position;
//        float distance = glm::length(to_sphere);
//        if (distance > (radius - sphere.Radius))
//        {
//            const glm::vec3 n = to_sphere / distance;
//            sphere.Position = position + (n * (radius - sphere.Radius));
//            sphere.Velocity = 0.99999999f * (sphere.Velocity - n * glm::dot(n, sphere.Velocity));
//        }
//    }
//}

void Scene::solveCollisions()
{
    for (int i = 0; i < GetObjectCount() - 1; i++)
    {
        for (int j = (i+1); j < GetObjectCount(); j++)
        {
            Object& Object1 = Objects[i];
            Object& Object2 = Objects[j];
            
            glm::vec3 direction = Object2.CenterOfMass - Object1.CenterOfMass;
            
            bool Collided = 0;
            
            if (glm::length(direction) > Object1.boundingRadius + Object2.boundingRadius)
                continue;
            
            if (glm::length(direction) == 0)
                Collided = true;
            
            //std::vector<glm::vec3> PointsA;
            //std::vector<glm::vec3> simplex;
            //
            //PointsA = Object1.getSupportPoints(direction);
            //simplex = {PointsA[0] - Object2.getSupportPoint(-(PointsA[0] - Object1.CenterOfMass)), PointsA[1] - Object2.getSupportPoint(-(PointsA[1] - Object1.CenterOfMass)), PointsA[2] - //Object2.getSupportPoint(-(PointsA[2] - Object1.CenterOfMass))};
            //
            //
            //simplex.push_back(Object1.getSupportPoint(-direction) - Object2.getSupportPoint(direction));
            //
            ////std::cout << glm::to_string(PointsA[0]) <<std::endl;
            ////std::cout << glm::to_string(PointsA[1]) <<std::endl;
            ////std::cout << glm::to_string(PointsA[2]) <<std::endl;
            ////std::cout << glm::to_string(Object1.getAverageSupportPoint(-direction)) << std::endl;
            ////std::cout << glm::to_string(Object2.getSupportPoint(-(PointsA[0] - Object1.CenterOfMass))) << std::endl;
            ////std::cout << glm::to_string(Object2.getSupportPoint(-(PointsA[1] - Object1.CenterOfMass))) << std::endl;
            ////std::cout << glm::to_string(Object2.getSupportPoint(-(PointsA[2] - Object1.CenterOfMass))) << std::endl;
            ////std::cout << glm::to_string(Object2.getAverageSupportPoint(direction)) << std::endl;
            ////std::cout << glm::to_string(simplex[0]) << std::endl;
            ////std::cout << glm::to_string(simplex[1]) << std::endl;
            ////std::cout << glm::to_string(simplex[2]) << std::endl;
            ////std::cout << glm::to_string(simplex[3]) << std::endl;
            ////std::cout << "-------------------------"<< std::endl;
            //
            //glm::vec3 AO = glm::vec3{0.0f} - simplex[0];
            //glm::vec3 AB = simplex[1] - simplex[0];
            //glm::vec3 AC = simplex[2] - simplex[0];
            //
            //
            //glm::vec3 DO = glm::vec3{0.0f} - simplex[3];
            //glm::vec3 AD = simplex[3] - simplex[0];
            //glm::vec3 DB = simplex[1] - simplex[3];
            //glm::vec3 simplexCenter = (simplex[0] + simplex[1] + simplex[2] + simplex[3])/4.0f;
            //
            //glm::vec3 BADperp = glm::cross(AB, AD);
            //
            //if (glm::dot(BADperp, simplex[0] - simplexCenter) < 0)
            //    BADperp = -BADperp;
            //
            //glm::vec3 CADperp = glm::cross(AD, AC);
            //
            //if (glm::dot(CADperp, simplex[0] - simplexCenter) < 0)
            //    CADperp = -CADperp;
            //
            //glm::vec3 BDCperp = glm::cross(DB, DB);
            //
            //if (glm::dot(BDCperp, simplex[3] - simplexCenter) < 0)
            //    BDCperp = -BDCperp;
            //
            //glm::vec3 CABperp = glm::cross(AC, AB);
            //
            //if (glm::dot(CABperp, simplex[0] - simplexCenter) < 0)
            //    CABperp = -CABperp;
            //
            //if ( (glm::dot(CABperp, AO) <= 0) && (glm::dot(BDCperp, DO) <= 0) && (glm::dot(CADperp, AO) <= 0) && (glm::dot(BADperp, AO) <= 0))
            //{
            //    Collided = true;
            //    simplex[0] = -simplex[0];
            //    simplex[1] = -simplex[1];
            //    simplex[2] = -simplex[2];
            //    simplex[3] = -simplex[3];
            //}
            //
            //if (!Collided)
            //{
            //    glm::vec3 direction = Object1.CenterOfMass - Object2.CenterOfMass;
            //
            //    //std::vector<glm::vec3> PointsA;
            //    //std::vector<glm::vec3> simplex;
            //
            //    PointsA = Object2.getSupportPoints(direction);
            //    simplex = {PointsA[0] - Object1.getSupportPoint(-(PointsA[0] - Object2.CenterOfMass)), PointsA[1] - Object1.getSupportPoint(-(PointsA[1] - Object2.CenterOfMass)), PointsA[2] - //Object1.getSupportPoint(-(PointsA[2] - Object2.CenterOfMass))};
            //    simplex.push_back(Object2.getAverageSupportPoint(-direction) - Object1.getAverageSupportPoint(direction));
            //
            //    glm::vec3 AO = glm::vec3{0.0f} - simplex[0];
            //    glm::vec3 AB = simplex[1] - simplex[0];
            //    glm::vec3 AC = simplex[2] - simplex[0];
            //
            //    glm::vec3 DO = glm::vec3{0.0f} - simplex[3];
            //    glm::vec3 AD = simplex[3] - simplex[0];
            //    glm::vec3 DB = simplex[1] - simplex[3];
            //    glm::vec3 simplexCenter = (simplex[0] + simplex[1] + simplex[2] + simplex[3])/4.0f;
            //
            //    glm::vec3 BADperp = glm::cross(AB, AD);
            //
            //    if (glm::dot(BADperp, simplex[0] - simplexCenter) < 0)
            //        BADperp = -BADperp;
            //
            //    glm::vec3 CADperp = glm::cross(AD, AC);
            //
            //    if (glm::dot(CADperp, simplex[0] - simplexCenter) < 0)
            //        CADperp = -CADperp;
            //
            //    glm::vec3 BDCperp = glm::cross(DB, DB);
            //
            //    if (glm::dot(BDCperp, simplex[3] - simplexCenter) < 0)
            //        BDCperp = -BDCperp;
            //
            //    glm::vec3 CABperp = glm::cross(AC, AB);
            //
            //    if (glm::dot(CABperp, simplex[0] - simplexCenter) < 0)
            //        CABperp = -CABperp;
            //
            //    if ( (glm::dot(CABperp, AO) < 0) && (glm::dot(BDCperp, DO) < 0) && (glm::dot(CADperp, AO) < 0) && (glm::dot(BADperp, AO) < 0))
            //    {
            //        Collided = true;
            //    }
            //}
            
            std::vector<glm::vec3> simplex; //newest element is //called A, 2nd newest B etc
            
            //simplex.push_back(Object1.getSupportPoint(direction) - Object2.getSupportPoint(-direction));
            
            //direction = glm::vec3{0.0f} - simplex[0];
            
            while(!Collided)
            {
                if (simplex.size() == 0)
                {
                    direction = Object1.CenterOfMass - Object2.CenterOfMass;
                }
                else if (simplex.size() == 1)
                {
                    direction *= -1;
                }
                else if (simplex.size() == 2)
                {
                    direction = glm::cross(simplex[0] - simplex[1], glm::cross(-simplex[1], simplex[0] - simplex[1]));
                }
                else if (simplex.size() == 3)
                {
                    glm::vec3 AO = glm::vec3{0.0f} - simplex[0];
                    glm::vec3 AB = simplex[1] - simplex[0];
                    glm::vec3 AC = simplex[2] - simplex[0];
                    
                    direction = glm::cross(AB, AC);
                    
                    if (glm::dot(direction, AO) < 0)
                        direction *= -1;
                }
                else if (simplex.size() == 4)
                {
                    glm::vec3 DA = simplex[3] - simplex[0];
                    glm::vec3 DB = simplex[3] - simplex[1];
                    glm::vec3 DC = simplex[3] - simplex[2];

                    glm::vec3 DO = glm::vec3{0.0f} - simplex[3];
                    
                    glm::vec3 ADBperp = -glm::cross(DA, DB);
                    glm::vec3 BDCperp = -glm::cross(DB, DC);
                    glm::vec3 CDAperp = -glm::cross(DC, DA);

                    if (glm::dot(ADBperp, DO) > 0)
                    {
                        simplex.erase(simplex.begin() + 2);
                        direction = ADBperp;
                        std::cout << 1 << std::endl;
                    }
                    else if (glm::dot(BDCperp, DO) > 0)
                    {
                        simplex.erase(simplex.begin());
                        direction = BDCperp;
                        std::cout << 2 << std::endl;

                    }
                    else if (glm::dot(CDAperp, DO) > 0)
                    {
                        simplex.erase(simplex.begin() + 1);
                        direction = CDAperp;
                        std::cout << 3 << std::endl;

                    }
                    else
                    {
                        Collided = true;
                    }
                }

                glm::vec3 A = Object1.getSupportPoint(direction) - Object2.getSupportPoint(-direction);
                    
                if (glm::dot(A, direction) < 0)
                    break;
                
                simplex.push_back(A);
            }
            
            if(Collided)
                throw std::runtime_error("Collided");
            
            if (false)
            {
                std::cout << glm::to_string(simplex[0]) << std::endl;
                std::cout << glm::to_string(simplex[1]) << std::endl;
                std::cout << glm::to_string(simplex[2]) << std::endl;
                std::cout << glm::to_string(simplex[3]) << std::endl;
//
                float minDistance = MAXFLOAT; //shortest distance to not colliding
                glm::vec3 collisionNormal; //collision normal
                int minIndex;
            
                std::vector<glm::ivec3> faces;
                faces.push_back(glm::ivec3(0, 1, 2));
                faces.push_back(glm::ivec3(1, 2, 3));
                faces.push_back(glm::ivec3(2, 0, 3));
                faces.push_back(glm::ivec3(0, 1, 3));
//
                while (minDistance == MAXFLOAT)
                {
                    for (int a = 0; a < 4; a++)
                    {
                        glm::vec3 A = simplex[faces[a].x];
                        glm::vec3 B = simplex[faces[a].y];
                        glm::vec3 C = simplex[faces[a].z];
//
                        //glm::vec3 A = simplex[a];
                        //int b = (a+1) % simplex.size();
                        //glm::vec3 B = simplex[b];
                        //glm::vec3 C = simplex[(a+2) % simplex.size()];
            
                        glm::vec3 AB = B - A;
                        glm::vec3 AC = C - A;
            
                        //std::cout << glm::to_string(AB) << "\n" << glm::to_string(AC) << std::endl;
                        glm::vec3 norm = glm::normalize(glm::cross(AB, AC));
            
                        float distance = glm::dot(norm, (B + C) / 2.0f);
            
                        if (distance < 0)
                        {
                            norm *= -1;
                            distance *= -1;
                        }
            
                        if (distance < minDistance)
                        {
                            minDistance = distance;
                            collisionNormal = norm; //collision normal
                            minIndex = a;
                        }
                        std::cout << glm::to_string(norm) << std::endl;
                        std::cout << distance << std::endl;
            
                    }
            
                    glm::vec3 support = Object1.getAverageSupportPoint(collisionNormal) - Object2.getAverageSupportPoint(-collisionNormal);
                    float sDistance = glm::dot(support, collisionNormal);
//
                    std::cout << sDistance << std::endl;
                    std::cout << minDistance << std::endl;
                    std::cout << abs(sDistance - minDistance) << std::endl;
                    std::cout << "<><><><><><><><><><><><><><><><><>" << std::endl;
//
                    if (abs(sDistance - minDistance) > 0.0001f)
                    {
                        minDistance = MAXFLOAT;
                        simplex.erase(simplex.begin() + faces[minIndex].x);
                        simplex.push_back(support);
                    }
                }
                //minNormal is collision normal
                //get collision point in world space by finding max dot of any of the vertices with the collision axis
            
                //collisionNormal = -collisionNormal;
            
                if (glm::dot(collisionNormal, Object1.CenterOfMass - Object2.CenterOfMass) < 0)
                    collisionNormal *= -1;
                
                float e = (Object1.material.Restitution + Object2.material.Restitution) / 2.0f; //average restitution
            
            
                glm::vec3 collisionPosition;
//
                if (Object1.getMaxDot(Object2.CenterOfMass - Object1.CenterOfMass) > Object2.getMaxDot(-(Object2.CenterOfMass - Object1.CenterOfMass)))
                    collisionPosition = Object1.getAverageSupportPoint(Object2.CenterOfMass - Object1.CenterOfMass);
                else
                    collisionPosition = Object2.getAverageSupportPoint(-(Object2.CenterOfMass - Object1.CenterOfMass));
            
                glm::vec3 relativeVelocity = (Object2.Velocity) - (Object1.Velocity);
//
            
                glm::vec3 r1 = collisionPosition - Object1.CenterOfMass;
            
                glm::vec3 r2 = collisionPosition - Object2.CenterOfMass;
            
                float impulseMagnitude = -(1 + e) * glm::dot(relativeVelocity, collisionNormal) / ( 1/Object1.Mass + 1/Object2.Mass + glm::dot(glm::cross(glm::inverse(Object1.inertiaTensor) * (glm::cross(r1, collisionNormal)), r1) + glm::cross(glm::inverse(Object2.inertiaTensor) * (glm::cross(r2, collisionNormal)), r2), collisionNormal) );
            
                glm::vec3 impulseVector = impulseMagnitude * collisionNormal;
//
                Object1.Velocity = Object1.Velocity - impulseVector/Object1.Mass;
                Object2.Velocity = Object2.Velocity + impulseVector/Object2.Mass;
            
                //float temp = Object1.angularVelocity.x;
            
                //Object1.angularVelocity.x = -Object1.angularVelocity.z;
                //Object1.angularVelocity.z = temp;
            
                Object1.angularVelocity = Object1.angularVelocity - impulseMagnitude * glm::inverse(Object1.inertiaTensor) * glm::cross(r1, collisionNormal);
            
                //temp = Object1.angularVelocity.z;
            
                //Object1.angularVelocity.z = -Object1.angularVelocity.x;
                //Object1.angularVelocity.x = temp;
            
                //Object2.angularVelocity.x = -Object2.angularVelocity.z;
                //Object2.angularVelocity.z = temp;
            
                Object2.angularVelocity = Object2.angularVelocity + impulseMagnitude * glm::inverse(Object2.inertiaTensor) * glm::cross(r2, collisionNormal);
            
                //temp = Object2.angularVelocity.z;
            
                //Object2.angularVelocity.z = -Object2.angularVelocity.x;
            
                //Object2.angularVelocity.x = temp;
            
            
                std::cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << std::endl;
                std::cout << impulseMagnitude << std::endl;
                std::cout << -(1 + e) * glm::dot(relativeVelocity, collisionNormal) << std::endl;
                std::cout << -(1 + e) * ( 1/Object1.Mass + 1/Object2.Mass + glm::dot(glm::cross(glm::inverse(Object1.inertiaTensor) * (glm::cross(r1, collisionNormal)), r1) + glm::cross(glm::inverse(Object2.inertiaTensor) * (glm::cross(r2, collisionNormal)), r2), collisionNormal) ) << std::endl;
                std::cout << glm::to_string(relativeVelocity) << std::endl;
                std::cout << glm::to_string(collisionNormal) << std::endl;
                std::cout << glm::to_string(collisionPosition) << std::endl;
                std::cout << glm::to_string(r1) << std::endl;
                std::cout << glm::to_string(r2) << std::endl;
                std::cout << glm::to_string(Object1.CenterOfMass) << std::endl;
                std::cout << glm::to_string(Object2.CenterOfMass) << std::endl;
                std::cout << glm::to_string(Object1.angularVelocity) << std::endl;
                std::cout << glm::to_string(Object2.angularVelocity) << std::endl;
                std::cout << glm::to_string(Object1.Velocity) << std::endl;
                std::cout << glm::to_string(Object2.Velocity) << std::endl;
                std::cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << std::endl;
                
                //glm::vec3 collisionAxis = minNormal * (minDistance + 0.001f); //put it slightly outside of the other object so not colliding still
                //
                //const glm::vec3 V1 = Object1.Velocity;
                //
                //for (glm::vec3& Vertex : Object1.Vertices)
                //    Vertex += 0.5f * collisionAxis;
                //
                //Object1.CenterOfMass += 0.5f * collisionAxis;
                //
                //for (glm::vec3& Vertex : Object2.Vertices)
                //    Vertex -= 0.5f * collisionAxis;
                //
                //Object2.CenterOfMass -= 0.5f * collisionAxis;
                //
                //Object1.Velocity = (2*Object2.Mass*Object2.Velocity + Object1.Velocity*Object1.Mass - Object2.Mass*Object1.Velocity)/(Object1.Mass + Object2.Mass);
                //Object2.Velocity = Object1.Velocity + V1 - Object2.Velocity;
                
                //Object1.Velocity = -0.9999f * glm::reflect(Object1.Velocity, glm::normalize(-minNormal));
                //Object2.Velocity = -0.9999f * glm::reflect(Object2.Velocity, glm::normalize(-minNormal));
            }
        }
    }
}
    
    
    
    //for (int i = 0; i < GetSphereCount(); i++)
    //{
    //    Sphere& Sphere1 = Spheres[i];
    //    for (int k = (i+1); k < GetSphereCount(); k++)
    //    {
    //        Sphere& Sphere2 = Spheres[k];
    //        const glm::vec3 collisionAxis = Sphere1.Position - Sphere2.Position;
    //        const float distance = glm::length(collisionAxis);
    //        if (distance < (Sphere1.Radius + Sphere2.Radius))
    //        {
    //
    //            const glm::vec3 n = collisionAxis / distance;
    //            const float delta = Sphere1.Radius + Sphere2.Radius - distance;
    //            Sphere1.Position += 0.5f * delta * n;
    //            Sphere2.Position -= 0.5f * delta * n;
    //
    //            glm::vec3 V1 = Sphere1.Velocity;
    //            //glm::vec3 V2 = Sphere2.Velocity;
    //
    //            Sphere1.Velocity = (2*Sphere2.Mass*Sphere2.Velocity + Sphere1.Velocity*Sphere1.Mass - Sphere2.Mass*Sphere1.Velocity)/(Sphere1.Mass + Sphere2.Mass);
    //            Sphere2.Velocity = Sphere1.Velocity + V1 - Sphere2.Velocity;
    //
    //            Sphere1.Velocity = -0.9999f * glm::reflect(Sphere1.Velocity, glm::normalize(-collisionAxis));
    //            Sphere2.Velocity = -0.9999f * glm::reflect(Sphere2.Velocity, glm::normalize(-collisionAxis));
    //
    //        }
    //    }
    //}

