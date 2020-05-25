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
#include "comm_def.h"
#include "main.h"
#include <windows.h>
#include <commctrl.h>
#include "resource.h"

/* Private declaration -------------------------------------------------------*/
/* Extern variables declaration ----------------------------------------------*/
/* Global variables definition -----------------------------------------------*/
/* Local functions declaration -----------------------------------------------*/
/* Local functions definition ------------------------------------------------*/

HICON hDlgIcon;
int iSelect=0;
int flag=0;
HWND hEdit;
LVITEM LvItem;  // ListView Item struct
bool escKey=0;
char tempstr[100]="";
TCHAR tchar;
MSG msg;
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

LRESULT
ProcessCustomDraw(LPARAM lParam)
{
    LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW)lParam;
    switch (lplvcd->nmcd.dwDrawStage)
    {
        case CDDS_PREPAINT: // Before the paint cycle begins
            return CDRF_NOTIFYITEMDRAW;// request notifications for individual listview items
        case CDDS_ITEMPREPAINT: // Before an item is drawn
            return CDRF_NOTIFYSUBITEMDRAW;

        case CDDS_SUBITEM | CDDS_ITEMPREPAINT: // Before a subitem is drawn
        {
            switch (lplvcd->iSubItem)
            {
                case 0:
                {
                    lplvcd->clrText = RGB(255, 255, 255);
                    lplvcd->clrTextBk = RGB(240, 55, 23);
                    return CDRF_NEWFONT;
                }
                case 1:
                {
                    lplvcd->clrText = RGB(255, 255, 0);
                    lplvcd->clrTextBk = RGB(0, 0, 0);
                    return CDRF_NEWFONT;
                }
                case 2:
                {
                    lplvcd->clrText = RGB(20, 26, 158);
                    lplvcd->clrTextBk = RGB(200, 200, 10);
                    return CDRF_NEWFONT;
                }
                case 3:
                {
                    lplvcd->clrText = RGB(12, 15, 46);
                    lplvcd->clrTextBk = RGB(200, 200, 200);
                    return CDRF_NEWFONT;
                }
                case 4:
                {
                    lplvcd->clrText = RGB(120, 0, 128);
                    lplvcd->clrTextBk = RGB(20, 200, 200);
                    return CDRF_NEWFONT;
                }
                case 5:
                {
                    lplvcd->clrText = RGB(255, 255, 255);
                    lplvcd->clrTextBk = RGB(0, 0, 150);
                    return CDRF_NEWFONT;
                }
            }
            break;
        }
    }
    return CDRF_DODEFAULT;
}

void InsertListviewFirstItem(HWND hList,
                            int nPos,
                            LPSTR pszText)
{
    LVITEM lvi;
    memset(&lvi,0,sizeof(lvi));
    lvi.mask = LVIF_TEXT | LVIF_IMAGE;
    lvi.iImage = 0;
    lvi.cchTextMax = 256;
    lvi.iItem = nPos;
    lvi.iSubItem=0; // Put in first coluom
    lvi.pszText=pszText; // Text to display (can be from a char variable) (Items)
    ListView_InsertItem(hList, &lvi);
}

//sprintf(Temp,(LPSTR)"sub%d",i);
void InsertListviewSubItem(HWND hList,
                        int nPos,
                        int subCnt, //1base
                        LPSTR pszText)
{
    for(int i=1; i<=subCnt; i++) // Add SubItems in a loop
    {
        ListView_SetItemText(hList, nPos, i, pszText);
    }
}


