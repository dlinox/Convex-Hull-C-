#include <vector>
#include <iostream>
#include <algorithm>
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <stdio.h>
#include <iomanip>
#include <cstdlib>
#include <stdlib.h>
#include "resource.h"
#define M_PI 3.1416
using namespace std;
HINSTANCE hInst;
HDC hdc;
PAINTSTRUCT ps;
HPEN pen, oldPen;
long xPos, yPos;

class Punto
{   public:
    int x,y;
    Punto(int xx=0,int yy=0){
        x=xx;
        y=yy;
    }
};
vector<Punto> A;
vector<Punto> hull;

void Insertar(Punto P){
    A.push_back(P);
    cout<<(A[A.size()-1]).x<<","<<(A[A.size()-1]).y<<endl;
}

int orientacion(Punto p, Punto q, Punto r){
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

    if (val == 0){
        return 0;
    }
    return (val > 0)? 1: 2;
}
void convexHull(vector<Punto> P, int n){

    if (n < 3){

        return;
    }

    int l = 0;
    for (int i = 1; i < n; i++){
        if (P[i].x < P[l].x)
            l = i;
    }

    int p = l, q;

    do{
        hull.push_back(P[p]);

        q = (p+1)%n;
        for (int i = 0; i < n; i++){

           if (orientacion(P[p], P[i], P[q]) == 2){
               q = i;
            }
        }
        p = q;

    }
    while (p != l);

    for (int i = 0; i < int (hull.size()); i++){

        cout << "(" << hull[i].x << ", "
              << hull[i].y << ")\n";
    }

}
int aux = int(hull.size());

void cHull(){

    int n = int(A.size());
    cout<<"N = "<<n<<endl;
    convexHull(A, n);
}

void imprimir(){

    for (int i = 0; i < int(A.size()); i++){
        cout<<" --> "<<(A[i]).x<<","<<(A[i]).y<<endl;
    }
}

int cont = 0;

void dibujo(){
    pen = CreatePen(PS_SOLID, 6, RGB(255,0,0));
    oldPen = (HPEN)SelectObject(hdc, pen);

    for (int i = 0; i <int(A.size()); i++){

        Ellipse(hdc,(A[i]).x-2.5,(A[i]).y-2.5,(A[i]).x+2.5,(A[i]).y+2.5);
        //Rectangle(hdc,(A[i]).x-2.5,(A[i]).y-2.5,(A[i]).x+2.5,(A[i]).y+2.5);
    }
    SelectObject(hdc, oldPen);
    DeleteObject(pen);

    pen = CreatePen(PS_SOLID, 2, RGB(0,0,0));
    oldPen = (HPEN)SelectObject(hdc, pen);

    for (int i = 0; i < int(hull.size()); i++){

        MoveToEx(hdc,(hull[i].x),(hull[i]).y,NULL);
        if(cont >2){
            LineTo(hdc,(hull[i-1]).x,(hull[i-1]).y);
        }

        MoveToEx(hdc,hull[0].x,hull[0].y,NULL);
        LineTo(hdc,hull[hull.size()-1].x,hull[hull.size()-1].y);
    }
    SelectObject(hdc, oldPen);
    DeleteObject(pen);
}

BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_LBUTTONDOWN:
        {
            long xPos = GET_X_LPARAM(lParam);
            long yPos = GET_Y_LPARAM(lParam);
            Insertar(Punto(xPos,yPos));
            if(int(hull.size())  != aux){
                hull.clear();
                cHull();
            }
            cont ++;
            InvalidateRect(hwndDlg,NULL,true);

        }
        return TRUE;

        case WM_LBUTTONDBLCLK:
        {
            cHull();
            InvalidateRect(hwndDlg,NULL,true);

        }
        return TRUE;

        case WM_PAINT:
        {
            hdc=BeginPaint(hwndDlg,&ps);
            dibujo();
            EndPaint(hwndDlg,&ps);
        }
        return TRUE;
        case WM_INITDIALOG:
        {
        }
        return TRUE;

        case WM_CLOSE:
        {
            EndDialog(hwndDlg, 0);
        }
        return TRUE;

        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                case DLGC_BUTTON:{
                    hull.clear();
                    A.clear();
                    system("cls");
                }
                return TRUE;
            }
        }
        return TRUE;
    }
    return FALSE;
}
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    hInst=hInstance;
    InitCommonControls();
    return DialogBox(hInst, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);
}
