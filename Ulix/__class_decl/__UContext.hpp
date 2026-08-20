#pragma once

#include <UAlignment.hpp>
#include <UFile.hpp>
#include <UFont.hpp>
#include <UPixmap.hpp>
#include <URect.hpp>
#include <vulkan/vulkan_core.h>
#include <UAppInfo.hpp>
#include <URenderInfo.hpp>
#include <UWindowInfo.hpp>
#include <__inside_impl/__VulkanClasses.hpp>
#include <__inside_impl/__VulkanShaderDataClasses.hpp>
#include <UScene.hpp>
#include <UTimer.hpp>


template<> struct std::hash<UFile> {
    size_t operator()(const UFile& f) const noexcept {
        return std::hash<std::string>{}(f.get_file_path());
    }
};

template<> struct std::equal_to<UFile> {
    bool operator()(const UFile& a, const UFile& b) const noexcept {
        return a.get_file_path() == b.get_file_path();
    }
};

template<> struct std::hash<URect> {
    size_t operator()(const URect& r) const noexcept {
        size_t h = hash<uts::f32>{}(r.get_x());
        h ^= hash<uts::f32>{}(r.get_y()) + 0x9e3779b9 + (h<<6) + (h>>2);
        h ^= hash<uts::f32>{}(r.get_width()) + 0x9e3779b9 + (h<<6) + (h>>2);
        h ^= hash<uts::f32>{}(r.get_height()) + 0x9e3779b9 + (h<<6) + (h>>2);
        return h;
    }
};

template<> struct std::equal_to<URect> {
    bool operator()(const URect& a, const URect& b) const noexcept {
        return a.get_x() == b.get_x() && a.get_y() == b.get_y() && a.get_width() == b.get_width() && a.get_height() == b.get_height();
    }
};

template<> struct std::hash<UFont> {
    size_t operator()(const UFont& f) const noexcept {
        size_t h = hash<string>{}(f.get_name());
        h ^= hash<uts::f32>{}(f.get_size()) + 0x9e3779b9 + (h<<6) + (h>>2);
        h ^= hash<bool>{}(f.is_bold()) + 0x9e3779b9 + (h<<6) + (h>>2);
        h ^= hash<bool>{}(f.is_italic()) + 0x9e3779b9 + (h<<6) + (h>>2);
        h ^= hash<bool>{}(f.is_underline()) + 0x9e3779b9 + (h<<6) + (h>>2);
        h ^= hash<bool>{}(f.is_strikethrough()) + 0x9e3779b9 + (h<<6) + (h>>2);
        return h;
    }
};

template<> struct std::equal_to<UFont> {
    bool operator()(const UFont& a, const UFont& b) const noexcept {
        return a.get_name() == b.get_name() && a.get_size() == b.get_size() && a.is_bold() == b.is_bold() && a.is_italic() == b.is_italic() && a.is_underline() == b.is_underline() && a.is_strikethrough() == b.is_strikethrough();
    }
};

