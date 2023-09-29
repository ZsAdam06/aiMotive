// Dear ImGui: standalone example application for DirectX 11

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "implot.h"
#include "imuRecord.hpp"
#include "dbwRecord.hpp"
#include <vector>
#include <d3d11.h>
#include <tchar.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>

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
void makePlot1();
void makePlot2();
void makePlot3();
void imuReader(std::vector<ImuRecord>*);
void dbwReader(std::vector<DbwRecord>*);

//Functions
void RenderMenuBar(bool* sp1, bool* sp2, bool* sp3) {
    ImGui::BeginMenuBar();
    ImGui::Text("aiMotive");
    ImGui::SetCursorPosX(ImGui::GetWindowSize().x-600);
    if (ImGui::Button("Speed", ImVec2(200, 20))) {
        *sp1 = true;
        *sp2 = false;
        *sp3 = false;
    }
    ImGui::SameLine(0, 0);
    if (ImGui::Button("Wheels", ImVec2(200, 20))) {
        *sp1 = false;
        *sp2 = true;
        *sp3 = false;
    }
    ImGui::SameLine(0, 0);
    if (ImGui::Button("Accelerations", ImVec2(200, 20))) {
        *sp1 = false;
        *sp2 = false;
        *sp3 = true;
    }
    ImGui::EndMenuBar();
}

void makePlot1() {
    std::vector<DbwRecord>* dbwData = new std::vector<DbwRecord>();
    dbwReader(dbwData);
    static int count = dbwData->size();
    static double* xs1 = new double[count]();
    static double* ys1 = new double[count]();

    for (int i = 0; i < count; i++) {
        xs1[i] = dbwData->at(i).timestamp;
        ys1[i] = dbwData->at(i).speed;
    }

    ImPlot::SetNextFillStyle(ImVec4(40.0f / 255.0f, 48.0f / 255.0f, 51.0f / 255.0f, 1.0f));
    if (ImPlot::BeginPlot("Speed of the car")) {
        ImPlot::SetupAxes("Timestamp", "Speed");
        ImPlot::PlotLine("Speed", xs1, ys1, count);
        ImPlot::EndPlot();
    }
}

void makePlot2() {
    std::vector<DbwRecord>* dbwData = new std::vector<DbwRecord>();
    dbwReader(dbwData);
    static int count = dbwData->size();
    double* xs = new double[count]();
    double* ys = new double[count]();
    double* ys1 = new double[count]();
    double* ys2 = new double[count]();
    double* ys3 = new double[count]();
    double* ys4 = new double[count]();

    for (int i = 0; i < count; i++) {
        xs[i] = dbwData->at(i).timestamp;
        ys[i] = dbwData->at(i).speed;
        ys1[i] = dbwData->at(i).v_front_left;
        ys2[i] = dbwData->at(i).v_front_right;
        ys3[i] = dbwData->at(i).v_rear_left;
        ys4[i] = dbwData->at(i).v_rear_right;
    }
    
    
    if (ImPlot::BeginPlot("Speed of the car and the velocity of the wheels\n(the first 15 records)")) {
        ImPlot::PlotLine("Speed", xs, ys, 15);
        ImPlot::PlotLine("Front left", xs, ys1, 15);
        ImPlot::PlotLine("Front right", xs, ys2, 15);
        ImPlot::PlotLine("Rear left", xs, ys3, 15);
        ImPlot::PlotLine("Rear right", xs, ys4, 15);
        ImPlot::EndPlot();
    }
}

void makePlot3() {
    std::vector<DbwRecord>* dbwData = new std::vector<DbwRecord>();
    dbwReader(dbwData);
    std::vector<ImuRecord>* imuData = new std::vector<ImuRecord>();
    imuReader(imuData);
    static int imuCount = imuData->size();
    static int dbwCount = dbwData->size();
    int count;
    if (imuCount < dbwCount) {
        count = imuCount;
    }
    else {
        count = dbwCount;
    }
    double* xs = new double[count]();
    double* ys = new double[count]();
    double* ys1 = new double[count]();
    double* ys2 = new double[count]();
    double* ys3 = new double[count]();

    
    for (int i = 0; i < count; i++) {
        xs[i] = dbwData->at(i).timestamp;
        ys[i] = dbwData->at(i).speed;
        ys1[i] = imuData->at(i).acc_x;
        ys2[i] = imuData->at(i).acc_y;
        ys3[i] = imuData->at(i).acc_z;
    }


    if (ImPlot::BeginPlot("Speed of the car and accelerations")) {
        ImPlot::PlotLine("Speed", xs, ys, count);
        ImPlot::PlotLine("Acc_X", xs, ys1, count);
        ImPlot::PlotLine("Acc_Y", xs, ys2, count);
        ImPlot::PlotLine("Acc_Z", xs, ys3, count);
        ImPlot::EndPlot();
    }
    
}

// Main code
int main(int, char**)
{
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"aiMotive Test Project", WS_OVERLAPPEDWINDOW, 100, 100, 1215, 388, nullptr, nullptr, wc.hInstance, nullptr);

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
    ImPlot::CreateContext();
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
    ImVec4 clear_color = ImVec4(4.0f / 255.0f, 207.0f / 255.0f, 88.0f / 255.0f, 1.0f);
    ImVec4 color2 = ImVec4(51.0f / 255.0f, 66.0f / 255.0f, 71.0f / 255.0f, 1.0f);
    bool* showPlot1 = new bool(false);
    bool* showPlot2 = new bool(false);
    bool* showPlot3 = new bool(false);

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


        ImGui::SetNextWindowSize(ImVec2(1200, 550));
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, clear_color);
        if (ImGui::Begin("Plots", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_MenuBar)) {
            ImGui::PopStyleColor();
            
            RenderMenuBar(showPlot1, showPlot2, showPlot3);
            if (*showPlot1 == true) {
                makePlot1();
            }
            if (*showPlot2 == true) {
                makePlot2();
            }
            if (*showPlot3 == true) {
                makePlot3();
            }

        }
        ImGui::End();


        

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
    ImPlot::DestroyContext();
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
