/**
 * @file    main.cpp
 * @author  meng_yu
 * @brief   main function realize
 * @version 0.0.1
 * @date    2020-04-25
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */

/* Includes ------------------------------------------------------------------*/
#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include "resource.h"
#include "comm_def.h"
#include "main.h"

/* Private declaration -------------------------------------------------------*/
/* Extern variables declaration ----------------------------------------------*/
/* Global variables definition -----------------------------------------------*/
/* Local functions declaration -----------------------------------------------*/
/* Local functions definition ------------------------------------------------*/

HICON hDlgIcon;

char tmpBuf[128];

void FindFile(const std::string strPath)
{

    WIN32_FIND_DATAA findData = {0};
    const std::string strFindPath = strPath + "\\*.*";
    HANDLE hFindFine = FindFirstFileA(strFindPath.c_str(), &findData);
    if (INVALID_HANDLE_VALUE == hFindFine)
    {
        LOG_DBG("FindFile Error:%ld", GetLastError());
        goto GoOut;
    }
    do
    {
        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if (findData.cFileName[0] == '.')
                continue;
            LOG_DBG("%s FIND DIRECTORY==%s\t", strPath.c_str(), findData.cFileName);
            const std::string strNeedFindPaht = strPath + "\\" + findData.cFileName;
            FindFile(strNeedFindPaht);
        }
        else
        {
            LOG_DBG("%s FIND FILE--%s\t", strPath.c_str(), findData.cFileName);
        }
    } while (FindNextFileA(hFindFine, &findData));
GoOut:
    FindClose(hFindFine);
}

void InsertListviewItem(HWND hList, int nPos, LPSTR wszFirst, LPSTR wszSecond, LPSTR wszThrid)
{
    LVITEM lvi;
    lvi.mask = LVIF_TEXT | LVIF_IMAGE;
    lvi.iImage = 0;
    lvi.pszText = (LPSTR) "";
    lvi.cchTextMax = 1024;

    lvi.iItem = nPos;
    lvi.iSubItem = 0;
    ListView_InsertItem(hList, &lvi);
    ListView_SetItemText(hList, nPos, 0, wszFirst);
    lvi.iSubItem = 1;
    ListView_SetItemText(hList, nPos, 1, wszSecond);
    lvi.iSubItem = 2;
    ListView_SetItemText(hList, nPos, 2, wszThrid);
    return;
}

void CreateListView(HWND hwndDlg, HWND hList)
{
    char tmp1[64];
    char tmp2[64];
    char tmp3[64];

    // HMODULE hShell = LoadLibraryA("shell32.dll");
    // HICON hIcon = LoadIcon(hShell, MAKEINTRESOURCE(17));
    // HIMAGELIST hImageList = ImageList_Create(GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), ILC_MASK, 1, 0);
    // ImageList_AddIcon(hImageList, hIcon);
    // ListView_SetImageList(hList, hImageList, LVSIL_SMALL); //lStyle = LV_VIEW_SAMLLICON or LV_VIEW_TILE ThiPara LVSIL_NORMAL
    LVCOLUMN col;
    col.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
    col.cx = 45;
    col.iSubItem = 0;
    col.pszText = (LPSTR) "ID";

    ListView_InsertColumn(hList, 0, &col);
    col.cx = 82;
    col.iSubItem = 0;
    col.pszText = (LPSTR) "SecTest";
    ListView_InsertColumn(hList, 1, &col);

    col.cx = 82;
    col.iSubItem = 0;
    col.pszText = (LPSTR) "ThiTest";
    ListView_InsertColumn(hList, 2, &col);
    for (int i = 0; i < 20; i++)
    {
        sprintf(tmp1, "%d", i + 1);
        sprintf(tmp2, "test%d", i);
        sprintf(tmp3, "TEST%d", i);
        InsertListviewItem(hList, i, (LPSTR)tmp1, (LPSTR)tmp2, (LPSTR)tmp3);
    }
    LONG lStyle = GetWindowLong(hList, GWL_STYLE);
    lStyle &= ~LVS_TYPEMASK;
    lStyle |= LV_VIEW_DETAILS; //LV_VIEW_DETAILS LV_VIEW_ICON LV_VIEW_SMALLICON LV_VIEW_LIST LV_VIEW_TILE
    ListView_SetExtendedListViewStyle(hList, LVS_EX_FULLROWSELECT);
    SetWindowLong(hList, GWL_STYLE, lStyle);
}

void ReadTnifileStr(LPCTSTR Section, LPCTSTR Key, LPTSTR Buf, LPCTSTR IniName)
{
    assert(NULL != Section);
    assert(NULL != Key);
    assert(NULL != Buf);
    assert(NULL != IniName);
    GetPrivateProfileString(Section, Key, (LPCTSTR) "Read init didn't work!", Buf, sizeof(Buf), IniName);
}

