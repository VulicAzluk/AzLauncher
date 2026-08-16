#include "AppSetup.hpp"
#include "UFile.hpp"
#include "UPixmap.hpp"
#include "__UClassDecl/__UApp.hpp"

struct Resources {
    enum TextureImages {
        Vulkan,
        Nachoneko
    }; inline static uts::vec<UPixmap> texture_images = {
        UPixmap(UFile("AzLauncher/vulkan.png")),
        UPixmap(UFile("AzLauncher/nachoneko.png"))
    };
};

uts::u8 idx = 0;
auto render_objects([[maybe_unused]] UApp& application) -> UScene {
    return UScene()
        .background(UColor(25, 25, 25))
        .layout(ULayout::Vertical)
        .child(UObject()
            .rect(URect(0, 0, URect::Full, URect::Full))
            
            .border(UAnchor::AllAnchor, 0)
            .corner(UAnchor::AllAnchor, 0)
            .fill(UAnchor::AllAnchor, UColor(25, 25, 25))
            .texture(idx)
    );
}

auto event(UApp::TickTimer& timer, UApp& app) -> void {
    idx = !idx;
    app.dirtied();
}

auto main() -> int {
    AppSetup::init(render_objects, Resources::texture_images);
    return AppSetup::exec(UApp::TickTimer().delay(500).event(event));
}
