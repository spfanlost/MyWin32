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


HINSTANCE hInst;
HICON hIcon;
HWND hWin;
static HWND hList=NULL;  // List View identifier

char tmpstr[128];
CHAR inBuf[80];


void FindFile(const std::string strPath)
{

    WIN32_FIND_DATAA  findData = { 0 };
    const std::string strFindPath = strPath + "\\*.*";
    HANDLE hFindFine = FindFirstFileA(strFindPath.c_str(), &findData);
    if (INVALID_HANDLE_VALUE == hFindFine)
        LOG_DBG("Error:%ld", GetLastError());
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
    }
    while (FindNextFileA(hFindFine, &findData));
    FindClose(hFindFine);
}

void InsertListviewItem(HWND hList, int nPos, LPSTR wszFirst, LPSTR wszSecond,LPSTR wszThrid)
{
    LVITEM lvi;
    lvi.mask = LVIF_TEXT | LVIF_IMAGE;
    lvi.iImage = 0;
    lvi.pszText = (LPSTR)"";
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
    int i;

    HMODULE hShell = LoadLibraryA("shell32.dll"); 
    HICON hIcon = LoadIcon(hShell, MAKEINTRESOURCE(17)); 
    HIMAGELIST hImageList = ImageList_Create(GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), ILC_MASK, 1, 0);
    ImageList_AddIcon(hImageList, hIcon);
    ListView_SetImageList(hList, hImageList, LVSIL_SMALL); //lStyle = LV_VIEW_SAMLLICON or LV_VIEW_TILE ThiPara LVSIL_NORMAL
    LVCOLUMN col;
    col.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
    col.cx = 45;
    col.iSubItem = 0;
    col.pszText = (LPSTR)"ID";

    ListView_InsertColumn(hList, 0, &col);
    col.cx = 82;
    col.iSubItem = 0;
    col.pszText = (LPSTR)"SecTest";
    ListView_InsertColumn(hList, 1, &col);

    col.cx = 82;
    col.iSubItem = 0;
    col.pszText = (LPSTR)"ThiTest";
    ListView_InsertColumn(hList, 2, &col);
    for(i=0; i<20; i++)
    {
        sprintf(tmp1,"%d",i+1);
        sprintf(tmp2,"test%d",i);
        sprintf(tmp3,"TEST%d",i);
        InsertListviewItem(hList, i, (LPSTR)tmp1, (LPSTR)tmp2, (LPSTR)tmp3);
    }
    LONG lStyle = GetWindowLong(hList, GWL_STYLE);
    lStyle &= ~LVS_TYPEMASK;
    lStyle |= LV_VIEW_DETAILS; //LV_VIEW_DETAILS LV_VIEW_ICON LV_VIEW_SMALLICON LV_VIEW_LIST LV_VIEW_TILE
    ListView_SetExtendedListViewStyle(hList, LVS_EX_FULLROWSELECT); 
    SetWindowLong(hList, GWL_STYLE, lStyle);

}

BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
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

        SetDlgItemText(hwndDlg, IDC_STATIC1, (LPCTSTR) "INITDIALOG");

        hWin = GetDlgItem(hwndDlg, IDC_SLIDER_CTRL1);
        SendMessage(hWin, TBM_SETRANGE, TRUE, (LPARAM)MAKELONG(0, 100));
        hWin = GetDlgItem(hwndDlg, IDC_PROGRESS_BAR1);
        SendMessage(hWin, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
        SendMessage(hWin, PBM_SETSTEP, (WPARAM)1, 0);

        hList=GetDlgItem(hwndDlg,IDC_LISTVIEW_CTRL1); // get the ID of the ListView
        CreateListView(hwndDlg, hList);

        return TRUE;
    case WM_CLOSE:
        EndDialog(hwndDlg, 0);
        return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_CHK_StayOnTop:
            hWin = GetDlgItem(hwndDlg, IDC_CHK_StayOnTop);
            LOG_DBG("StayOnTop:%lld", SendMessage(hWin, BM_GETCHECK, 0, 0));
            sprintf(tmpstr, "StayOnTop:%lld", SendMessage(hWin, BM_GETCHECK, 0, 0));
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
        return TRUE;
    }
    return FALSE;
}
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    hInst = hInstance;
    InitCommonControls();
    hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
    FindFile("D:\\Documents\\_Project_\\git\\myWin32");
    return DialogBox(hInst, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);
}