void StayOnTop(HWND hwndDlg)
{
    HWND hWin = GetDlgItem(hwndDlg, IDC_CHK_StayOnTop);
    LOG_DBG("StayOnTop:%lld", SendMessage(hWin, BM_GETCHECK, 0, 0));
    sprintf(tmpBuf, "StayOnTop:%lld", SendMessage(hWin, BM_GETCHECK, 0, 0));
    SetDlgItemText(hwndDlg, IDC_STATIC1, (LPCTSTR)tmpBuf);
    SetDlgItemText(hwndDlg, IDC_EDIT_CTRL1, (LPCTSTR)tmpBuf);
    if (SendMessage(hWin, BM_GETCHECK, 0, 0))
    {
        SetWindowPos(hwndDlg, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    }
    else
    {
        SetWindowPos(hwndDlg, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    }
}
void ListBoxCtrl(HWND hwndDlg, WPARAM wParam)
{
    switch (LOWORD(wParam))
    {
    case IDC_BUTN_ListBoxClear:
        SendDlgItemMessage(hwndDlg, IDC_LISTBOX, LB_RESETCONTENT, 0, 0);
        break;
    case IDC_BUTN_ListBoxAdd:
    {
        BOOL bSuccess;
        int nTimes = GetDlgItemInt(hwndDlg, IDC_EDIT_Times, &bSuccess, FALSE);
        if (bSuccess)
        {
            int len = GetWindowTextLength(GetDlgItem(hwndDlg, IDC_EDIT_AddStr));
            if (len > 0)
            {
                LOG_DBG("Times:%d,strlen:%d", nTimes, len);
                int i;
                char *buf;
                buf = (char *)GlobalAlloc(GPTR, len + 1);
                GetDlgItemText(hwndDlg, IDC_EDIT_AddStr, buf, len + 1);
                for (i = 0; i < nTimes; i++)
                {
                    int index = SendDlgItemMessage(hwndDlg, IDC_LISTBOX, LB_ADDSTRING, 0, (LPARAM)buf);
                    SendDlgItemMessage(hwndDlg, IDC_LISTBOX, LB_SETITEMDATA, (WPARAM)index, (LPARAM)nTimes);
                }
                GlobalFree((HANDLE)buf);
            }
            else
            {
                MessageBox(hwndDlg, "You didn't enter anything!", "Warning", MB_OK);
            }
        }
        else
        {
            MessageBox(hwndDlg, "Couldn't translate that number :(", "Warning", MB_OK);
        }
        break;
    }
    case IDC_BUTN_ListBoxRemove:
    {
        HWND hList = GetDlgItem(hwndDlg, IDC_LISTBOX);
        int cnt = SendMessage(hList, LB_GETSELCOUNT, 0, 0);
        if (cnt != LB_ERR)
        {
            if (cnt != 0)
            {
                int *buf = (int *)GlobalAlloc(GPTR, sizeof(int) * cnt);
                SendMessage(hList, LB_GETSELITEMS, (WPARAM)cnt, (LPARAM)buf);
                for (int i = cnt - 1; i >= 0; i--)
                {
                    SendMessage(hList, LB_DELETESTRING, (WPARAM)buf[i], 0);
                }
                GlobalFree(buf);
            }
            else
            {
                MessageBox(hwndDlg, "No items selected.", "Warning", MB_OK);
            }
        }
        else
        {
            MessageBox(hwndDlg, "Error counting items :(", "Warning", MB_OK);
        }
        break;
    }
    case IDC_LISTBOX:
    {
        switch (HIWORD(wParam))
        {
        case LBN_SELCHANGE:
        {
            HWND hList = GetDlgItem(hwndDlg, IDC_LISTBOX);
            int cnt = SendMessage(hList, LB_GETSELCOUNT, 0, 0);
            if (cnt != LB_ERR)
            {
                if (cnt == 1)
                {
                    int index;
                    int err = SendMessage(hList, LB_GETSELITEMS, (WPARAM)1, (LPARAM)&index);
                    if (err != LB_ERR)
                    {
                        int data = SendMessage(hList, LB_GETITEMDATA, (WPARAM)index, 0);
                        SetDlgItemInt(hwndDlg, IDC_TEXT_ListBoxText, data, FALSE);
                    }
                    else
                    {
                        MessageBox(hwndDlg, "Error getting selected item :(", "Warning", MB_OK);
                    }
                }
                else
                {
                    SetDlgItemText(hwndDlg, IDC_TEXT_ListBoxText, "-");
                }
            }
            else
            {
                MessageBox(hwndDlg, "Error counting items :(", "Warning", MB_OK);
            }
            break;
        }
        default:
            break;
        }
        break;
    }
    }
}
BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
    {
        if (hDlgIcon)
            SendMessage(hwndDlg, WM_SETICON, ICON_SMALL, (LPARAM)hDlgIcon);

        RECT rect;
        GetWindowRect(hwndDlg, &rect);
        LOG_DBG("rect l:%ld t:%ld r:%ld b:%ld", rect.left, rect.top, rect.right, rect.bottom);

        WritePrivateProfileString(INI_SECTION1, INI_KEY2_NAME, (LPCTSTR) "55", INI_NAME);
        memset(tmpBuf, 0, sizeof(tmpBuf));
        GetPrivateProfileString(INI_SECTION1, INI_KEY1_STAYONTOP, (LPCTSTR) "Get didn't work", (LPTSTR)tmpBuf, 80, INI_NAME);
        LOG_DBG("%s", tmpBuf);
        LOG_DBG(INI_SECTION1 INI_KEY2_NAME ":%d", GetPrivateProfileInt(INI_SECTION1, INI_KEY2_NAME, INI_DEF_VAL, INI_NAME));

        SetDlgItemText(hwndDlg, IDC_STATIC1, (LPCTSTR) "INITDIALOG");

        HWND hSlder = GetDlgItem(hwndDlg, IDC_SLIDER_CTRL1);
        SendMessage(hSlder, TBM_SETRANGE, TRUE, (LPARAM)MAKELONG(0, 100));
        HWND hProgressBar = GetDlgItem(hwndDlg, IDC_PROGRESS_BAR1);
        SendMessage(hProgressBar, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
        SendMessage(hProgressBar, PBM_SETSTEP, (WPARAM)1, 0);

        HWND hListview = GetDlgItem(hwndDlg, IDC_LISTVIEW_CTRL1); // get the ID of the ListView
        CreateListView(hwndDlg, hListview);

        SetDlgItemText(hwndDlg, IDC_EDIT_AddStr, "Hello world!");
        SetDlgItemInt(hwndDlg, IDC_EDIT_Times, 2, FALSE);
        return TRUE;
    }
    case WM_CLOSE:
    {
        // RECT rect;
        // GetWindowRect(hwndDlg,&rect);
        //WritePrivateProfileString(INI_SECTION1, INI_KEY1_STAYONTOP, rect.left, INI_NAME);
        EndDialog(hwndDlg, 0);
        return TRUE;
    }
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case IDC_CHK_StayOnTop:
            StayOnTop(hwndDlg);
            break;
        case IDC_BUTN_L:
        {
            HWND hChk2 = GetDlgItem(hwndDlg, IDC_CHK2);
            SendMessage(hChk2, BM_SETCHECK, (WPARAM)!SendMessage(hChk2, BM_GETCHECK, 0, 0), 0);
            LOG_DBG("IDC_BUTN_L");
            break;
        }
        case IDC_BUTN_R:
        {
            HWND hRado1 = GetDlgItem(hwndDlg, IDC_RADO1);
            SendMessage(hRado1, BM_SETCHECK, (WPARAM)!SendMessage(hRado1, BM_GETCHECK, 0, 0), 0);
            LOG_DBG("IDC_BUTN_R");
            break;
        }
        case IDC_RADO1:
            LOG_DBG("IDC_RADO1");
            break;
        case IDC_LISTBOX:
        case IDC_BUTN_ListBoxAdd:
        case IDC_BUTN_ListBoxRemove:
        case IDC_BUTN_ListBoxClear:
            ListBoxCtrl(hwndDlg, wParam);
            break;
        }
        return TRUE;
    }
    case WM_LBUTTONDOWN:
        LOG_DBG("WM_LBUTTONDOWN");
        return TRUE;
    case WM_RBUTTONDOWN:
        LOG_DBG("WM_RBUTTONDOWN");
        return TRUE;
    case WM_HSCROLL:
    {
        HWND hSlder = GetDlgItem(hwndDlg, IDC_SLIDER_CTRL1);
        LONG value = (LONG)SendMessage(hSlder, TBM_GETPOS, 0, 0);
        sprintf(tmpBuf, "Pos:%ld", value);
        SetDlgItemText(hwndDlg, IDC_STATIC1, (LPCTSTR)tmpBuf);
        SetDlgItemText(hwndDlg, IDC_EDIT_CTRL1, (LPCTSTR)tmpBuf);
        HWND hProgressBar = GetDlgItem(hwndDlg, IDC_PROGRESS_BAR1);
        SendMessage(hProgressBar, PBM_SETPOS, value, 0);
        return TRUE;
    }
    }
    return FALSE;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    InitCommonControls();
    hDlgIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    //FindFile("D:\\Documents\\_Project_\\git\\myWin32");
    return DialogBox(hInstance, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);
}
