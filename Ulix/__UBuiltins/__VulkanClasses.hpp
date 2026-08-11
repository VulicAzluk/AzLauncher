#pragma once

#include <URenderScene.hpp>
#include <UTypes.hpp>
#include <ULogger.hpp>
#include <__UBuiltins/__VulkanRequirements.hpp>
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <sys/stat.h>
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include <optional>
#include <algorithm>

namespace __ubtins::vkclses {
    struct OptionalQueueFamilyIndices {
        std::optional<uts::u32> graphics_queue_family_index;
        std::optional<uts::u32> present_queue_family_index;

        inline auto is_complete() const -> bool {
            return graphics_queue_family_index.has_value() &&
                    present_queue_family_index.has_value();
        }
    };

    struct SwapchainSupportDetails {
        VkSurfaceCapabilitiesKHR surface_capabilities;
        uts::vec<VkSurfaceFormatKHR> surface_formats;
        uts::vec<VkPresentModeKHR> present_modes;

        SwapchainSupportDetails() = default;
        SwapchainSupportDetails(VkPhysicalDevice device, VkSurfaceKHR window_surface) {
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, window_surface, &surface_capabilities);

            uts::u32 surface_format_count = 0;
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, window_surface, &surface_format_count, VK_NULL_HANDLE);
            if (surface_format_count) {
                surface_formats.resize(surface_format_count);
                vkGetPhysicalDeviceSurfaceFormatsKHR(device, window_surface, &surface_format_count, surface_formats.data());
            }

