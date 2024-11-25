//
//  Shader.hpp
//  Raytracing
//
//  Created by Nathan Thurber on 6/10/22.
//

#pragma once

#include "vulkan.h"
#include "glm.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#include <array>


class Shader
{
public:
    Shader(const std::string& filename);
    VkShaderModule createShaderModule(VkDevice& Device);
        
private:
    std::vector<char> ParseShader(const std::string filename);
    std::vector<char> m_ShaderBuffer;

public:
    struct Vertex
    {
        glm::vec2 pos;
        glm::vec3 color;
        
        static VkVertexInputBindingDescription getBindingDescription()
        {
            VkVertexInputBindingDescription bindingDescription{};
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(Vertex);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            
            return bindingDescription;
        }
        
        static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions()
        {
            std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(Vertex, pos);
            
            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(Vertex, color);
            
            return attributeDescriptions;
        }
    };
};

