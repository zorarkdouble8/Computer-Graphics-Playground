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

#define JSON_DIAGNOSTICS 1

#define D3DCOMPILE_DEBUG 1 //This enables debug info information from the shader

#include <iostream>
#include <vector>
#include <stdexcept>
#include <fstream>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>

#include <glad/glad.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "../Libraries/Image_Loader/stb_image.h"
#include "../Libraries/JSON/nlohmann/json.hpp"

#include "Shader.h"
#include "Event.h"
#include "System_Manager.h"
#include "helpers.h"
#include "State.h"

//#include "../Assets/Game_Scripts/Playground.h"
#include "../Assets/Game_Scripts/CameraMovement.h"
#include "../Assets/Game_Scripts/TestGameScript.h"
#include "../Assets/Game_Scripts/Light_Scene/Light_Scene.h"
#include "../Assets/Game_Scripts/DirectX/DirectXTest.h"
#include "DataFuncs.h"

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
class TestCOM: public IUnknown
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

ComPtr<IDXGIFactory2> CreateFactory()
{
    ComPtr<IDXGIFactory2> GIFactory;
    DXThrowIfFail(CreateDXGIFactory1(IID_PPV_ARGS(&GIFactory)));
    cout << "Created the Factory successfully" << endl;

    return GIFactory;
}

//Creates a DXGI Factory
ComPtr<ID3D12Device3> CreateDevice(ComPtr<IDXGIFactory2> GIFactory, bool createWARP = false)
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

ComPtr<IDXGISwapChain> CreateSwapChain(ComPtr<IDXGIFactory2> factory, ComPtr<ID3D12CommandQueue> cmdQueue, HWND windowHandle)
{
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };

    //Used to specify the flip model for swap chains (Directx12 HAS to use this)
    DXGI_SAMPLE_DESC flipModelDesc = { 0 };
    flipModelDesc.Count = 1;
    flipModelDesc.Quality = 0;

    //Creating the swapChainDesc!!!
    swapChainDesc.SampleDesc = flipModelDesc;
    swapChainDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
    swapChainDesc.Stereo = false;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 2;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

    //I would like DXGI_ALPHA_MODE_STRAIGHT to represent that alpha is color transparency, but it gives a error. 
    //It might have to do with the format or a DirectX12 thing
    swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE; 
    
    //create the actual swap chain
    ComPtr<IDXGISwapChain1> swapChain;
    DXThrowIfFail(factory->CreateSwapChainForHwnd(cmdQueue.Get(), windowHandle, &swapChainDesc, NULL, NULL, &swapChain));

    return swapChain;
}

