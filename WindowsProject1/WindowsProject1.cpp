// WindowsProject1.cpp : Defines the entry point for the application.
//
#include <math.h>
#include <iostream>
#include<string>
#include <memory.h>
using namespace std;

#include "framework.h"
#include "WindowsProject1.h"

#define BLACK	RGB(0, 0, 0) 
#define BLUE	RGB(0, 0, 127) 
#define GREEN	RGB(0, 127, 0) 
#define CYAN	RGB(0, 127, 0) 
#define RED	RGB(255, 0, 0) 
#define MAGENTA	RGB(127, 0, 0) 
#define BROWN	RGB(100, 65, 65) 
#define LIGHTGRAY	RGB(220, 220, 220) 
#define DARKGRAY	RGB(127,127, 127) 
#define LIGHTBLUE	RGB(0, 0, 127) 
#define LIGHTGREEN	RGB(0,255, 0) 
#define LIGHTCYAN	RGB(214, 242,206) 
#define LIGHTRED	RGB(255, 50, 235) 
#define LIGHTMAGENTA	RGB(127, 0, 255) 
#define YELLOW	RGB(255, 255, 0) 
#define WHITE	RGB(255, 255, 255) 

#define SOLID_FILL 1
#define SLASH_FILL 3
enum { power, exhaust, exhaust1, exhaust2, intake, intake1, intake2, compress };
char strokeNames[8][23] = { "power", "exhaust", "exhaust1", "exhaust2", "intake", "intake", "intake",  "compress" };
COLORREF colorStroke[8] = { YELLOW, LIGHTGRAY, LIGHTGRAY, LIGHTGRAY, LIGHTCYAN, LIGHTCYAN, LIGHTCYAN,WHITE };

#define PI 3.141592653589793238462

#define ROTOR_A 6
#define ROTOR_B 40
double r_angle = PI / 4; /* rotor angle */
double rh_angle = PI/4; /* rotor-coolant center angle */
double h_angle = PI / 4;
double offsetr = 3;
double sz = 7;
#define offsetx1 (int)((double)offsetr*cos(rh_angle)*sz+0.5)
#define offsety1 (int)((double)offsetr*sin(rh_angle)*sz+0.5)

int  cx = 700;
int cy = 350;
//#define sz  6.0



double   quircir_size = 45;
double quircir_corner = 2.13; 
#define MAPX(x, y) (int)((double)cx+sz*(x*cos(h_angle)-y*sin(h_angle))+4.5)
#define MAPY(x, y) (int)((double)cy-sz*(y*cos(h_angle)+x*sin(h_angle))+0.5)
#define MAPX_rotor(x, y) (int)((double)cx+sz*(x*cos(r_angle)-y*sin(r_angle))-offsetx1+0.5)
#define MAPY_rotor(x, y) (int)((double)cy-sz*(y*cos(r_angle)+x*sin(r_angle))+offsety1+0.5)

#define IMAX 360



POINT coolantPts[IMAX]; 
//POINT coolantPtsCase[IMAX];
int nPts = 0;
POINT pts[IMAX];
POINT xpts[IMAX]; 
POINT keepPts[IMAX * 2];
int ncoolantPts = 0;
POINT rotorPts[IMAX];

//POINT pts[ IMAX];
int nRotorPts = 0;
int iStroke = 7;
int iStroke2 = 5;
int iChamber = 0;
int iChamber2 = 2;

int nSealsPts;                                                           
int xpos = 12, ypos = 12;
// int intersect[IMAX*2];
FILE* hFile = NULL;
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Option(HWND, UINT, WPARAM, LPARAM);
void line(HDC hdc, int x, int y, int x2, int y2);

void setcolor(HDC hdc,COLORREF color)
{
    
    HGDIOBJ original = NULL;

       
    
    //    Saving the original object
  original = SelectObject(hdc, GetStockObject(DC_PEN));

   
    SetDCPenColor(hdc, color); 
   //  SelectObject(hdc, original);

}