void CreateListView(HWND hwndDlg, HWND hList)
{
    char tmp1[64];
    char tmp2[64];
    char tmp3[64];

    // HMODULE hShell = LoadLibraryA("shell32.dll");
    // HICON hIcon = LoadIcon(hShell, MAKEINTRESOURCE(17));
    // HIMAGELIST hImageList = ImageList_Create(GetSystemMetrics(SM_CXSMICON),
    // GetSystemMetrics(SM_CYSMICON), ILC_MASK, 1, 0);
    // ImageList_AddIcon(hImageList, hIcon);
    // ListView_SetImageList(hList, hImageList, LVSIL_SMALL); //lStyle =
    // LV_VIEW_SAMLLICON or LV_VIEW_TILE ThiPara LVSIL_NORMAL
    LVCOLUMN col;
    col.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
    col.cx = 40;
    col.pszText = (LPSTR) "ID";
    ListView_InsertColumn(hList, 0, &col);
    col.cx = 50;
    col.pszText = (LPSTR) "One";
    ListView_InsertColumn(hList, 1, &col);
    col.cx = 50;
    col.pszText = (LPSTR) "Two";
    ListView_InsertColumn(hList, 2, &col);
    col.cx = 50;
    col.pszText = (LPSTR) "Thi";
    ListView_InsertColumn(hList, 3, &col);
    col.cx = 50;
    col.pszText = (LPSTR) "For";
    ListView_InsertColumn(hList, 4, &col);
    col.cx = 50;
    col.pszText = (LPSTR) "Fiv";
    ListView_InsertColumn(hList, 5, &col);
    for (int i=0, j = 0; i < 2; i++)
    {
        sprintf(tmp1, "%d", i + 1);
        InsertListviewFirstItem(hList,i,(LPSTR)tmp1);
        for(int sub=1; sub<=5; sub++)
        {
            sprintf(tmp1,(LPSTR)"sub%d",sub);
            InsertListviewSubItem(hList,i,sub,tmp1);
        }
    }
    LONG lStyle = GetWindowLong(hList, GWL_STYLE);
    lStyle &= ~LVS_TYPEMASK;
    lStyle |= LV_VIEW_DETAILS; // LV_VIEW_DETAILS LV_VIEW_ICON LV_VIEW_SMALLICON
        // LV_VIEW_LIST LV_VIEW_TILE
    ListView_SetExtendedListViewStyle(hList, LVS_EX_FULLROWSELECT);
    SetWindowLong(hList, GWL_STYLE, lStyle);
}

