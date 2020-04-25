#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include "resource.h"
#include "comm_def.h"
#include "main.h"

HINSTANCE hInst;
HICON hIcon;
HWND hWin;

char tmpstr[128];
CHAR inBuf[80];

BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // LPSCROLLINFO si;
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;
    switch (uMsg)
    {
    case WM_INITDIALOG:
        {
            if (hIcon)
                SendMessage(hwndDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
            WritePrivateProfileString((LPCTSTR)INI_SECTION1, (LPCTSTR)INI_KEY1_NAME, (LPCTSTR) "It all worked out okay.", (LPCTSTR)INI_NAME);
            WritePrivateProfileString((LPCTSTR)INI_SECTION1, (LPCTSTR)INI_KEY2_NAME, (LPCTSTR) "55", (LPCTSTR)INI_NAME);
            WritePrivateProfileString((LPCTSTR)INI_SECTION2, (LPCTSTR)INI_KEY1_NAME, (LPCTSTR) "is my test string", (LPCTSTR)INI_NAME);
            WritePrivateProfileString((LPCTSTR)INI_SECTION2, (LPCTSTR)INI_KEY2_NAME, (LPCTSTR) "123", (LPCTSTR)INI_NAME);
            memset(inBuf, 0, sizeof(inBuf));
            GetPrivateProfileString((LPCTSTR)INI_SECTION1, (LPCTSTR)INI_KEY1_NAME, (LPCTSTR) "Get didn't work", (LPTSTR)inBuf, 80, (LPCTSTR)INI_NAME);
            LOG_DBG("%s", inBuf);
            memset(inBuf, 0, sizeof(inBuf));
            GetPrivateProfileString((LPCTSTR)INI_SECTION2, (LPCTSTR)INI_KEY1_NAME, (LPCTSTR) "Get didn't work", (LPTSTR)inBuf, 80, (LPCTSTR)INI_NAME);
            LOG_DBG("%s", inBuf);
            LOG_DBG(INI_SECTION1 INI_KEY2_NAME ":%d", GetPrivateProfileInt((LPCTSTR)INI_SECTION1, (LPCTSTR)INI_KEY2_NAME,
                    INI_DEF_VAL, (LPCTSTR)INI_NAME));
            LOG_DBG(INI_SECTION2 INI_KEY2_NAME ":%d", GetPrivateProfileInt((LPCTSTR)INI_SECTION2, (LPCTSTR)INI_KEY2_NAME,
                    INI_DEF_VAL, (LPCTSTR)INI_NAME));

            SetDlgItemText(hwndDlg, IDC_STATIC1, (LPCTSTR) "WM_INITDIALOG");
            // hWin = GetDlgItem(hwndDlg, IDC_SCROLLBAR1);
            // SetScrollRange(hWin, SB_CTL, 0, 100, FALSE);
            // SetScrollPos(hWin, SB_CTL, 25, FALSE);
            hWin = GetDlgItem(hwndDlg, IDC_SLIDER_CTRL1);
            SendMessage(hWin, TBM_SETRANGE, TRUE, (LPARAM)MAKELONG(0, 100));
            hWin = GetDlgItem(hwndDlg, IDC_PROGRESS_BAR1);
            SendMessage(hWin, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
            SendMessage(hWin, PBM_SETSTEP, (WPARAM)1, 0);
        }
        return TRUE;
    case WM_PRINT:
        hdc = BeginPaint(hwndDlg, &ps);
        GetClientRect(hwndDlg, &rect);
        DrawText(hdc, TEXT("Hello, Win32 API!"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        EndPaint(hwndDlg, &ps);
        return TRUE;
    case WM_CLOSE:
        {
            EndDialog(hwndDlg, 0);
        }
        return TRUE;

    case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
            case IDC_CHK_StayOnTop:
                hWin = GetDlgItem(hwndDlg, IDC_CHK_StayOnTop);
                LOG_DBG("StayOnTop:%ld", SendMessage(hWin, BM_GETCHECK, 0, 0));
                sprintf(tmpstr, "StayOnTop:%ld", SendMessage(hWin, BM_GETCHECK, 0, 0));
                SetDlgItemText(hwndDlg, IDC_STATIC1, (LPCTSTR)tmpstr);
                SetDlgItemText(hwndDlg, IDC_EDIT_CTRL1, (LPCTSTR)tmpstr);
                if (SendMessage(hWin, BM_GETCHECK, 0, 0))
                {
                    SetWindowPos(hwndDlg, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
                }
                else
                {
                    SetWindowPos(hwndDlg, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
                }

                break;
            case IDC_BUTN_L:
                hWin = GetDlgItem(hwndDlg, IDC_CHK2);
                SendMessage(hWin, BM_SETCHECK, (WPARAM)!SendMessage(hWin, BM_GETCHECK, 0, 0), 0);
                LOG_DBG("IDC_BUTN_L");
                break;
            case IDC_BUTN_R:
                hWin = GetDlgItem(hwndDlg, IDC_RADO1);
                SendMessage(hWin, BM_SETCHECK, (WPARAM)!SendMessage(hWin, BM_GETCHECK, 0, 0), 0);
                LOG_DBG("IDC_BUTN_R");
                break;
            case IDC_RADO1:
                LOG_DBG("IDC_RADO1");
                break;
            }
        }
        return TRUE;
    case WM_LBUTTONDOWN:
        LOG_DBG("WM_LBUTTONDOWN");
        return TRUE;
    case WM_RBUTTONDOWN:
        LOG_DBG("WM_RBUTTONDOWN");
        return TRUE;
    case WM_HSCROLL:
        hWin = GetDlgItem(hwndDlg, IDC_SLIDER_CTRL1);
        LONG value = (LONG)SendMessage(hWin, TBM_GETPOS, 0, 0);
        sprintf(tmpstr, "Pos:%ld", value);
        SetDlgItemText(hwndDlg, IDC_STATIC1, (LPCTSTR)tmpstr);
        SetDlgItemText(hwndDlg, IDC_EDIT_CTRL1, (LPCTSTR)tmpstr);
        hWin = GetDlgItem(hwndDlg, IDC_PROGRESS_BAR1);
        SendMessage(hWin, PBM_SETPOS, value, 0);

        // hWin = GetDlgItem(hwndDlg, IDC_SCROLLBAR1);
        // switch (LOWORD(wParam))
        // {
        // case SB_LINEUP:
        //     value -= 1;
        //     break;
        // case SB_LINEDOWN:
        //     value += 1;
        //     break;
        // case SB_PAGEUP:
        //     value -= 10;
        //     break;
        // case SB_PAGEDOWN:
        //     value += 10;
        //     break;
        // case SB_THUMBTRACK:
        //     value = HIWORD(wParam);
        //     break;
        // default:
        //     break;
        // }
        // if (value != GetScrollPos(hWin, SB_CTL))
        // {
        //     SetScrollPos(hWin, SB_CTL, value, TRUE);
        //     InvalidateRect(hWin, NULL, FALSE);
        // }
        return TRUE;
    }
    return FALSE;
}
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    hInst = hInstance;
    InitCommonControls();
    hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
    return DialogBox(hInst, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);
}
