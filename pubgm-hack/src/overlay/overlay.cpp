#include "../includes.h"

LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

namespace overlay
{
	static void initialize_imgui()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsClassic();

		auto& style = ImGui::GetStyle();

		style.FrameRounding = 3.f;
		style.ChildRounding = 3.f;
		style.ChildBorderSize = 1.f;
		style.ScrollbarSize = 0.6f;
		style.ScrollbarRounding = 3.f;
		style.GrabRounding = 3.f;
		style.WindowRounding = 0.f;

		style.Colors[ImGuiCol_FrameBg] = ImColor(200, 200, 200);
		style.Colors[ImGuiCol_FrameBgHovered] = ImColor(220, 220, 220);
		style.Colors[ImGuiCol_FrameBgActive] = ImColor(230, 230, 230);
		style.Colors[ImGuiCol_Separator] = ImColor(180, 180, 180);
		style.Colors[ImGuiCol_CheckMark] = ImColor(255, 172, 19);
		style.Colors[ImGuiCol_SliderGrab] = ImColor(255, 172, 19);
		style.Colors[ImGuiCol_SliderGrabActive] = ImColor(255, 172, 19);
		style.Colors[ImGuiCol_ScrollbarBg] = ImColor(120, 120, 120);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImColor(255, 172, 19);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImGui::GetStyleColorVec4(ImGuiCol_ScrollbarGrab);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImGui::GetStyleColorVec4(ImGuiCol_ScrollbarGrab);
		style.Colors[ImGuiCol_Header] = ImColor(160, 160, 160);
		style.Colors[ImGuiCol_HeaderHovered] = ImColor(200, 200, 200);
		style.Colors[ImGuiCol_Button] = ImColor(180, 180, 180);
		style.Colors[ImGuiCol_ButtonHovered] = ImColor(200, 200, 200);
		style.Colors[ImGuiCol_ButtonActive] = ImColor(230, 230, 230);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.78f, 0.78f, 0.78f, 1.f);
		style.Colors[ImGuiCol_WindowBg] = ImColor(220, 220, 220, 0.7 * 255);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.40f, 0.40f, 0.55f, 0.80f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.72f, 0.72f, 0.72f, 0.70f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.83f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.75f, 0.75f, 0.75f, 0.87f);
		style.Colors[ImGuiCol_Text] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.72f, 0.72f, 0.72f, 0.76f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.81f, 0.81f, 0.81f, 1.00f);
		style.Colors[ImGuiCol_Tab] = ImVec4(0.61f, 0.61f, 0.61f, 0.79f);
		style.Colors[ImGuiCol_TabHovered] = ImVec4(0.71f, 0.71f, 0.71f, 0.80f);
		style.Colors[ImGuiCol_TabActive] = ImVec4(0.77f, 0.77f, 0.77f, 0.84f);
		style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.73f, 0.73f, 0.73f, 0.82f);
		style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.58f, 0.58f, 0.58f, 0.84f);

		auto& io = ImGui::GetIO();

		io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 15.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());

		ImGui_ImplWin32_Init(DXWFGetHWND());
		ImGui_ImplDX9_Init(DXWFGetD3DDevice());
		ImGui_ImplDX9_CreateDeviceObjects();

		/*auto flags_color_edit = ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoInputs;
		ImGui::SetColorEditOptions(flags_color_edit);*/
	}

	void AddCircle(const ImVec2& position, float radius, const ImColor& color, int segments)
	{
		auto window = ImGui::GetCurrentWindow();

		window->DrawList->AddCircle(position, radius, ImGui::ColorConvertFloat4ToU32(color), segments);
	}

	void begin_scene()
	{
		static auto once = []()
		{
			initialize_imgui();
			return true;
		}();

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();

		ImGui::NewFrame();

		static bool is_pressed = false;
		auto menu_key_pressed = GetAsyncKeyState(VK_INSERT);
		if (menu_key_pressed && is_pressed == false)
		{
			global_vars::menu_open = !global_vars::menu_open;
			if (global_vars::menu_open)
			{
				SetForegroundWindow(DXWFGetHWND());
				SetWindowLongA(DXWFGetHWND(), GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_COMPOSITED | WS_EX_TRANSPARENT);
			}
			else
			{
				SetWindowLongA(DXWFGetHWND(), GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_COMPOSITED | WS_EX_TRANSPARENT);
			}
			is_pressed = true;
		}
		else if (!menu_key_pressed)
		{
			is_pressed = false;
		}

		if (global_vars::menu_open)
		{
			ImGui::Begin("exquse me sir...");
			const char* pcszBoxesType[] = { "off", "box", "box outline", "corner box", "corner box outline",  "round box", "round box outline" };
			ImGui::Combo("bounding box", &vars::visuals::box_type, pcszBoxesType, IM_ARRAYSIZE(pcszBoxesType));
			ImGui::ColorEdit4("color", vars::visuals::col_enemy_vis, ImGuiColorEditFlags_AlphaBar);
			ImGui::End();

			ImGui::Begin("test entity list");
			for (auto ent : features::game_data::founded_player_entity)
			{
				ImGui::Text("address: 0x%x", ent);
			}
			ImGui::End();
		}

		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4());
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
		ImGui::Begin("##BackBuffer", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoSavedSettings);
		ImGui::SetWindowPos(ImVec2(), ImGuiCond_Always);
		ImGui::SetWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), ImGuiCond_Always);

		features::run();

		ImGui::GetCurrentWindow()->DrawList->PushClipRectFullScreen();
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();

		ImGui::EndFrame();
	}

	void render_callback()
	{

		begin_scene();
	}

	void begin_scene_callback()
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}

	void reset_callback()
	{
		ImGui_ImplDX9_InvalidateDeviceObjects();
	}

	void wndproc_callback(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		ImGui_ImplWin32_WndProcHandler(hwnd, message, wParam, lParam);
	}

	BOOL initialize()
	{
		if (DXWFInitialization(memory_utils::get_base()) == FALSE)
			return FALSE;

		DXWFWndProcCallbacks(DXWF_WNDPROC_WNDPROCHANDLER_, wndproc_callback);

		DXWFRendererCallbacks(DXWF_RENDERER_RESET_, reset_callback);
		DXWFRendererCallbacks(DXWF_RENDERER_LOOP_, render_callback);
		DXWFRendererCallbacks(DXWF_RENDERER_BEGIN_SCENE_LOOP_, begin_scene_callback);

		DXWFSetFramerateLimit(global_vars::overlay_max_fps);

		return DXWFCreateWindow("VirtualBox CTX",
			0, 0,
			1920, 1080,
			WS_POPUP,
			WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_COMPOSITED | WS_EX_TRANSPARENT,
			user_dxwf_flags::ENABLE_WINDOW_ALPHA,
			my_color(0, 0, 0, 1),
			NULL);
	}

	void render_loop()
	{
		DXWFRenderLoop();
	}

	void terminate()
	{
		DXWFTerminate();
	}
}