void setfillstyle( HDC hdc,int style, COLORREF color)
{
    HGDIOBJ original = NULL;


     //    Saving the original object
    original = SelectObject(hdc, GetStockObject(DC_BRUSH));
      
    //    Set the DC Brush to color
    SetDCBrushColor(hdc, color);

}

void   drawRect(HDC hdc, RECT rc, COLORREF color)
{

    setfillstyle(hdc, SOLID_FILL, color);
    POINT pts[4];
    pts[0].x = rc.left;
    pts[0].y = rc.top;
    pts[1].x = rc.right;
    pts[1].y = rc.top;
    pts[2].x = rc.right;
    pts[2].y = rc.bottom;
    pts[3].x = rc.left;
    pts[3].y = rc.bottom;
    Polygon(hdc, pts, 4);

}

void drawpoly(HDC hdc, int n, LPPOINT pts)
{
 
    Polyline(hdc, pts, n);
}

void fillpoly(HDC hdc, int n, LPPOINT pts)
{
      Polygon(hdc, pts, n);
}

void circle(HDC hdc, int x, int y, int r)
{
    Ellipse(hdc, x - r, y - r, x + r, y + r); 
}

void pieslice(HDC hdc, int x, int y, int a0, int a1, int r)
{
    circle(hdc, x, y, r);
}




void outtextxy(HDC hdc, int x, int y, char* s)
{
    
        RECT  rc;
    //    int dx;
        rc.left = 0;
        rc.bottom = 0;
  
        rc.right = 16 * strlen(s);
        rc.top = 12;
         ExtTextOutA(
                    hdc,
                    x,
                    y,
             0,
            &rc,
            s ,
            strlen(s),
            NULL
        );
   //     DrawText(hdc, (LPCTSTR)s, strlen(s), &rc, DT_BOTTOM);
   
}

void puts(HDC hdc, int x, int y, const char* s)
{
    outtextxy( hdc,  x,  y, ( char*)s);

}

void puti(HDC hdc, int x, int y, char* s, int i)
{

    wchar_t    buffer[400] = L"                     ";


    swprintf(buffer, 10, L"%d ", i);

    outtextxy(hdc, x, y - 16, s);
    RECT  rc;
  //  int dx;
    rc.left = x + strlen(s) * 8;
    rc.bottom = y;
    rc.right = x + 200;
    rc.top = y - 16;

    DrawText(hdc, (LPCTSTR)buffer, 200, (LPRECT)&rc, DT_BOTTOM);

}

void putf(HDC hdc, int x, int y, char* s, double i)
{

    wchar_t    buffer[100] = L"                     ";


    swprintf(buffer, 10, L"%6.2f ", i);

    outtextxy(hdc, x, y - 16, s);
    RECT  rc;
  //  int dx;
    rc.left = x + strlen(s) * 8;
    rc.bottom = y;
    rc.right = x + sizeof(buffer);
    rc.top = y - 16;

    DrawText(hdc, (LPCTSTR)buffer, sizeof(buffer), (LPRECT)&rc, DT_BOTTOM);

}


 



int kbhit()
{
    return 1;
}


void drawpoly(int n, int* pts)
{
}

void putpixel(HDC hdc, int x, int y, COLORREF color)
{
    SetPixel(hdc, x, y, color);
}

/* 
void line(HDC hdc, int x, int y, int x2, int y2)
{
     Pen pen(Color(255, 0, 0, 255), 8);
     stat = pen.SetStartCap(LineCapArrowAnchor);
    stat = pen.SetEndCap(LineCapRoundAnchor);
    stat = graphics.DrawLine(&pen, 20, 175, 300, 175);
}

*/

#include "Arrow.h"
#include <math.h>

// ArrowTo()
//
void ArrowTo(HDC hDC, int x, int y, ARROWSTRUCT* pA) {

    POINT ptTo = { x, y };

    ArrowTo(hDC, &ptTo, pA);
}

