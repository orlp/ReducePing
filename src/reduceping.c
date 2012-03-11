#include <windows.h>
#include <stdlib.h>

#include "resource.h"

/* this is where TCPAckFrequency sub keys can be found */
const char ROOT_KEY[] = "SYSTEM\\CurrentControlSet\\services\\Tcpip\\Parameters\\Interfaces\\";

int is_reduceping_enabled() {
    HKEY rootkey;
    
    /* open the root key */
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, ROOT_KEY, 0, KEY_READ, &rootkey) != ERROR_SUCCESS) {
        MessageBox(NULL, "ReducePing can't find any network interface keys in the registry and can not continue.", "ReducePing", MB_ICONEXCLAMATION | MB_OK);
        exit(1);
    }
    
    /* get number of sub keys (these are the TCP interfaces, assuming a regular windows registry) */
    DWORD numsubkeys;
    if (RegQueryInfoKey(rootkey, NULL, NULL, NULL, &numsubkeys, NULL, NULL, NULL, NULL, NULL, NULL, NULL) != ERROR_SUCCESS) {
        MessageBox(NULL, "ReducePing has encountered an error during querying the amount of sub keys and can not continue.", "ReducePing", MB_ICONEXCLAMATION | MB_OK);
        RegCloseKey(rootkey);
        exit(1);
    }
    
    /* iterate over sub keys, counting the amount of times we see TcpAckFrequency */
    long i;
    long num_tcp_ack_keys = 0;
    for (i = 0; i < numsubkeys; i++) {
        char subkey[256 + sizeof(ROOT_KEY)];
        
        strncpy(subkey, ROOT_KEY, strlen(ROOT_KEY));
        
        if (RegEnumKey(rootkey, i, subkey + strlen(ROOT_KEY), sizeof(subkey)) != ERROR_SUCCESS) {
            MessageBox(NULL, "ReducePing has encountered an error during enumerating registry keys, attempting to continue.", "ReducePing", MB_ICONEXCLAMATION | MB_OK);
            continue;
        }
        
        /* open sub key for reading */
        HKEY tcpkey;
        if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, subkey, 0, KEY_READ, &tcpkey) != ERROR_SUCCESS) {
            MessageBox(NULL, "ReducePing encountered an error while reading a key (this should never happen). Attempting to continue.", "ReducePing", MB_ICONEXCLAMATION | MB_OK);
            RegCloseKey(rootkey);
            continue;
        }
        
        /* check if the TcpAckFrequency value exists, and if it equals 1 - if yes add to our count */
        DWORD value;
        DWORD size = sizeof(value);
        DWORD type = REG_DWORD;
        if (RegQueryValueEx(tcpkey, "TcpAckFrequency", NULL, &type, (BYTE *) &value, &size) == ERROR_SUCCESS) {
            if (type == REG_DWORD && size == sizeof(value) && value == 1) {
                num_tcp_ack_keys++;
            }
        }
        
        RegCloseKey(tcpkey);
    }
    
    RegCloseKey(rootkey);
    
    /* if we found no keys we are not installed */
    if (num_tcp_ack_keys == 0) {
        return 0;
    /* if they amount of tcpack keys equals the total amount of keys we are fully installed */
    } else if (num_tcp_ack_keys == i) {
        return 1;
    /* and if not we're partially installed */
    } else {
        return -1;
    }
}

