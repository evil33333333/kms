#include <iostream>
#include <windows.h>
#include <shellapi.h>
#include "ansi.h"
#include "key_map.h"

std::string get_product_name();

class GUI 
{
public:
    const std::string ok = Colors[White] + "{ " + Colors[Green] + "!" + Colors[White] + " }";
    const std::string err = Colors[White] + "{ " + Colors[Red] + "x" + Colors[White] + " }";
    const std::string idk = Colors[White] + "{ " + Colors[Blue] + "*" + Colors[White] + " }";

    GUI()
    {
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD current_conosle_mode{};
        GetConsoleMode(handle, &current_conosle_mode);
        current_conosle_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
        SetConsoleMode(handle, current_conosle_mode);
    }
};


int main()
{
    GUI gui;
    HWND hwnd = GetConsoleWindow();
    SetConsoleTitle(L"windows kms activator | developed by lucifers wife");
    std::string product_name = get_product_name();
    std::cout << gui.ok << " operating system detected: " << product_name << std::endl;
    try
    {
        std::string product_key = keyMap.at(product_name);
        std::cout << gui.ok << " associated kms activation key: " << product_key << std::endl;

        std::string slmgr_cmd = "slmgr /ipk " + product_key;
        std::cout << gui.idk << "attempting to activate windows..." << std::endl;
        ShellExecuteA(NULL, "open", "cscript", slmgr_cmd.c_str(), NULL, SW_HIDE);
        ShellExecuteA(NULL, "open", "cscript", "slmgr /skms s8.uk.to", NULL, SW_HIDE);
        ShellExecuteA(NULL, "open", "cscript", "slmgr /ato", NULL, SW_HIDE);
        Sleep(5000);
        MessageBeep(MB_OK);

        STARTUPINFOW si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(STARTUPINFOW));
        ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

        CreateProcess(L"C:\\Windows\\System32\\winver.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
        Sleep(5000);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    catch (...)
    {
        std::cout << gui.err << "this operating system is not supported by this tool :(" << std::endl;
        MessageBox(hwnd, L"this operating system is not supported by this tool :(", L"err", MB_ICONERROR);

    }

    return 0;
    
}

std::string get_product_name()
{
    DWORD buffer_size = 255;
    char value[255]{};

    RegGetValueA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "ProductName", RRF_RT_ANY, NULL, &value, &buffer_size);
    return value;
}