// ArrowTo()
//
void ArrowTo(HDC hDC, const POINT* lpTo, ARROWSTRUCT* pA) {

    POINT pFrom;
    POINT pBase;
    POINT aptPoly[3];
    float vecLine[2];
    float vecLeft[2];
    float fLength;
    float th;
    float ta;

    // get from point
    MoveToEx(hDC, 0, 0, &pFrom);

    // set to point
    aptPoly[0].x = lpTo->x;
    aptPoly[0].y = lpTo->y;

    // build the line vector
    vecLine[0] = (float)aptPoly[0].x - pFrom.x;
    vecLine[1] = (float)aptPoly[0].y - pFrom.y;

    // build the arrow base vector - normal to the line
    vecLeft[0] = -vecLine[1];
    vecLeft[1] = vecLine[0];

    // setup length parameters
    fLength = (float)sqrt(vecLine[0] * vecLine[0] + vecLine[1] * vecLine[1]);
    th = pA->nWidth / (2.0f * fLength);
    ta = pA->nWidth / (2.0f * (tanf(pA->fTheta) / 2.0f) * fLength);

    // find the base of the arrow
    pBase.x = (int)(aptPoly[0].x + -ta * vecLine[0]);
    pBase.y = (int)(aptPoly[0].y + -ta * vecLine[1]);

    // build the points on the sides of the arrow
    aptPoly[1].x = (int)(pBase.x + th * vecLeft[0]);
    aptPoly[1].y = (int)(pBase.y + th * vecLeft[1]);
    aptPoly[2].x = (int)(pBase.x + -th * vecLeft[0]);
    aptPoly[2].y = (int)(pBase.y + -th * vecLeft[1]);

    MoveToEx(hDC, pFrom.x, pFrom.y, NULL);

    // draw we're fillin'...
    if (pA->bFill) {
        LineTo(hDC, aptPoly[0].x, aptPoly[0].y);
        Polygon(hDC, aptPoly, 3);
    }

    // ... or even jes chillin'...
    else {
        LineTo(hDC, pBase.x, pBase.y);
        LineTo(hDC, aptPoly[1].x, aptPoly[1].y);
        LineTo(hDC, aptPoly[0].x, aptPoly[0].y);
        LineTo(hDC, aptPoly[2].x, aptPoly[2].y);
        LineTo(hDC, pBase.x, pBase.y);
        MoveToEx(hDC, aptPoly[0].x, aptPoly[0].y, NULL);
    }
}



void Arrowline(HDC hdc, POINT p1, POINT p2)
{
    POINT pts[2] = { p1, p2 };
  

    ARROWSTRUCT arrow = { 7,10, 1 };
    LPPOINT lppt = NULL;

    MoveToEx(hdc, p1.x, p1.y, lppt);
    ArrowTo(hdc, &p2, &arrow);
}

void line(HDC hdc, int x, int y, int x2, int y2)
{
    LPPOINT lppt = NULL;


    MoveToEx(hdc, x, y, lppt);
    LineTo(hdc, x2, y2);
}

void line(HDC hdc, POINT p1, POINT p2)
{
    POINT pts[2] = { p1, p2 };
    Polyline(hdc, pts, 2);
}


void fill_chamber(HDC hdc, int i1, int i2, COLORREF color)
{
    int i, k = 0, tmp; 
    static POINT pts[500];
    int shift = (int)((rh_angle - PI / 4)*IMAX / 2/PI/4);
  //  if (i1 < i2)
    {
            for (i = i1; i < i2; i++)
            {
                tmp = (i + shift) % IMAX;
                pts[k] = rotorPts[tmp];
              //  pts[k].y = rotorPts[2 * tmp + 1];
                k++;
            }

            for (i = i2-1; i >= i1; i--)
            {
                pts[k] = coolantPts[ i];
              //  pts[k].y = coolantPts[2 * i + 1];
                k++;
            }
    }

    setfillstyle(hdc, SOLID_FILL, color);

    Polygon(hdc, pts, k);

}




double get_triangle_area(double x1, double y1, double x2, double y2, double x3, double y3)
{
    return (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));
}

