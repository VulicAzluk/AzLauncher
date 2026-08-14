#pragma once

#include <UApplicationInfo.hpp>
#include <URenderInfo.hpp>
#include <UWindowInfo.hpp>
#include <__UInsideImpl/__VulkanClasses.hpp>
#include <__UInsideImpl/__VulkanShaderDataClasses.hpp>
#include <URenderScene.hpp>
#include <UTimer.hpp>

class UApplication {
    public:
        using RenderCallback = URenderScene(*)(UApplication&);
        using TickTimer = UTimer<UApplication&>;

    private:
        TickTimer tick_timer;
        RenderCallback render_callback;
        URenderScene current_render_scene;
        uts::vec<__uii::vsdces::Vertex2D> render_vertices;
        uts::vec<uts::u32> render_indices;
        __uii::vsdces::PushConstant push_constant;
        UColor current_background_color;
        bool render_dirty = false;

        auto update_push_constant() -> void;
        auto push_objects(uts::vec<UObject>& objects, uts::u32& render_index) -> void;
        auto update_scene_objects() -> void;

    public:
        auto set_tick_timer(const TickTimer& timer) -> void;
        auto emit_dirty() -> void;

    private:
        UINT window_dpi;
        HWND window_hwnd = nullptr;
        bool window_titled_state = false;
        bool window_resizable_state = false;
        uts::u32 window_current_width;
        uts::u32 window_current_height;

        auto create_window(const UWindowInfo& window_info) -> void;

    public:
        auto window_titled() const -> bool;
        auto window_resizable() const -> bool;
        auto get_window_dpi() const -> UINT;
        auto execute() -> int;
        auto show_window() -> void;
        auto hide_window() -> void;
        auto close_window() -> void;
        auto get_window_rect() -> URect;
        auto set_window_rect(const URect& rect) -> void;
        auto add_attribute(UWindowAttribute attribute) -> void;
        auto remove_attribute(UWindowAttribute attribute) -> void;
        static auto initialize() -> void;

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

        auto create_vulkan_objects(const UApplicationInfo& application_info, const RenderCallback& render_callback) -> void;
        auto destroy_vulkan_objects() -> void;
        auto record_command_buffer(VkCommandBuffer command_buffer, uts::u32 image_index) -> void;
        auto draw_frame() -> void;
        auto create_vulkan_instance(const UApplicationInfo& application_info) -> void;
        auto check_validation_layer_support() -> bool;
        auto create_debug_messenger(const UApplicationInfo& application_info) -> void;
        auto destroy_debug_utils_messenger() -> void;
        auto populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT& debug_utils_messenger_create_info) -> void;
        auto select_physical_device() -> void;
        auto get_device_score(const __uii::vkclses::PhysicalDeviceInfos& device_infos) -> uts::u32;
        auto create_logical_device() -> void;
        auto create_window_surface() -> void;
        auto create_swapchain() -> void;
        auto destroy_swapchain() -> void;
        auto recreate_swapchain() -> void;
        auto get_swapchain_images() -> void;
        auto create_swapchain_image_views() -> void;
        auto create_shader_module(unsigned char* code, unsigned int length) -> VkShaderModule;
        auto populate_pipeline_shader_stage_create_info(VkPipelineShaderStageCreateInfo& create_info, VkShaderModule shader_module, VkShaderStageFlagBits stage) -> void;
        auto create_graphics_pipeline(const UApplicationInfo& application_info) -> void;
        auto create_render_pass() -> void;
        auto create_swapchain_frame_buffers() -> void;
        auto create_command_pool() -> void;
        auto create_command_buffers() -> void;
        auto create_sync_objects() -> void;
        auto create_staging_buffer() -> void;
        auto create_vertex_buffer() -> void;
        auto create_index_buffer() -> void;
        auto update_object_buffers() -> void;

    private:
        static auto WINAPI window_process(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) -> LRESULT;
        static VKAPI_ATTR auto VKAPI_CALL debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT message_type, const VkDebugUtilsMessengerCallbackDataEXT* callback_data, [[maybe_unused]] auto* user_data) -> VkBool32;

    public:
        inline UApplication() = default;
        UApplication(const UApplicationInfo& application_info, const UWindowInfo& window_info, const URenderInfo& render_info);
        ~UApplication();
};