            uts::u32 present_mode_count = 0;
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, window_surface, &present_mode_count, VK_NULL_HANDLE);
            if (present_mode_count) {
                present_modes.resize(present_mode_count);
                vkGetPhysicalDeviceSurfacePresentModesKHR(device, window_surface, &present_mode_count, present_modes.data());
            }
        }

        inline auto is_adequate() -> bool {
            return !surface_formats.empty() &&
                    !present_modes.empty();
        }

        inline auto select_surface_format() -> VkSurfaceFormatKHR {
            for (const auto& format : surface_formats)
                if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
                    return format;

            return surface_formats[0];
        }

        inline auto select_extent(HWND window_hwnd) -> VkExtent2D {
            if (surface_capabilities.currentExtent.width != std::numeric_limits<uts::u32>::max()) {
                return surface_capabilities.currentExtent;
            } else {
                RECT window_rect;
                GetWindowRect(window_hwnd, &window_rect);

                VkExtent2D actual_extent = {
                    static_cast<uts::u32>(window_rect.right - window_rect.left),
                    static_cast<uts::u32>(window_rect.bottom - window_rect.top)
                };

                actual_extent.width = std::clamp(actual_extent.width, surface_capabilities.minImageExtent.width, surface_capabilities.maxImageExtent.width);
                actual_extent.height = std::clamp(actual_extent.height, surface_capabilities.minImageExtent.height, surface_capabilities.maxImageExtent.height);

                return actual_extent;
            }
        }
    };

    struct PhysicalDeviceInfos {
        VkPhysicalDeviceProperties properties;
        VkPhysicalDeviceMemoryProperties memory_properties;
        VkPhysicalDeviceFeatures features;
        OptionalQueueFamilyIndices queue_family_indices;
        SwapchainSupportDetails swapchain_support_details;
        bool extension_support;

        PhysicalDeviceInfos() = default;
        PhysicalDeviceInfos(VkPhysicalDevice device, VkSurfaceKHR window_surface) {
            vkGetPhysicalDeviceProperties(device, &properties);
            vkGetPhysicalDeviceMemoryProperties(device, &memory_properties);
            vkGetPhysicalDeviceFeatures(device, &features);

            // Select suitable queue family indices from queue family properties
            uts::u32 queue_family_count = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, VK_NULL_HANDLE);
            uts::vec<VkQueueFamilyProperties> queue_family_properties_list = uts::vec<VkQueueFamilyProperties>(queue_family_count);
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_family_properties_list.data());
            uts::u32 index = 0;
            for (const auto& queue_family_properties : queue_family_properties_list) {
                if (queue_family_properties.queueFlags & VK_QUEUE_GRAPHICS_BIT)
                    queue_family_indices.graphics_queue_family_index = index;

                VkBool32 present_queue_family_support = false;
                vkGetPhysicalDeviceSurfaceSupportKHR(device, index, window_surface, &present_queue_family_support);
                if (present_queue_family_support) {
                    queue_family_indices.present_queue_family_index = index;
                }

                if (queue_family_indices.is_complete())
                    break;

                index++;
            }

            extension_support = check_extension_support(device);
            if (!extension_support) return;
            swapchain_support_details = SwapchainSupportDetails(device, window_surface);
        }

        inline auto get_unique_queue_family_indices() -> std::set<uts::u32> {
            std::set<uts::u32> unique_queue_family_indices = {
                queue_family_indices.graphics_queue_family_index.value(),
                queue_family_indices.present_queue_family_index.value()
            };

            return unique_queue_family_indices;
        }

        inline auto match_gpu_type(VkPhysicalDeviceType gpu_type) const -> bool {
            return properties.deviceType == gpu_type;
        }

        inline auto is_suitable() -> bool {
            return (
                match_gpu_type(VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) ||
                match_gpu_type(VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
            )&& features.geometryShader && features.tessellationShader &&
                queue_family_indices.is_complete() &&
                extension_support &&
                swapchain_support_details.is_adequate() &&
                features.samplerAnisotropy;
        }

        inline auto check_extension_support(VkPhysicalDevice device) -> bool {
            uts::u32 extension_count = 0;
            vkEnumerateDeviceExtensionProperties(device, VK_NULL_HANDLE, &extension_count, VK_NULL_HANDLE);
            uts::vec<VkExtensionProperties> available_extensions = uts::vec<VkExtensionProperties>(extension_count);
            vkEnumerateDeviceExtensionProperties(device, VK_NULL_HANDLE, &extension_count, available_extensions.data());

            for (const auto& extension : available_extensions) {
                if (std::strcmp(extension.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0)
                    return true;
            }
            return false;
        }
    };

    struct DeviceQueues {
        VkQueue graphics_queue;
        VkQueue present_queue;

        DeviceQueues() = default;
        DeviceQueues(VkDevice logical_device, const PhysicalDeviceInfos& device_infos) {
            vkGetDeviceQueue(logical_device, device_infos.queue_family_indices.graphics_queue_family_index.value(), 0, &graphics_queue);
            vkGetDeviceQueue(logical_device, device_infos.queue_family_indices.present_queue_family_index.value(), 0, &present_queue);
        }
    };

    struct Buffer {
        VkBuffer buffer;
        VkDeviceMemory buffer_memory;
        VkDeviceSize memory_size;

        Buffer() = default;
        Buffer(VkPhysicalDevice physical_device, VkDevice logical_device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties): memory_size(size) {
            VkBufferCreateInfo buffer_create_info{};
            buffer_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            buffer_create_info.size = size;
            buffer_create_info.usage = usage;
            buffer_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            if (vkCreateBuffer(logical_device, &buffer_create_info, nullptr, &buffer) != VK_SUCCESS)
                ULogger::ulixerr("Failed to create buffer");

            VkMemoryRequirements buffer_memory_requirements;
            vkGetBufferMemoryRequirements(logical_device, buffer, &buffer_memory_requirements);

            VkMemoryAllocateInfo buffer_memory_allocate_info{};
            buffer_memory_allocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            buffer_memory_allocate_info.allocationSize = buffer_memory_requirements.size;
            buffer_memory_allocate_info.memoryTypeIndex = find_memory_type(physical_device, buffer_memory_requirements.memoryTypeBits, properties);
            if (vkAllocateMemory(logical_device, &buffer_memory_allocate_info, nullptr, &buffer_memory) != VK_SUCCESS)
                ULogger::ulixerr("Failed to allocate buffer memory");

            if (vkBindBufferMemory(logical_device, buffer, buffer_memory, 0) != VK_SUCCESS)
                ULogger::ulixerr("Failed to bind buffer memory");
        }

        inline auto map_memory(VkDevice device, const void* data, VkDeviceSize size) -> void {
            void* dst_data;
            vkMapMemory(device, buffer_memory, 0, size, 0, &dst_data);
                memcpy(dst_data, data, static_cast<std::size_t>(size));
            vkUnmapMemory(device, buffer_memory);
        }

        inline auto copy_buffer_to(const Buffer& buffer, VkDevice device, VkCommandPool command_pool, VkQueue graphics_queue, VkDeviceSize size) -> void {
            VkCommandBufferAllocateInfo command_buffer_allocate_info{};
            command_buffer_allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            command_buffer_allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            command_buffer_allocate_info.commandBufferCount = 1;
            command_buffer_allocate_info.commandPool = command_pool;

            VkCommandBuffer command_buffer;
            vkAllocateCommandBuffers(device, &command_buffer_allocate_info, &command_buffer);

            VkCommandBufferBeginInfo command_buffer_begin_info{};
            command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            command_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
            vkBeginCommandBuffer(command_buffer, &command_buffer_begin_info);

            VkBufferCopy buffer_copy_region{};
            buffer_copy_region.srcOffset = 0;
            buffer_copy_region.dstOffset = 0;
            buffer_copy_region.size = size;
            vkCmdCopyBuffer(command_buffer, this->buffer, buffer.buffer, 1, &buffer_copy_region);
            vkEndCommandBuffer(command_buffer);

            VkSubmitInfo submit_info{};
            submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submit_info.commandBufferCount = 1;
            submit_info.pCommandBuffers = &command_buffer;
            vkQueueSubmit(graphics_queue, 1, &submit_info, VK_NULL_HANDLE);
            vkQueueWaitIdle(graphics_queue);

            vkFreeCommandBuffers(device, command_pool, 1, &command_buffer);
        }

        inline auto release(VkDevice device) -> void {
            vkDestroyBuffer(device, buffer, nullptr);
            vkFreeMemory(device, buffer_memory, nullptr);
        }

        inline static auto find_memory_type(VkPhysicalDevice device, uts::u32 type_filter, VkMemoryPropertyFlags properties) -> uts::u32 {
            VkPhysicalDeviceMemoryProperties physical_device_memory_properties;
            vkGetPhysicalDeviceMemoryProperties(device, &physical_device_memory_properties);

            for (uts::u32 index = 0; index < physical_device_memory_properties.memoryTypeCount; index++) {
                if ((type_filter & (1 << index)) && (physical_device_memory_properties.memoryTypes[index].propertyFlags & properties) == properties) {
                    return index;
                }
            }

            ULogger::ulixerr("Failed to find suitable memory type");
        }
    };
}
