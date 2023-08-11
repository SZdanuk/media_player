#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>

#include "implot.h"
#include "bass.h"

#include <string>

std::string SONG_PATH = "C:\\Repo\\Mentoring\\song.mp3";



//HWND win;
//
//HSTREAM* strs;
//int strc;
//HMUSIC* mods;
//int modc;
//HSAMPLE* sams;
//int samc;

HSAMPLE sam = BASS_SampleLoad(FALSE, &SONG_PATH, 0, 0, 3, BASS_SAMPLE_OVER_POS);

//HCHANNEL ch = BASS_SampleGetChannel(sams[s], FALSE);
//BASS_SetConfig(BASS_CONFIG_GVOL_SAMPLE, 10000);
//BASS_ChannelSetAttribute(ch, BASS_ATTRIB_VOL, 0.5f);
//BASS_ChannelSetAttribute(ch, BASS_ATTRIB_PAN, ((rand() % 201) - 100) / 100.f);

//(const void*)file
// Data
static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Main code
int main(int, char**)
{
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX11 Example", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, nullptr, nullptr, wc.hInstance, nullptr);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);




    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Handle window resize (we don't resize directly in the WM_SIZE handler)
        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();



        //int n = 0;
        //static float thickness = 3.0f;
        //const ImVec2 p = ImGui::GetCursorScreenPos();
        //static float sz = 36.0f;
        //float x = p.x + 4.0f;
        //float y = p.y + 4.0f;
        //float th = (n == 0) ? 1.0f : thickness;
        //const float spacing = 10.0f;
        //const ImU32 col = ImColor(1.0f, 1.0f, 0.4f, 1.0f);
        //int paddle_y = 150;
        //int paddle_x = 25;
        //int paddle_pos = 20;
        //float mouse_y = input_output.MousePos.y;

       // HCHANNEL channel = NULL;
       // HSTREAM stream = NULL;

       // stream = BASS_StreamCreateFile(FALSE, SONG_PATH.c_str(), 0, 0, 0);

       // BASS_ChannelFree(stream);
       // std::string path = "";
       // //wxTimer* myTimer = new wxTimer(this, 1);
       //// myTimer->SetOwner(this, 1);
       // bool isPlaying = 1;

       // //path = "" + dialog.GetPath();

       // //songName = wxFileName(path).GetName();
       // //staticText->SetLabel("" + songName);
       // BASS_Init(-1, 44100, 0, 0, NULL);
       // BASS_SetVolume(.02);
       // HSAMPLE sample = BASS_SampleLoad(false, SONG_PATH.c_str(), 0, 0, 1, BASS_SAMPLE_MONO);

       // stream = BASS_StreamCreateFile(FALSE, SONG_PATH.c_str(), 0, 0, 0);
       // //myTimer->Start();
       // channel = BASS_SampleGetChannel(stream, FALSE);
       // BASS_ChannelPlay(stream, TRUE);
       // //staticText2 = new wxStaticText(panel_top, wxID_ANY, wxT(""), wxPoint(0, 45));
       // bool isMediaLoaded = true;
       // //const char* comments = BASS_ChannelGetTags(channel, BASS_TAG_OGG); // get a pointer to the 1st comment
       // /*if (comments) {

       //     wxMessageBox(comments);
       // }*/

       // //int filterIndex = dialog.GetFilterIndex();




        if (true)
        {
            ImGui::Begin("MediaPlayer", 0, /*flags*/0);

            //ImGui::PlotLines("Visualizer", )

            //int  data[10] = { 1,2,3,4,5,6,7,8,9,10 };
            //ImPlot::BeginPlot("plot");
            //ImPlot::PlotBars("Vertical", data, 10, 0.7, 1);
            //ImPlot::EndPlot();
            float fft_data[10] = { 5, 2, 3, 4, 5, 6, 7, 8, 9, 5 };
            ImVec2 histogram_size(ImGui::GetWindowWidth() * 0.6, ImGui::GetWindowHeight() * 0.2);
            ImGui::PlotHistogram("", fft_data, 10, 0, "", 0, 10, histogram_size, 4);
            ImVec2 button_size(ImGui::GetWindowWidth()*0.15, ImGui::GetWindowHeight()*0.1);
            ImGui::Button("OpenFile", button_size);
            ImGui::SameLine();
            ImGui::Button("<", button_size);
            ImGui::SameLine();
            ImGui::Button("Play", button_size);
            ImGui::SameLine();
            ImGui::Button("Pause", button_size);
            ImGui::SameLine();
            ImGui::Button("Stop", button_size);
            ImGui::SameLine();
            ImGui::Button(">", button_size);
            ImGui::SameLine();



            //float window_width = ImGui::GetWindowWidth();
            //float window_height = ImGui::GetWindowHeight();
            //ImVec2 window_position = ImGui::GetWindowPos();
            //ImGui::Text("%f", window_width);
            //ImGui::Text("%f", window_height);
            //ImGui::Text("Window position:");
            //ImGui::Text("X %f", window_position.x);
            //ImGui::Text("Y %f", window_position.y);
            //ImDrawList* draw_list = ImGui::GetWindowDrawList();

            //float mouse_x = ImGui::GetIO().MousePos.x;
            //float mouse_y = ImGui::GetIO().MousePos.y;

            //ImGui::Text("Mouse");
            //ImGui::Text("X %f", mouse_x);
            //ImGui::Text("Y %f", mouse_y);




            ImGui::End();

            //draw_list->AddRectFilled(const ImVec2 & p_min, const ImVec2 & p_max, ImU32 col, float rounding, ImDrawFlags flags)
            //ImVec2(input_output.MousePos.x, input_output.MousePos.y)
        }



        // Rendering
        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0); // Present with vsync
        //g_pSwapChain->Present(0, 0); // Present without vsync
    }

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
