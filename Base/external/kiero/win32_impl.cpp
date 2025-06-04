#include "../../includes.h"

static WNDPROC oWndProc = NULL;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT hkWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui::GetCurrentContext() && globals::g_MenuState)
		ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);

    switch (uMsg)
    {
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        globals::g_keyState[wParam] = true;
        break;
    case WM_KEYUP:
    case WM_SYSKEYUP:
        globals::g_keyState[wParam] = false;
        break;
    }

	return CallWindowProcA(oWndProc, hwnd, uMsg, wParam, lParam);
}

void impl::win32::init(void* hwnd)
{
    oWndProc = reinterpret_cast<WNDPROC>((SetWindowLongPtrA)((HWND)hwnd, GWLP_WNDPROC, LONG_PTR(hkWindowProc)));
}

void impl::win32::unhook(void* hwnd)
{
    if (oWndProc != NULL)
    {
        SetWindowLongPtrA((HWND)hwnd, GWLP_WNDPROC, (LONG_PTR)oWndProc);
        oWndProc = NULL;
    }
}