#pragma once

#include <URenderInfo.hpp>
#include <UTimer.hpp>
#include <URenderScene.hpp>
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <__UImpl/__VulkanClasses.hpp>
#include <__UImpl/__VulkanShaderDataClasses.hpp>
#include <__UImpl/__VulkanAlgorithm.hpp>
#include <__UImpl/__VulkanRequirements.hpp>
#include <URect.hpp>
#include <ULogger.hpp>
#include <UColor.hpp>
#include <UObject.hpp>
#include <UWindowInfo.hpp>
#include <UApplicationInfo.hpp>
#include <UWin32Function.hpp>
#include <UMacros.hpp>
#include <UTypes.hpp>
#include <basetsd.h>
#include <cstdint>
#include <windef.h>
#include <winuser.h>
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include <vector>
#include <wincodec.h>

#include <Shaders/fragment_shader.inl>
#include <Shaders/vertex_shader.inl>

class UApplication {
    public:
        using RenderCallback = URenderScene(*)(UApplication&);
        using TickTimer = UTimer<UApplication&>;

    private:
        TickTimer timer;

    private:
        RenderCallback render_callback;
        URenderScene current_render_scene;
        uts::vec<__uimpl::vsdces::Vertex2D> render_vertices;
        uts::vec<uts::u32> render_indices;
        __uimpl::vsdces::PushConstant push_constant;
        UColor current_background_color;
        bool render_dirty = false;

        forceinline auto update_push_constant() -> void {
            push_constant.projection = glm::ortho(
                0.0f, static_cast<float>(window_current_width),
                static_cast<float>(window_current_height), 0.0f,
                -1.0f, 1.0f );
        }

        inline auto push_objects(uts::vec<UObject>& objects, uts::u32& render_index) -> void {
            [[maybe_unused]] float window_height = static_cast<float>(window_current_height);
            std::sort(objects.begin(), objects.end(), [](const UObject& object1, const UObject& object2) -> bool { return object1.get_z_index() < object2.get_z_index(); });
            current_background_color = current_render_scene.get_background_color();
            
            for (const auto& object : objects) {
                UColor top_left_color = object.get_top_left_color();
                UColor top_right_color = object.get_top_right_color();
                UColor bottom_left_color = object.get_bottom_left_color();
                UColor bottom_right_color = object.get_bottom_right_color();
                UColor top_left_border_color = object.get_top_left_border_color();
                UColor top_right_border_color = object.get_top_right_border_color();
                UColor bottom_left_border_color = object.get_bottom_left_border_color();
                UColor bottom_right_border_color = object.get_bottom_right_border_color();
                float top_left_corner_radius = UWin32Function::logical_pixels_to_physical(object.get_top_left_corner_radius(), window_dpi);
                float top_right_corner_radius = UWin32Function::logical_pixels_to_physical(object.get_top_right_corner_radius(), window_dpi);
                float bottom_left_corner_radius = UWin32Function::logical_pixels_to_physical(object.get_bottom_left_corner_radius(), window_dpi);
                float bottom_right_corner_radius = UWin32Function::logical_pixels_to_physical(object.get_bottom_right_corner_radius(), window_dpi);
                float top_left_border_width = UWin32Function::logical_pixels_to_physical(object.get_top_left_border_width(), window_dpi);
                float top_right_border_width = UWin32Function::logical_pixels_to_physical(object.get_top_right_border_width(), window_dpi);
                float bottom_left_border_width = UWin32Function::logical_pixels_to_physical(object.get_bottom_left_border_width(), window_dpi);
                float bottom_right_border_width = UWin32Function::logical_pixels_to_physical(object.get_bottom_right_border_width(), window_dpi);
                URect rect = object.get_rect();
                float x = UWin32Function::logical_pixels_to_physical(rect.get_x(), window_dpi);
                float y = UWin32Function::logical_pixels_to_physical(rect.get_y(), window_dpi);
                float width = UWin32Function::logical_pixels_to_physical(rect.get_width(), window_dpi);
                float height = UWin32Function::logical_pixels_to_physical(rect.get_height(), window_dpi);
                glm::vec2 half_size = {width * 0.5f, height * 0.5f};
                glm::vec2 center = {x + half_size.x + 0.5f, window_height - (y + half_size.y) + 0.5f};
                glm::vec2 top_left = center - half_size;
                glm::vec2 top_right = {center.x + half_size.x, center.y - half_size.y};
                glm::vec2 bottom_right = center + half_size;
                glm::vec2 bottom_left = {center.x - half_size.x, center.y + half_size.y};
                glm::vec4 bottom_right_color_vec = __uimpl::vkalg::color_to_vec4(bottom_right_color);
                glm::vec4 bottom_left_color_vec = __uimpl::vkalg::color_to_vec4(bottom_left_color);
                glm::vec4 top_left_color_vec = __uimpl::vkalg::color_to_vec4(top_left_color);
                glm::vec4 top_right_color_vec = __uimpl::vkalg::color_to_vec4(top_right_color);
                glm::vec4 bottom_right_border_color_vec = __uimpl::vkalg::color_to_vec4(bottom_right_border_color);
                glm::vec4 bottom_left_border_color_vec = __uimpl::vkalg::color_to_vec4(bottom_left_border_color);
                glm::vec4 top_left_border_color_vec = __uimpl::vkalg::color_to_vec4(top_left_border_color);
                glm::vec4 top_right_border_color_vec = __uimpl::vkalg::color_to_vec4(top_right_border_color);


                render_vertices.insert(render_vertices.end(), {
                    {top_right, bottom_right_color_vec, bottom_right_corner_radius, half_size, bottom_right_border_width, bottom_right_border_color_vec, center},
                    {top_left, bottom_left_color_vec, bottom_left_corner_radius, half_size, bottom_left_border_width, bottom_left_border_color_vec, center},
                    {bottom_left, top_left_color_vec, top_left_corner_radius, half_size, top_left_border_width, top_left_border_color_vec, center},
                    {bottom_right, top_right_color_vec, top_right_corner_radius, half_size, top_right_border_width, top_right_border_color_vec, center}});
                render_indices.insert(render_indices.end(), {
                    render_index, render_index + 1, render_index + 2,
                    render_index + 2, render_index + 3, render_index});
                render_index += 4;

                
                uts::vec<UObject> child_objects = object.get_objects();
                if (!child_objects.empty()) push_objects(child_objects, render_index);
            }
        }

        inline auto update_scene_objects() -> void {
            uts::u32 render_index = 0;
            render_vertices.clear();
            render_indices.clear();
            current_render_scene = render_callback(*this);
            uts::vec<UObject> objects = current_render_scene.get_objects();

            push_objects(objects, render_index);
        }

    public:
        inline auto set_tick_timer(const TickTimer& timer) -> void { this->timer = timer; }
        forceinline auto emit_dirty() -> void { render_dirty = true; }

    private:
        UINT window_dpi;
        HWND window_hwnd = nullptr;
        bool window_titled_state = false;
        bool window_resizable_state = false;
        uts::u32 window_current_width;
        uts::u32 window_current_height;

