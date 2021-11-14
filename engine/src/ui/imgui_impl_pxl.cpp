#ifdef PXL_IMGUI

#include <pxl/ui/imgui_impl_pxl.h>
#include <pxl/backends/mouse.h>
#include <pxl/backends/keyboard.h>
#include <pxl/backends/platform_backend.h>
#include <pxl/time.h>
#include <pxl/graphics/texture.h>
#include <pxl/graphics/mesh.h>
#include <pxl/graphics/vertex.h>
#include <pxl/graphics/batch.h>
#include <pxl/backbuffer.h>

using namespace pxl;

namespace
{
	int g_fontTextureId = 0;
	int g_textureIds = 0;

	pxl::TextureRef g_fontTexture;

	pxl::Vector<pxl::MeshRef> g_mesh;
	pxl::Map<pxl::Texture*, pxl::TextureWeakRef> textures;
	pxl::MaterialRef g_material;

	pxl::Vector<Key> keys;

	const pxl::VertexFormat vertexFormat = pxl::VertexFormat(
		{
			{ 0, pxl::VertexType::Float2, false }, //position
			{ 1, pxl::VertexType::Float2, false }, // tex
			{ 2, pxl::VertexType::UByte4, true } // col
		}
	);

	bool ImGUI_ImplPXL_UpdateInputs()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DeltaTime = time::delta;
		io.DisplaySize = ImVec2(pxl::platform::size().x, pxl::platform::size().y);
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);

		// mouse
		if (mouse::wheelx() > 0)
		{
			io.MouseWheelH += 1;
		}
		else if (mouse::wheelx() < 0)
		{
			io.MouseWheelH -= 1;
		}

		if (mouse::wheely() > 0)
		{
			io.MouseWheel += 1;
		}
		else if (mouse::wheely() < 0)
		{
			io.MouseWheel -= 1;
		}

		io.MouseDown[0] = mouse::down(pxl::MouseButton::Left);
		io.MouseDown[1] = mouse::down(pxl::MouseButton::Right);
		io.MouseDown[2] = mouse::down(pxl::MouseButton::Middle);

		io.MousePos = ImVec2(mouse::position().x - platform::position().x, mouse::position().y - platform::position().y);

		// Keyboard
		io.KeyShift = keyboard::down(Key::LeftShift);
		io.KeyCtrl = keyboard::down(Key::LeftControl);
		io.KeyAlt = keyboard::down(Key::LeftAlt);
		io.KeySuper = keyboard::down(Key::LeftOS);


		for (int i = 0; i < 22; i++)
		{
			auto key = static_cast<Key>(io.KeyMap[i]);
			io.KeysDown[i] = keyboard::down(key);
		}

		return false;
	}


	void ImGUI_ImplPXL_BuildFontAtlas()
	{
		ImGuiIO& io = ImGui::GetIO();
		unsigned char* data;
		int w;
		int h;
		int bpp;
		io.Fonts->GetTexDataAsRGBA32(&data, &w, &h, &bpp);
		g_fontTexture = pxl::Texture::create(w, h, pxl::TextureFormat::RGBA, data);
		io.Fonts->SetTexID(g_fontTexture.get());

		textures[g_fontTexture.get()] = g_fontTexture;

		io.Fonts->ClearTexData();
	}

}

