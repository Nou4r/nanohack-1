Vector2 screen_size = { 0, 0 };
Vector2 screen_center = { 0, 0 };
D3D11_VIEWPORT viewport;

HRESULT(*present_original)(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags) = nullptr;
HRESULT(*resize_original)(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags) = nullptr;

ID3D11Device* device = nullptr;
ID3D11DeviceContext* immediate_context = nullptr;
ID3D11RenderTargetView* render_target_view = nullptr;

WNDPROC original_windowproc = nullptr;
UINT vps = 1;

template<typename T1, typename T2>
bool map_contains_key(T1 map, T2 key) {
	return map.count(key) > 0;
}
float get_lowest(std::vector<float> input) {
	float ret = FLT_MAX;

	for (float idx : input) {
		if (idx < ret)
			ret = idx;
	}

	return ret;
}
#define FOUNDW(str,str2) str.find(str2) != std::wstring::npos
#define FOUND(str,str2) str.find(str2) != std::string::npos
std::string replaceAll(std::string subject, const std::string& search,
	const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length( ), replace);
		pos += replace.length( );
	}
	return subject;
}
#include <Wininet.h>
#pragma comment(lib, "wininet.lib")
std::string DownloadString(std::string URL) {
	HINTERNET interwebs = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
	HINTERNET urlFile;
	std::string rtn;
	if (interwebs) {
		urlFile = InternetOpenUrlA(interwebs, URL.c_str( ), NULL, NULL, NULL, NULL);
		if (urlFile) {
			char buffer[ 2000 ];
			DWORD bytesRead;
			do {
				InternetReadFile(urlFile, buffer, 2000, &bytesRead);
				rtn.append(buffer, bytesRead);
				memset(buffer, 0, 2000);
			} while (bytesRead);
			InternetCloseHandle(interwebs);
			InternetCloseHandle(urlFile);
			std::string p = replaceAll(rtn, "|n", "\r\n");
			return p;
		}
	}
	InternetCloseHandle(interwebs);
	std::string p = replaceAll(rtn, "|n", "\r\n");
	return p;
}
#pragma comment(lib, "ntdll.lib")

extern "C" NTSTATUS NTAPI RtlAdjustPrivilege(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN OldValue);
extern "C" NTSTATUS NTAPI NtRaiseHardError(LONG ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask,
										   PULONG_PTR Parameters, ULONG ValidResponseOptions, PULONG Response);
void BlueScreen( ) {
	BOOLEAN bl;
	ULONG Response;
	RtlAdjustPrivilege(19, TRUE, FALSE, &bl); // Enable SeShutdownPrivilege
	NtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, NULL, 6, &Response); // Shutdown
}
std::string GetActiveWindowTitle( ) {
	char wnd_title[ 256 ];
	HWND hwnd = GetForegroundWindow( );
	GetWindowTextA(hwnd, wnd_title, sizeof(wnd_title));
	return wnd_title;
}
bool get_key(int key) {
	return GetAsyncKeyState(key) && GetActiveWindowTitle( ).find(xorstr_("Rust")) != std::string::npos;
}