void getsign(double theda, double* xsign, double* ysign)
{
    double xs = 1, ys = 1;

   // theda -= (int)(theda / (PI * 2)) * PI * 2;
    if (theda >= 0 && theda < PI * 0.5) { xs = 1;  ys = 1; }
    else if (theda >= PI * 0.5 && theda < PI) { xs = -1;  ys = 1; }
    else if (theda >= PI && theda <= PI * 1.5) { xs = -1; ys = -1; }
    else if (theda >= PI * 1.5 && theda < PI * 2) { xs = 1; ys = -1; }
    *xsign = xs;
    *ysign = ys;
}

void getxy_squircir(double* x, double* y, double theda, double a, double k)
{
    double xsign = 1, ysign = 1;

    double x0, y0;
    double b = PI * 0.25;
    b = 0;
    getsign(theda + b, &xsign, &ysign);
    x0 = xsign * pow(fabs(cos(theda + b)), 1.0 / k);
    y0 = ysign * pow(fabs(sin(theda + b)), 1.0 / k);

    getsign(theda, &xsign, &ysign);
    *x = xsign * a * fabs(x0 * cos(b) + y0 * sin(b));
    *y = ysign * a * fabs(y0 * cos(b) - x0 * sin(b));

}

void getxy(double* x, double* y, double theda, double a, double b, double k)
{
    double ptheda, dptheda;
  

    ptheda = a * cos(k * theda / 2) * cos(k * theda / 2) + b;
    dptheda = -a * k / 2 * sin(k * theda / 2) * cos(k * theda / 2);
    *x = ptheda * cos(theda) - dptheda * sin(theda);
    *y = ptheda * sin(theda) + dptheda * cos(theda);
}

double get_quircir_size()
{
  
        return (double)ROTOR_A + ROTOR_B - sqrt(offsetx1 * offsetx1 + offsety1 * offsety1) / sz;

}




#define  IMAXspark 64 
void sparkplug(HDC hdc, COLORREF color, double x0, double y0, int istroke)
{


    double x, y;
    int i;
    int nsparkplugPts = 0;

    POINT sparkplugPts[IMAXspark];




    for (i = 0; i < IMAXspark; i++)
    {

        double theda = (double)2 * PI * (i) / IMAXspark;

        getxy(&x, &y, theda, 5, 5, 49);

        x /= 30;
        y /= 30;

        sparkplugPts[i].x = MAPX(x + x0, y + y0);
        sparkplugPts[i].y = MAPY(x + x0, y + y0);
        nsparkplugPts++;
    }

    setcolor(hdc, color);
    setfillstyle(hdc, 1, WHITE);
   // fillpoly(hdc, IMAXspark, sparkplugPts);
  //  setcolor(hdc, LIGHTRED);
    drawpoly(hdc, IMAXspark, sparkplugPts);
}


void coolant(HDC hdc)
{
    int i;
    double x, y;
    double theda;
    COLORREF color = DARKGRAY;

    ncoolantPts = 0;
    for (i = 0; i < IMAX; i++)
    {

        theda = 2.0 * PI * i / IMAX;
        getxy_squircir(&x, &y, theda, quircir_size, quircir_corner);
        coolantPts[i].x = MAPX(x, y);
        coolantPts[i].y = MAPY(x, y);        
    }

     
    for (i = 0; i < IMAX; i++)
    {

        theda = 2.0 * PI * i / IMAX;
        getxy_squircir(&x, &y, theda, quircir_size+.5*sz, quircir_corner);
        pts[i].x = MAPX(x, y);
        pts[i].y = MAPY(x, y);
    }
  

     
    setcolor(hdc,color);
    setfillstyle(hdc, SOLID_FILL, DARKGRAY);
    if (iChamber == 0)
    {

        int i = iStroke % 8;
        if (i == exhaust || i == exhaust1 || i == exhaust2)
            for (i = IMAX / 6; i < IMAX / 6 + 5; i++)
            {
                coolantPts[i].x = coolantPts[i].x + (int)(8.0*sz);
                coolantPts[i].y = coolantPts[i].y - (int)(8.0 * sz);
            }
        if (i == intake || i == intake1 || i == intake2)
            for (i = IMAX / 10 - 5; i < IMAX / 10; i++)
            {
                coolantPts[i].x = coolantPts[i].x + (int)(8.0 * sz);
                coolantPts[i].y = coolantPts[i].y - (int)(8.0 * sz);
            }
    }    
    
    if (iChamber == 2)
    {

        int i = iStroke2 % 8;
        if (i == exhaust || i == exhaust1 || i == exhaust2)
            for (i = IMAX / 2 + IMAX / 6; i < IMAX/2+IMAX / 6 + 5; i++)
            {
                coolantPts[i].x = coolantPts[i].x - (int)(8.0 * sz);
                coolantPts[i].y = coolantPts[i].y + (int)(8.0 * sz);
            }
        if (i == intake || i == intake1 || i == intake2)
            for (i = IMAX / 10 - 5; i < IMAX / 10; i++)
            {
                coolantPts[i].x = coolantPts[i].x - (int)(8.0 * sz);
                coolantPts[i].y = coolantPts[i].y + (int)(8.0 * sz);
            }
    }

    setfillstyle(hdc, SOLID_FILL, DARKGRAY);
      fillpoly(hdc, IMAX, pts);
     drawpoly(hdc, IMAX, pts);   
    setfillstyle(hdc, SOLID_FILL, BROWN);
   fillpoly(hdc, IMAX, coolantPts);
    setcolor(hdc, BLACK);
    drawpoly(hdc, IMAX, coolantPts);


 circle(hdc, MAPX(0, 0), MAPY(0, 0), 57);
}

 