    public:
        forceinline auto window_titled() const -> bool { return window_titled_state; }
        forceinline auto window_resizable() const -> bool { return window_resizable_state; }
        forceinline auto get_window_dpi() const -> UINT { return window_dpi; }

    private:
        inline static auto WINAPI window_process(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) -> LRESULT {
            switch (message) {
                case WM_NCCREATE: {
                    auto* create_structure = reinterpret_cast<CREATESTRUCTW*>(lparam);
                    auto* application = static_cast<UApplication*>(create_structure->lpCreateParams);

                    SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(application));
                    return DefWindowProcW(hwnd, message, wparam, lparam);
                } case WM_DESTROY: {
                    PostQuitMessage(0);
                    return 0;
                } case WM_NCDESTROY: {
                    SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(nullptr));
                    return DefWindowProcW(hwnd, message, wparam, lparam);
                } case WM_NCHITTEST: {
                    auto* application = reinterpret_cast<UApplication*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
                    if (!application) return DefWindowProcW(hwnd, message, wparam, lparam);
                    if (!application->window_resizable() || IsZoomed(hwnd)) return HTCLIENT;

                    POINT cursor_position; GetCursorPos(&cursor_position);
                    RECT window_rect; GetWindowRect(hwnd, &window_rect);
                    int frame = GetSystemMetricsForDpi(SM_CYFRAME, application->get_window_dpi());

                    // Top edge
                    if (cursor_position.y >= window_rect.top - frame && cursor_position.y < window_rect.top + frame)
                        return HTTOP;

                    return DefWindowProcW(hwnd, message, wparam, lparam); // Another edges and "corner" cases
                } case WM_NCCALCSIZE: {
                    auto* application = reinterpret_cast<UApplication*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
                    if (!application || application->window_titled()) return DefWindowProcW(hwnd, message, wparam, lparam);

                    int caption_height = GetSystemMetricsForDpi(SM_CYCAPTION, application->get_window_dpi());
                    int border_size = GetSystemMetricsForDpi(SM_CXPADDEDBORDER, application->get_window_dpi());

                    /* Move the top of the client area to the top of the window
                       to cover the titlebar */
                    auto* params = reinterpret_cast<NCCALCSIZE_PARAMS*>(lparam);
                    params->rgrc[0].top -= caption_height + border_size*2 + 1;

                    return DefWindowProcW(hwnd, WM_NCCALCSIZE, wparam, lparam);
                } case WM_SIZING: {
                    auto* application = reinterpret_cast<UApplication*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
                    if (!application) return DefWindowProcW(hwnd, message, wparam, lparam);

                    RECT rect; GetClientRect(application->window_hwnd, &rect);
                    application->window_current_width = rect.right - rect.left;
                    application->window_current_height = rect.bottom - rect.top;

                    application->render_dirty = true;
                    application->update_push_constant();

                    InvalidateRect(hwnd, nullptr, FALSE);
                    UpdateWindow(hwnd);

                    return 0;
                } case WM_PAINT: {
                    auto* application = reinterpret_cast<UApplication*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
                    if (!application) return DefWindowProcW(hwnd, message, wparam, lparam);

                    application->draw_frame();

                    ValidateRect(hwnd, nullptr);
                    return 0;
                }
            }
            return DefWindowProcW(hwnd, message, wparam, lparam);
        };

        inline static VKAPI_ATTR auto VKAPI_CALL debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT message_type, const VkDebugUtilsMessengerCallbackDataEXT* callback_data, [[maybe_unused]] auto* user_data) -> VkBool32 {
            switch (message_severity) {
                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
                    ULogger::ulixwarn(callback_data->pMessage);
                    break;
                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
                    ULogger::ulixerr(callback_data->pMessage);
                default: break;
            }

            return VK_FALSE;
        }

    private:
        inline auto create_window(const UWindowInfo& window_info) -> void {
            uts::str window_class_name = window_info.get_window_class_name();
            uts::str window_title = window_info.get_window_title();
            uts::wstr window_wide_class_name = uts::wstr(window_class_name.begin(), window_class_name.end());
            uts::wstr window_wide_title = uts::wstr(window_title.begin(), window_title.end());
            URect window_rect = window_info.get_initial_window_rect();
            uts::u32 physical_window_width = UWin32Function::logical_pixels_to_physical(window_rect.get_width());
            uts::u32 physical_window_height = UWin32Function::logical_pixels_to_physical(window_rect.get_height());
            uts::i32 physical_window_x = UWin32Function::logical_pixels_to_physical(window_rect.get_x());
            uts::i32 physical_window_y = UWin32Function::logical_pixels_to_physical(window_rect.get_y());

            window_current_width = physical_window_width;
            window_current_height = physical_window_height;

            DWORD window_style = WS_OVERLAPPED |  WS_CAPTION |  WS_THICKFRAME |  WS_MINIMIZEBOX |  WS_MAXIMIZEBOX;
            if (window_info.get_window_attributes() & UWindowAttribute::Titled) {
                window_titled_state = true;
            } else if (!(window_info.get_window_attributes() & UWindowAttribute::Bordered)) {
                window_style &= ~WS_BORDER;
                window_resizable_state = false;
            }

            if (window_info.get_window_attributes() & UWindowAttribute::Resizable) {
                window_resizable_state = true;
            } else {
                window_style &= ~WS_THICKFRAME;
                window_style &= ~WS_MAXIMIZEBOX;
            }

            WNDCLASSEXW wcex{};
            wcex.cbSize = sizeof(WNDCLASSEXW);
            wcex.style = CS_HREDRAW | CS_VREDRAW;
            wcex.lpfnWndProc = window_process;
            wcex.hInstance = UWin32Function::hinstance;
            wcex.hCursor = LoadCursorW(nullptr, UWin32Function::make_int_resource(32512));
            wcex.lpszClassName = window_wide_class_name.c_str();
            if (!RegisterClassExW(&wcex))
                ULogger::ulixerr("Failed to register window class, error code: {}", GetLastError());

            window_hwnd = CreateWindowExW(
                0, window_wide_class_name.c_str(), window_wide_title.c_str(),
                window_style, physical_window_x, physical_window_y, physical_window_width, physical_window_height,
                nullptr, nullptr, UWin32Function::hinstance, this);
            if (!window_hwnd) ULogger::ulixerr("Failed to create window, error code: {}", GetLastError());

            window_dpi = GetDpiForWindow(window_hwnd);
            SetWindowLongPtrW(window_hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
            SetWindowPos(window_hwnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED); // Update window
        }

    public:
        UApplication() = default;
        UApplication(const UApplicationInfo& application_info, const UWindowInfo& window_info, const URenderInfo& render_info) {
            create_window(window_info);
            create_vulkan_objects(application_info, reinterpret_cast<RenderCallback>(render_info.get_render_callback()));
        }
        ~UApplication() { destroy_vulkan_objects(); }

        inline auto execute() -> int {
            MSG msg{}; while (true) {
                if (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
                    if (msg.message == WM_QUIT) return msg.wParam;
                    DispatchMessageW(&msg);
                } else draw_frame();
            }

            return 0;
        }

        inline auto show_window() -> void {
            ShowWindow(window_hwnd, SW_SHOW);
            UpdateWindow(window_hwnd);
        }

        inline auto hide_window() -> void {
            ShowWindow(window_hwnd, SW_HIDE);
            UpdateWindow(window_hwnd);
        }

        inline auto get_window_rect() -> URect {
            RECT window_rect{};
            GetWindowRect(window_hwnd, &window_rect);
            return URect(window_rect.left, window_rect.top,
                         UWin32Function::physical_pixels_to_logical(window_rect.right - window_rect.left),
                         UWin32Function::physical_pixels_to_logical(window_rect.bottom - window_rect.top));
        }

        inline auto set_window_rect(const URect& rect) -> void {
            uts::i32 physical_x = UWin32Function::logical_pixels_to_physical(rect.get_x());
            uts::i32 physical_y = UWin32Function::logical_pixels_to_physical(rect.get_y());
            uts::u32 physical_width = UWin32Function::logical_pixels_to_physical(rect.get_width());
            uts::u32 physical_height = UWin32Function::logical_pixels_to_physical(rect.get_height());

            MoveWindow(window_hwnd, physical_x, physical_y, physical_width, physical_height, TRUE);
        }

        inline auto close_window() -> void {
            PostMessageW(window_hwnd, WM_CLOSE, 0, 0);
        }

        inline auto add_attribute(UWindowAttribute attribute) -> void {
            switch (attribute) {
                case UWindowAttribute::Resizable: {
                    window_resizable_state = true;

                    DWORD style = GetWindowLongPtrW(window_hwnd, GWL_STYLE);
                    style |= WS_THICKFRAME | WS_MAXIMIZEBOX;

                    SetWindowLongPtrW(window_hwnd, GWL_STYLE, style);
                    break;
                } case UWindowAttribute::Titled: {
                    window_titled_state = true;
                    break;
                } case UWindowAttribute::Bordered: {
                    DWORD style = GetWindowLongPtrW(window_hwnd, GWL_STYLE);
                    style |= WS_BORDER;

                    SetWindowLongPtrW(window_hwnd, GWL_STYLE, style);
                    break;
                } case UWindowAttribute::Normal: {
                    add_attribute(UWindowAttribute::Resizable);
                    add_attribute(UWindowAttribute::Titled);
                    add_attribute(UWindowAttribute::Bordered);
                }
            }
            SetWindowPos(window_hwnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED); // Update window
        }

        inline auto remove_attribute(UWindowAttribute attribute) -> void {
            switch (attribute) {
                case UWindowAttribute::Resizable: {
                    window_resizable_state = false;

                    DWORD style = GetWindowLongPtrW(window_hwnd, GWL_STYLE);
                    style &= ~WS_THICKFRAME;
                    style &= ~WS_MAXIMIZEBOX;

                    SetWindowLongPtrW(window_hwnd, GWL_STYLE, style);
                    break;
                } case UWindowAttribute::Titled: {
                    window_titled_state = false;
                    break;
                } case UWindowAttribute::Bordered: {
                    DWORD style = GetWindowLongPtrW(window_hwnd, GWL_STYLE);
                    style &= ~WS_BORDER;

                    SetWindowLongPtrW(window_hwnd, GWL_STYLE, style);
                    break;
                } case UWindowAttribute::Normal: {
                    remove_attribute(UWindowAttribute::Resizable);
                    remove_attribute(UWindowAttribute::Titled);
                    remove_attribute(UWindowAttribute::Bordered);
                }
            }
            SetWindowPos(window_hwnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED); // Update window
        }

    public:
        inline static auto initialize() -> void {
            if (!SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2))
            ULogger::ulixerr("Failed to set DPI awareness, error code: {}", GetLastError());
        }

    private:
        VkInstance vulkan_instance;
        VkDebugUtilsMessengerEXT debug_messenger = VK_NULL_HANDLE;
        VkPhysicalDevice physical_device = VK_NULL_HANDLE;
        __uimpl::vkclses::PhysicalDeviceInfos physical_device_infos;
        VkDevice logical_device;
        __uimpl::vkclses::DeviceQueues device_queues;
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

        __uimpl::vkclses::Buffer staging_buffer;
        __uimpl::vkclses::Buffer vertex_buffer;
        __uimpl::vkclses::Buffer index_buffer;

    private:
        inline auto create_vulkan_instance(const UApplicationInfo& application_info) -> void {
            VkApplicationInfo vulkan_application_info{};
            vulkan_application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            vulkan_application_info.pApplicationName = application_info.get_application_id().c_str();
            vulkan_application_info.applicationVersion = application_info.get_application_version();
            vulkan_application_info.pEngineName = "No Engine";
            vulkan_application_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
            vulkan_application_info.apiVersion = VK_API_VERSION_1_0;
            VkInstanceCreateInfo vulkan_instance_create_info{};
            vulkan_instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            vulkan_instance_create_info.pApplicationInfo = &vulkan_application_info;
            vulkan_instance_create_info.enabledLayerCount = 0;
            vulkan_instance_create_info.ppEnabledLayerNames = __uimpl::vkreqs::required_validation_layer;
            vulkan_instance_create_info.enabledExtensionCount = 2;
            vulkan_instance_create_info.ppEnabledExtensionNames = __uimpl::vkreqs::required_extensions;
            VkDebugUtilsMessengerCreateInfoEXT debug_utils_messenger_create_info;

            if (application_info.get_enabled_vulkan_debug()) {
                if (!check_validation_layer_support())
                    ULogger::ulixerr("Requested validation layers not available");

                vulkan_instance_create_info.enabledLayerCount++;
                vulkan_instance_create_info.enabledExtensionCount++;

                debug_utils_messenger_create_info = {};
                debug_utils_messenger_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
                debug_utils_messenger_create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
                debug_utils_messenger_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
                debug_utils_messenger_create_info.pfnUserCallback = debug_callback;
                vulkan_instance_create_info.pNext = &debug_utils_messenger_create_info;
            }

            switch (vkCreateInstance(&vulkan_instance_create_info, VK_NULL_HANDLE, &vulkan_instance)) {
                case VK_SUCCESS: return;
                case VK_ERROR_INCOMPATIBLE_DRIVER:
                    ULogger::ulixerr("Failed to create vulkan instance: Incompatible driver");
                case VK_ERROR_EXTENSION_NOT_PRESENT:
                    ULogger::ulixerr("Failed to create vulkan instance: Extension not present");
                default:
                    ULogger::ulixerr("Failed to create vulkan instance: Unknown type error");
            }
        }

        inline auto check_validation_layer_support() -> bool {
            uts::u32 layer_count = 0;
            vkEnumerateInstanceLayerProperties(&layer_count, VK_NULL_HANDLE);
            uts::vec<VkLayerProperties> available_layers = uts::vec<VkLayerProperties>(layer_count);
            vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

            for (const auto& layer_properties : available_layers) {
                if (std::strcmp("VK_LAYER_KHRONOS_validation", layer_properties.layerName) == 0)
                    return true;
            }
            return false;
        }
    
        inline auto create_debug_messenger(const UApplicationInfo& application_info) -> void {
            if (!application_info.get_enabled_vulkan_debug())
                return;

            VkDebugUtilsMessengerCreateInfoEXT debug_utils_messenger_create_info;
            populate_debug_messenger_create_info(debug_utils_messenger_create_info);

            auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(vulkan_instance, "vkCreateDebugUtilsMessengerEXT"));
            if (func == VK_NULL_HANDLE)
                ULogger::ulixerr("Failed to get function pointer vkCreateDebugUtilsMessengerEXT from Vulkan instance");
            if (func(vulkan_instance, &debug_utils_messenger_create_info, VK_NULL_HANDLE, &debug_messenger) != VK_SUCCESS)
                ULogger::ulixerr("Failed to create debug messenger");
        }

        inline auto destroy_debug_utils_messenger() -> void {
            if (debug_messenger == VK_NULL_HANDLE)
                return;

            auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(vulkan_instance, "vkDestroyDebugUtilsMessengerEXT");
            if (func == VK_NULL_HANDLE)
                ULogger::ulixerr("Failed to get function pointer vkDestroyDebugUtilsMessengerEXT from Vulkan instance");
            func(vulkan_instance, debug_messenger, VK_NULL_HANDLE);
        }

        inline static auto populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT& debug_utils_messenger_create_info) -> void {
            debug_utils_messenger_create_info = {};
            debug_utils_messenger_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            debug_utils_messenger_create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
            debug_utils_messenger_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            debug_utils_messenger_create_info.pfnUserCallback = debug_callback;
        }

        inline auto select_physical_device() -> void {
            uts::u32 physical_device_count = 0;
            vkEnumeratePhysicalDevices(vulkan_instance, &physical_device_count, VK_NULL_HANDLE);
            if (physical_device_count == 0) ULogger::ulixerr("Failed to find GPUs with Vulkan support");
            uts::vec<VkPhysicalDevice> physical_devices = uts::vec<VkPhysicalDevice>(physical_device_count);
            vkEnumeratePhysicalDevices(vulkan_instance, &physical_device_count, physical_devices.data());

            VkPhysicalDevice alternative_device = VK_NULL_HANDLE;
            __uimpl::vkclses::PhysicalDeviceInfos alternative_device_infos;
            uts::u32 last_discrete_gpu_score = 0;
            uts::u32 last_alternative_device_score = 0;

            for (const auto& device : physical_devices) {
                __uimpl::vkclses::PhysicalDeviceInfos device_infos = __uimpl::vkclses::PhysicalDeviceInfos(device, window_surface);
                if (!device_infos.is_suitable()) continue;
                uts::u32 score = get_device_score(device_infos);
                if (device_infos.match_gpu_type(VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) && score > last_discrete_gpu_score) {
                    last_discrete_gpu_score = score;
                    physical_device = device;
                    physical_device_infos = device_infos;
                } else if (device_infos.match_gpu_type(VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) && score > last_alternative_device_score) {
                    last_alternative_device_score = score;
                    alternative_device = device;
                    alternative_device_infos = device_infos;
                }
            }

            if (physical_device != VK_NULL_HANDLE) return;
            if (alternative_device == VK_NULL_HANDLE) ULogger::ulixerr("Failed to pick a suitable GPU");
            physical_device = alternative_device;
            physical_device_infos = alternative_device_infos;
        }

        inline auto get_device_score(const __uimpl::vkclses::PhysicalDeviceInfos& device_infos) -> uts::u32 {
            uts::u64 device_local_bytes = 0;
            for (uts::u32 index = 0; index < device_infos.memory_properties.memoryHeapCount; ++index) {
                if (!(device_infos.memory_properties.memoryHeaps[index].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)) continue;
                device_local_bytes += device_infos.memory_properties.memoryHeaps[index].size;
            }

            return device_infos.properties.limits.maxImageDimension2D +
                device_infos.properties.limits.maxUniformBufferRange / (1 << 20) +
                device_infos.properties.limits.maxComputeWorkGroupCount[0] / 65536 +
                device_infos.properties.limits.maxComputeWorkGroupInvocations / 1024 +
                device_infos.properties.limits.maxBoundDescriptorSets +
                device_infos.properties.limits.maxColorAttachments +
                device_local_bytes / (1ULL << 30);
        }

        inline auto create_logical_device() -> void {
            uts::vec<VkDeviceQueueCreateInfo> device_queue_create_infos;
            uts::set<uts::u32> unique_queue_family_indices = physical_device_infos.get_unique_queue_family_indices();
            float queue_priorities = 1.0f;
            for (uts::u32 index : unique_queue_family_indices) {
                VkDeviceQueueCreateInfo device_queue_create_info{};
                device_queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                device_queue_create_info.queueFamilyIndex = index;
                device_queue_create_info.queueCount = 1;
                device_queue_create_info.pQueuePriorities = &queue_priorities;
                device_queue_create_infos.push_back(device_queue_create_info);
            }

            VkPhysicalDeviceFeatures physical_device_features{};
            VkDeviceCreateInfo device_create_info{};
            device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            device_create_info.queueCreateInfoCount = static_cast<uts::u32>(device_queue_create_infos.size());
            device_create_info.pQueueCreateInfos = device_queue_create_infos.data();
            device_create_info.pEnabledFeatures = &physical_device_features;
            device_create_info.enabledExtensionCount = 1;
            device_create_info.ppEnabledExtensionNames = __uimpl::vkreqs::enabled_extension;
            if (vkCreateDevice(physical_device, &device_create_info, VK_NULL_HANDLE, &logical_device) != VK_SUCCESS)
                ULogger::ulixerr("Failed to create logical device");

            device_queues = __uimpl::vkclses::DeviceQueues(logical_device, physical_device_infos);
        }

        inline auto create_window_surface() -> void {
            VkWin32SurfaceCreateInfoKHR win32_surface_create_info{};
            win32_surface_create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
            win32_surface_create_info.hwnd = window_hwnd;
            win32_surface_create_info.hinstance = UWin32Function::hinstance;

            if (vkCreateWin32SurfaceKHR(vulkan_instance, &win32_surface_create_info, VK_NULL_HANDLE, &window_surface) != VK_SUCCESS)
                ULogger::ulixerr("Failed to create win32 window surface");
        }

        inline auto create_swapchain() -> void {
            __uimpl::vkclses::SwapchainSupportDetails& swapchain_support_details = physical_device_infos.swapchain_support_details;
            __uimpl::vkclses::OptionalQueueFamilyIndices& optional_queue_family_indices = physical_device_infos.queue_family_indices;
            VkSurfaceFormatKHR surface_format = swapchain_support_details.select_surface_format();
            VkExtent2D extent = swapchain_support_details.select_extent(window_hwnd);
            uts::u32 queue_family_indices[] = { optional_queue_family_indices.graphics_queue_family_index.value(), optional_queue_family_indices.present_queue_family_index.value() };
            uts::u32 min_image_count = swapchain_support_details.surface_capabilities.minImageCount + 1;
            if (swapchain_support_details.surface_capabilities.maxImageCount > 0 && min_image_count > swapchain_support_details.surface_capabilities.maxImageCount)
                min_image_count = swapchain_support_details.surface_capabilities.maxImageCount;

            VkSwapchainCreateInfoKHR swapchain_create_info{};
            swapchain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            swapchain_create_info.surface = window_surface;
            swapchain_create_info.minImageCount = min_image_count;
            swapchain_create_info.imageFormat = surface_format.format;
            swapchain_create_info.imageColorSpace = surface_format.colorSpace;
            swapchain_create_info.imageExtent = extent;
            swapchain_create_info.imageArrayLayers = 1;
            swapchain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
            swapchain_create_info.preTransform = swapchain_support_details.surface_capabilities.currentTransform;
            swapchain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            swapchain_create_info.presentMode = VK_PRESENT_MODE_FIFO_KHR;
            swapchain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            if (optional_queue_family_indices.graphics_queue_family_index != optional_queue_family_indices.present_queue_family_index) {
                swapchain_create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
                swapchain_create_info.queueFamilyIndexCount = 2;
                swapchain_create_info.pQueueFamilyIndices = queue_family_indices; };
            if (vkCreateSwapchainKHR(logical_device, &swapchain_create_info, VK_NULL_HANDLE, &swapchain) != VK_SUCCESS)
                ULogger::ulixerr("Failed to create swapchain");

            swapchain_image_format = surface_format.format;
            swapchain_extent = extent;

            get_swapchain_images();
        }

        inline auto get_swapchain_images() -> void {
            uts::u32 image_count = 0;
            vkGetSwapchainImagesKHR(logical_device, swapchain, &image_count, VK_NULL_HANDLE);
            swapchain_images.resize(image_count);
            vkGetSwapchainImagesKHR(logical_device, swapchain, &image_count, swapchain_images.data());

            max_frames_in_flight = static_cast<uts::size>(image_count);
        }

        inline auto create_swapchain_image_views() -> void {
            swapchain_image_views.resize(swapchain_images.size());

            for (uts::size index = 0; index < swapchain_images.size(); index++) {
                VkImageViewCreateInfo image_view_create_info{};
                image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
                image_view_create_info.image = swapchain_images[index];
                image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
                image_view_create_info.format = swapchain_image_format;
                image_view_create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
                image_view_create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
                image_view_create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
                image_view_create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
                image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                image_view_create_info.subresourceRange.baseMipLevel = 0;
                image_view_create_info.subresourceRange.levelCount = 1;
                image_view_create_info.subresourceRange.baseArrayLayer = 0;
                image_view_create_info.subresourceRange.layerCount = 1;
                if (vkCreateImageView(logical_device, &image_view_create_info, VK_NULL_HANDLE, &swapchain_image_views[index]) != VK_SUCCESS)
                    ULogger::ulixerr("Failed to create swapchain image views");
            }
        }

        inline auto create_shader_module(unsigned char* code, unsigned int length) -> VkShaderModule {
            VkShaderModuleCreateInfo shader_module_create_info{};
            shader_module_create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
            shader_module_create_info.codeSize = length;
            shader_module_create_info.pCode = reinterpret_cast<uts::u32*>(code);

            VkShaderModule shader_module;
            if (vkCreateShaderModule(logical_device, &shader_module_create_info, VK_NULL_HANDLE, &shader_module) != VK_SUCCESS)
                ULogger::ulixerr("Failed to create shader module");

            return shader_module;
        }

        inline static auto populate_pipeline_shader_stage_create_info(VkPipelineShaderStageCreateInfo& create_info, VkShaderModule shader_module, VkShaderStageFlagBits stage) -> void {
            create_info = {};
            create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            create_info.stage = stage;
            create_info.module = shader_module;
            create_info.pName = "main";
        }

        inline auto create_graphics_pipeline(const UApplicationInfo& application_info) -> void {
            uts::str cache_file_path = application_info.get_pipeline_cache_file_path().get_file_path();

            VkShaderModule fragment_shader_module = create_shader_module(fragment_shader, fragment_shader_length);
            VkShaderModule vertex_shader_module = create_shader_module(vertex_shader, vertex_shader_length);

            VkPipelineShaderStageCreateInfo fragment_shader_stage_create_info{};
            VkPipelineShaderStageCreateInfo vertex_shader_stage_create_info{};
            populate_pipeline_shader_stage_create_info(fragment_shader_stage_create_info, fragment_shader_module, VK_SHADER_STAGE_FRAGMENT_BIT);
            populate_pipeline_shader_stage_create_info(vertex_shader_stage_create_info, vertex_shader_module, VK_SHADER_STAGE_VERTEX_BIT);

            VkPipelineShaderStageCreateInfo pipeline_shader_stage_create_infos[] = { fragment_shader_stage_create_info, vertex_shader_stage_create_info };
            VkPipelineDynamicStateCreateInfo pipeline_dynamic_state_create_info{};
            pipeline_dynamic_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
            pipeline_dynamic_state_create_info.dynamicStateCount = 2;
            pipeline_dynamic_state_create_info.pDynamicStates = __uimpl::vkreqs::dynamic_states;

            VkVertexInputBindingDescription vertex_input_binding_description = __uimpl::vsdces::Vertex2D::get_binding_description();
            uts::arr<VkVertexInputAttributeDescription, 7> vertex_input_attribute_descriptions = __uimpl::vsdces::Vertex2D::get_attribute_descriptions();

            VkPipelineVertexInputStateCreateInfo pipeline_vertex_input_state_create_info{};
            pipeline_vertex_input_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
            pipeline_vertex_input_state_create_info.vertexBindingDescriptionCount = 1;
            pipeline_vertex_input_state_create_info.pVertexBindingDescriptions = &vertex_input_binding_description;
            pipeline_vertex_input_state_create_info.vertexAttributeDescriptionCount = static_cast<uts::u32>(vertex_input_attribute_descriptions.size());
            pipeline_vertex_input_state_create_info.pVertexAttributeDescriptions = vertex_input_attribute_descriptions.data();

            VkPipelineInputAssemblyStateCreateInfo pipeline_input_assembly_state_create_info{};
            pipeline_input_assembly_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
            pipeline_input_assembly_state_create_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
            pipeline_input_assembly_state_create_info.primitiveRestartEnable = VK_FALSE;

            VkViewport viewport{};
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.width = static_cast<float>(swapchain_extent.width);
            viewport.height = static_cast<float>(swapchain_extent.height);
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;

            VkRect2D scissor{};
            scissor.offset = {0, 0};
            scissor.extent = swapchain_extent;

            VkPipelineViewportStateCreateInfo pipeline_viewport_state_create_info{};
            pipeline_viewport_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
            pipeline_viewport_state_create_info.viewportCount = 1;
            pipeline_viewport_state_create_info.pViewports = &viewport;
            pipeline_viewport_state_create_info.scissorCount = 1;
            pipeline_viewport_state_create_info.pScissors = &scissor;

            VkPipelineRasterizationStateCreateInfo pipeline_rasterization_state_create_info{};
            pipeline_rasterization_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
            pipeline_rasterization_state_create_info.depthClampEnable = VK_FALSE;
            pipeline_rasterization_state_create_info.rasterizerDiscardEnable = VK_FALSE;
            pipeline_rasterization_state_create_info.polygonMode = VK_POLYGON_MODE_FILL;
            pipeline_rasterization_state_create_info.lineWidth = 1.0f;
            pipeline_rasterization_state_create_info.cullMode = VK_CULL_MODE_BACK_BIT;
            pipeline_rasterization_state_create_info.frontFace = VK_FRONT_FACE_CLOCKWISE;
            pipeline_rasterization_state_create_info.depthBiasEnable = VK_FALSE;
            pipeline_rasterization_state_create_info.depthBiasConstantFactor = 0.0f;
            pipeline_rasterization_state_create_info.depthBiasClamp = 0.0f;
            pipeline_rasterization_state_create_info.depthBiasSlopeFactor = 0.0f;

            VkPipelineMultisampleStateCreateInfo pipeline_mulisample_state_create_info{};
            pipeline_mulisample_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
            pipeline_mulisample_state_create_info.sampleShadingEnable = VK_FALSE;
            pipeline_mulisample_state_create_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
            pipeline_mulisample_state_create_info.minSampleShading = 1.0f;
            pipeline_mulisample_state_create_info.pSampleMask = VK_NULL_HANDLE;
            pipeline_mulisample_state_create_info.alphaToCoverageEnable = VK_FALSE;
            pipeline_mulisample_state_create_info.alphaToOneEnable = VK_FALSE;

            VkPipelineColorBlendAttachmentState pipeline_color_blend_attachment_state{};
            pipeline_color_blend_attachment_state.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
            pipeline_color_blend_attachment_state.blendEnable = VK_TRUE;
            pipeline_color_blend_attachment_state.colorBlendOp = VK_BLEND_OP_ADD;
            pipeline_color_blend_attachment_state.alphaBlendOp = VK_BLEND_OP_ADD;
            pipeline_color_blend_attachment_state.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
            pipeline_color_blend_attachment_state.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
            pipeline_color_blend_attachment_state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
            pipeline_color_blend_attachment_state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;

            VkPipelineColorBlendStateCreateInfo pipeline_color_blend_state_create_info{};
            pipeline_color_blend_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
            pipeline_color_blend_state_create_info.logicOpEnable = VK_FALSE;
            pipeline_color_blend_state_create_info.logicOp = VK_LOGIC_OP_COPY;
            pipeline_color_blend_state_create_info.attachmentCount = 1;
            pipeline_color_blend_state_create_info.pAttachments = &pipeline_color_blend_attachment_state;
            pipeline_color_blend_state_create_info.blendConstants[0] = 0.0f;
            pipeline_color_blend_state_create_info.blendConstants[1] = 0.0f;
            pipeline_color_blend_state_create_info.blendConstants[2] = 0.0f;
            pipeline_color_blend_state_create_info.blendConstants[3] = 0.0f;

            VkPushConstantRange push_constant_range{};
            push_constant_range.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
            push_constant_range.offset = 0;
            push_constant_range.size = sizeof(__uimpl::vsdces::PushConstant);

            VkPipelineLayoutCreateInfo pipeline_layout_create_info{};
            pipeline_layout_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            pipeline_layout_create_info.setLayoutCount = 0;
            pipeline_layout_create_info.pSetLayouts = VK_NULL_HANDLE;
            pipeline_layout_create_info.pushConstantRangeCount = 1;
            pipeline_layout_create_info.pPushConstantRanges = &push_constant_range;
            if (vkCreatePipelineLayout(logical_device, &pipeline_layout_create_info, VK_NULL_HANDLE, &pipeline_layout) != VK_SUCCESS)
                ULogger::ulixerr("Failed to create pipeline layout");


            __uimpl::vkalg::ensure_cache_exists(cache_file_path);
            uts::vec<char> cache_data = __uimpl::vkalg::read_cache(cache_file_path);
            VkPipelineCacheCreateInfo pipeline_cache_create_info{};
            pipeline_cache_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
            pipeline_cache_create_info.initialDataSize = cache_data.size();
            pipeline_cache_create_info.pInitialData = cache_data.data();
            VkPipelineCache pipeline_cache;
            if (vkCreatePipelineCache(logical_device, &pipeline_cache_create_info, nullptr, &pipeline_cache) != VK_SUCCESS)
                ULogger::ulixerr("Failed to create pipeline cache");

            VkGraphicsPipelineCreateInfo graphics_pipeline_create_info{};
            graphics_pipeline_create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
            graphics_pipeline_create_info.stageCount = 2;
            graphics_pipeline_create_info.pStages = pipeline_shader_stage_create_infos;
            graphics_pipeline_create_info.pVertexInputState = &pipeline_vertex_input_state_create_info;
            graphics_pipeline_create_info.pInputAssemblyState = &pipeline_input_assembly_state_create_info;
            graphics_pipeline_create_info.pViewportState = &pipeline_viewport_state_create_info;
            graphics_pipeline_create_info.pRasterizationState = &pipeline_rasterization_state_create_info;
            graphics_pipeline_create_info.pMultisampleState = &pipeline_mulisample_state_create_info;
            graphics_pipeline_create_info.pDepthStencilState = VK_NULL_HANDLE;
            graphics_pipeline_create_info.pColorBlendState = &pipeline_color_blend_state_create_info;
            graphics_pipeline_create_info.pDynamicState = &pipeline_dynamic_state_create_info;
            graphics_pipeline_create_info.layout = pipeline_layout;
            graphics_pipeline_create_info.renderPass = render_pass;
            graphics_pipeline_create_info.subpass = 0;
            graphics_pipeline_create_info.basePipelineHandle = VK_NULL_HANDLE;
            graphics_pipeline_create_info.basePipelineIndex = -1;
            if (vkCreateGraphicsPipelines(logical_device, pipeline_cache, 1, &graphics_pipeline_create_info, VK_NULL_HANDLE, &graphics_pipeline) != VK_SUCCESS)
                ULogger::ulixerr("Failed to create graphics pipeline");


            size_t cache_data_size;
            uts::vec<char> new_cache_data;
            vkGetPipelineCacheData(logical_device, pipeline_cache, &cache_data_size, nullptr);
            new_cache_data.resize(cache_data_size);
            vkGetPipelineCacheData(logical_device, pipeline_cache, &cache_data_size, new_cache_data.data());
            __uimpl::vkalg::write_cache(cache_file_path, new_cache_data);
            vkDestroyPipelineCache(logical_device, pipeline_cache, nullptr);
            vkDestroyShaderModule(logical_device, fragment_shader_module, VK_NULL_HANDLE);
            vkDestroyShaderModule(logical_device, vertex_shader_module, VK_NULL_HANDLE);
        }

        inline auto create_render_pass() -> void {
            VkAttachmentDescription color_attachment_description{};
            color_attachment_description.format = swapchain_image_format;
            color_attachment_description.samples = VK_SAMPLE_COUNT_1_BIT;
            color_attachment_description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            color_attachment_description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            color_attachment_description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            color_attachment_description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            color_attachment_description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            color_attachment_description.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

            VkAttachmentReference color_attachment_reference{};
            color_attachment_reference.attachment = 0;
            color_attachment_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

            VkSubpassDescription subpass_description{};
            subpass_description.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
            subpass_description.colorAttachmentCount = 1;
            subpass_description.pColorAttachments = &color_attachment_reference;

            VkSubpassDependency subpass_dependency{};
            subpass_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
            subpass_dependency.dstSubpass = 0;
            subpass_dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            subpass_dependency.srcAccessMask = 0;
            subpass_dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            subpass_dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

            VkRenderPassCreateInfo render_pass_info{};
            render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
            render_pass_info.attachmentCount = 1;
            render_pass_info.pAttachments = &color_attachment_description;
            render_pass_info.subpassCount = 1;
            render_pass_info.pSubpasses = &subpass_description;
            render_pass_info.dependencyCount = 1;
            render_pass_info.pDependencies = &subpass_dependency;
            if (vkCreateRenderPass(logical_device, &render_pass_info, VK_NULL_HANDLE, &render_pass) != VK_SUCCESS)
                ULogger::ulixerr("Failed to create render pass");
        }

        inline auto create_swapchain_frame_buffers() -> void {
            swapchain_frame_buffers.resize(swapchain_images.size());

            for (uts::size index = 0; index < swapchain_images.size(); index++) {
                VkFramebufferCreateInfo frame_buffer_create_info{};
                frame_buffer_create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
                frame_buffer_create_info.renderPass = render_pass;
                frame_buffer_create_info.attachmentCount = 1;
                frame_buffer_create_info.pAttachments = &swapchain_image_views[index];
                frame_buffer_create_info.width = swapchain_extent.width;
                frame_buffer_create_info.height = swapchain_extent.height;
                frame_buffer_create_info.layers = 1;
                if (vkCreateFramebuffer(logical_device, &frame_buffer_create_info, VK_NULL_HANDLE, &swapchain_frame_buffers[index]) != VK_SUCCESS)
                    ULogger::ulixerr("Failed to create frame buffers");
            }
        }

        inline auto create_command_pool() -> void {
            VkCommandPoolCreateInfo command_pool_create_info{};
            command_pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            command_pool_create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
            command_pool_create_info.queueFamilyIndex = physical_device_infos.queue_family_indices.graphics_queue_family_index.value();
            if (vkCreateCommandPool(logical_device, &command_pool_create_info, VK_NULL_HANDLE, &command_pool) != VK_SUCCESS)
                ULogger::ulixerr("Failed to create command pool");
        }

        inline auto create_command_buffers() -> void {
            command_buffers.resize(max_frames_in_flight);

            VkCommandBufferAllocateInfo command_buffer_allocate_info{};
            command_buffer_allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            command_buffer_allocate_info.commandPool = command_pool;
            command_buffer_allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            command_buffer_allocate_info.commandBufferCount = static_cast<uts::u32>(command_buffers.size());
            if (vkAllocateCommandBuffers(logical_device, &command_buffer_allocate_info, command_buffers.data()) != VK_SUCCESS)
                ULogger::ulixerr("Failed to allocate command buffer");
        }

        inline auto create_sync_objects() -> void {
            image_available_semaphores.resize(max_frames_in_flight);
            render_finished_semaphores.resize(max_frames_in_flight);
            in_flight_fences.resize(max_frames_in_flight);

            VkSemaphoreCreateInfo semaphore_create_info{};
            semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

            VkFenceCreateInfo fence_create_info{};
            fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

            for (uts::size index = 0; index < max_frames_in_flight; index++) {
                if (vkCreateSemaphore(logical_device, &semaphore_create_info, VK_NULL_HANDLE, &image_available_semaphores[index]) != VK_SUCCESS)
                    ULogger::ulixerr("failed to create sync object: image available semaphore");
                if (vkCreateSemaphore(logical_device, &semaphore_create_info, VK_NULL_HANDLE, &render_finished_semaphores[index]) != VK_SUCCESS)
                    ULogger::ulixerr("failed to create sync object: render finished semaphore");
                if (vkCreateFence(logical_device, &fence_create_info, VK_NULL_HANDLE, &in_flight_fences[index]) != VK_SUCCESS)
                    ULogger::ulixerr("failed to create sync object: in flight fence");
            }
        }

        inline auto destroy_swapchain() -> void {
            for (const auto& frame_buffer : swapchain_frame_buffers)
                vkDestroyFramebuffer(logical_device, frame_buffer, VK_NULL_HANDLE);
            for (const auto& image_view : swapchain_image_views)
                vkDestroyImageView(logical_device, image_view, VK_NULL_HANDLE);
            vkDestroySwapchainKHR(logical_device, swapchain, VK_NULL_HANDLE);
        }

        inline auto recreate_swapchain() -> void {
            RECT window_rect;
            GetWindowRect(window_hwnd, &window_rect);
            uts::u32 width = window_rect.right - window_rect.left;
            uts::u32 height = window_rect.bottom - window_rect.top;
            if (width == 0 || height == 0) return;

            vkDeviceWaitIdle(logical_device);
            destroy_swapchain();

            create_swapchain();
            create_swapchain_image_views();
            create_swapchain_frame_buffers();
        }

        inline auto create_staging_buffer() -> void {
            constexpr VkDeviceSize size = 1 << 22;
            staging_buffer = __uimpl::vkclses::Buffer(
                physical_device, logical_device,  size, 
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
            );
        }

        inline auto create_vertex_buffer() -> void {
            VkDeviceSize size = render_vertices.size() * sizeof(__uimpl::vsdces::Vertex2D);
            vertex_buffer = __uimpl::vkclses::Buffer(
                physical_device, logical_device, size, 
                VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
            );
        }

        inline auto create_index_buffer() -> void {
            VkDeviceSize size = render_indices.size() * sizeof(uts::u32);
            index_buffer = __uimpl::vkclses::Buffer(
                physical_device, logical_device, size, 
                VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, 
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
            );
        }

        inline auto update_object_buffers() -> void {
            staging_buffer.map_memory(logical_device, render_vertices.data(), render_vertices.size() * sizeof(__uimpl::vsdces::Vertex2D));
            staging_buffer.copy_buffer_to(vertex_buffer, logical_device, command_pool, device_queues.graphics_queue, render_vertices.size() * sizeof(__uimpl::vsdces::Vertex2D));
            staging_buffer.map_memory(logical_device, render_indices.data(), render_indices.size() * sizeof(uts::u32));
            staging_buffer.copy_buffer_to(index_buffer, logical_device, command_pool, device_queues.graphics_queue, render_indices.size() * sizeof(uts::u32));
        }

    private:
        inline auto create_vulkan_objects(const UApplicationInfo& application_info, const RenderCallback& render_callback) -> void {
            this->render_callback = render_callback;

            create_vulkan_instance(application_info);
            create_debug_messenger(application_info);
            create_window_surface();
            select_physical_device();
            create_logical_device();
            create_swapchain();
            create_swapchain_image_views();
            create_render_pass();
            create_graphics_pipeline(application_info);
            create_swapchain_frame_buffers();
            create_command_pool();
            update_push_constant();
            update_scene_objects();
            create_staging_buffer();
            create_vertex_buffer();
            create_index_buffer();
            update_object_buffers();
            create_command_buffers();
            create_sync_objects();
        }

        inline auto destroy_vulkan_objects() -> void {
            vkDeviceWaitIdle(logical_device);

            destroy_swapchain();
            index_buffer.release(logical_device);
            vertex_buffer.release(logical_device);
            staging_buffer.release(logical_device);
            for (uts::size index = 0; index < max_frames_in_flight; index++) {
                vkDestroySemaphore(logical_device, image_available_semaphores[index], VK_NULL_HANDLE);
                vkDestroySemaphore(logical_device, render_finished_semaphores[index], VK_NULL_HANDLE);
                vkDestroyFence(logical_device, in_flight_fences[index], VK_NULL_HANDLE);
            }
            vkDestroyCommandPool(logical_device, command_pool, VK_NULL_HANDLE);
            vkDestroyPipeline(logical_device, graphics_pipeline, VK_NULL_HANDLE);
            vkDestroyPipelineLayout(logical_device, pipeline_layout, VK_NULL_HANDLE);
            vkDestroyRenderPass(logical_device, render_pass, VK_NULL_HANDLE);
            vkDestroyDevice(logical_device, VK_NULL_HANDLE);
            destroy_debug_utils_messenger();
            vkDestroySurfaceKHR(vulkan_instance, window_surface, VK_NULL_HANDLE);
            vkDestroyInstance(vulkan_instance, VK_NULL_HANDLE);
        }

    private:
        inline auto record_command_buffer(VkCommandBuffer command_buffer, uts::u32 image_index) -> void {
            VkCommandBufferBeginInfo command_buffer_begin_info{};
            command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            command_buffer_begin_info.flags = 0;
            command_buffer_begin_info.pInheritanceInfo = VK_NULL_HANDLE;
            if (vkBeginCommandBuffer(command_buffer, &command_buffer_begin_info) != VK_SUCCESS)
                ULogger::ulixerr("Failed to begin command buffer");
    
            VkClearValue clear_color = {{{current_background_color.get_red() / 255.0f, current_background_color.get_green() / 255.0f, current_background_color.get_blue() / 255.0f, current_background_color.get_alpha() / 255.0f}}};
            VkRenderPassBeginInfo render_pass_begin_info{};
            render_pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            render_pass_begin_info.renderPass = render_pass;
            render_pass_begin_info.framebuffer = swapchain_frame_buffers[image_index];
            render_pass_begin_info.renderArea.offset = {0, 0};
            render_pass_begin_info.renderArea.extent = swapchain_extent;
            render_pass_begin_info.clearValueCount = 1;
            render_pass_begin_info.pClearValues = &clear_color;
            vkCmdBeginRenderPass(command_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);
            vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline);
    
            VkViewport viewport{};
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.width = static_cast<float>(swapchain_extent.width);
            viewport.height = static_cast<float>(swapchain_extent.height);
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;
            vkCmdSetViewport(command_buffer, 0, 1, &viewport);
    
            VkRect2D scissor{};
            scissor.offset = {0, 0};
            scissor.extent = swapchain_extent;
            vkCmdSetScissor(command_buffer, 0, 1, &scissor);
    
            constexpr VkDeviceSize offsets[] = {0};
            vkCmdBindVertexBuffers(command_buffer, 0, 1, &vertex_buffer.buffer, offsets);
            vkCmdBindIndexBuffer(command_buffer, index_buffer.buffer, 0, VK_INDEX_TYPE_UINT32);
    
            vkCmdPushConstants(command_buffer, pipeline_layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(__uimpl::vsdces::PushConstant), &push_constant);
    
            vkCmdDrawIndexed(command_buffer, static_cast<uts::u32>(render_indices.size()), 1, 0, 0, 0);
            vkCmdEndRenderPass(command_buffer);
            if (vkEndCommandBuffer(command_buffer) != VK_SUCCESS)
                ULogger::ulixerr("Failed to end command buffer");
        }

    
        inline auto draw_frame() -> void {
            vkWaitForFences(logical_device, 1, &in_flight_fences[current_frame], VK_TRUE, UINT64_MAX);
            if (render_dirty) {
                render_dirty = false;
                update_scene_objects();
                update_object_buffers();
            }

            uts::u32 image_index;
            VkResult acquire_result = vkAcquireNextImageKHR(logical_device, swapchain, UINT64_MAX, image_available_semaphores[current_frame], VK_NULL_HANDLE, &image_index);
            if (acquire_result == VK_ERROR_OUT_OF_DATE_KHR) {
                recreate_swapchain();
                return;
            } else if (acquire_result != VK_SUCCESS && acquire_result != VK_SUBOPTIMAL_KHR)
                ULogger::ulixerr("Failed to acquire next image");

            vkResetFences(logical_device, 1, &in_flight_fences[current_frame]);
            vkResetCommandBuffer(command_buffers[current_frame], 0);
            record_command_buffer(command_buffers[current_frame], image_index);

            constexpr VkPipelineStageFlags wait_stages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
            VkSubmitInfo submit_info{};
            submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submit_info.waitSemaphoreCount = 1;
            submit_info.pWaitSemaphores = &image_available_semaphores[current_frame];
            submit_info.pWaitDstStageMask = wait_stages;
            submit_info.commandBufferCount = 1;
            submit_info.pCommandBuffers = &command_buffers[current_frame];
            submit_info.signalSemaphoreCount = 1;
            submit_info.pSignalSemaphores = &render_finished_semaphores[current_frame];
            if (vkQueueSubmit(device_queues.graphics_queue, 1, &submit_info, in_flight_fences[current_frame]) != VK_SUCCESS)
                ULogger::ulixerr("failed to submit queue");

            VkPresentInfoKHR present_info{};
            present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
            present_info.waitSemaphoreCount = 1;
            present_info.pWaitSemaphores = &render_finished_semaphores[current_frame];
            present_info.swapchainCount = 1;
            present_info.pSwapchains = &swapchain;
            present_info.pImageIndices = &image_index;
            vkQueuePresentKHR(device_queues.present_queue, &present_info);

            current_frame = (current_frame + 1) % max_frames_in_flight;
            timer.update(*this);
        }
};
