//
//  Application.cpp
//  Raytracing
//
//  Created by Nathan Thurber on 15/9/22.
//
#include "VulkanInstance.hpp"
#include "Application.hpp"
#include "Timer.hpp"
//#include "Camera.hpp"
#include <iostream>
#include <thread>

#define GLFW_INCLUDE_VULKAN 

#include "glfw3.h"
#include "vulkan.h"
#include "glfw3native.h"

extern bool g_ApplicationRunning;

GLFWAPI GLFWwindow*             g_WindowHandle;
VulkanInstance                  VulkanInstance;
float                           g_TimeStep = 0.0f;

Application::Application(const ApplicationSpecification& specification)
    : m_Specification(specification), g_Camera(45.0f, 0.1f, 1000.0f)
{
    Init();
}

Application::~Application()
{
    Shutdown();
}

void Application::Init()
{
    if (!glfwInit())
        throw std::runtime_error("[GLFW] Could not initalize GLFW!");
    
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    g_WindowHandle = glfwCreateWindow(m_Specification.Width, m_Specification.Height, m_Specification.Name.c_str(), NULL, NULL);
    glfwSetWindowUserPointer(g_WindowHandle, this);
    glfwSetFramebufferSizeCallback(g_WindowHandle, &Application::framebufferResizeCallback);
    glfwSetWindowIconifyCallback(g_WindowHandle, iconifyCallback);
            
    if (!glfwVulkanSupported())
        throw std::runtime_error("[GLFW] Vulkan not supported!");
    
    VulkanInstance.setUpVulkan();
    
    Object objects[2];
    
    objects[0].Vertices.push_back(glm::vec3(-1.0f,  1.0f, -1.0f));
    objects[0].Vertices.push_back(glm::vec3( 1.0f,  1.0f, -1.0f));
    objects[0].Vertices.push_back(glm::vec3( 1.0f,  1.0f,  1.0f));
    objects[0].Vertices.push_back(glm::vec3(-1.0f,  1.0f,  1.0f));
    objects[0].Vertices.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
    objects[0].Vertices.push_back(glm::vec3( 1.0f, -1.0f, -1.0f));
    objects[0].Vertices.push_back(glm::vec3( 1.0f, -1.0f,  1.0f));
    objects[0].Vertices.push_back(glm::vec3(-1.0f, -1.0f,  1.0f));
    
    objects[0].Indices.push_back(glm::ivec3(0, 1, 2));
    objects[0].Indices.push_back(glm::ivec3(2, 3, 0));
    objects[0].Indices.push_back(glm::ivec3(0, 1, 4));
    objects[0].Indices.push_back(glm::ivec3(1, 4, 5));
    objects[0].Indices.push_back(glm::ivec3(1, 2, 5));
    objects[0].Indices.push_back(glm::ivec3(2, 5, 6));
    objects[0].Indices.push_back(glm::ivec3(2, 3, 6));
    objects[0].Indices.push_back(glm::ivec3(3, 6, 7));
    objects[0].Indices.push_back(glm::ivec3(3, 7, 0));
    objects[0].Indices.push_back(glm::ivec3(0, 7, 4));
    objects[0].Indices.push_back(glm::ivec3(4, 5, 6));
    objects[0].Indices.push_back(glm::ivec3(6, 7, 4));

    objects[0].material.Albedo = glm::vec3(0.2f, 0.3f, 1.0f);
    objects[0].material.Roughness = 0.6f;
    objects[0].material.Emission = glm::vec3(0.2f, 0.3f, 1.0f) * 0.0f;
    objects[0].material.Restitution = 1.0f;
    
    objects[0].CenterOfMass = glm::vec3(0.0f, 0.0f, 0.0f);
    objects[0].boundingRadius = 1.56f; //actually 1.55377397
    
    objects[0].Velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    objects[0].angularVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
    objects[0].Mass = 5.0f;
    
    objects[0].inertiaTensor = glm::mat3{1.0f/12.0f * objects[0].Mass * (2*2 + 2*2), 0.0f, 0.0f,
                                            0.0f, 1.0f/12.0f * objects[0].Mass * (2*2 + 2*2), 0.0f,
                                            0.0f, 0.0f, 1.0f/12.0f * objects[0].Mass * (2*2 + 2*2)};
    
    objects[1].Vertices.push_back(glm::vec3(-2.0f, 2.8f +  2.0f, 5.0f + (-0.5f)));
    objects[1].Vertices.push_back(glm::vec3( 2.0f, 2.8f +  2.0f, 5.0f + (-0.5f)));
    objects[1].Vertices.push_back(glm::vec3( 2.0f, 2.8f +  2.0f, 5.0f + ( 0.5f)));
    objects[1].Vertices.push_back(glm::vec3(-2.0f, 2.8f +  2.0f, 5.0f + ( 0.5f)));
    objects[1].Vertices.push_back(glm::vec3(-2.0f, 2.8f + -2.0f, 5.0f + (-0.5f)));
    objects[1].Vertices.push_back(glm::vec3( 2.0f, 2.8f + -2.0f, 5.0f + (-0.5f)));
    objects[1].Vertices.push_back(glm::vec3( 2.0f, 2.8f + -2.0f, 5.0f + ( 0.5f)));
    objects[1].Vertices.push_back(glm::vec3(-2.0f, 2.8f + -2.0f, 5.0f + ( 0.5f)));
    
    objects[1].Indices.push_back(glm::ivec3(0, 1, 2));
    objects[1].Indices.push_back(glm::ivec3(2, 3, 0));
    objects[1].Indices.push_back(glm::ivec3(0, 1, 4));
    objects[1].Indices.push_back(glm::ivec3(1, 4, 5));
    objects[1].Indices.push_back(glm::ivec3(1, 2, 5));
    objects[1].Indices.push_back(glm::ivec3(2, 5, 6));
    objects[1].Indices.push_back(glm::ivec3(2, 3, 6));
    objects[1].Indices.push_back(glm::ivec3(3, 6, 7));
    objects[1].Indices.push_back(glm::ivec3(3, 7, 0));
    objects[1].Indices.push_back(glm::ivec3(0, 7, 4));
    objects[1].Indices.push_back(glm::ivec3(4, 5, 6));
    objects[1].Indices.push_back(glm::ivec3(6, 7, 4));
    
    objects[1].material.Albedo = glm::vec3(0.2f, 0.3f, 1.0f);
    objects[1].material.Roughness = 0.0f;
    objects[1].material.Emission = glm::vec3(0.2f, 0.3f, 2.0f) * 5.0f;
    objects[1].material.Restitution = 1.0f;
    
    objects[1].CenterOfMass = glm::vec3(0.0f, 0.0f, 5.0f);
    objects[1].boundingRadius = 2.83f; //actually 2.828....

    objects[1].Velocity = glm::vec3(0.0f, 0.0f, -1.0f);
    objects[1].angularVelocity = glm::vec3(0.0f, 1.0f, 0.0f);
    objects[1].Mass = 10.0f;
    
    objects[1].inertiaTensor = glm::mat3{1.0f/12.0f * objects[1].Mass * (2*2 + 2*2), 0.0f, 0.0f,
                                            0.0f, 1.0f/12.0f * objects[1].Mass * (2*2 + 2*2), 0.0f,
                                            0.0f, 0.0f, 1.0f/12.0f * objects[1].Mass * (2*2 + 2*2)}; //not actually correct
    
    g_Scene.AddObject(objects[0]);
    g_Scene.AddObject(objects[1]);
    
}