bool pxl::ImGUI_ImplPXL_awake()
{

	ImGuiIO& io = ImGui::GetIO();
	
	io.BackendRendererName = "PXL";

	io.KeyMap[ImGuiKey_Tab] = (int)Key::Tab;
	io.KeyMap[ImGuiKey_LeftArrow] = (int)Key::Left;
	io.KeyMap[ImGuiKey_RightArrow] = (int)Key::Right;
	io.KeyMap[ImGuiKey_UpArrow] = (int)Key::Up;
	io.KeyMap[ImGuiKey_DownArrow] = (int)Key::Down;
	io.KeyMap[ImGuiKey_PageUp] = (int)Key::PageUp;
	io.KeyMap[ImGuiKey_PageDown] = (int)Key::PageDown;
	io.KeyMap[ImGuiKey_Home] = (int)Key::Home;
	io.KeyMap[ImGuiKey_End] = (int)Key::End;
	io.KeyMap[ImGuiKey_Insert] = (int)Key::Insert;
	io.KeyMap[ImGuiKey_Delete] = (int)Key::Delete;
	io.KeyMap[ImGuiKey_Backspace] = (int)Key::Backspace;
	io.KeyMap[ImGuiKey_Space] = (int)Key::Space;
	io.KeyMap[ImGuiKey_Enter] = (int)Key::Enter;
	io.KeyMap[ImGuiKey_Escape] = (int)Key::Escape;
	io.KeyMap[ImGuiKey_KeyPadEnter] = (int)Key::KeypadEnter;
	io.KeyMap[ImGuiKey_A] = (int)Key::A;
	io.KeyMap[ImGuiKey_C] = (int)Key::C;
	io.KeyMap[ImGuiKey_V] = (int)Key::V;
	io.KeyMap[ImGuiKey_X] = (int)Key::X;
	io.KeyMap[ImGuiKey_Y] = (int)Key::Y;
	io.KeyMap[ImGuiKey_Z] = (int)Key::Z;

	ImGUI_ImplPXL_BuildFontAtlas();

	return true;
}

void pxl::ImGUI_ImplPXL_registerTexture(pxl::TextureRef texture)
{
    textures[texture.get()] = texture;
}

void pxl::ImGUI_ImplPXL_destroy()
{
	ImGuiIO& io = ImGui::GetIO();
	io.BackendRendererName = nullptr;
}

void pxl::ImGUI_ImplPXL_update()
{
	ImGUI_ImplPXL_UpdateInputs();
}


void pxl::ImGUI_ImplPXL_draw(const pxl::RenderTargetRef& target)
{
	auto data = ImGui::GetDrawData();
	if (data->TotalVtxCount <= 0) return;

	while (g_mesh.size() < data->CmdListsCount)
	{
		g_mesh.push_back(pxl::Mesh::create());
	}

	if (g_material == nullptr)
	{
		g_material = Material::create(Shader::create());
	}

	auto l = data->DisplayPos.x;
	auto r = data->DisplayPos.x + data->DisplaySize.x;
	auto t = data->DisplayPos.y;
	auto b = data->DisplayPos.y + data->DisplaySize.y;
	auto matrix = Mat4x4::createOrthoOffcenter(l, r, b, t, 0.01f, 1000.0f);
	g_material->setFloat("u_matrix", &matrix.m11, 16);
	g_material->setSampler("u_texture_sampler", pxl::TextureSampler::LinearClamp);
	int offset = 0;
	for (int cl = 0; cl < data->CmdListsCount; cl++)
	{
		auto cmdList = data->CmdLists[cl];
		g_mesh[cl]->setIndexData(pxl::IndexFormat::UInt16, cmdList->IdxBuffer.Data, cmdList->IdxBuffer.size());
		g_mesh[cl]->setVertexData(vertexFormat, cmdList->VtxBuffer.Data, cmdList->VtxBuffer.size());
		for (int c = 0; c < cmdList->CmdBuffer.Size; c++)
		{
			auto drawCommand = cmdList->CmdBuffer[c];
			DrawCall call;
			call.mesh = g_mesh[cl];
			call.material = g_material;
			if (drawCommand.TextureId != nullptr)
			{
				auto texture = textures[(pxl::Texture*)drawCommand.TextureId].lock();
				g_material->setTexture("u_texture", texture);
			}
			call.blend = pxl::BlendState::NormalNonPremultiplied;
			call.target = target;
			call.scissors = pxl::Rect(drawCommand.ClipRect.x, drawCommand.ClipRect.y, drawCommand.ClipRect.z - drawCommand.ClipRect.x, drawCommand.ClipRect.w - drawCommand.ClipRect.y);
			call.useScissors = true;
			call.indices_count = drawCommand.ElemCount;
			call.indices_start = drawCommand.IdxOffset;
			call.draw();
		}
	}
}

#endif
