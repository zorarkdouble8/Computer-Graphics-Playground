////CONFINGURATIONS////
//To configure, go to project->properties->preprocessor->preprocessor definitions->set 1 or 0's accordingly

#if TESTMODE 1
    
#else
//don't run this main if tests are enabled

    #if DEBUGMODE 1
        #include <vld.h>
    #endif

#pragma once

#ifndef UNICODE
    #define UNICODE
#endif

#ifndef _UNICODE
    #define _UNICODE
#endif

#define D3DCOMPILE_DEBUG 1 //This enables debug info information from the shader

#include <iostream>
#include <vector>
#include <stdexcept>

#include <glad/glad.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "../Libraries/Image_Loader/stb_image.h"

#include "Shader.h"
#include "Event.h"
#include "System_Manager.h"

//#include "../Assets/Game_Scripts/Playground.h"
#include "../Assets/Game_Scripts/CameraMovement.h"
#include "../Assets/Game_Scripts/TestGameScript.h"
#include "../Assets/Game_Scripts/Light_Scene/Light_Scene.h"
#include "../Assets/Game_Scripts/DirectX/DirectXTest.h"

//----Windows Window classes----
#include <WinUser.h>
#include <Windows.h>
#include <WinBase.h> //for the entry point

//----Modern Windows API----
#include <wrl.h>

using namespace Microsoft::WRL;

//----Direct X----
#include <d3d12sdklayers.h> //To get debug layer
#include <d3d12.h> //For general API
#include <dxgi.h> //For DXGI objects
#include <dxgi1_2.h> //For display modes

using namespace std;


static float deltaTime;

void CalculateDeltaTime()
{

}

void DeleteScripts(vector<Observer<>*> scripts)
{
    for (Observer<>* observer: scripts)
    {
        delete observer;
    }
}
class Test: public IUnknown
{
public:
    long unsigned int AddRef()
    {
        return 0;
    }

    long unsigned int Release()
    {
        return 0;
    }

    HRESULT QueryInterface(REFIID riid, void** ppvObject)
    {

    }
};

void DXThrowIfFail(HRESULT result, bool printException = true)
{
    //there are some unknown errors
    if (FAILED(result))
    {
        if (printException)
            cout << "EXCEPTION: " << "Failure to create a object! " << "HRESULT CODE: " << "0x" << hex << (unsigned int) result << endl;

        throw runtime_error("");
    }
}

//This enables the debug layer inside DirectX
void EnableDebugLayer()
{
    //TODO: Enable GPU based validation: https://learn.microsoft.com/en-us/windows/win32/direct3d12/using-d3d12-debug-layer-gpu-based-validation

    //Get debug interface
    ComPtr<ID3D12Debug> debugLayer;

    if (S_OK == D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer)))
    {
        debugLayer->EnableDebugLayer();

        cout << "Enabled debug layer" << endl;
    }
    else
    {
        cout << "Failed to enable the debug layer" << endl;
    }
}

ComPtr<IDXGIFactory1> CreateFactory()
{
    ComPtr<IDXGIFactory1> GIFactory;
    DXThrowIfFail(CreateDXGIFactory1(IID_PPV_ARGS(&GIFactory)));
    cout << "Created the Factory successfully" << endl;

    return GIFactory;
}

//Creates a DXGI Factory
ComPtr<ID3D12Device3> CreateDevice(ComPtr<IDXGIFactory1> GIFactory, bool createWARP = false)
{
    //TODO: Option to creat warp adapter (use method CreateWarpAdapter)
    ComPtr<IDXGIAdapter> adapter;
    cout << setfill('-') << setw(20) << " " << endl;
    cout << "Available adapters: " << endl;

    for (int x = 0; ; x++)
    {
        try
        {
            DXThrowIfFail(GIFactory->EnumAdapters(x, &adapter), false);
            DXGI_ADAPTER_DESC adapterDesc;
            adapter->GetDesc(&adapterDesc);
            wcout << "ID: " << x << " " << adapterDesc.Description << endl;
            
        }
        catch(...)
        {
            break;
        }
    }

    cout << endl << "Chosing the invidia graphics card (ID 0)" << endl;
    cout << setfill('-') << setw(20) << " " << endl;
    
    DXThrowIfFail(GIFactory->EnumAdapters(0, &adapter));

    ComPtr<ID3D12Device3> device;
    DXThrowIfFail(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device)));
    return device;
}

ComPtr<ID3D12CommandQueue> CreateCommandQueue(ComPtr<ID3D12Device> device)
{
    D3D12_COMMAND_QUEUE_DESC commandDesc;
    commandDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    commandDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    commandDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    commandDesc.NodeMask = 0;

    ComPtr<ID3D12CommandQueue> cmdQueue;
    DXThrowIfFail(device->CreateCommandQueue(&commandDesc, IID_PPV_ARGS(&cmdQueue)));

    return cmdQueue;
}

ComPtr<IDXGISwapChain> CreateSwapChain(ComPtr<IDXGIFactory1> factory, ComPtr<ID3D12CommandQueue> cmdQueue)
{
    DXGI_SWAP_CHAIN_DESC swapChainDesc;

    //Creating the swapChainDesc!!!
    swapChainDesc.BufferDesc.Width = 0;
    swapChainDesc.BufferDesc.Height = 0;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_UNKNOWN;
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
    swapChainDesc.BufferCount = 1;
    swapChainDesc.OutputWindow; //TODO, add handle to output!
    swapChainDesc.Windowed = false;

    //
    //-----------------------------
    ComPtr<IDXGISwapChain> swapChain;
    DXThrowIfFail(factory->CreateSwapChain(cmdQueue.Get(), &swapChainDesc, &swapChain));

    return swapChain;
}

