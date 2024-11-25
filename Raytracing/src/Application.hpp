//
//  Application.hpp
//  Raytracing
//
//  Created by Nathan Thurber on 15/9/22.
//
#pragma once

#include <string>
#include "Camera.hpp"
#include "Scene.hpp"


struct GLFWwindow;


struct ApplicationSpecification
{
    std::string Name = "App";
    uint32_t Width = 1440;
    uint32_t Height = 720;
};

class Application
{
public:
    Application(const ApplicationSpecification& specification = ApplicationSpecification());
    ~Application();
    void Run();
    void Close();
        
public:
    Camera g_Camera;
    Scene g_Scene;
    
private:
    void Init();
    void OnUpdate(float ts);
    //static void PhysicsUpdate();
    void Shutdown();
    
    float GetTime();
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    static void iconifyCallback(GLFWwindow* window, int iconified);
    
    
private:
    bool m_Running = false;
    GLFWwindow* m_WindowHandle = nullptr;
    ApplicationSpecification m_Specification;
    
private:
    float m_FrameTime = 0.0f;
    float m_LastFrameTime = 0.0f;
};

Application* CreateApplication(int argc, char** argv);


