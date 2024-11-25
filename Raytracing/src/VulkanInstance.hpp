//
//  VulkanInstance.hpp
//  Raytracing
//
//  Created by Nathan Thurber on 4/10/22.
//
#pragma once

#include "vulkan.h"
#include "glfw3.h"
#include "glfw3native.h"
#include <iostream>
#include <vector>
#include <map>

#include "Shader.hpp"
#include "Timer.hpp"
#include "Camera.hpp"
#include "Application.hpp"

#define GLM_FORCE_RADIANS
#include "glm.hpp"
#include "matrix_transform.hpp"
#include "matrix_clip_space.hpp" 
#include "string_cast.hpp"

#include <chrono>

class VulkanInstance
{
public:
    VulkanInstance();
    const void setUpVulkan();
    const void cleanUp();
    
public:
    const void drawFrame(Camera& camera, Scene& scene);
    static const void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    
public:
    bool g_FrameBufferResized = false;
    
private: 
    const void CreateVulkanInstance();
    const void setupPhysicalDevice();
    const void findQueueFamilies();
    const void createLogicalDevice();
    const void createSwapChain();
    const void createImageViews();
    const void createShaderModule();
    const void createDescriptorPool();
    const void createDescriptorSets();
    const void createDescriptorSetLayout();
    const void createGraphicsPipeline();
    const void createRenderPass();
    const void createFramebuffers();
    const void createCommandPool();
    const void createVertexBuffer();
    const void createIndexBuffer();
    const void createUniformBuffers();
    const void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    const void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    const void createCommandBuffers();
    const void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
    const void createSyncObjects();
    
    const void updateUniformBuffer(uint32_t currentImage, Camera& camera, Scene& scene);
    
    const void recreateSwapChain();
    const void cleanupSwapChain();
    
    struct SwapChainSupportDetails;
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    int rateDeviceSuitability(VkPhysicalDevice device);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

private:
    VkInstance                          m_Instance;
    VkPhysicalDevice                    m_PhysicalDevice;
    uint32_t                            m_QueueFamily;
    VkQueue                             m_GraphicsQueue;
    VkQueue                             m_PresentQueue;
    VkDevice                            m_Device;
    VkAllocationCallbacks*              m_Allocator;
    VkSurfaceKHR                        m_Surface;
    VkSwapchainKHR                      m_SwapChain;
    std::vector<VkImage>                m_SwapChainImages;
    VkFormat                            m_SwapChainImageFormat;
    VkExtent2D                          m_SwapChainExtent;
    std::vector<VkImageView>            m_SwapChainImageViews;
    VkPipelineShaderStageCreateInfo     m_ShaderStages[2];
    VkDescriptorPool                    m_DescriptorPool;
    VkDescriptorSetLayout               m_DescriptorSetLayout;
    std::vector<VkDescriptorSet>        m_DescriptorSets;
    VkPipelineLayout                    m_PipelineLayout;
    VkRenderPass                        m_RenderPass;
    VkPipeline                          m_GraphicsPipeline;
    VkShaderModule                      m_VertShaderModule;
    VkShaderModule                      m_FragShaderModule;
    std::vector<VkFramebuffer>          m_SwapChainFramebuffers;
    VkCommandPool                       m_CommandPool;
    VkBuffer                            m_VertexBuffer;
    VkDeviceMemory                      m_VertexBufferMemory;
    VkBuffer                            m_IndexBuffer;
    VkDeviceMemory                      m_IndexBufferMemory;
    
    std::vector<VkBuffer>               m_UniformBuffers;
    std::vector<VkDeviceMemory>         m_UniformBuffersMemory;
    
    std::vector<VkCommandBuffer>        m_CommandBuffers;
    
    std::vector<VkSemaphore>            m_ImageAvailableSemaphores;
    std::vector<VkSemaphore>            m_RenderFinishedSemaphores;
    std::vector<VkFence>                m_InFlightFences;
    
    int m_ViewportWidth, m_ViewportHeight;

    
public:
    inline VkInstance GetInstance() const { return m_Instance; }
    inline VkPhysicalDevice GetPhysicalDevice() const { return m_PhysicalDevice; }
    inline uint32_t GetQueueFamily() const { return m_QueueFamily; }
    inline VkDevice GetDevice() const { return m_Device; }

private:
    const int                           MAX_FRAMES_IN_FLIGHT = 3;
    uint32_t                            m_CurrentFrame = 0;

    const std::vector<Shader::Vertex> vertices = {
        {{-1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}},
        {{ 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}},
        {{ 1.0f,  1.0f}, {0.0f, 0.0f, 1.0f}},
        {{-1.0f,  1.0f}, {1.0f, 1.0f, 1.0f}},
    };
    
    const std::vector<uint16_t> indices = {
        0, 1, 2,
        2, 3, 0
    };
};

