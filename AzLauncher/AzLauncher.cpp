#include "ApplicationSetup.hpp"

struct Resources {
    // The enum is texture image indices
    enum TextureImages {
        Vulkan
    }; inline static uts::vec<UPixmap> texture_images = {
        UPixmap(UFile("AzLauncher/vulkan.png"))
    };
};

auto render_objects([[maybe_unused]] UApplication& application) -> URenderScene {
    return URenderScene()
        .with_background_color(UColor(11, 45, 14))
        .with_object(UObject()
            .with_z_index(1)
            .with_fill_color(UAnchor::TopRight, UColor(0, 255, 0))
            .with_fill_color(UAnchor::TopLeft, UColor(255, 0, 0))
            .with_fill_color(UAnchor::BottomLeft, UColor(0, 0, 255))
            .with_border_width(UAnchor::AllAnchor, 4)
            .with_border_color(UAnchor::BottomLeft, UColor(255, 0, 0))
            .with_object(UObject()
                .with_fill_color(UAnchor::AllAnchor, UColor(0, 0, 255))
                .with_rect(URect(5, 5, 50, 50))
                .with_fill_color(UAnchor::TopRight, UColor(0, 255, 0))
                .with_fill_color(UAnchor::TopLeft, UColor(255, 0, 0))
                .with_fill_color(UAnchor::BottomLeft, UColor(0, 0, 255))
                .with_fill_color(UAnchor::BottomRight, UColor(255, 255, 255))
                .with_border_width(UAnchor::AllAnchor, 1.5)
                .with_border_color(UAnchor::BottomLeft, UColor(255, 0, 0))
        )).with_object(UObject()
            .with_z_index(0)
            .with_rect(URect(150, 150, 300, 300))
        ).with_object(UObject()
            .with_z_index(0)
            .with_rect(URect(250, 250, 150, 150))
            .with_fill_color(UAnchor::AllAnchor, UColor(255, 255, 0))
        );
}

auto main() -> int {
    ApplicationSetup::initialize(render_objects, Resources::texture_images);
    return ApplicationSetup::execute(UApplication::TickTimer());
}
