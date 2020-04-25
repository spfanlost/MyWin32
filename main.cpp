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
LVCOLUMN LvCol; // Make Coluom struct for ListView
LVITEM LvItem;  // ListView Item struct
LV_DISPINFO lvd;

char tmpstr[128];
CHAR inBuf[80];

BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int i;
    char Temp[255];
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
        SendMessage(hList,LVM_SETTEXTBKCOLOR, 0,(LPARAM)CLR_NONE);

        memset(&LvCol,0,sizeof(LvCol));                 // Reset Coluom
        LvCol.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;   // Type of mask
        LvCol.cx=0x42;                                  // width between each coloum
        LvCol.pszText=(LPSTR)"Item";                    // First Header
        SendMessage(hList,LVM_INSERTCOLUMN,0,(LPARAM)&LvCol);   // Insert/Show the coloum
        LvCol.cx=0x56;
        LvCol.pszText=(LPSTR)"SubItem1";                        // Next coloum
        SendMessage(hList,LVM_INSERTCOLUMN,1,(LPARAM)&LvCol);   // ...
        LvCol.pszText=(LPSTR)"SubItem2";                        //
        SendMessage(hList,LVM_INSERTCOLUMN,2,(LPARAM)&LvCol);   //

        memset(&LvItem,0,sizeof(LvItem)); // Reset Item Struct
        LvItem.mask=LVIF_TEXT;   // Text Style
        LvItem.cchTextMax = 256; // Max size of test
        LvItem.iItem=0;          // choose item
        LvItem.iSubItem=0;       // Put in first coluom
        LvItem.pszText=(LPSTR)"Item0"; // Text to display (can be from a char variable) (Items)
        SendMessage(hList,LVM_INSERTITEM,0,(LPARAM)&LvItem);    // Send to the Listview

        for(i=1; i<=2; i++) // Add SubItems in a loop
        {
            LvItem.iSubItem=i;
            sprintf(Temp,"SubItem%d",i);
            LvItem.pszText=Temp;
            SendMessage(hList,LVM_SETITEM,0,(LPARAM)&LvItem); // Enter text to SubItems
        }

        // lets add a new Item:
        LvItem.iItem=1;            // choose item
        LvItem.iSubItem=0;         // Put in first coluom
        LvItem.pszText=(LPSTR)"Item1";   // Text to display (can be from a char variable) (Items)
        SendMessage(hList,LVM_INSERTITEM,0,(LPARAM)&LvItem); // Send to the Listview

        for(i=1; i<=2; i++) // Add SubItems in a loop
        {
            LvItem.iSubItem=i;
            sprintf(Temp,(LPSTR)"SubItem%d",i);
            LvItem.pszText=Temp;
            SendMessage(hList,LVM_SETITEM,0,(LPARAM)&LvItem); // Enter etxt to SubItems
        }
        //ListView_SetItemState(hList,0,LVIS_SELECTED	,LVIF_STATE);
        //ShowWindow(hWnd,SW_NORMAL);
        //UpdateWindow(hWnd);

        return TRUE;
    case WM_CLOSE:
        EndDialog(hwndDlg, 0);
        return TRUE;

    case WM_COMMAND:
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
    return DialogBox(hInst, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);
}
