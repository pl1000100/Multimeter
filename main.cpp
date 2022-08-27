#include <windows.h>
#include <stdio.h>


#define ID_TEXT 101
#define ID_PRZYCISK1 102
#define POMIAR 201

const char g_szClassName[] = "myWindowClass";

HWND hText;
HFONT hFont;

LPTSTR findPort() {
    static DCB dcb;
    static HANDLE hNumPort;

    dcb.DCBlength = sizeof(dcb);

    dcb.BaudRate = CBR_4800;
    dcb.fParity = FALSE;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
    dcb.ByteSize = 6;

    dcb.fDtrControl = DTR_CONTROL_ENABLE;
    dcb.fRtsControl = RTS_CONTROL_ENABLE;

    dcb.fOutxCtsFlow = FALSE;
    dcb.fOutxDsrFlow = FALSE;
    dcb.fDsrSensitivity = FALSE;
    dcb.fAbortOnError = FALSE;
    dcb.fOutX = FALSE;
    dcb.fInX = FALSE;
    dcb.fErrorChar = FALSE;
    dcb.fNull = FALSE;

    char *port = (char *) malloc(4);
    char szChar[1] = "";

    for (int i = 9; i > 1; i--) {
        sprintf(szChar, "%d", i);
        strcpy(port, "COM");
        strcat(port, szChar);
        hNumPort = CreateFile(port, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
        if (SetCommState(hNumPort, &dcb)) {
            CloseHandle(hNumPort);
            return port;
        }
    }
    return 0;
}

char *getData(LPTSTR port) {
    static DCB dcb;
    static HANDLE hNumPort;
    static char lpBuffor_read[33];
    static DWORD RS_ile;

    hNumPort = CreateFile(port, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);

    dcb.DCBlength = sizeof(dcb);

    dcb.BaudRate = CBR_4800;
    dcb.fParity = FALSE;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
    dcb.ByteSize = 6;

    dcb.fDtrControl = DTR_CONTROL_ENABLE;
    dcb.fRtsControl = RTS_CONTROL_ENABLE;

    dcb.fOutxCtsFlow = FALSE;
    dcb.fOutxDsrFlow = FALSE;
    dcb.fDsrSensitivity = FALSE;
    dcb.fAbortOnError = FALSE;
    dcb.fOutX = FALSE;
    dcb.fInX = FALSE;
    dcb.fErrorChar = FALSE;
    dcb.fNull = FALSE;

    if (SetCommState(hNumPort, &dcb)) {

        ReadFile(hNumPort, lpBuffor_read, 30, &RS_ile, 0);
        CloseHandle(hNumPort);
        return lpBuffor_read;
    }

    return 0;
}

char *setOutput(char *lpBuffor_read) {
    char *output = (char *) malloc(20);
    LPTSTR buffor[5];
    if (lpBuffor_read[0] != 31)
        for (int i = 0; i < 11; i++) {
            if (lpBuffor_read[i] == 31) {
                for (int j = 0; j < 22; j++)lpBuffor_read[j] = lpBuffor_read[j + i];
                break;
            }

        }
    if (lpBuffor_read[0] == 31)//&& lpBuffor_read[10]==30)
    {
        for (int i = 4; i < 9; i++) {
            switch (lpBuffor_read[i]) {
                case 32 :
                    buffor[i - 4] = "0";
                    break;
                case 33 :
                    buffor[i - 4] = "1";
                    break;
                case 34 :
                    buffor[i - 4] = "2";
                    break;
                case 35 :
                    buffor[i - 4] = "3";
                    break;
                case 36 :
                    buffor[i - 4] = "4";
                    break;
                case 37 :
                    buffor[i - 4] = "5";
                    break;
                case 38 :
                    buffor[i - 4] = "6";
                    break;
                case 39 :
                    buffor[i - 4] = "7";
                    break;
                case 40 :
                    buffor[i - 4] = "8";
                    break;
                case 41 :
                    buffor[i - 4] = "9";
                    break;
            }
        }

        switch (lpBuffor_read[1])//funkcja
        {
            case 32 ://DC
                strcpy(output, "DC ");
                if (lpBuffor_read[3] == 38)strcat(output, "+");
                if (lpBuffor_read[3] == 41)strcat(output, "-");
                switch (lpBuffor_read[2])//range
                {
                    case 32 :
                        if (buffor[0] != "0")strcat(output, buffor[0]);
                        if (buffor[0] != "0" || buffor[1] != "0")strcat(output, buffor[1]);
                        strcat(output, buffor[2]);
                        strcat(output, ".");
                        strcat(output, buffor[3]);
                        strcat(output, "mV");
                        break;
                    case 33 :
                        strcat(output, buffor[0]);
                        strcat(output, ".");
                        strcat(output, buffor[1]);
                        strcat(output, buffor[2]);
                        strcat(output, buffor[3]);
                        strcat(output, "V");
                        break;
                    case 34 :
                        if (buffor[0] != "0")strcat(output, buffor[0]);
                        strcat(output, buffor[1]);
                        strcat(output, ".");
                        strcat(output, buffor[2]);
                        strcat(output, buffor[3]);
                        strcat(output, "V");
                        break;
                    case 35 :
                        if (buffor[0] != "0")strcat(output, buffor[0]);
                        if (buffor[0] != "0" || buffor[1] != "0")strcat(output, buffor[1]);
                        strcat(output, buffor[2]);
                        strcat(output, ".");
                        strcat(output, buffor[3]);
                        strcat(output, "V");
                        break;
                }
                break;
            case 40 ://AC
                strcpy(output, "AC ");
                switch (lpBuffor_read[2])//range
                {
                    case 32 :
                        if (buffor[0] != "0")strcat(output, buffor[0]);
                        if (buffor[0] != "0" || buffor[1] != "0")strcat(output, buffor[1]);
                        strcat(output, buffor[2]);
                        strcat(output, ".");
                        strcat(output, buffor[3]);
                        strcat(output, "mV");
                        break;
                    case 33 :
                        strcat(output, buffor[0]);
                        strcat(output, ".");
                        strcat(output, buffor[1]);
                        strcat(output, buffor[2]);
                        strcat(output, buffor[3]);
                        strcat(output, "V");
                        break;
                    case 34 :
                        if (buffor[0] != "0")strcat(output, buffor[0]);
                        strcat(output, buffor[1]);
                        strcat(output, ".");
                        strcat(output, buffor[2]);
                        strcat(output, buffor[3]);
                        strcat(output, "V");
                        break;
                    case 35 :
                        if (buffor[0] != "0")strcat(output, buffor[0]);
                        if (buffor[0] != "0" || buffor[1] != "0")strcat(output, buffor[1]);
                        strcat(output, buffor[2]);
                        strcat(output, ".");
                        strcat(output, buffor[3]);
                        strcat(output, "V");
                        break;
                }
                break;
            case 46 ://Hz
                strcpy(output, "Hz ");
                switch (lpBuffor_read[2])//range
                {
                    case 32 :
                        strcat(output, buffor[0]);
                        strcat(output, ".");
                        strcat(output, buffor[1]);
                        strcat(output, buffor[2]);
                        strcat(output, buffor[3]);
                        strcat(output, "Hz");
                        break;
                    case 33 :
                        if (buffor[0] != "0")strcat(output, buffor[0]);
                        strcat(output, buffor[1]);
                        strcat(output, ".");
                        strcat(output, buffor[2]);
                        strcat(output, buffor[3]);
                        strcat(output, "Hz");
                        break;
                    case 34 :
                        if (buffor[0] != "0")strcat(output, buffor[0]);
                        if (buffor[0] != "0" || buffor[1] != "0")strcat(output, buffor[1]);
                        strcat(output, buffor[2]);
                        strcat(output, ".");
                        strcat(output, buffor[3]);
                        strcat(output, "Hz");
                        break;
                    case 35 :
                        strcat(output, buffor[0]);
                        strcat(output, ".");
                        strcat(output, buffor[1]);
                        strcat(output, buffor[2]);
                        strcat(output, buffor[3]);
                        strcat(output, "kHz");
                        break;
                    case 36 :
                        if (buffor[0] != "0")strcat(output, buffor[0]);
                        strcat(output, buffor[1]);
                        strcat(output, ".");
                        strcat(output, buffor[2]);
                        strcat(output, buffor[3]);
                        strcat(output, "kHz");
                        break;
                    case 37 :
                        if (buffor[0] != "0")strcat(output, buffor[0]);
                        if (buffor[0] != "0" || buffor[1] != "0")strcat(output, buffor[1]);
                        strcat(output, buffor[2]);
                        strcat(output, ".");
                        strcat(output, buffor[3]);
                        strcat(output, "kHz");
                        break;
                }
                break;

        }
        return output;
    }
    return (LPTSTR) "0";

}

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    switch (msg) {

        case WM_TIMER:
            SendMessage(hwnd, WM_COMMAND, POMIAR, 0);
            break;
        case WM_COMMAND:
            switch (wParam) {
                case ID_PRZYCISK1:
                    SendMessage(hwnd, WM_COMMAND, POMIAR, 0);
                    break;
                case POMIAR:
                    SendMessage( hText, WM_SETFONT,( WPARAM ) hFont, TRUE );
//                    //wyświetl port
//                    SetWindowText(hText, findPort());
//                    //wyswietl dane
//                    SetWindowText(hText, getData(findPort()));
//                    //wyswietl sformatowane
                    LPTSTR port = findPort();
                    if (port != 0) {
                        char *data = getData(port);
                        if (data != 0) {
                            char *show = setOutput(data);
                            if (show != 0)
                                SetWindowText(hText, show);
                            if (data[1] == 46) {
                                KillTimer(hwnd, 1);
                                for (int i = 0; i < 5; i++) {
                                    data = getData(port);
                                    if (data != 0 && data[1] == 46) {
                                        show = setOutput(data);
                                        if (show != 0) {
                                            SetWindowText(hText, show);
                                            i = 0;
                                        }
                                    }
                                }
                                SetTimer(hwnd, 1, 100, (TIMERPROC) NULL);
                            }
                        }
                    }
                    break;
            }
            break;
        case WM_SIZE:
            RECT rect;
            double size;
            GetWindowRect(hwnd, &rect);

            SetWindowPos(hText, 0, 0, 0, rect.right - rect.left - 20,
                         rect.bottom - rect.top - 40, SWP_SHOWWINDOW);

            if((rect.right - rect.left)/4>(rect.bottom - rect.top)/4*3)size=(rect.bottom - rect.top)/4*3;
            else size=(rect.right - rect.left)/4;

            hFont = CreateFont(size, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Agency FB");

            break;
        case WM_CREATE:
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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    //Step 1: Registering the Window Class
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
                   MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Step 2: Creating the Window
    hwnd = CreateWindowEx(
            WS_EX_CLIENTEDGE,
            g_szClassName,
            "MULTIMETR",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, 600, 200,
            NULL, NULL, hInstance, NULL);

    RECT rect;
    GetWindowRect(hwnd, &rect);

    hFont = CreateFont(150, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Agency FB");
    SendMessage( hText, WM_SETFONT,( WPARAM ) hFont, TRUE );

    hText = CreateWindowEx(0, "STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_CENTER, 0, 0, rect.right - rect.left - 20,
                           rect.bottom - rect.top - 40, hwnd, (HMENU) ID_TEXT, hInstance, NULL);

    SetTimer(hwnd,             // handle to main window
             1,            // timer identifier
             100,                 // 1-second interval
             (TIMERPROC) NULL);     // no timer callback

    if (hwnd == NULL) {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
                   MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Step 3: The Message Loop
    while (GetMessage(&Msg, NULL, 0, 0) > 0) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}