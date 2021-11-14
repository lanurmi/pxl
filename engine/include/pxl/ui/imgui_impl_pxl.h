#ifdef PXL_IMGUI

#pragma once

#include <imgui/imgui.h>
#include <pxl/graphics/texture.h>
#include <pxl/graphics/rendertarget.h>
namespace pxl
{
	IMGUI_IMPL_API bool ImGUI_ImplPXL_awake();
	IMGUI_IMPL_API void ImGUI_ImplPXL_destroy();
	IMGUI_IMPL_API void ImGUI_ImplPXL_update();
    IMGUI_IMPL_API void ImGUI_ImplPXL_registerTexture(pxl::TextureRef texture);
	IMGUI_IMPL_API void ImGUI_ImplPXL_draw(const pxl::RenderTargetRef &target);
}

#endif