template<> struct std::hash<std::pair<UFile, URect>> {
    size_t operator()(const std::pair<UFile, URect>& p) const noexcept {
        size_t h = std::hash<UFile>{}(p.first);
        h ^= std::hash<URect>{}(p.second) + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};
template<> struct std::hash<std::pair<std::string, UFont>> {
    size_t operator()(const std::pair<std::string, UFont>& p) const noexcept {
        size_t h = std::hash<std::string>{}(p.first);
        h ^= std::hash<UFont>{}(p.second) + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};

class UContext {
    public:
        using RenderCallback = UScene(*)(UContext&);
        using TickTimer = UTimer<UContext&>;

    private:
        TickTimer tick_timer;
        RenderCallback render_callback;
        UScene current_render_scene;
        uts::vec<__uii::vsdces::Vertex2D> render_vertices;
        uts::vec<uts::u32> render_indices;
        __uii::vsdces::PushConstant push_constant;
        UColor current_background_color;
        bool render_dirty = false;
        uts::u32 texture_index_max;

        uts::hashmap<UFile, uts::u32> bitmap_texture_cache;
        uts::hashmap<uts::pair<UFile, URect>, uts::u32> vector_texture_cache;
        uts::hashmap<uts::pair<uts::str, UFont>, uts::u32> font_texture_cache;

        auto update_push_constant() -> void;
        auto push_objects(uts::vec<UObject>& objects, uts::u32& render_index, uts::f32 parent_width, uts::f32 parent_height) -> void;
        auto update_scene_objects() -> void;

    public:
        auto set_timer(const TickTimer& timer) -> void;
        auto dirtied(bool data_dirty = false) -> void;

    private:
        UINT window_dpi;
        HWND window_hwnd = nullptr;
        bool window_titled_state = false;
        bool window_resizable_state = false;
        uts::u32 window_current_width;
        uts::u32 window_current_height;

        auto create_window(const UWindowInfo& window_info) -> void;

    public:
        auto titled() const -> bool;
        auto resizable() const -> bool;
        auto dpi() const -> UINT;
        auto exec() -> int;
        auto show() -> void;
        auto hide() -> void;
        auto exit() -> void;
        auto rect() -> URect;
        auto set_rect(const URect& rect) -> void;
        static auto init() -> void;

    private:
        VkInstance vulkan_instance;
        VkDebugUtilsMessengerEXT debug_messenger = VK_NULL_HANDLE;
        VkPhysicalDevice physical_device = VK_NULL_HANDLE;
        __uii::vkclses::PhysicalDeviceInfos physical_device_infos;
        VkDevice logical_device;
        __uii::vkclses::DeviceQueues device_queues;
        VkSurfaceKHR window_surface;
        VkSwapchainKHR swapchain;
        uts::vec<VkImage> swapchain_images;
        uts::vec<VkImageView> swapchain_image_views;
        VkFormat swapchain_image_format;
        VkExtent2D swapchain_extent;
        VkRenderPass render_pass;
        VkPipelineLayout pipeline_layout;
        VkPipeline graphics_pipeline;
        uts::vec<VkFramebuffer> swapchain_frame_buffers;
        VkCommandPool command_pool;
        uts::vec<VkCommandBuffer> command_buffers;
        uts::vec<VkSemaphore> image_available_semaphores;
        uts::vec<VkSemaphore> render_finished_semaphores;
        uts::vec<VkFence> in_flight_fences;
        uts::size max_frames_in_flight;
        uts::u32 current_frame = 0;
        __uii::vkclses::Buffer staging_buffer;
        __uii::vkclses::Buffer vertex_buffer;
        __uii::vkclses::Buffer index_buffer;
        VkDescriptorSetLayout descriptor_set_layout;
        uts::vec<UPixmap> texture_image_pixmaps;
        bool enabled_debug = false;
        VkDescriptorPool descriptor_pool;
        VkDescriptorSet descriptor_set;
        uts::vec<__uii::vkclses::TextureImage> texture_images;
        VkDeviceMemory texture_image_memory;
        VkSampler texture_image_sampler;
        uts::str cache_file_path;
        uts::u32 max_texture_count;
        uts::str application_id;
        uts::u32 application_version;
        uts::u64 staging_buffer_size;
        bool data_dirty = false;

        auto create_vulkan_objects(const UAppInfo& application_info, const URenderInfo& render_info) -> void;
        auto destroy_vulkan_objects() -> void;
        auto record_command_buffer(VkCommandBuffer command_buffer, uts::u32 image_index) -> void;
        auto draw_frame() -> void;
        auto create_vulkan_instance() -> void;
        auto check_validation_layer_support() -> bool;
        auto create_debug_messenger() -> void;
        auto destroy_debug_utils_messenger() -> void;
        auto populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT& debug_utils_messenger_create_info) -> void;
        auto select_physical_device() -> void;
        auto create_logical_device() -> void;
        auto create_window_surface() -> void;
        auto create_swapchain() -> void;
        auto destroy_swapchain() -> void;
        auto recreate_swapchain() -> void;
        auto get_swapchain_images() -> void;
        auto create_swapchain_image_views() -> void;
        auto create_shader_module(unsigned char* code, unsigned int length) -> VkShaderModule;
        auto populate_pipeline_shader_stage_create_info(VkPipelineShaderStageCreateInfo& create_info, VkShaderModule shader_module, VkShaderStageFlagBits stage) -> void;
        auto create_graphics_pipeline() -> void;
        auto create_render_pass() -> void;
        auto create_swapchain_frame_buffers() -> void;
        auto create_command_pool() -> void;
        auto create_command_buffers() -> void;
        auto create_sync_objects() -> void;
        auto create_staging_buffer() -> void;
        auto create_vertex_buffer() -> void;
        auto create_index_buffer() -> void;
        auto update_object_buffers() -> void;
        auto create_descriptor_set_layout() -> void;
        auto create_descriptor_pool() -> void;
        auto create_descriptor_set() -> void;
        auto map_texture_images() -> void;
        auto create_texture_image(uts::size index) -> VkMemoryRequirements;
        auto create_texture_image_views() -> void;
        auto create_texture_image_sampler() -> void;
        auto push_texture_image(const UPixmap& pixmap) -> void;
        auto recreate_staging_buffer() -> void;
        auto create_texture_images() -> VkDeviceSize;

    private:
        static auto WINAPI window_process(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) -> LRESULT;
        static VKAPI_ATTR auto VKAPI_CALL debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT message_type, const VkDebugUtilsMessengerCallbackDataEXT* callback_data, [[maybe_unused]] auto* user_data) -> VkBool32;

    public:
        inline UContext() = default;
        UContext(const UAppInfo& application_info, const UWindowInfo& window_info, const URenderInfo& render_info);
        ~UContext();
};