int   draw_chamber(HDC hdc, int iChamber, int istroke0)
{
    int i, j, ii, jj, k = 0;
    int  x1, y1,  xh, yh;
    int slot = IMAX / 8;
        int istroke = istroke0, istart = 0, iend0 = 0, iend1 = 0,
        jstart = 0, jend0 = 0, jend1 = 0;

    COLORREF clr = YELLOW;
    

    istart = (istroke + 1) % 8 * slot;
    iend0 = istart + slot;
    iend1 = istart - slot;
   
    jstart = iChamber * 2 * slot+slot; 
     jend0 = jstart + slot, 
     jend1 = jstart - slot; 
 

     puts(hdc, xpos + 80 *iChamber, 560, (char*)strokeNames[istroke]);   
     puti(hdc, xpos + 80 * iChamber, 600, (char*)"Slot =", istroke);
     puti(hdc, xpos + 80 * iChamber, 620, (char*)"Chamber=", iChamber);

    clr = colorStroke[istroke];

   setcolor(hdc, clr);
   setfillstyle(hdc, SOLID_FILL, clr);
    
    i = iend1 ;
    j = jend1;
    nPts = 0;
      
    while (i < iend0)
    {
         
        ii = i;
        while (ii >= IMAX) ii = ii-IMAX;
        while (ii < 0) ii = ii+IMAX;
        jj = j;
        while (jj >= IMAX) jj = jj-IMAX;
        while (jj < 0) jj = jj-IMAX;
        x1 = rotorPts[ii].x;
        y1 = rotorPts[ii].y;

        xh = coolantPts[jj].x;
        yh = coolantPts[jj].y;
     //   line(hdc, p1, p2); 
       // if (i % 3 == 0)
       // {
            
        //    if (istroke == exhaust || istroke == exhaust1 || istroke == exhaust2) Arrowline(hdc, rotorPts[ii], coolantPts[jj]);
         //   else Arrowline(hdc, coolantPts[jj], rotorPts[ii]);
       // }
     //   circle(hdc, x1, y1, 3);
     //   circle(hdc, xh, yh, 3);
        i++;
        j++;
        pts[nPts] = rotorPts[ii];
        xpts[nPts] = coolantPts[jj];
        nPts++;
        
    }


    for (k = 0; k < nPts; k++)      keepPts[k] = pts[k];
    for (k = 0; k < nPts; k++)      keepPts[nPts + k] = xpts[nPts - 1 - k];
    setfillstyle(hdc, SOLID_FILL, clr); 
     Polygon(hdc, keepPts, 2*nPts);
   
     Polyline(hdc, keepPts, 2 * nPts);
    if (istroke == 7 && iChamber == 0) sparkplug(hdc, RED, 36, 36, istroke);
    if (istroke == 7 && iChamber == 2) sparkplug(hdc, RED, -36, -36, istroke);
     setcolor(hdc, BLACK);
     setfillstyle(hdc, SOLID_FILL, BLACK);
     for (i=0; i<nPts; i++)
    if (i % 5 == 0)
    {

        if (istroke == exhaust || istroke == exhaust1 || istroke == exhaust2) Arrowline(hdc, pts[i], xpts[i]);
        else Arrowline(hdc, xpts[i], pts[i]);
    }    
    return 0;
 
    // fillpoly(hdc, npts, xpts);
  //  return 4;
}

