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