//This get's called to initialize window or other things (https://learn.microsoft.com/en-us/windows/win32/winmsg/using-window-procedures)
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CREATE:
        {
            //Create a window pointer to the system state
            CREATESTRUCT* stru = reinterpret_cast<CREATESTRUCT*>(lParam);
            SystemState* sysState = reinterpret_cast<SystemState*>(stru->lpCreateParams);

            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR) sysState);

            
           //start initializing DirectX
           //---Initialize the pipeline---

           //Enable debug layer
           EnableDebugLayer();

           //Create the device
           ComPtr<IDXGIFactory2> GIFactory = CreateFactory();
           ComPtr<ID3D12Device3> device = CreateDevice(GIFactory);
           cout << "Created the Device successfully" << endl;

           //Create the command queue
           ComPtr<ID3D12CommandQueue> cmdQueue = CreateCommandQueue(device);
           cout << "Created the Command Queue successfully" << endl;

           //create the swap chain
           ComPtr<IDXGISwapChain> swapChain = CreateSwapChain(GIFactory, cmdQueue, hwnd);
           cout << "Created the Swap Chain successfully" << endl;

           //Create a render target view(RTV) descriptor heap
           //Create frame resources
           //Create a command allocator

            break;
        }
        case WM_PAINT:
        {
            //get the system state
            SystemState* sysState = reinterpret_cast<SystemState*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

            PAINTSTRUCT paintSet = { 0 };
            HDC context = BeginPaint(hwnd, &paintSet);

            //Paint stuff here
            FillRect(context, &paintSet.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

            

            LPCWSTR lpcwData = stringToLPCWSTR(sysState->getCurrentState().dump());
            DrawText(context, lpcwData, -1, &paintSet.rcPaint, DT_CENTER);

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

nlohmann::json RetrieveSavedData(bool& isSuccessful)
{
    ifstream json("C:/Users/User101/Desktop/SFML and OpenGL Playground/SFML Playground/System_Scripts/Data.json");
    nlohmann::json data;
    try
    {
        data = nlohmann::json::parse(json);
        isSuccessful = true;
    }
    catch (const nlohmann::json::exception e)
    {
        isSuccessful = false;
    }

    return data;
}

//Creates and returns a window handle
HWND CreateWindowHandle(HINSTANCE hInstance, nlohmann::json& rootData, const string& windowName)
{
    //Create a window!
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
    window.lpszClassName = stringToLPCWSTR(windowName);
    window.hIconSm = NULL;

    //Register window to the operating system
    ATOM windowId = RegisterClassEx(&window);
    if (windowId == 0)
    {
        //use https://learn.microsoft.com/en-us/windows/win32/debug/system-error-codes--0-499- to determine errors meaning
        cout << "ERROR occurred when registering window" << endl;
        DWORD error = GetLastError();
        //return -1;
    }

    //Create the window
    SystemState* state = SystemState::getInstance();
    HWND windowHandle = CreateWindowEx(0, stringToLPCWSTR(windowName), stringToLPCWSTR(windowName), WS_OVERLAPPEDWINDOW, 100, 100, 500, 500, NULL, NULL, hInstance, state);
    if (windowHandle == NULL)
    {
        system("PAUSE");
        cout << "ERROR occurred when making window" << endl;
        DWORD error = GetLastError();
        //return -1;
    }

    return windowHandle;
}

//Code to connect the std::___ to the terminal, this is from: https://stackoverflow.com/questions/311955/redirecting-cout-to-a-console-in-windows
void BindCrtHandlesToStdHandles(bool bindStdIn, bool bindStdOut, bool bindStdErr)
{
    // Re-initialize the C runtime "FILE" handles with clean handles bound to "nul". We do this because it has been
    // observed that the file number of our standard handle file objects can be assigned internally to a value of -2
    // when not bound to a valid target, which represents some kind of unknown internal invalid state. In this state our
    // call to "_dup2" fails, as it specifically tests to ensure that the target file number isn't equal to this value
    // before allowing the operation to continue. We can resolve this issue by first "re-opening" the target files to
    // use the "nul" device, which will place them into a valid state, after which we can redirect them to our target
    // using the "_dup2" function.
    if (bindStdIn)
    {
        FILE* dummyFile;
        freopen_s(&dummyFile, "nul", "r", stdin);
    }
    if (bindStdOut)
    {
        FILE* dummyFile;
        freopen_s(&dummyFile, "nul", "w", stdout);
    }
    if (bindStdErr)
    {
        FILE* dummyFile;
        freopen_s(&dummyFile, "nul", "w", stderr);
    }

    // Redirect unbuffered stdin from the current standard input handle
    if (bindStdIn)
    {
        HANDLE stdHandle = GetStdHandle(STD_INPUT_HANDLE);
        if (stdHandle != INVALID_HANDLE_VALUE)
        {
            int fileDescriptor = _open_osfhandle((intptr_t)stdHandle, _O_TEXT);
            if (fileDescriptor != -1)
            {
                FILE* file = _fdopen(fileDescriptor, "r");
                if (file != NULL)
                {
                    int dup2Result = _dup2(_fileno(file), _fileno(stdin));
                    if (dup2Result == 0)
                    {
                        setvbuf(stdin, NULL, _IONBF, 0);
                    }
                }
            }
        }
    }

    // Redirect unbuffered stdout to the current standard output handle
    if (bindStdOut)
    {
        HANDLE stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        if (stdHandle != INVALID_HANDLE_VALUE)
        {
            int fileDescriptor = _open_osfhandle((intptr_t)stdHandle, _O_TEXT);
            if (fileDescriptor != -1)
            {
                FILE* file = _fdopen(fileDescriptor, "w");
                if (file != NULL)
                {
                    int dup2Result = _dup2(_fileno(file), _fileno(stdout));
                    if (dup2Result == 0)
                    {
                        setvbuf(stdout, NULL, _IONBF, 0);
                    }
                }
            }
        }
    }

    // Redirect unbuffered stderr to the current standard error handle
    if (bindStdErr)
    {
        HANDLE stdHandle = GetStdHandle(STD_ERROR_HANDLE);
        if (stdHandle != INVALID_HANDLE_VALUE)
        {
            int fileDescriptor = _open_osfhandle((intptr_t)stdHandle, _O_TEXT);
            if (fileDescriptor != -1)
            {
                FILE* file = _fdopen(fileDescriptor, "w");
                if (file != NULL)
                {
                    int dup2Result = _dup2(_fileno(file), _fileno(stderr));
                    if (dup2Result == 0)
                    {
                        setvbuf(stderr, NULL, _IONBF, 0);
                    }
                }
            }
        }
    }

    // Clear the error state for each of the C++ standard stream objects. We need to do this, as attempts to access the
    // standard streams before they refer to a valid target will cause the iostream objects to enter an error state. In
    // versions of Visual Studio after 2005, this seems to always occur during startup regardless of whether anything
    // has been read from or written to the targets or not.
    if (bindStdIn)
    {
        std::wcin.clear();
        std::cin.clear();
    }
    if (bindStdOut)
    {
        std::wcout.clear();
        std::cout.clear();
    }
    if (bindStdErr)
    {
        std::wcerr.clear();
        std::cerr.clear();
    }
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE noUse, PWSTR lpCmdLine, int nShowCmd)
{
    AllocConsole();
    BindCrtHandlesToStdHandles(true, true, true);

    SystemState* state = SystemState::getInstance();

    //Create all windows
    nlohmann::json rootData = state->getCurrentState();
    nlohmann::json windows = rootData.at("Windows");
    for (auto w = windows.items().begin(); w != windows.items().end(); w++)
    {
        HWND handle = CreateWindowHandle(hInstance, rootData, w.key());
       
        state->commitState(WindowData::AddWindowHandle(rootData, w.key(), handle));
    }

    state->pushToNextState();
    rootData = state->getCurrentState();

    string data = state->getCurrentState().dump();
    for (auto w = windows.items().begin(); w != windows.items().end(); w++)
    {
        int test = rootData.at("Windows").at(w.key()).at("State").at("Handle");
        HWND handle = (HWND) (test);

        //Command the window to show
        ShowWindow(handle, SW_SHOW);
    }

    //Get messages and distribute them to windows
    MSG msgInfo;
    while(GetMessage(&msgInfo, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msgInfo);
        DispatchMessage(&msgInfo);
    }

    return 0;
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