void ReadTnifileStr(LPCTSTR Section, LPCTSTR Key, LPTSTR Buf, LPCTSTR IniName)
{
    assert(NULL != Section);
    assert(NULL != Key);
    assert(NULL != Buf);
    assert(NULL != IniName);
    GetPrivateProfileString(Section,
                            Key,
                            (LPCTSTR) "Read init didn't work!",
                            Buf,
                            sizeof(Buf),
                            IniName);
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
                    int index = SendDlgItemMessage(
                        hwndDlg, IDC_LISTBOX, LB_ADDSTRING, 0, (LPARAM)buf);
                    SendDlgItemMessage(hwndDlg,
                                       IDC_LISTBOX,
                                       LB_SETITEMDATA,
                                       (WPARAM)index,
                                       (LPARAM)nTimes);
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
            MessageBox(
                hwndDlg, "Couldn't translate that number :(", "Warning", MB_OK);
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
                        MessageBox(
                            hwndDlg, "Error getting selected item :(", "Warning", MB_OK);
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

BOOL ListViewNotify(HWND hwndDlg, WPARAM wParam, LPARAM lParam)
{
    HWND hListview = GetDlgItem(hwndDlg, IDC_LISTVIEW); // get the ID of the ListView
    LPNMLISTVIEW pnm = (LPNMLISTVIEW)lParam;
    if(pnm->hdr.hwndFrom == hListview &&pnm->hdr.code == NM_CUSTOMDRAW)
    {
        SetWindowLong(hwndDlg, 0/*DWL_MSGRESULT*/, (LONG)ProcessCustomDraw(lParam));
        return TRUE;
    }

    if(((LPNMHDR)lParam)->code == NM_DBLCLK)
    {
        char Text[255]= {0};
        char Temp[255]= {0};
        char Temp1[255]= {0};
        int iSlected=0;
        int j=0;

        iSlected=SendMessage(hListview,LVM_GETNEXTITEM,-1,LVNI_FOCUSED);

        if(iSlected==-1)
        {
            MessageBox(hwndDlg,"No Items in ListView","Error",MB_OK|MB_ICONINFORMATION);
            return TRUE;
        }

        memset(&LvItem,0,sizeof(LvItem));
        LvItem.mask=LVIF_TEXT;
        LvItem.iSubItem=0;
        LvItem.pszText=Text;
        LvItem.cchTextMax=256;
        LvItem.iItem=iSlected;

        SendMessage(hListview,LVM_GETITEMTEXT, iSlected, (LPARAM)&LvItem);

        sprintf(Temp1,Text);

        for(j=1; j<=5; j++)
        {
            LvItem.iSubItem=j;
            SendMessage(hListview,LVM_GETITEMTEXT, iSlected, (LPARAM)&LvItem);
            sprintf(Temp," %s",Text);
            lstrcat(Temp1,Temp);
        }

        MessageBox(hwndDlg,Temp1,"test",MB_OK);

    }
    if(((LPNMHDR)lParam)->code == NM_CLICK)
    {
        iSelect=SendMessage(hListview,LVM_GETNEXTITEM,-1,LVNI_FOCUSED);

        if(iSelect==-1)
        {
            return TRUE;
        }
        flag=1;
    }

    if(((LPNMHDR)lParam)->code == LVN_BEGINLABELEDIT)
    {
        //Editing=1;
        hEdit=ListView_GetEditControl(hListview);
        GetWindowText(hEdit, tempstr, sizeof(tempstr));
    }

    if(((LPNMHDR)lParam)->code == LVN_ENDLABELEDIT)
    {
        int iIndex;
        char text[255]="";

        tchar = (TCHAR)msg.wParam;
        if(tchar == 0x1b)
            escKey=1;

        iIndex=SendMessage(hListview,LVM_GETNEXTITEM,-1,LVNI_FOCUSED);
        if(iIndex==-1)
            return TRUE;

        LvItem.iSubItem=0;

        if(escKey==0)
        {
            LvItem.pszText=text;
            GetWindowText(hEdit, text, sizeof(text));
            SendMessage(hListview,LVM_SETITEMTEXT,(WPARAM)iIndex,(LPARAM)&LvItem);
        }
        else
        {
            LvItem.pszText=tempstr;
            SendMessage(hListview,LVM_SETITEMTEXT,(WPARAM)iIndex,(LPARAM)&LvItem);
            escKey=0;
        }
        //Editing=0;
    }
}
BOOL CALLBACK
DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
    {
        if (hDlgIcon)
            SendMessage(hwndDlg, WM_SETICON, ICON_SMALL, (LPARAM)hDlgIcon);

        RECT rect;
        GetWindowRect(hwndDlg, &rect);
        LOG_DBG("rect l:%ld t:%ld r:%ld b:%ld",
                rect.left,
                rect.top,
                rect.right,
                rect.bottom);

        WritePrivateProfileString(
            INI_SECTION1, INI_KEY2_NAME, (LPCTSTR) "55", INI_NAME);
        memset(tmpBuf, 0, sizeof(tmpBuf));
        GetPrivateProfileString(INI_SECTION1,
                                INI_KEY1_STAYONTOP,
                                (LPCTSTR) "Get didn't work",
                                (LPTSTR)tmpBuf,
                                80,
                                INI_NAME);
        LOG_DBG("%s", tmpBuf);
        LOG_DBG(INI_SECTION1 INI_KEY2_NAME ":%d",
                GetPrivateProfileInt(
                    INI_SECTION1, INI_KEY2_NAME, INI_DEF_VAL, INI_NAME));

        SetDlgItemText(hwndDlg, IDC_STATIC1, (LPCTSTR) "INITDIALOG");

        HWND hSlder = GetDlgItem(hwndDlg, IDC_SLIDER_CTRL1);
        SendMessage(hSlder, TBM_SETRANGE, TRUE, (LPARAM)MAKELONG(0, 100));
        HWND hProgressBar = GetDlgItem(hwndDlg, IDC_PROGRESS_BAR1);
        SendMessage(hProgressBar, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
        SendMessage(hProgressBar, PBM_SETSTEP, (WPARAM)1, 0);

        HWND hListview = GetDlgItem(hwndDlg, IDC_LISTVIEW); // get the ID of the ListView
        CreateListView(hwndDlg, hListview);

        SetDlgItemText(hwndDlg, IDC_EDIT_AddStr, "Hello world!");
        SetDlgItemInt(hwndDlg, IDC_EDIT_Times, 2, FALSE);
        return TRUE;
    }
    case WM_NOTIFY:
        ListViewNotify(hwndDlg, wParam,lParam);
        return TRUE;
    case IDC_LISTVIEW:
        return TRUE;
    case WM_CLOSE:
    {
        // RECT rect;
        // GetWindowRect(hwndDlg,&rect);
        // WritePrivateProfileString(INI_SECTION1, INI_KEY1_STAYONTOP, rect.left,
        // INI_NAME);
        EndDialog(hwndDlg, 0);
    }
        return TRUE;
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
            SendMessage(hChk2,
                        BM_SETCHECK,
                        (WPARAM)!SendMessage(hChk2, BM_GETCHECK, 0, 0),
                        0);
            LOG_DBG("IDC_BUTN_L");
            break;
        }
        case IDC_BUTN_R:
        {
            HWND hRado1 = GetDlgItem(hwndDlg, IDC_RADO1);
            SendMessage(hRado1,
                        BM_SETCHECK,
                        (WPARAM)!SendMessage(hRado1, BM_GETCHECK, 0, 0),
                        0);
            LOG_DBG("IDC_BUTN_R");
            break;
        }
        case IDC_RADO1:
            LOG_DBG("IDC_RADO1");
            break;
        case IDC_LISTBOX:
            LOG_DBG("LISTBOX");
        case IDC_BUTN_ListBoxAdd:
        case IDC_BUTN_ListBoxRemove:
        case IDC_BUTN_ListBoxClear:
            ListBoxCtrl(hwndDlg, wParam);
            break;
        case IDC_BUTN_Listview_AddItem:
            {
                int iItem;
                char ItemText[100];
                HWND hListview = GetDlgItem(hwndDlg, IDC_LISTVIEW); // get the ID of the ListView
                iItem=SendMessage(hListview,LVM_GETITEMCOUNT,0,0);
                GetDlgItemText(hwndDlg,IDC_EDIT_ADD,ItemText,100);
                if((lstrlen(ItemText))==0)
                {
                    MessageBox(hwndDlg,"Please Write Some Text","Error",MB_OK|MB_ICONINFORMATION);
                    break;
                }
                InsertListviewFirstItem(hListview,iItem,ItemText);
            }
            LOG_DBG(TEST_FAIL);
            break;
        case IDC_BUTN_Listview_AddSubItem:
            {
                int Item,i;
                char SubItemText[100];
                HWND hListview = GetDlgItem(hwndDlg, IDC_LISTVIEW); // get the ID of the ListView
                Item=SendMessage(hListview,LVM_GETITEMCOUNT,0,0);

                GetDlgItemText(hwndDlg,IDC_EDIT_ADDSUB,SubItemText,100);

                if((lstrlen(SubItemText))==0)
                {
                    MessageBox(hwndDlg,"Please Write Some Text","Error",MB_OK|MB_ICONINFORMATION);
                    break;
                }
                for(int sub=1; sub<=5; sub++)
                {
                    InsertListviewSubItem(hListview,Item-1,sub,SubItemText);
                }
            }
            LOG_DBG(TEST_PASS);
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

int APIENTRY
WinMain(HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR lpCmdLine,
        int nShowCmd)
{
    InitCommonControls();
    hDlgIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    // FindFile("D:\\Documents\\_Project_\\git\\myWin32");
    return DialogBox(
        hInstance, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);
}