void Application::Shutdown()
{
    VulkanInstance.cleanUp();
    
    glfwDestroyWindow(m_WindowHandle);
    
    glfwTerminate();

    g_ApplicationRunning = false;
}

void PhysicsUpdate(bool &Running, Scene &scene)
{
    double UpdateTime = 0.0f;
    double LastUpdateTime = (float)glfwGetTime();
    
    while(!glfwWindowShouldClose(g_WindowHandle) && Running)
    {
        auto start = std::chrono::steady_clock::now();
        //std::cout << "[Phyiscs loop] " << 1.0f/UpdateTime << " Hz" << std::endl;
        
        double time = (double)glfwGetTime();
        UpdateTime = time - LastUpdateTime;
        LastUpdateTime = time;
        
        scene.OnUpdate(UpdateTime);
    
        using std::chrono::operator""ns;
        std::this_thread::sleep_until(start + 10000ns);
    }
}

void Application::Run()
{
    m_Running = true;
    
    std::thread physicsUpdate(PhysicsUpdate, std::ref(m_Running), std::ref(g_Scene));
    
    while(!glfwWindowShouldClose(g_WindowHandle) && m_Running)
    {
        glfwPollEvents();
        
        OnUpdate(g_TimeStep);
        
        float time = GetTime();
        m_FrameTime = time - m_LastFrameTime;
        g_TimeStep = glm::min<float>(m_FrameTime, 0.0333f); //0.0333f is slowest it will run (30fps)
        m_LastFrameTime = time;
        //std::cout << "[Main loop] " << 1.0f/m_FrameTime << " fps" << std::endl;
    }
    vkDeviceWaitIdle(VulkanInstance.GetDevice());
    
    physicsUpdate.join();
}

void Application::OnUpdate(float ts)
{
    g_Camera.OnUpdate(g_TimeStep);
    //g_Scene.OnUpdate(g_TimeStep);
    VulkanInstance.drawFrame(g_Camera, g_Scene);
}

void Application::Close()
{
    m_Running = false;
}

float Application::GetTime()
{
    return (float)glfwGetTime();
}

void Application::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
    auto app = reinterpret_cast<class VulkanInstance*>(glfwGetWindowUserPointer(window));
    app->g_FrameBufferResized = true;
    //VulkanInstance.drawFrame(&g_Camera, &g_Scene); //draws during resize
}

void Application::iconifyCallback(GLFWwindow* window, int iconified)
{
    while (iconified)
    {
        iconified = glfwGetWindowAttrib(window, GLFW_ICONIFIED);
        glfwWaitEvents();
    }
}