//This get's called to initialize window or other things (https://learn.microsoft.com/en-us/windows/win32/winmsg/using-window-procedures)
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_PAINT:
        {
            PAINTSTRUCT paintSet = { 0 };
            HDC context = BeginPaint(hwnd, &paintSet);

            //Paint stuff here

            FillRect(context, &paintSet.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
            EndPaint(hwnd, &paintSet);
            
            return 1;
        }
        case WM_CLOSE:
        {
            DestroyWindow(hwnd);
            PostQuitMessage(WM_QUIT); // this will message all window procedures to quit
            return 1;
        }
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE noUse, PWSTR lpCmdLine, int nShowCmd)
{
    //Create a window!
    string className = "My windows class";
    WNDCLASSEX window = { 0 }; //Add onto this when you want to add a Icon
    window.cbSize = sizeof(WNDCLASSEX);
    window.style = 0;
    window.lpfnWndProc = WindowProcedure;
    window.cbClsExtra = 0;
    window.cbWndExtra = 0;
    window.hInstance = hInstance;
    window.hIcon = NULL;
    window.hCursor = NULL;
    window.hbrBackground = NULL;
    window.lpszMenuName = NULL;
    window.lpszClassName = L"Main";
    window.hIconSm = NULL;
    
    //Register window to the operating system
    ATOM windowId = RegisterClassEx(&window);
    if (windowId == 0)
    {
        //use https://learn.microsoft.com/en-us/windows/win32/debug/system-error-codes--0-499- to determine errors meaning
        cout << "ERROR occurred when registering window" << endl;
        DWORD error = GetLastError();
        return -1;
    }

    HWND windowHandle = CreateWindowEx(0, L"Main", L"Testing Window", WS_OVERLAPPEDWINDOW, 100, 100, 500, 500, NULL, NULL, hInstance, NULL);
    if (windowHandle == NULL)
    {
        system("PAUSE");
        cout << "ERROR occurred when making window" << endl;
        DWORD error = GetLastError();
        return -1;
    }

    ShowWindow(windowHandle, SW_SHOW);

    MSG msgInfo;
    while(GetMessage(&msgInfo, windowHandle, 0, 0) >= 0)
    {
        TranslateMessage(&msgInfo);
        DispatchMessage(&msgInfo);


    }

    /*
    return 0;
    //start initializing DirectX
    //---Initialize the pipeline---
    
    //Enable debug layer
    EnableDebugLayer();

    //Create the device
    ComPtr<IDXGIFactory1> GIFactory = CreateFactory();
    ComPtr<ID3D12Device3> device = CreateDevice(GIFactory);
    cout << "Created the Device successfully" << endl;
    
    //Create the command queue
    ComPtr<ID3D12CommandQueue> cmdQueue = CreateCommandQueue(device);

    //create the swap chain
    //ComPtr<IDXGISwapChain> swapChain = CreateSwapChain(GIFactory, cmdQueue);

        //Create a render target view(RTV) descriptor heap
        //Create frame resources
        //Create a command allocator

    return 0;*/
}


//int main()
//{
//    sf::Context context;
//
//    //Initialize glad
//    if (!gladLoadGLLoader((GLADloadproc)(context.getFunction)))
//    {
//        cout << "Could not initialize GLAD!!!!" << endl;
//        return -1;
//    }
//    //
//
//    //Initialize the system
//    SystemManager* sysManager = SystemManager::GetInstance();
//    sysManager->PrintSystemInfomation();    
//
//    // create the window
//    sf::ContextSettings windowSettings(24);
//    sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, windowSettings);
//    window.setVerticalSyncEnabled(true);
//
//    //Initialize main window and scripts
//    sysManager->SetMainWindow(&window);
//
//    /*LightScene* play = new LightScene();
//    play->handle = play;*/
//    vector<Observer<>*> scripts = { new DirectX()/*new TestGameScript, new CameraMovement*/};
//
//    // activate the window
//    window.setActive(true);
//
//    // run the main loop
//    bool running = true;
//    while (running)
//    {
//        // handle events
//        sf::Event event;
//        while (window.pollEvent(event))
//        {
//            if (event.type == sf::Event::Closed)
//            {
//                // end the program
//                running = false;
//            }
//            else if (event.type == sf::Event::Resized)
//            {
//                // adjust the viewport when the window is resized
//                glViewport(0, 0, event.size.width, event.size.height);
//            }
//        }
//
//        // clear the buffers
//        glClearColor(0, 0, 0, 1);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    
//        // Call scripts
//        sysManager->UpdateSystemScripts();
//        sysManager->UpdateGameScripts();
//        
//        // end the current frame (internally swaps the front and back buffers)
//        window.display();
//    }
//
//    // release resources...
//    DeleteScripts(scripts);
//    sysManager->DeleteInstance();
//
//    //TODO destroy scripts and get valgrind to tell me about memory leaks
//
//    return 0;
//}
#endif