void enable_reduceping() {
    HKEY rootkey;
    
    /* open the root key */
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, ROOT_KEY, 0, KEY_READ, &rootkey) != ERROR_SUCCESS) {
        MessageBox(NULL, "ReducePing can't find any network interface keys in the registry and can not continue.", "ReducePing", MB_ICONEXCLAMATION | MB_OK);
        exit(1);
    }
    
    /* get number of sub keys (these are the TCP interfaces, assuming a regular windows registry) */
    DWORD numsubkeys;
    if (RegQueryInfoKey(rootkey, NULL, NULL, NULL, &numsubkeys, NULL, NULL, NULL, NULL, NULL, NULL, NULL) != ERROR_SUCCESS) {
        MessageBox(NULL, "ReducePing has encountered an error during querying the amount of sub keys and can not continue.", "ReducePing", MB_ICONEXCLAMATION | MB_OK);
        RegCloseKey(rootkey);
        exit(1);
    }
    
    /* iterate over sub keys */
    long i;
    for (i = 0; i < numsubkeys; i++) {
        char subkey[256 + sizeof(ROOT_KEY)];
        
        strncpy(subkey, ROOT_KEY, strlen(ROOT_KEY));
        
        if (RegEnumKey(rootkey, i, subkey + strlen(ROOT_KEY), sizeof(subkey)) != ERROR_SUCCESS) {
            MessageBox(NULL, "ReducePing has encountered an error during enumerating registry keys, attempting to continue.", "ReducePing", MB_ICONEXCLAMATION | MB_OK);
            continue;
        }
        
        /* open sub key for writing */
        HKEY tcpkey;
        if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, subkey, 0, KEY_WRITE, &tcpkey) != ERROR_SUCCESS) {
            MessageBox(NULL, "ReducePing can't write to the registry, make sure that your Windows account has Administrator rights.", "ReducePing", MB_ICONEXCLAMATION | MB_OK);
            RegCloseKey(rootkey);
            return;
        }
        
        /* set the TcpAckFrequency value to 1 */
        const DWORD value = 1;
        if (RegSetValueEx(tcpkey, "TcpAckFrequency", 0, REG_DWORD, (const BYTE *) &value, sizeof(value)) != ERROR_SUCCESS) {
            MessageBox(NULL, "ReducePing encountered an error while setting the TcpAckFrequency registry value, attempting to continue (this should never happen).", "ReducePing", MB_ICONEXCLAMATION | MB_OK);
            RegCloseKey(tcpkey);
            continue;
        }
        
        RegCloseKey(tcpkey);
    }
    
    RegCloseKey(rootkey);
}

void disable_reduceping() {
    HKEY rootkey;
    
    /* open the root key */
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, ROOT_KEY, 0, KEY_READ, &rootkey) != ERROR_SUCCESS) {
        MessageBox(NULL, "ReducePing can't find any network interface keys in the registry and can not continue.", "ReducePing", MB_ICONEXCLAMATION | MB_OK);
        exit(1);
    }
    
    /* get number of sub keys (these are the TCP interfaces, assuming a regular windows registry) */
    DWORD numsubkeys;
    if (RegQueryInfoKey(rootkey, NULL, NULL, NULL, &numsubkeys, NULL, NULL, NULL, NULL, NULL, NULL, NULL) != ERROR_SUCCESS) {
        MessageBox(NULL, "ReducePing has encountered an error during querying the amount of sub keys and can not continue.", "ReducePing", MB_ICONEXCLAMATION | MB_OK);
        RegCloseKey(rootkey);
        exit(1);
    }
    
    /* iterate over sub keys */
    long i;
    for (i = 0; i < numsubkeys; i++) {
        char subkey[256 + sizeof(ROOT_KEY)];
        
        strncpy(subkey, ROOT_KEY, strlen(ROOT_KEY));
        
        if (RegEnumKey(rootkey, i, subkey + strlen(ROOT_KEY), sizeof(subkey)) != ERROR_SUCCESS) {
            MessageBox(NULL, "ReducePing has encountered an error during enumerating registry keys, attempting to continue.", "ReducePing", MB_ICONEXCLAMATION | MB_OK);
            continue;
        }
        
        /* open sub key for writing (in our case deletion) */
        HKEY tcpkey;
        if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, subkey, 0, KEY_WRITE, &tcpkey) != ERROR_SUCCESS) {
            MessageBox(NULL, "ReducePing can't write to the registry, make sure that your Windows account has Administrator rights.", "ReducePing", MB_ICONEXCLAMATION | MB_OK);
            RegCloseKey(rootkey);
            return;
        }
        
        /* delete the TcpAckFrequency value */
        if (RegDeleteValue(tcpkey, "TcpAckFrequency") != ERROR_SUCCESS) {
            MessageBox(NULL, "ReducePing encountered an error while deleting the TcpAckFrequency registry value, attempting to continue.", "ReducePing", MB_ICONEXCLAMATION | MB_OK);
            RegCloseKey(tcpkey);
            continue;
        }
        
        RegCloseKey(tcpkey);
    }
    
    RegCloseKey(rootkey);
}

