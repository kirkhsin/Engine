// WindowsProject1.cpp : Defines the entry point for the application.
//
#include <math.h>
#include <iostream>
#include<string>
using namespace std;

#include "framework.h"
#include "WindowsProject1.h"

#define BLACK	RGB(0, 0, 0) 
#define BLUE	RGB(0, 0, 255) 
#define GREEN	RGB(0, 255, 0) 
#define CYAN	RGB(0, 127, 0) 
#define RED	RGB(255, 0, 0) 
#define MAGENTA	RGB(127, 0, 0) 
#define BROWN	RGB(122, 85, 85) 
#define LIGHTGRAY	RGB(127, 127, 127) 
#define DARKGRAY	RGB(63,63, 63) 
#define LIGHTBLUE	RGB(0, 0, 127) 
#define LIGHTGREEN	RGB(0,127, 0) 
#define LIGHTCYAN	RGB(0, 0,127) 
#define LIGHTRED	RGB(127, 0, 0) 
#define LIGHTMAGENTA	RGB(127, 0, 127) 
#define YELLOW	RGB(0, 255, 255) 
#define WHITE	RGB(255, 255, 255) 

#define SOLID_FILL 1
#define SLASH_FILL 3


#define PI 3.141592653589793238462
/* #define HOUSE_A 7.6 */
//#define HOUSE_A 7.6
//#define HOUSE_B 44
#define ROTOR_A 8
#define ROTOR_B 40
double r_angle = PI / 4; /* rotor angle */
double rh_angle = PI / 4; /* rotor-housing center angle */
double h_angle = PI / 4;
double offsetr = 3;
double sz = 7;
#define offsetx1 (int)((double)offsetr*cos(rh_angle)*sz+0.5)
#define offsety1 (int)((double)offsetr*sin(rh_angle)*sz+0.5)
//#define offsetx 14*cos(rh_angle)
//#define offsety 14*sin(rh_angle)
#define cx 500
#define cy 350
//#define sz  6.0



double   quircir_size = 45;
#define quircir_corner 2.13
#define MAPX(x, y) (int)((double)cx+sz*(x*cos(h_angle)-y*sin(h_angle)))
#define MAPY(x, y) (int)((double)cy-sz*(y*cos(h_angle)+x*sin(h_angle)))
#define MAPX_rotor(x, y) (int)((double)cx+sz*(x*cos(r_angle)-y*sin(r_angle))-offsetx1)
#define MAPY_rotor(x, y) (int)((double)cy-sz*(y*cos(r_angle)+x*sin(r_angle))+offsety1)
#define IMAX 3600
int housingPts[IMAX * 2];
int nHousingPts = 0;
int rotorPts[IMAX * 2];
int nRotorPts = 0;

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
     
 
      //    Restoring the original object
  //  SelectObject(hdc, original);

}

void drawpoly(HDC hdc, int n, int* pts)
{
    POINT cpt[IMAX*2];
    int i;
    for (i = 0; i < n; i++)
    {
        cpt[i].x = pts[2 * i];
        cpt[i].y = pts[2 * i + 1];
    }
    Polyline(hdc, cpt, n);
}

