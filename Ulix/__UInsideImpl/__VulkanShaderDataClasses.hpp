#pragma once

#include <UTypes.hpp>
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace __uii::vsdces {
    struct Vertex2D {
        glm::vec2 position;
        glm::vec4 color;
        uts::f32 corner_radius;
        glm::vec2 half_size;
        uts::f32 border_width;
        glm::vec4 border_color;
        glm::vec2 center;
        glm::vec2 uv;
        uts::u32 tex_index;

        inline static auto get_binding_description() -> VkVertexInputBindingDescription {
            VkVertexInputBindingDescription binding_description{};
            binding_description.binding = 0;
            binding_description.stride = sizeof(Vertex2D);
            binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            return binding_description;
        }

        inline static auto get_attribute_descriptions() -> std::array<VkVertexInputAttributeDescription, 9> {
            std::array<VkVertexInputAttributeDescription, 9> attribute_descriptions{};
            attribute_descriptions[0].binding = 0;
            attribute_descriptions[0].location = 0;
            attribute_descriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
            attribute_descriptions[0].offset = offsetof(Vertex2D, position);

            attribute_descriptions[1].binding = 0;
            attribute_descriptions[1].location = 1;
            attribute_descriptions[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attribute_descriptions[1].offset = offsetof(Vertex2D, color);

            attribute_descriptions[2].binding = 0;
            attribute_descriptions[2].location = 2;
            attribute_descriptions[2].format = VK_FORMAT_R32_SFLOAT;
            attribute_descriptions[2].offset = offsetof(Vertex2D, corner_radius);

            attribute_descriptions[3].binding = 0;
            attribute_descriptions[3].location = 3;
            attribute_descriptions[3].format = VK_FORMAT_R32G32_SFLOAT;
            attribute_descriptions[3].offset = offsetof(Vertex2D, half_size);

            attribute_descriptions[4].binding = 0;
            attribute_descriptions[4].location = 4;
            attribute_descriptions[4].format = VK_FORMAT_R32_SFLOAT;
            attribute_descriptions[4].offset = offsetof(Vertex2D, border_width);

            attribute_descriptions[5].binding = 0;
            attribute_descriptions[5].location = 5;
            attribute_descriptions[5].format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attribute_descriptions[5].offset = offsetof(Vertex2D, border_color);

            attribute_descriptions[6].binding = 0;
            attribute_descriptions[6].location = 6;
            attribute_descriptions[6].format = VK_FORMAT_R32G32_SFLOAT;
            attribute_descriptions[6].offset = offsetof(Vertex2D, center);

            attribute_descriptions[7].binding = 0;
            attribute_descriptions[7].location = 7;
            attribute_descriptions[7].format = VK_FORMAT_R32G32_SFLOAT;
            attribute_descriptions[7].offset = offsetof(Vertex2D, uv);

            attribute_descriptions[8].binding = 0;
            attribute_descriptions[8].location = 8;
            attribute_descriptions[8].format = VK_FORMAT_R32_UINT;
            attribute_descriptions[8].offset = offsetof(Vertex2D, tex_index);

            return attribute_descriptions;
        }
    };

    struct PushConstant {
        glm::mat4 projection;
    };
}
