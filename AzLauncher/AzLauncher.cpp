#include "ApplicationSetup.hpp"

struct Resources {
    enum TextureImages {
        Vulkan
    }; inline static uts::vec<UPixmap> texture_images = {
        UPixmap(UFile("AzLauncher/vulkan.png"))
    };
};

auto render_objects([[maybe_unused]] UApplication& application) -> URenderScene {
    return URenderScene()
        .background(UColor(11, 45, 14))
        .child(UObject()
            .zindex(1)
            .fill(UAnchor::TopRight, UColor(0, 255, 0))
            .fill(UAnchor::TopLeft, UColor(255, 0, 0))
            .fill(UAnchor::BottomLeft, UColor(0, 0, 255))
            .border(UAnchor::AllAnchor, 4)
            .border(UAnchor::BottomLeft, UColor(255, 0, 0))
            .child(UObject()
                .rect(URect(5, 5, 50, 50))
                .fill(UAnchor::TopRight, UColor(0, 255, 0))
                .fill(UAnchor::TopLeft, UColor(255, 0, 0))
                .fill(UAnchor::BottomLeft, UColor(0, 0, 255))
                .fill(UAnchor::BottomRight, UColor(255, 255, 255))
                .border(UAnchor::AllAnchor, 1.5)
                .border(UAnchor::BottomLeft, UColor(255, 0, 0))
        )).child(UObject()
            .zindex(0)
            .rect(URect(150, 150, 300, 300))
        ).child(UObject()
            .zindex(0)
            .rect(URect(250, 250, 150, 150))
            .fill(UAnchor::AllAnchor, UColor(255, 255, 0))
        );
}

auto main() -> int {
    ApplicationSetup::init(render_objects, Resources::texture_images);
    return ApplicationSetup::exec(UApplication::TickTimer());
}
