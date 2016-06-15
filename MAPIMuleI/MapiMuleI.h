#pragma once

#define CORg(_hr) \
	do { \
		hr = _hr; \
		if (FAILED(hr)) \
		{ \
			MessageBox(hwnd, L"MAPI Initialization Failed", L"MAPI Init", MB_OK); \
			goto Error; \
		} \
	} while (0)
				//std::wcout << L"FAILED! hr = " << std::hex << hr << L".  LINE = " << std::dec << __LINE__ << std::endl; \
							//std::wcout << L" >>> " << (wchar_t*) L#_hr <<  std::endl; \
	