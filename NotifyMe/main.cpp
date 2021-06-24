#include <Windows.h>
#include <WinUser.h>
#include <tlhelp32.h>

struct WindowHandleData {
    unsigned long process_id;
    HWND windowHandle;
};

DWORD findProcessWithName(LPCTSTR processName)
{
    PROCESSENTRY32 processEntry;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(0x00000002, 0);
    processEntry.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hSnapshot, &processEntry)) { 
        do {
            if (!lstrcmpiW(processEntry.szExeFile, processName)) {
                CloseHandle(hSnapshot);
                return processEntry.th32ProcessID;
            }
        } while (Process32Next(hSnapshot, &processEntry));
    }
    CloseHandle(hSnapshot); 
    return 0;
}

BOOL isProcessMainWindow(HWND handle)
{
    return GetWindow(handle, GW_OWNER) == (HWND)0 && IsWindowVisible(handle);
}

BOOL CALLBACK enumWindowsCallback(HWND handle, LPARAM lParam)
{
    WindowHandleData& data = *(WindowHandleData*)lParam;
    unsigned long processId = 0;
    GetWindowThreadProcessId(handle, &processId);
    if (data.process_id != processId || !isProcessMainWindow(handle))
        return TRUE;
    data.windowHandle = handle;
    return FALSE;
}

HWND findMainWindowsForProcess(unsigned long processId)
{
    WindowHandleData data;
    data.process_id = processId;
    data.windowHandle = 0;
    EnumWindows(enumWindowsCallback, (LPARAM)&data);
    return data.windowHandle;
}

int main() {

    auto pid = findProcessWithName(L"WindowsTerminal.exe");

    if (pid == 0) {
        return -1;
    }

    auto hWindow = findMainWindowsForProcess(pid);

    if (!hWindow) {
        return -2;
    }

	for (int i = 0; i < 5; i++) {
		FlashWindow(hWindow, true);
		Sleep(1000);
	}
	FlashWindow(hWindow, false);
}