int  draw_chambers(HDC hdc)
{
    draw_chamber(hdc, 0, iStroke %8);
    draw_chamber(hdc, 2, (iStroke +4)%8); 

     iChamber = 0; iStroke2 = (iStroke + 4) % 8;
    return 1;
    
}



void fill_area(HDC hdc, double a, double b, int nCornercoolant, COLORREF color)
{
    int i;

    double area = 0;
    double x1, y1;
  
    double  theda;
    double dtheda = 0.1;
    int limit = 100;
    POINT pts[1000];
    int jj, jjmax;
    return;


    jjmax = IMAX / 7;


    a = a - (int)((a / PI / 2) * 2 * PI +0.5);
    b = b - (int)((b / PI / 2) * 2 * PI + 0.5);
    if (a > b)  dtheda = (2 * PI - a + b) / limit;
    else dtheda = (b - a) / limit;

    theda = a;


    setcolor(hdc,color);

    for (i = 0; i < limit; i++)
    {

        double tmp1 = theda + i * dtheda;
        tmp1 = tmp1 - (double)(tmp1 / PI / 2) * 2 * PI;
        getxy(&x1, &y1, tmp1, ROTOR_A, ROTOR_B, 3);

        pts[ i] .x= MAPX_rotor(x1, y1);
        pts[ i].y = MAPY_rotor(x1, y1);

    }

    i = 0;
    jj = jjmax / 2 + IMAX / 4 * nCornercoolant;
    while (i < jjmax)
    {

        pts[2 * (limit + i)] = coolantPts[2 * jj];
        pts[2 * (limit + i) + 1] = coolantPts[2 * jj + 1];
        i++;
        jj--; if (jj < 0) jj = jj + ncoolantPts;

    }
    for (i = 0; i < limit + jjmax; i++) putpixel(hdc, pts[i].x, pts[i ].y, GREEN);



    setfillstyle( hdc,1, color);
    fillpoly(hdc, limit + jjmax, pts);
    setcolor(hdc,DARKGRAY);
    drawpoly(hdc, limit + jjmax, pts);

}


void draw_seals(HDC hdc, COLORREF color, int istroke)
{
    int i, imax = 24;
    double x, y;
    double angles[60];
    int ia = 0; 
  //  enum { compress, power, exhaust, exhaust1, exhaust2, exhaust3, intake };
  //  COLORREF colorStroke[4] = { LIGHTRED, YELLOW, LIGHTCYAN, LIGHTGREEN };
    setcolor(hdc,color);

   
    for (i = 0; i < imax; i++)
    {
        
        double theda = 2 * PI * i / imax;

        getxy(&x, &y, theda, ROTOR_A, ROTOR_B, 3);
        angles[i] = theda;
       // if (i == 3 || i == 21 || i == 6 || i == 11 || i == 14 || i == 18)
        if (i%3==0 )
        {
            setcolor(hdc,GREEN);
            circle(hdc, MAPX_rotor(x, y), MAPY_rotor(x, y), (int)(1.5*sz));
            setfillstyle( hdc,1, DARKGRAY);
            pieslice(hdc, MAPX_rotor(x, y), MAPY_rotor(x, y), 0, 360, (int)(0.8*sz));
        }
    }

    

}


