#ifndef LAYOUT_H_INCLUDED
#define LAYOUT_H_INCLUDED
#include <windows.h>

HMENU hMenuBar;
HMENU hFile;
HMENU hHelp;

#define ID_FILE_OPEN 100
#define ID_FILE_SAVE 101
#define ID_FILE_QUIT 102

#define ID_HELP_ABOUT 103
#define ID_HELP_APP 104

#define ID_BUT_PREV 105
#define ID_BUT_NEXT 106
#define ID_BUT_LARR 107
#define ID_BUT_RARR 108

static HWND control[10], l2Control[10];


void Menu(HWND hwnd)
{
    hMenuBar = CreateMenu();
    hFile = CreateMenu();
    hHelp = CreateMenu();

    AppendMenuW(hFile, MF_STRING, ID_FILE_OPEN, L"Open");
    AppendMenuW(hFile, MF_STRING, ID_FILE_SAVE, L"Save");
    AppendMenuW(hFile, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hFile, MF_STRING, ID_FILE_QUIT, L"Quit");

    AppendMenuW(hHelp, MF_STRING, ID_HELP_ABOUT, L"Author");
    AppendMenuW(hHelp, MF_STRING, ID_HELP_APP, L"About");

    AppendMenuW(hMenuBar, MF_POPUP, (UINT_PTR)hFile, L"File");
    AppendMenuW(hMenuBar, MF_POPUP, (UINT_PTR)hHelp, L"Help");

    SetMenu(hwnd, hMenuBar);
}

void layout(HWND hwnd)
{

    control[0] = CreateWindowW(L"Static", L"Place", WS_CHILD | WS_VISIBLE, 520, 10, 100, 20, hwnd, (HMENU)0, NULL, NULL);
    control[2] = CreateWindowW(L"Static", L"12/07/50", WS_CHILD | WS_VISIBLE, 630, 10, 100, 20, hwnd, (HMENU)0, NULL, NULL);
    control[1] = CreateWindowW(L"Button", L"Prev", WS_CHILD | WS_VISIBLE, 520, 40, 70, 30, hwnd, (HMENU)ID_BUT_PREV, NULL, NULL);
    control[3] = CreateWindowW(L"Button", L"Next", WS_CHILD | WS_VISIBLE, 620, 40, 70, 30, hwnd, (HMENU)ID_BUT_NEXT, NULL, NULL);
    control[4] = CreateWindowW(L"Button", L"<", WS_CHILD | WS_VISIBLE, 520, 75, 70, 30, hwnd, (HMENU)ID_BUT_LARR, NULL, NULL);
    control[5] = CreateWindowW(L"Button", L">", WS_CHILD | WS_VISIBLE, 620, 75, 70, 30, hwnd, (HMENU)ID_BUT_RARR, NULL, NULL);
    control[6] = CreateWindowW(L"Static", L"Angle: ", WS_CHILD | WS_VISIBLE, 520, 115, 70, 30, hwnd, (HMENU)0, NULL, NULL);
    control[7] = CreateWindowW(L"Edit", L"0", WS_CHILD | WS_VISIBLE, 620, 115, 90, 20, hwnd, (HMENU)0, NULL, NULL);
}


#endif // LAYOUT_H_INCLUDED
