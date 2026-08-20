// #define ULIXRELEASEAPPLICATION
#include <AppSetup.hpp>
#include <UAnchor.hpp>
#include <UColor.hpp>
#include <UFont.hpp>
#include <UObject.hpp>
#include <UPixmap.hpp>
#include <URect.hpp>

auto render_objects([[maybe_unused]] UContext& ctx) -> UScene {
    ctx.dirtied(true);

    return UScene()
        .background(UColor(25, 25, 25))
        .layout(ULayout::Vertical)
        .vpadding(5)
        .hpadding(5)

        .child(UObject()
            .rect(URect(50, 0, 575, 125))

            .border(UAnchor::AllAnchor, 0)
            .corner(UAnchor::AllAnchor, 5)
            .fill(UAnchor::AllAnchor, UColor(0, 0, 0))
            .texture(UFile("AzLauncher/vulkan.png"))
            .child(UObject()
                .rect(URect(10, 10, 100, 100))
                .fill(UAnchor::AllAnchor, UColor(255, 255, 255))
                .texture("Hello", UFont().color(UColor(0, 0, 0)).size(128))
            )
        )
        .child(UObject()
            .rect(URect(200, 200, 300, 300))

            .border(UAnchor::AllAnchor, 0)
            .corner(UAnchor::AllAnchor, 5)
            .fill(UAnchor::AllAnchor, UColor(0, 0, 0))
            .texture(UFile("AzLauncher/verified.svg"), URect(0, 0, 512, 512))
        )

        .child(UObject()
            .rect(URect(600, 200, 300, 300))

            .border(UAnchor::AllAnchor, 0)
            .corner(UAnchor::AllAnchor, 5)
            .fill(UAnchor::AllAnchor, UColor(0, 0, 0))
            .texture(UFile("AzLauncher/verified.svg"), URect(0, 0, 512, 512))
        );
}

auto main() -> int {
    AppSetup::init({0, 0, 3}, render_objects);
    return AppSetup::exec(UContext::TickTimer());
}