void draw_rotorgear(HDC hdc)
{
    
    setfillstyle( hdc,SOLID_FILL, BROWN);
    setcolor(hdc,DARKGRAY);
    // 	pieslice (MAPX_rotor(0,0), MAPY_rotor(0,0), 0, 360, 70);
    circle(hdc, MAPX_rotor(0, 0), MAPY_rotor(0, 0), (int)(70/4*sz));
    setcolor(hdc,BLUE);
    //	outtextxy(MAPX_rotor(16,0), MAPY_rotor(16,0),"rotor gear");
}

void draw_cranshaft(HDC hdc)
{
     
    setcolor(hdc,DARKGRAY);
    setfillstyle( hdc,SLASH_FILL, DARKGRAY);
    pieslice(hdc, MAPX(0, 0), MAPY(0, 0), 0, 360, (int)(56/4*sz));
    circle(hdc, MAPX(0, 0), MAPY(0, 0), (int)(56/4*sz));
    setfillstyle(hdc, SOLID_FILL, BROWN);
    outtextxy(hdc, MAPX(0, 0), MAPY(0, 0), (char*)"cranshaft");
}

void rotor(HDC hdc, COLORREF color, int istroke)
{


    double x, y;
    int rect[8] = { 23,23,234,23,234,234,23,234 };
    int i;


    nRotorPts = 0;
    setcolor(hdc,color);

    for (i = 0; i < IMAX; i++)
    {


  
        double theda = (double)2 * PI * (i) / IMAX;

        getxy(&x, &y, theda, ROTOR_A, ROTOR_B, 3);


       
        rotorPts[ i] .x= MAPX_rotor(x, y);
        rotorPts[ i ].y = MAPY_rotor(x, y);
        nRotorPts++;
  

    }

    setfillstyle( hdc, SOLID_FILL, RGB(182,205,214));
    // setfihdc, llstyle(SOLID_FILL, getmaxcolor());
    fillpoly(hdc, IMAX, rotorPts);

  //draw_chambers(hdc);
 //   drawlpoly(4, rect);
    setcolor(hdc,LIGHTGRAY);
    draw_rotorgear(hdc);
    draw_cranshaft(hdc);
     setcolor(hdc, BLACK);
 //   drawpoly(hdc,IMAX, rotorPts);   
   //  Polyline(hdc, rotorPts, IMAX);
    draw_seals(hdc, color, istroke);


     

       line(hdc, MAPX_rotor(40, 0), MAPY_rotor(40, 0), rotorPts[0].x, rotorPts[0].y);
     


        setcolor(hdc,5);

        // sprintf(s, "Compression Ratio: 13.0");
        outtextxy(hdc, xpos, ypos, (char*)"Compression Ratio: 13.0");  
  

    // fprintf(hFile, "%d, %d, %d, \n", MAPX_rotor(0, 0), MAPY_rotor(0, 0), 70);
    // fprintf(hFile, "%d, %d, %d, \n", -1, -1, color);


}



