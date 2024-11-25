//
//  Shader.cpp
//  Raytracing
//
//  Created by Nathan Thurber on 6/10/22.
//

#include "Shader.hpp"
#include "VulkanInstance.hpp"

Shader::Shader(const std::string& filename)
{
    m_ShaderBuffer = ParseShader(filename);
}

std::vector<char> Shader::ParseShader(const std::string filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
        
    if (!file.is_open())
        throw std::runtime_error("[Vulkan] Failed to open shader!");
    
    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);
    
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    
    file.close();
    
    return buffer;
}

VkShaderModule Shader::createShaderModule(VkDevice& Device)
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = m_ShaderBuffer.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(m_ShaderBuffer.data());
        
    VkShaderModule ShaderModule;
    if (vkCreateShaderModule(Device, &createInfo, nullptr, &ShaderModule) != VK_SUCCESS)
        throw std::runtime_error("[Vulkan] Failed to create shader module!");
    
    return ShaderModule;
}
