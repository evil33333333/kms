#include <array>
#include <iostream>
#include <windows.h>
#include <shellapi.h>
#include "ansi.h"
#include "key_map.h"

bool is_activated();
std::string get_operating_system();
bool ShellExecuteAndWait(HWND hwnd, LPCSTR file, LPCSTR directory, LPCSTR params, WORD show);

class GUI
{
public:
	const std::string ok = Colors[White] + "{ " + Colors[Green] + "!" + Colors[White] + " }";
	const std::string err = Colors[White] + "{ " + Colors[Red] + "x" + Colors[White] + " }";
	const std::string idk = Colors[White] + "{ " + Colors[Blue] + "*" + Colors[White] + " }";

	GUI()
	{
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		DWORD current_conosle_mode = {0};
		GetConsoleMode(handle, &current_conosle_mode);
		current_conosle_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
		SetConsoleMode(handle, current_conosle_mode);
	}
};


int main()
{
	GUI gui;
	HWND hwnd = GetConsoleWindow();
	SetConsoleTitle("windows kms activator | developed by lucifers wife");
	std::string product_name = get_operating_system();
	std::cout << gui.ok << " Operating System Detected: " << product_name << std::endl;

	if (keyMap.find(product_name) == keyMap.end())
	{
		std::cout << gui.err << "This Operating System is not supported by this tool :(" << std::endl;
		MessageBox(hwnd, "This Operating System is not supported by this tool :(", "err", MB_ICONERROR);
		std::exit(-1);
	}


	std::string product_key = keyMap.at(product_name);
	std::cout << gui.ok << " Associated KMS Activation Key: " << product_key << std::endl;

	std::string slmgr_cmd = "C:\\Windows\\System32\\slmgr.vbs /ipk " + product_key;
	std::cout << gui.idk << " Attempting to activate " << product_name << ". Please wait..." << std::endl;
	ShellExecuteA(NULL, "open", "cscript", slmgr_cmd.c_str(), NULL, SW_HIDE);
	ShellExecuteA(NULL, "open", "cscript", "C:\\Windows\\System32\\slmgr.vbs /skms kms8.msguides.com", NULL, SW_HIDE);
	ShellExecuteAndWait(hwnd, "cmd", NULL, "/c slmgr /ato", SW_HIDE);

	if (is_activated())
	{
		std::cout << gui.ok << " Successfully activated " << product_name  << "!" << std::endl;
	}
	else 
	{
		std::cout << gui.err << " Failed to activate " << product_name << "... :(" << std::endl;
	}
	Sleep(3000);

	return 0;

}

std::string get_operating_system()
{
	const unsigned int buffer_size = 255;
	char value[buffer_size] = {0};

	RegGetValueA(
		HKEY_LOCAL_MACHINE,
		"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",
		"ProductName",
		RRF_RT_ANY,
		NULL,
		&value,
		(LPDWORD)&buffer_size
	);

	return std::string{ value };
}

bool ShellExecuteAndWait(HWND hwnd, LPCSTR file, LPCSTR directory, LPCSTR params, WORD show)
{
	SHELLEXECUTEINFO ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = hwnd;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = file;
	ShExecInfo.lpParameters = params;
	ShExecInfo.lpDirectory = directory;
	ShExecInfo.nShow = show;
	ShExecInfo.hInstApp = NULL;
	bool ok = ShellExecuteExA(&ShExecInfo);
	if (ok)
	{
		WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
		CloseHandle(ShExecInfo.hProcess);
	}
	
	return ok;
}

std::string exec(const char* cmd) 
{
	std::array<char, 128> buffer;
	std::string result;
	std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
	if (!pipe) 
	{
		throw std::runtime_error("popen() failed!");
	}
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) 
	{
		result += buffer.data();
	}
	return result;
}

bool is_activated() 
{
	std::string result = exec("cscript C:\\Windows\\System32\\slmgr.vbs /dli");
	return result.find("License Status: Licensed") != std::string::npos;
}