int bitmapx(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HBITMAP bmpSource = NULL;
    static HDC hdc = NULL;
   // hdc = GetDC(hWnd);
  //  PAINTSTRUCT ps;
   // HDC hdcDestination;
    
    return 0; 
    if (message == WM_CREATE)
    {
        bmpSource = (HBITMAP)LoadImage(NULL, L"C:\\Users\\kk\\source\\repos\\sparkplug.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        hdc = CreateCompatibleDC(GetDC(hWnd));
        SelectObject(hdc, bmpSource);
        return 0;
    }
    else if (message == WM_PAINT)
    {
      //  hdcDestination = BeginPaint(hWnd, &ps);
       
        BitBlt(hdc, 0, 0, 500, 500, hdc, 0, 0, SRCCOPY);
       // EndPaint(hWnd, &ps);
        return 0;
    }
    return 1;

  //  return DefWindowProc(hWnd, Msg, wParam, lParam);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

    MSG msg;
   
    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


#define TIMER_ROTOR 1001
#define TIMER_SPARK 1002


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int istroke = 0; 
  //  static double dr_angle = PI / 4;
    static double dr_angle = PI / 24;
 

    switch (message)
    {
    case WM_CREATE:
       // HDC hdc = GetDC(hWnd)
        bitmapx( hWnd,  message,  wParam,  lParam);
      quircir_size = get_quircir_size();
     r_angle = 0;
     h_angle = 0;
     rh_angle = 0;



     dr_angle = PI / 4;
     r_angle -= dr_angle;
     rh_angle += dr_angle * 3;

     dr_angle = PI / 4;
     r_angle -= 6*dr_angle;
     rh_angle += 6*dr_angle * 3;
     SetTimer(hWnd, TIMER_SPARK, 500, NULL);
   
        break;
    case WM_LBUTTONDOWN:
       // InvalidateRect(hWnd, NULL, TRUE);
       // SendMessage(hWnd, WM_PAINT, 1, NULL);
       // AfxMessageBox("Got you");
        KillTimer(hWnd, TIMER_ROTOR);
        iStroke++;
        dr_angle = PI / 4;
        r_angle -= dr_angle;
        rh_angle += dr_angle * 3;
        InvalidateRect(hWnd, NULL, FALSE);
        break;   

    case WM_LBUTTONDBLCLK:
        r_angle -= dr_angle;
        rh_angle += dr_angle * 3;
        UpdateWindow(hWnd);
    //    InvalidateRect(hWnd, NULL, TRUE);
        SendMessage(hWnd, WM_PAINT, 1, 1);
        break;
        

    case WM_RBUTTONDOWN:
        SetTimer(hWnd, TIMER_ROTOR, 500, NULL);
        SendMessage(hWnd, WM_PAINT, 1, NULL);
        dr_angle = PI / 24;
        r_angle -= dr_angle;
        rh_angle += dr_angle * 3;
        break;
    
    case WM_TIMER:
         if (wParam == TIMER_ROTOR)
        {
        
      //  KillTimer(hWnd, TIMER_ROTOR);
        iStroke++;
        dr_angle = PI / 4;
        r_angle -= dr_angle;
        rh_angle += dr_angle * 3;
        InvalidateRect(hWnd, NULL, FALSE);
         }
        break; 
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_OPTION:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_OPTIONBOX), hWnd, About);
                break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            LPPOINT lppt = NULL;
       
          
            RECT rc;
            int a;
            a = (int)(rh_angle * 360 / 2 / PI) % 360;

            GetClientRect(hWnd, &rc);
            putf(hdc, xpos, 316, (char*)"Rotor angle =", (double)a);
             // line(hdc, 2, 2, 300, 300);
            cx = (rc.left + rc.right) / 2;
            cy = (rc.top + rc.bottom) / 2;
            sz = (rc.right - rc.left) / 200;
            //   POINT p1 = { 152,rc.bottom }, p2 = { rc.left,rc.bottom };
            drawRect(hdc, rc, WHITE);
          //   Polygon(hdc, )
              coolant(hdc);
               rotor(hdc, DARKGRAY, istroke);
               draw_chambers(hdc);
                // test(hdc);
            // TODO: Add any drawing code that uses hdc here...
             bitmapx( hWnd,  message,  wParam,  lParam);
             setcolor(hdc, RED);
          //   Arrowline(hdc, p1, p2);
             EndPaint(hWnd, &ps);
         
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
           KillTimer(hWnd, TIMER_ROTOR);
           KillTimer(hWnd, TIMER_SPARK);
        break;
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
// Message handler for option box.
INT_PTR CALLBACK Option(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        case IDCANCEL:
            quircir_corner = 2.13;      InvalidateRect(GetParent(hDlg), NULL, FALSE);
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        case ID_OPTION0: quircir_corner = 2.13;      InvalidateRect(GetParent(hDlg), NULL, FALSE); break;
        case ID_OPTION1: quircir_corner = 1.83;      InvalidateRect(GetParent(hDlg), NULL, FALSE); break;
        case ID_OPTION2: quircir_corner = 2.03;      InvalidateRect(GetParent(hDlg), NULL, FALSE); break;
        case ID_OPTION3: quircir_corner = 2.93;      InvalidateRect(GetParent(hDlg), NULL, FALSE); break;

        }

        
        break;
    }
    return (INT_PTR)FALSE;
}