void update_ui(HWND hWnd) {
    switch (is_reduceping_enabled()) {
    case 0:
        EnableWindow(GetDlgItem(hWnd, BTN_DISABLE_REDUCEPING), FALSE);
        EnableWindow(GetDlgItem(hWnd, BTN_ENABLE_REDUCEPING), TRUE);
        SetDlgItemText(hWnd, TEXT_REDUCEPING_ENABLED, "Disabled");
        break;
    case 1:
        EnableWindow(GetDlgItem(hWnd, BTN_ENABLE_REDUCEPING), FALSE);
        EnableWindow(GetDlgItem(hWnd, BTN_DISABLE_REDUCEPING), TRUE);
        SetDlgItemText(hWnd, TEXT_REDUCEPING_ENABLED, "Enabled");
        break;
    case -1:
        EnableWindow(GetDlgItem(hWnd, BTN_ENABLE_REDUCEPING), TRUE);
        EnableWindow(GetDlgItem(hWnd, BTN_DISABLE_REDUCEPING), TRUE);
        SetDlgItemText(hWnd, TEXT_REDUCEPING_ENABLED, "Partially enabled");
        break;
    }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_COMMAND:
            /* did we get a button press? */
            if (((HWND)lParam) && (HIWORD(wParam) == BN_CLICKED)) {
                int btn = LOWORD(wParam);
                
                switch (btn) {
                case BTN_DISABLE_REDUCEPING:
                    disable_reduceping();
                    update_ui(hwnd);
                    break;
                case BTN_ENABLE_REDUCEPING:
                    enable_reduceping();
                    update_ui(hwnd);
                    break;
                }
            }
            break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    /* start with creating our application's default fonts */
    HFONT default_font;
    HFONT bold_font;
    
    NONCLIENTMETRICS nonclientmetrics;
    nonclientmetrics.cbSize = sizeof(NONCLIENTMETRICS);
    
    if (!SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &nonclientmetrics, 0)) {
        MessageBox(NULL, "Error while retrieving default font", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 1;
    }
    
    LOGFONT bold_logfont = nonclientmetrics.lfMessageFont;
    bold_logfont.lfWeight = 700;
    bold_logfont.lfHeight *= 1.1;
    
    default_font = CreateFontIndirect(&nonclientmetrics.lfMessageFont);
    bold_font = CreateFontIndirect(&bold_logfont);
    
    if (!default_font || !bold_font) {
        MessageBox(NULL, "Error while retrieving default font", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 1;
    }

    /* register window class */
    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) COLOR_WINDOW;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "ReducePingClass";
    wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    /* create window */
    HWND hwnd = CreateWindowEx(0, "ReducePingClass", "ReducePing", WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 398, 260, NULL, NULL, hInstance, NULL);

    if (!hwnd) {
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
    
    /* create controls */
    HWND text = CreateWindow("static", "This is ReducePing, a small utility to tune the \"TCPAckFrequency\" setting of Windows, which can drastically improve latency for some applications (mainly games) at the cost of slightly less throughput.\n\nBy default Windows waits with replying to a server so it can combine replies into one. This improves bandwidth, at the cost of latency. ReducePing disables this feature, so your latency is improved at the cost of a bit less bandwidth.\n\nAfter enabling/disabling ReducePing a restart is required for the change to take effect.",  WS_CHILD | WS_VISIBLE | SS_LEFT, 10, 10, 370, 280,  hwnd, NULL, hInstance, NULL);
    
    HWND reduceping_enabled = CreateWindow("static", "Enabled", WS_CHILD | WS_VISIBLE | SS_CENTER, 398 / 2 - 130 / 2, 260 - 10 - 40 - 15, 130, 20, hwnd, (HMENU) TEXT_REDUCEPING_ENABLED, hInstance, NULL);
    
    HWND disable_reduceping = CreateWindowEx(0, "BUTTON", "Disable ReducePing", WS_VISIBLE |  WS_CHILD |  BS_PUSHBUTTON, 10, 260 - 10 - 60, 115, 30, hwnd, (HMENU)BTN_DISABLE_REDUCEPING, hInstance, NULL);
    
    HWND enable_reduceping = CreateWindowEx(0, "BUTTON", "Enable ReducePing", WS_VISIBLE |  WS_CHILD |  BS_PUSHBUTTON, 400 - 115 - 20, 260 - 10 - 60, 115, 30, hwnd, (HMENU)BTN_ENABLE_REDUCEPING, hInstance, NULL);
    
    /* set fonts */
    SendMessage(text, WM_SETFONT, (WPARAM) default_font, MAKELPARAM(TRUE, 0));
    SendMessage(disable_reduceping, WM_SETFONT, (WPARAM) default_font, MAKELPARAM(TRUE, 0));
    SendMessage(enable_reduceping, WM_SETFONT, (WPARAM) default_font, MAKELPARAM(TRUE, 0));
    SendMessage(reduceping_enabled, WM_SETFONT, (WPARAM) bold_font, MAKELPARAM(TRUE, 0));
    
    /* update the UI */
    update_ui(hwnd);

    /* update and show window */
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    /* start message loop */
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    /* clean up */
    DeleteObject(default_font);
    DeleteObject(bold_font);
    
    return msg.wParam;
}