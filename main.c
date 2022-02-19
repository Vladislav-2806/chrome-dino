#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

typedef struct COORDS{
    int left, right, up, down;
} CRDS;

RECT rct;
int coor[10][3][4];

int playerSize=60, nextCactus=1, speed=6, pastCactus=0;
int p=0;
float playerUp = 0, gravitation = 7;

void ClearDisplay(HDC dc){
    // Clear Display
    SelectObject(dc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(dc, RGB(255, 213, 115));

    Rectangle(dc, 0, 0, 1000, 600);
}

void MenagerOfCactus(HDC dc){
    for(int i=0; i<9; ++i){
        SelectObject(dc, GetStockObject(DC_BRUSH));
        SetDCBrushColor(dc, RGB(0, 255, 0));
        Rectangle(dc, coor[i][0][0], coor[i][0][1], coor[i][0][2], coor[i][0][3]);

        SelectObject(dc, GetStockObject(DC_BRUSH));
        SetDCBrushColor(dc, RGB(0, 255, 0));
        Rectangle(dc, coor[i][1][0], coor[i][1][1], coor[i][1][2], coor[i][1][3]);

        SelectObject(dc, GetStockObject(DC_BRUSH));
        SetDCBrushColor(dc, RGB(0, 255, 0));
        Rectangle(dc, coor[i][2][0], coor[i][2][1], coor[i][2][2], coor[i][2][3]);
        if(coor[i][0][0]>-200){
            coor[i][0][0] -= speed;
            coor[i][0][2] -= speed;

            coor[i][1][0] -= speed;
            coor[i][1][2] -= speed;

            coor[i][2][0] -= speed;
            coor[i][2][2] -= speed;
        }
    }
    if(coor[pastCactus][0][0]<200 && (rand()+time(NULL))%50==20){
        CreateCactus(((rand()+time(NULL)) % 4), coor[nextCactus]);
    }
}

void CreateCactus(int lengh, int crds[lengh][3][4]){
    for(int i=0; i<lengh; ++i){
        crds[i][0][0] = 1050+100*i;
        crds[i][0][1] = 400;
        crds[i][0][2] = 1110+100*i;
        crds[i][0][3] = 562;

        crds[i][1][0] = 1019+100*i;
        crds[i][1][1] = 420;
        crds[i][1][2] = 1051+100*i;
        crds[i][1][3] = 450;

        crds[i][2][0] = 1109+100*i;
        crds[i][2][1] = 500;
        crds[i][2][2] = 1140+100*i;
        crds[i][2][3] = 530;
    }
    pastCactus = nextCactus;
    if(nextCactus+lengh < 8){
        nextCactus += lengh;
    }
    else{
        nextCactus = (nextCactus+lengh)%8;
    }
}

void JumpPlayer(){
    if(gravitation>-7) gravitation -= 0.1;
    if(playerUp>=0){
        playerUp += gravitation;
    }
    else playerUp = 0, gravitation = 7, p=0;
}

int Collision(){
    int ret=1;
    for(int i=0; i<9; ++i){
        if(210>coor[i][0][0] && 150<coor[i][0][2] && 562-playerUp>400){
            ret=0;
        }
    }

    return ret;
}

void WinShow(HDC dc){
    HDC memDC = CreateCompatibleDC(dc);
    HBITMAP memBM = CreateCompatibleBitmap(dc, rct.right - rct.left, rct.bottom - rct.top);
    SelectObject(memDC, memBM);

    ClearDisplay(memDC);

    if(GetKeyState(VK_SPACE) < 0){
        p=1;
    }

    // Create Player
    SelectObject(memDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(memDC, RGB(150, 150, 150));
    Rectangle(memDC, 150, 502-playerUp, 150+playerSize, 502+playerSize-playerUp);

    // Create Kaktues
    MenagerOfCactus(memDC);

    BitBlt(dc, 0,0, rct.right - rct.left, rct.bottom - rct.top, memDC, 0,0, SRCCOPY);
    DeleteDC(memDC);
    DeleteObject(memBM);
}

void Timer(){
    static int second=0;

    second+=1;
    printf("You score: %d\n", second/10);
}

LRESULT WndProc(HWND hwnd, UINT message, WPARAM wp, LPARAM lp){
    if(message==WM_DESTROY){
        PostQuitMessage(0);
    }
    else if(message==WM_SIZE){
        GetClientRect(hwnd, &rct); // Don't miganie!
	}
    else return DefWindowProcA(hwnd, message, wp, lp);
}

int main(){
    WNDCLASSA wcl;
    memset(&wcl, 0, sizeof(WNDCLASSA));

    wcl.lpszClassName = "my Window";
    wcl.lpfnWndProc = WndProc;

    RegisterClassA(&wcl);

    HWND hwnd;
    hwnd = CreateWindow("my Window", "Dino 2D", WS_OVERLAPPEDWINDOW,
                        300, 10, 1000, 600, NULL, NULL, NULL, NULL);

    ShowWindow(hwnd, SW_SHOWNORMAL);

    HDC dc = GetDC(hwnd);
    MSG msg;

    while(Collision()){
        if(PeekMessageA(&msg, NULL, 0,0, PM_REMOVE)){
            DispatchMessage(&msg);
            if(msg.message==WM_QUIT) break;
        }
        else{
            if(p) JumpPlayer();
            Timer();
            WinShow(dc);
            Sleep(5);
        }
    }
    system("@cls||clear");
    Timer();
    printf("\nGame is over!\n\n");

    return 0;
}
