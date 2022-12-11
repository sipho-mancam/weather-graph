//219957564 Musana S
// Assignment 4
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include "LinkedList.h"
#include "LAYOUT.h"
#include <fstream>
#include <math.h>

using namespace std;

LRESULT CALLBACK windowProcedure(HWND, UINT, WPARAM, LPARAM);

void pullData(int i);
void clearScreen(HWND hwnd);
void drawGraph(HWND hwnd, DATA data, int t);
void redPointer(HWND hwnd, int index);
void readFile(NodePtr *data);
int writeFile(NodePtr *data);


TCHAR winClass [] = _T("Main");

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
    WNDCLASSEX wclex;
    MSG msg;
    HWND hwnd;

    //Important stuff....
    wclex.hInstance = hInstance;
    wclex.lpfnWndProc = windowProcedure;
    wclex.lpszClassName = winClass;
    wclex.cbSize = sizeof(WNDCLASSEX);
    wclex.style = CS_DBLCLKS;
    wclex.lpszMenuName = NULL;
    wclex.cbClsExtra = 0;
    wclex.cbWndExtra = 0;

    //Looks and stuff....
    wclex.hbrBackground = CreateSolidBrush(RGB(240, 240, 240));
    wclex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wclex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wclex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wclex))
       return 2;

    hwnd = CreateWindowEx(0,
                          winClass,
                          _T("Assignment 4"),
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          800, 600,
                          HWND_DESKTOP,
                          (HMENU)NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

wchar_t tempText[200];
void DrawGrid(HWND hwnd);
void drawGraph(HWND hwnd, DATA data);
static DATA activeData;

LRESULT CALLBACK windowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static int index=0, i=0, toggle=0, k=0, xPos, yPos, mb_res, list_size =0, save_state =0;
    static NodePtr head, currentPtr;
    wchar_t tempText[200], tempVar[200];

    switch(msg)
    {
    case WM_CREATE:
        head = NULL;
        Menu(hwnd);
        activeData.state =10;
        break;

    case WM_COMMAND:
        if(HIWORD(wParam)==BN_CLICKED)
        {
            switch(LOWORD(wParam))
            {
                //Menu buttons processing
            case ID_FILE_QUIT:
                if(activeData.state != 10)
                    mb_res = MessageBoxW(hwnd, L"There may be some unsaved changes,\nIf you quit you will lose any unsaved changes.\nare you sure you want to quit?", L"Quit!", MB_YESNO |MB_ICONQUESTION);
                if(mb_res == IDYES)PostQuitMessage(0);
                else if(mb_res == IDNO)goto Next;
                PostQuitMessage(0);

                Next:
                    mb_res = 1000;
                break;
            case ID_FILE_OPEN:
                layout(hwnd);
                readFile(&head);
                activeData = *head;
                activeData.state = 1;
                SetWindowText(control[0], activeData.heading);
                SetWindowText(control[2], activeData.date);
                swprintf(tempVar, L"%.5f", activeData.data[i]);
                SetWindowTextW(control[7], tempVar);
                swprintf(tempVar, L"Angle: %d",i);
                SetWindowTextW(control[6], tempVar);
                drawGraph(hwnd, activeData, i);
                currentPtr = head;
                list_size = listSize(&head);
                for(int i=0; currentPtr->next != NULL; ++i)
                {
                    currentPtr = currentPtr->next;
                }

                if(currentPtr->next ==NULL)
                {
                    currentPtr = currentPtr->prev;
                    free(currentPtr->next);
                    currentPtr ->next = NULL;
                }
                currentPtr = head;
                break;
            case ID_FILE_SAVE:
                save_state = writeFile(&head);
                if(save_state){
                MessageBoxW(hwnd, L"Saved successfully!\n", L"Save", MB_OK|MB_ICONINFORMATION);
                activeData.state = 10;
                }
                break;

            case ID_HELP_ABOUT:
                MessageBoxW(hwnd, L"Musana Sthembiso\n 219957564\nwww.musana.co.za",  L"Author", MB_OK | MB_ICONINFORMATION);
                break;
            case ID_HELP_APP:
                MessageBoxW(hwnd, L"Version 1.0 \nWeather data Graphing calculator", L"About the App", MB_OK | MB_ICONINFORMATION);
                break;

                //generic buttons processing
            case ID_BUT_NEXT:
                if(currentPtr->next != NULL)
                    currentPtr = currentPtr->next;
                pullData(i);
                if(index<1)index++;
                else index=0;
                i=0;
                activeData = *(currentPtr);
                activeData.state = index;
                SetWindowText(control[0], activeData.heading);
                SetWindowText(control[2], activeData.date);
                swprintf(tempVar, L"%.5f", activeData.data[i]);
                SetWindowTextW(control[7], tempVar);
                swprintf(tempVar, L"Angle: %d",i);
                SetWindowTextW(control[6], tempVar);
                drawGraph(hwnd, activeData, i);
                SetFocus(hwnd);
                break;
            case ID_BUT_PREV:
                if(currentPtr->prev != NULL)
                    currentPtr = currentPtr->prev; // go back from the list...
                pullData(i);
                if(index>0)index--;
                else index=1;
                activeData = *(currentPtr);
                currentPtr->data[i] = activeData.data[i];
                activeData.state = index;
                SetWindowText(control[0], activeData.heading);
                SetWindowText(control[2], activeData.date);
                swprintf(tempVar, L"%.5f", activeData.data[i]);
                SetWindowTextW(control[7], tempVar);
                swprintf(tempVar, L"Angle: %d",i);
                SetWindowTextW(control[6], tempVar);
                drawGraph(hwnd, activeData, i);
                SetFocus(hwnd);
                break;

            case ID_BUT_RARR:
                pullData(i);
                *(currentPtr)=activeData;
                if(i<activeData.numDP)i++;
                if(i==activeData.numDP)i=0;
                toggle = 0;
                currentPtr->data[i] = activeData.data[i];
                swprintf(tempVar, L"%.5f", activeData.data[i]);
                SetWindowTextW(control[7], tempVar);
                swprintf(tempVar, L"Angle: %d",i);
                SetWindowTextW(control[6], tempVar);
                drawGraph(hwnd, activeData, i);
                SetFocus(hwnd);
                break;

            case ID_BUT_LARR:
                pullData(i);
                *(currentPtr)=activeData;
                if(i>=0)i--;
                if(i==-1)toggle = 1;
                if(toggle)
                {
                    ++k;
                    i = activeData.numDP - k;
                }
                swprintf(tempVar, L"%.5f", activeData.data[i]);
                SetWindowTextW(control[7], tempVar);
                swprintf(tempVar, L"Angle: %d",i);
                SetWindowTextW(control[6], tempVar);
                drawGraph(hwnd, activeData, i);
                SetFocus(hwnd);
                break;
            }
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_KEYDOWN:
        switch(wParam)
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            break;
        case VK_LEFT:
            pullData(i);
            if(i>=0)i--;
            if(i==-1)toggle = 1;
            if(toggle)
            {
                ++k;
                i = activeData.numDP - k;
            }
                //currentPtr->data[i] = activeData.data[i];
            swprintf(tempVar, L"%.5f", activeData.data[i]);
            SetWindowTextW(control[7], tempVar);
            swprintf(tempVar, L"Angle: %d",i);
            SetWindowTextW(control[6], tempVar);
            drawGraph(hwnd, activeData, i);
            break;
        case VK_RIGHT:
            pullData(i);
            if(i<activeData.numDP)i++;
            if(i==activeData.numDP)i=0;
            toggle = 0;
            currentPtr->data[i] = activeData.data[i];
            swprintf(tempVar, L"%.5f", activeData.data[i]);
            SetWindowTextW(control[7], tempVar);
            swprintf(tempVar, L"Angle: %d",i);
            SetWindowTextW(control[6], tempVar);
            drawGraph(hwnd, activeData, i);
            break;
        }
        break;
    case WM_LBUTTONDOWN:

        xPos = GET_X_LPARAM(lParam);
        yPos = GET_Y_LPARAM(lParam);

        i = 360.0*atan2((275-yPos)*1.0,(xPos-275)*1.0)/(-2*M_PI);

        if(i>=-179 &&i<=-1)i+=360;
        swprintf(tempVar, L"Angle: %d",i);
        SetWindowTextW(control[6], tempVar);

        swprintf(tempVar, L"%.5f", activeData.data[i]);
        SetWindowTextW(control[7], tempVar);

        drawGraph(hwnd, activeData, i);

        break;

    case WM_MOUSEMOVE:
        xPos = GET_X_LPARAM(lParam);
        yPos = GET_Y_LPARAM(lParam);

        if(wParam & MK_LBUTTON && xPos>=50 && yPos>=50 && xPos<=500 && yPos<=500)
        {
            i = 360.0*atan2((275-yPos)*1.0,(xPos-275)*1.0)/(-2*M_PI);

            if(i>=-179 &&i<=-1)i+=360;
            swprintf(tempVar, L"Angle: %d",i);
            SetWindowTextW(control[6], tempVar);

            swprintf(tempVar, L"%.5f", activeData.data[i]);
            SetWindowTextW(control[7], tempVar);

            drawGraph(hwnd, activeData, i);
        }
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}


void readFile(NodePtr *headPtr)
{
    ifstream myFile;
    char buf [50];
    int k;
    DATA data;

    myFile.open("Data.txt", ios::in);

    for(int i = 0;!myFile.eof(); i++)
    {
        myFile>>data.heading>>data.date;
        myFile>>data.numDP;
        if(data.heading == NULL)return;
        for(int j=0; j<data.numDP; j++)
        {
            myFile>>data.data[j];
        }
        if(myFile.eof())
        {
            data.data[data.numDP] = 105;
            data.state = NULL;
        }
        insertElement(headPtr, data);
    }

    myFile.close();
}

int writeFile(NodePtr *data)
{
    ofstream myFile;

    NodePtr currentPtr = *data;

    if(isEmpty(data))
    {
        MessageBoxW(NULL, L"Sorry List is Empty no data to save..", L"Error Save file", MB_OK | MB_ICONEXCLAMATION);
        return 0;
    }

    myFile.open("Data.txt", ios::out);

    for(int i = 0; currentPtr!=NULL; i++)
    {
        myFile<<currentPtr->heading<<"\n";
        myFile<<currentPtr->date<<"\n";
        myFile<<currentPtr->numDP<<"\n";

        for(int j=0; j<currentPtr->numDP; j++)
        {
            myFile<<currentPtr->data[j]<<"\n";
        }
        if(currentPtr->next == NULL)currentPtr->data[currentPtr->numDP]=EOF;
        currentPtr = currentPtr->next;
    }

    myFile.close();
    return 1;
}

void pullData(int i)
{
    static int len;

    len = GetWindowTextLengthW(control[7])+1;
    GetWindowTextW(control[7], tempText, len);
    activeData.data[i] = _wtof(tempText);
}

void DrawGrid(HWND hwnd)
{
    static int xPos, yPos;
    HBRUSH circles = CreateSolidBrush(RGB(0,0,0));
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(80,80,0));
    HDC hdc = GetDC(hwnd);

    SelectObject(hdc, circles);
    SelectObject(hdc, hPen);
    for(int i=0; i<5; i++)
    {
        Ellipse(hdc, 50+i*50, 50+i*50, 500-i*50, 500-i*50);
    }
    for(int j=0; j<12; j++)
    {
        xPos = 275 - 225*cos(2*M_PI*j/12.0);
        yPos = 275 - 225*sin(2*M_PI*j/12.0);
        MoveToEx(hdc,275,275, NULL);
        LineTo(hdc, xPos, yPos);
    }

    ReleaseDC(hwnd, hdc);
}