void fillpoly(HDC hdc, int n, int* pts)
{
    POINT cpt[IMAX*2];
    int i;
    for (i = 0; i < n; i++)
    {
        cpt[i].x = pts[2 * i];
        cpt[i].y = pts[2 * i + 1];
    }
    Polygon(hdc, cpt, n);
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
        int dx;
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

void puti(HDC hdc, int x, int y, char* s, int i)
{
  
    wchar_t    buffer[50] = L"xxxxxxxxxxxxxxxxxx";

    swprintf(buffer, sizeof(buffer), L" test= %d ",  i);
  

    RECT  rc;
    int dx;
    rc.left = x;
    rc.bottom = y;
    rc.right = x+sizeof(buffer);
    rc.top = y-16;

    DrawText(
        hdc,
        (LPCTSTR)buffer,
        sizeof(buffer),
        (LPRECT)&rc,
        DT_BOTTOM
    );


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


void line(HDC hdc, int x, int y, int x2, int y2)
{
    LPPOINT lppt = NULL;


    MoveToEx(hdc, x, y, lppt);
    LineTo(hdc, x2, y2);
}


void fill_chamber(HDC hdc, int i1, int i2, COLORREF color)
{
    int i, k = 0, tmp; 
    POINT pts[2*IMAX];
    int shift = (int)((rh_angle - PI / 4)*IMAX / 2/PI/4);
  //  if (i1 < i2)
    {
            for (i = i1; i < i2; i++)
            {
                tmp = (i + shift) % IMAX;
                pts[k].x = rotorPts[2 * tmp];
                pts[k].y = rotorPts[2 * tmp + 1];
                k++;
            }

            for (i = i2-1; i >= i1; i--)
            {
                pts[k].x = housingPts[2 * i];
                pts[k].y = housingPts[2 * i + 1];
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
    /*
    double x1, y1, theda1 = 0;
    double offsetr, newr2, r1, r2;
    double x2, y2, theda2 = 0;
  

    r_angle = 0;
    rh_angle = 0;
    h_angle = 0;
    getxy(&x1, &y1, theda1, ROTOR_A, ROTOR_B, 3);
    offsetr = sqrt(offsetx * offsetx + offsety * offsety);

    //  r1 = sqrt(x1*x1+y1*y1);

    r_angle = PI / 4;  
    rh_angle = PI / 4;  
    h_angle = PI / 4;
   return sqrt(x1 * x1 + y1 * y1)-offsetr / sz - 1; */
        return ROTOR_A + ROTOR_B - sqrt(offsetx1 * offsetx1 + offsety1 * offsety1) / sz;

}



/*
double compute_area(HDC hdc, double theda)
{
    int i;
    double area = 0;
    double x1, y1;
    double x2, y2;
    double theda2;
    double dtheda = 0.001;
    int limit = (int)(theda / dtheda + 0.5);
  

    return 0;


    if (theda < PI && theda >= PI / 2)

        limit = (int)((PI - theda) / dtheda);
    else if (theda < 2 * PI && theda >= 3 / 2 * PI)
    {
        limit = (int)( (2 * PI - theda) / dtheda);
    }
    else return 0;

    for (i = 0; i < limit; i++)
    {
        double tmp1 = theda + i * dtheda;
        getxy(&x1, &y1, tmp1, ROTOR_A, ROTOR_B, 3);
        theda2 = atan(tan(tmp1) * x1 / (x1 - offsetx1 ));


        if (theda2 < 0)
        {
            if (x1 < 0)
            {
                theda2 += PI; setcolor(hdc,LIGHTGREEN);
            }
            else
            {
                theda2 = 2.0 * PI + theda2; setcolor(hdc,LIGHTRED);
            }
        }
        else if (x1 < 0) theda2 += PI;


        getxy_squircir(&x2, &y2, theda2, quircir_size, quircir_corner);

        x2 -= sqrt(offsetx1 * offsetx1 + offsety1 * offsety1) / sz;

        line(hdc, MAPX_rotor(x1, y1), MAPY_rotor(x1, y1), MAPX_rotor(x2, y2), MAPY_rotor(x2, y2));

        area += (sqrt(x2 * x2 + y2 * y2) - sqrt(x1 * x1 + y1 * y1)) * dtheda;

    }

    return area;

}
*/

void housing1(HDC hdc)
{
    int i;
    double x, y;
    double theda;
    COLORREF color = DARKGRAY;

    nHousingPts = 0;
    for (i = 0; i < IMAX; i++)
    {

        theda = 2.0 * PI * i / IMAX;
        getxy_squircir(&x, &y, theda, quircir_size, quircir_corner);
        // fprintf(hFile, "%d,%d,%d,\n", MAPX(x, y), MAPY(x, y), color);
        housingPts[2 * i] = MAPX(x, y);
        housingPts[2 * i + 1] = MAPY(x, y);
      //  putpixel(hdc, MAPX(x,y), MAPY(x,y), GREEN);
    }


    setcolor(hdc,color);
    /*
    nHousingPts = 0;
    for (i = 0; i < IMAX - 1; i++)
    {
        line(hdc, housingPts[2 * i], housingPts[2 * i + 1],
            housingPts[2 * i + 2], housingPts[2 * i + 3]);
        nHousingPts++;

    }
    */

    // fprintf(hFile, "%d,%d,%d,\n", MAPX(0, 0), MAPY(0, 0), 57);

    // fprintf(hFile, "%d,%d,%d,\n", -1, -1, color);
    setfillstyle( hdc,SOLID_FILL, BROWN);
     fillpoly(hdc, IMAX, housingPts);

    circle(hdc, MAPX(0, 0), MAPY(0, 0), 57);
}



int  get_interset_rotor_housing(HDC hdc, LPPOINT pts)
{
    int i; 
    int x, y, x1, y1,  x2, y2;
        // , offsetr, theda1, theda2;
    int j;
    int npts = 0;
  
    LPSTR s[123] ;
    wchar_t hebrew_str[123] = L"test";
    wchar_t buffer[100];
   
    return 0;

    for (i = 0; i < IMAX; i++)
    {

   
        x1 = rotorPts[2 *i];
        y1 = rotorPts[2 * i + 1];


    
       // swprintf(buffer, sizeof(hebrew_str), L"test %d", 123);
       // wcout << L"Some hebrew letters\n" << buffer << endl;
        // swprintf(buffer + cx, 100 - cx - 1, L", and the half of that is %d.", 80 / 2 / 2);

        for (j = 0; j < IMAX; j++)
        {
            x2 = housingPts[2 * j];
            y2 = housingPts[2 * j + 1];
            if (j % 600)
            {
                setcolor(hdc, RED);
               // circle(hdc, x1, y1, 6);
                setcolor(hdc, WHITE);
               // circle(hdc, x1, y1, 6);
                

               // circle(hdc, x2, y2, 6);
            }
            
            if ( abs(x1 - x2) <=2 && abs(y1 - y2) <=2)
            {

               // string a;
               // a.Format(_T("npts %d"), npts);
             //   swprintf(s + cx, 100 - cx - 1, L", and the half of that is %d.", 80 / 2 / 2);
              //  wprintf(buffer, sizeof(hebrew_str), L"npts  %d", npts);
               // wsprintf(buffer,  L"npts  %d", npts);
               // swprintf(buffer, sizeof(buffer), L"test %d", npts);
                setcolor(hdc, i % 16);
                circle(hdc, x1, y1, 6); 
                setcolor(hdc, j % 16);
                circle(hdc, x2, y2, 8);
                swprintf(buffer, sizeof(hebrew_str), L"npts i j x1, y1 =  %d %d %d %f %f", npts, i, j, x1, y1);
             //   MessageBox(NULL, (LPCWSTR)buffer, (LPCWSTR)buffer, 1);
               // pts[npts].x = x1;
               // pts[npts].y = y1;
                npts++;
            }
        }

     
    }
    return 4;
}

void draw_chambers(HDC hdc)
{
    return;
  //  puti( hdc, "test i ", 435);
    // if (fabs(rh_angle - PI/4) < 0.1)
    {
        fill_chamber(hdc, 0, IMAX /9, RED);
        fill_chamber(hdc, IMAX - 1 - IMAX / 9, IMAX - 1, RED);

        fill_chamber(hdc, IMAX/2-IMAX/6, IMAX/2+ IMAX / 6, LIGHTBLUE); 

    }

}



void fill_area(HDC hdc, double a, double b, int nCornerHousing, COLORREF color)
{
    int i;

    double area = 0;
    double x1, y1;
    double x2, y2;
    double theda2, theda;
    double dtheda = 0.1;
    int limit = 100;
    static int pts[1000];
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

        double r, dr;
        double tmp1 = theda + i * dtheda;
        tmp1 = tmp1 - (double)(tmp1 / PI / 2) * 2 * PI;
        getxy(&x1, &y1, tmp1, ROTOR_A, ROTOR_B, 3);

        pts[2 * i] = MAPX_rotor(x1, y1);
        pts[2 * i + 1] = MAPY_rotor(x1, y1);

    }

    i = 0;
    jj = jjmax / 2 + IMAX / 4 * nCornerHousing;
    while (i < jjmax)
    {
        char s[123];


        pts[2 * (limit + i)] = housingPts[2 * jj];
        pts[2 * (limit + i) + 1] = housingPts[2 * jj + 1];
        i++;
        jj--; if (jj < 0) jj = jj + nHousingPts;

    }
    for (i = 0; i < limit + jjmax; i++) putpixel(hdc, pts[2 * i], pts[2 * i + 1], GREEN);



    setfillstyle( hdc,1, color);
    fillpoly(hdc, limit + jjmax, pts);
    setcolor(hdc,DARKGRAY);
    drawpoly(limit + jjmax, pts);

}


void draw_seals(HDC hdc, COLORREF color, int istroke)
{
    int i, imax = 24;
    double x, y;
    double angles[60];
    int ia = 0;
    char s[123];
    enum { compress, power, exhaust, exhaust1, exhaust2, exhaust3, intake };
    COLORREF colorStroke[4] = { LIGHTRED, YELLOW, LIGHTCYAN, LIGHTGREEN };
    setcolor(hdc,color);

   
    for (i = 0; i < imax; i++)
    {
        double delta1, delta2;
        double theda = 2 * PI * i / imax;

        getxy(&x, &y, theda, ROTOR_A, ROTOR_B, 3);
        angles[i] = theda;
        if (i == 3 || i == 21 || i == 6 || i == 11 || i == 14 || i == 18)
        {
            setcolor(hdc,GREEN);
            circle(hdc, MAPX_rotor(x, y), MAPY_rotor(x, y), 5);
            setfillstyle( hdc,1, DARKGRAY);
            pieslice(hdc, MAPX_rotor(x, y), MAPY_rotor(x, y), 0, 360, 4);
        }
    }

    

    switch (istroke)
    {
    case 0:
        fill_area(hdc,angles[3], angles[8], 1, DARKGRAY);
        fill_area(hdc,angles[9], angles[15], 2, colorStroke[exhaust]);
        fill_area(hdc,angles[15], angles[21], 3, DARKGRAY);
        outtextxy(hdc, xpos, ypos, (char *)"Compress and Ignite");
         while (!kbhit()) 
        {
            fill_area(hdc,angles[21], angles[3], 0, colorStroke[compress]);
            fill_area(hdc,angles[21], angles[3], 0, RED);
        }

        break;
        //COMPRESS
    case 1:

        fill_area(hdc,angles[5], angles[10], 1, DARKGRAY);
        fill_area(hdc,angles[10], angles[16], 2, colorStroke[exhaust]);
        fill_area(hdc,angles[16], angles[23], 3, DARKGRAY);
        outtextxy(hdc, xpos, ypos, (char *)"Power and combust");
        while (!kbhit()) {
            fill_area(hdc,angles[23], angles[5], 0, colorStroke[power]);
            fill_area(hdc,angles[23], angles[5], 0, WHITE);
        }
        break;
        //POWER
    case 2:      // EXHAUST
        fill_area(hdc,angles[7], angles[12], 1, DARKGRAY);
        fill_area(hdc,angles[12], angles[18], 2, colorStroke[power]);
        fill_area(hdc,angles[18], angles[0], 3, DARKGRAY);
        outtextxy(hdc, xpos, ypos, (char*)"Exhaust");
        while (!kbhit()) {
            fill_area(hdc,angles[0], angles[7], 0, colorStroke[exhaust]);
            fill_area(hdc,angles[0], angles[7], 0, WHITE);
        }
        break;
    case 3: //EXHAUST 1
        fill_area(hdc,angles[8], angles[14], 1, DARKGRAY);
        fill_area(hdc,angles[14], angles[19], 2, colorStroke[intake]);
        fill_area(hdc,angles[19], angles[1], 3, DARKGRAY);
        outtextxy(hdc, xpos, ypos, (char*)"exhaust 1");
        while (!kbhit()) {

            fill_area(hdc,angles[1], angles[8], 0, colorStroke[exhaust]);
            fill_area(hdc,angles[1], angles[8], 0, LIGHTCYAN);
        }
        break;

    case 4: //EXHAUST 2
        fill_area(hdc,angles[9], angles[15], 1, DARKGRAY);
        fill_area(hdc,angles[15], angles[21], 2, colorStroke[intake]);
        fill_area(hdc,angles[21], angles[3], 3, DARKGRAY);
        outtextxy(hdc, xpos, ypos, (char*)"exhaust 2");
        while (!kbhit()) {

            fill_area(hdc,angles[3], angles[9], 0, colorStroke[exhaust]);
            fill_area(hdc,angles[3], angles[9], 0, LIGHTCYAN);
        }
        break;
    case 5: //EXHAUST 3
        fill_area(hdc,angles[10], angles[16], 1, DARKGRAY);
        fill_area(hdc,angles[16], angles[23], 2, colorStroke[intake]);
        fill_area(hdc,angles[23], angles[4], 3, DARKGRAY);
        outtextxy(hdc, xpos, ypos, (char*)"exhaust 3");
        while (!kbhit()) {

            fill_area(hdc,angles[4], angles[10], 0, colorStroke[exhaust]);
            fill_area(hdc,angles[4], angles[10], 0, LIGHTCYAN);
        }
        break;

    case 6: //INTAKE
        fill_area(hdc,angles[12], angles[17], 1, DARKGRAY);
        fill_area(hdc,angles[17], angles[0], 2, colorStroke[compress]);
        fill_area(hdc,angles[0], angles[7], 3, WHITE);
        outtextxy(hdc, xpos, ypos, (char*)"Intake");
        while (!kbhit()) {

            fill_area(hdc,angles[7], angles[12], 0, colorStroke[intake]);
            fill_area(hdc,angles[7], angles[12], 0, LIGHTGRAY);
        }

        break;
    case 7: //INTAKE 1
        fill_area(hdc,angles[14], angles[19], 1, DARKGRAY);
        fill_area(hdc,angles[19], angles[1], 2, colorStroke[compress]);
        fill_area(hdc,angles[1], angles[8], 3, DARKGRAY);
        outtextxy(hdc, xpos, ypos, (char*)"Intake 1");
        while (!kbhit()) {

            fill_area(hdc,angles[8], angles[14], 0, colorStroke[intake]);
            fill_area(hdc,angles[8], angles[14], 0, LIGHTGRAY);
        }

        break;

    case 8: //INTAKE 2
        fill_area(hdc,angles[15], angles[21], 1, DARKGRAY);
        fill_area(hdc,angles[21], angles[2], 2, colorStroke[exhaust]);
        fill_area(hdc,angles[2], angles[9], 3, DARKGRAY);
        outtextxy(hdc, xpos, ypos, (char*)"Intake 2");
        while (!kbhit()) {

            fill_area(hdc,angles[9], angles[15], 0, colorStroke[intake]);
            fill_area(hdc,angles[9], angles[15], 0, LIGHTGRAY);
        }

        break;
    case 9: //INTAKE 3
        fill_area(hdc,angles[16], angles[23], 1, DARKGRAY);
        fill_area(hdc,angles[23], angles[5], 2, colorStroke[power]);
        fill_area(hdc,angles[5], angles[10], 3, DARKGRAY);
        outtextxy(hdc, xpos, ypos, (char*)"Intake 3");
        while (!kbhit()) {

            fill_area(hdc,angles[10], angles[16], 0, colorStroke[intake]);
            fill_area(hdc,angles[10], angles[16], 0, LIGHTGRAY);
        }

        break;

    case 10: //COMPRESS
        fill_area(hdc,angles[17], angles[0], 1, DARKGRAY);
        fill_area(hdc,angles[0], angles[6], 2, colorStroke[intake]);
        fill_area(hdc,angles[6], angles[12], 3, DARKGRAY);
        outtextxy(hdc, xpos, ypos, (char*)"Compress ");
        while (!kbhit()) {

            fill_area(hdc,angles[12], angles[17], 0, colorStroke[compress]);
            fill_area(hdc,angles[12], angles[17], 0, LIGHTGRAY);
        }

        break;
    case 11: //COMPRESS
        fill_area(hdc,angles[19], angles[1], 1, DARKGRAY);
        fill_area(hdc,angles[1], angles[8], 2, colorStroke[intake]);
        fill_area(hdc,angles[8], angles[14], 3, DARKGRAY);
        outtextxy(hdc, xpos, ypos, (char*)"Compress 1");
        while (!kbhit()) {

            fill_area(hdc,angles[14], angles[19], 0, colorStroke[compress]);
            fill_area(hdc,angles[14], angles[19], 0, LIGHTGRAY);
        }

        break;



    default:;

    }

}
void draw_rotorgear(HDC hdc)
{
    
    setfillstyle( hdc,SOLID_FILL, BROWN);
    setcolor(hdc,DARKGRAY);
    // 	pieslice (MAPX_rotor(0,0), MAPY_rotor(0,0), 0, 360, 70);
    circle(hdc, MAPX_rotor(0, 0), MAPY_rotor(0, 0), 70/4*sz);
    setcolor(hdc,BLUE);
    //	outtextxy(MAPX_rotor(16,0), MAPY_rotor(16,0),"rotor gear");
}

void draw_cranshaft(HDC hdc)
{
     
    setcolor(hdc,DARKGRAY);
    setfillstyle( hdc,SLASH_FILL, DARKGRAY);
    pieslice(hdc, MAPX(0, 0), MAPY(0, 0), 0, 360, 57/4*sz);
    circle(hdc, MAPX(0, 0), MAPY(0, 0), 57/4*sz);
    setfillstyle(hdc, SOLID_FILL, BROWN);
    outtextxy(hdc, MAPX(0, 0), MAPY(0, 0), (char*)"cranshaft");
}

void rotor(HDC hdc, COLORREF color, int istroke)
{


    double x, y, ptheda, dptheda;
    double  theda;
    int rect[8] = { 23,23,234,23,234,234,23,234 };
    int i;


    nRotorPts = 0;
    setcolor(hdc,color);

    for (i = 0; i < IMAX; i++)
    {


        double delta1, delta2;

        double theda = (double)2 * PI * (i) / IMAX;

        getxy(&x, &y, theda, ROTOR_A, ROTOR_B, 3);


       
        rotorPts[2 * i] = MAPX_rotor(x, y);
        rotorPts[2 * i + 1] = MAPY_rotor(x, y);
        nRotorPts++;
       
        if (i == IMAX / 2 - 1 && fabs(rh_angle) <0.01)
        {
            
            int xx, yy;
            puti(hdc, 12, 345, (char*)"tsss", i);
            xx = rotorPts[2 * i];
                yy= rotorPts[2 * i + 1];
                int hh = housingPts[(IMAX  - 1)]; 

                puti(hdc, 12, 365, (char*)"tsss",yy);

        }


    }

    setfillstyle( hdc,1, LIGHTGRAY);
    // setfihdc, llstyle(SOLID_FILL, getmaxcolor());
    fillpoly(hdc, IMAX, rotorPts);
    setcolor(hdc,LIGHTGREEN);
    drawpoly(IMAX, rotorPts);
    draw_chambers(hdc);
    //	fillpoly(4, rect);
    setcolor(hdc,LIGHTGRAY);
    draw_rotorgear(hdc);
    draw_cranshaft(hdc);
    draw_seals(hdc, color, istroke);

   // if (i == 0)
     

       line(hdc, MAPX_rotor(40, 0), MAPY_rotor(40, 0), rotorPts[0], rotorPts[1]);
     

    {
        char s[200];
        setcolor(hdc,5);

        // sprintf(s, "Compression Ratio: 13.0");
        outtextxy(hdc, xpos, ypos, (char*)"Compression Ratio: 13.0");  
    }

    // fprintf(hFile, "%d, %d, %d, \n", MAPX_rotor(0, 0), MAPY_rotor(0, 0), 70);
    // fprintf(hFile, "%d, %d, %d, \n", -1, -1, color);


}






void xmain(HDC hdc)
{
  //  int gd = DETECT, gm;
    double dr_angle = PI / 4;
    int times = 8;
    int c = 0;


    double x, y, ptheda, dptheda;
    int istroke;
    //initgraph(&gd, &gm, "c:\\turboc3\\bgi ");

    // testsetfillstyle( hdc,);
   // hFile = fopen("rotors.dat", "w+");


    dr_angle = PI / times;
    //  for (istroke=0; istroke<12; istroke++)
    istroke = 0;
    /*
    while (1)
    {
        static int pts[20] = { 0,0,600,0,600,450,0,450,0,0,
                 160,160,360,160,360,350,160,350,160,160 };
        char s[123];

        setcolor(hdc,LIGHTGRAY);
        setfillstyle( hdc,1, BLACK);

        fillpoly(hdc, 4, pts);
        housing1(hdc);
        setfillstyle( hdc,1, WHITE);
        rotor(3, istroke);
        //sprintf(s, "stroke id=%d", istroke);
        
        if (kbhit()) c = getch();
        if (c == 27) break;
        if (c == 0x48 || c == 0x4b || c == 0x08)
        {
            istroke -= 2;

            if (istroke < 0)
            {
                istroke = 16 - 1; continue;
            }
            r_angle += 2 * dr_angle;
            rh_angle -= 2 * dr_angle * 3;
        }
        else
        {
            istroke++;

            r_angle -= dr_angle;
            rh_angle += dr_angle * 3;
        }
        if (c == 'g')
        {
           
            istroke++;

            r_angle -= dr_angle / 12;
            rh_angle += dr_angle * 3 / 12;
        }


    }

    */

    //fclose(hFile);

    //closegraph();
}


void test(HDC hdc)
{
    int i;
    rh_angle = 0;
    housing1(hdc);
    rotor(hdc, RED, 0);
    i = rotorPts[0];
    puti(hdc, xpos, ypos,  (char*)"rotor x 0", i); ypos += 24;
    i = housingPts[0];
    puti(hdc, xpos, ypos, (char*)"housing  x0", i); ypos += 24;
    i = rotorPts[IMAX/2-1];
    puti(hdc, xpos, ypos, (char*)"rotor x  ", i); ypos += 24;
    i = housingPts[IMAX/2-1];
    puti(hdc, xpos, ypos, (char*)"housing  x ", i); ypos += 24;


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


#define TIMER1 1001


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
    RECT rc;

    switch (message)
    {
    case WM_CREATE:
        // SetTimer( hWnd, NULL, 50, (TIMERPROC) &Timerproc  );

            // SetTimer(TIMER1, 0, 300, (TIMERPROC)&f);
      quircir_size = get_quircir_size();
      r_angle = 0;
      h_angle = 0;
      rh_angle = 0;

       //  MessageBox(hWnd, (LPCWSTR)"set timer", (LPCWSTR)"set timer", 1);
        // MessageBox(hWnd, (LPCWSTR)"¬Ý¨ì", NULL, 1);
        break;
    case WM_LBUTTONDOWN:
       // InvalidateRect(hWnd, NULL, TRUE);
       // SendMessage(hWnd, WM_PAINT, 1, NULL);
        
        UpdateWindow(hWnd);
        break;   
    case WM_RBUTTONDOWN:
        SetTimer(hWnd, 0, 400, NULL);
        break;
    
    case WM_TIMER:
       //  if (wParam == TIMER1)
        {
        
        istroke++;

        r_angle -= dr_angle;
        rh_angle += dr_angle * 3;
         // MessageBox(hWnd, (LPCWSTR)"asdfasdf", (LPCWSTR)"asdfasdf", 1);
        SendMessage(hWnd, WM_PAINT, 1, NULL);
           InvalidateRect(hWnd, NULL, TRUE);
         }
        break; 
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
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
            POINT intersetPts[234];
            int nIntersetPts = 0;


           //  MoveToEx(hdc, 234, 234, lppt);
             // line(hdc, 2, 2, 300, 300);
              housing1(hdc);
               rotor(hdc, DARKGRAY, istroke);
               nIntersetPts = get_interset_rotor_housing(hdc, intersetPts);
               test(hdc);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
           KillTimer(hWnd, TIMER1);
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