void drawGraph(HWND hwnd, DATA data, int t)
{
    clearScreen(hwnd);
    DrawGrid(hwnd);
    redPointer(hwnd, t);
    static int xPos_c[361], yPos_c[361];
    HDC hdc = GetDC(hwnd);
    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(10, 0, 255));

    SelectObject(hdc, hPen);

    for(int j=0; j<361; j++)
    {
        xPos_c[j] = 275 + 225*(data.data[j]/100.0)*cos(2*M_PI*j/361.0);
        yPos_c[j] = 275 + 225*(data.data[j]/100.0)*sin(2*M_PI*j/361.0);
    }
    for(int k=0; k<361; k++)
    {
        if(k==360)
        {
            k=0;
            break;
        }
        MoveToEx(hdc, xPos_c[k], yPos_c[k],NULL);
        LineTo(hdc, xPos_c[k+1], yPos_c[k+1]);

    }
    ReleaseDC(hwnd, hdc);
}

void clearScreen(HWND hwnd)
{
    HDC hdc = GetDC(hwnd);
    RECT graphClean;
    SetRect(&graphClean, 40, 40, 510, 510);
    FillRect(hdc, &graphClean, CreateSolidBrush(RGB(240,240,240)));
}

void redPointer(HWND hwnd, int index)
{
    static int xPos, yPos;
    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    HDC hdc = GetDC(hwnd);

    SelectObject(hdc, hPen);

    xPos = 275 + 225*cos(2*M_PI*index/360.0);
    yPos = 275 + 225*sin(2*M_PI*index/360.0);

    MoveToEx(hdc, 275, 275, NULL);
    LineTo(hdc, xPos, yPos);

    ReleaseDC(hwnd, hdc);
}
