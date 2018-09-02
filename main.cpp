#include <opencv2/imgproc/imgproc.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;
using namespace cv;

struct Point
{
    int x = 0;
    int y = 0;
    int wide = 0;
    int height = 0;
};

const int COLS_MAX_SIZE = 15;
const int ROWS_MAX_SIZE = 15;
const char * filename {"C:\\Files\\three#2.jpg"};

bool arr[COLS_MAX_SIZE][ROWS_MAX_SIZE];

::Point getAttr(bool arr[COLS_MAX_SIZE][ROWS_MAX_SIZE]);

void drawArr(bool arr[COLS_MAX_SIZE][ROWS_MAX_SIZE])
{
    for(int x = 0; x < ROWS_MAX_SIZE; x++)
    {
        for(int y = 0; y < COLS_MAX_SIZE; y++)
        {
            cout << arr[x][y] << " ";
        }
        cout << endl;
    }
}

::Point getUpperLeftPoint(bool arr[COLS_MAX_SIZE][ROWS_MAX_SIZE])
{
    ::Point tmp;
    bool myExit = false;
    //  Цикл сверху вниз - слева на право
    for(int y = 0; y < COLS_MAX_SIZE; y++)
    {
        if (myExit) break;
        for(int x = 0; x < ROWS_MAX_SIZE; x++)
        {
            printf("arr[%d][%d]",x+1,y+1);
            if (arr[x][y] == 1)
            {
                printf(" <--\n");
                tmp.x = x;
                tmp.y = y;
                myExit = true;
            }
            else printf("\n");
            if (myExit) break;
        }
    }

    // Цикл слева на право - сверху вниз
    for(int x = 0; x < ROWS_MAX_SIZE; x++)
    for(int y = tmp.y; y < COLS_MAX_SIZE; y++)
    {
        printf("arr[%d][%d]",x+1,y+1);
        if (arr[x][y] == 1)
        {
            printf(" <--\n");
            tmp.x = tmp.y + 1;
            tmp.y = x+1;
            return tmp;
        }
        else printf("\n");
    }
}

::Point getAttr(bool arr[COLS_MAX_SIZE][ROWS_MAX_SIZE])
{
    cout << endl;
    ::Point point,tmp,tmp2;
    bool isBlack;
    if (arr[0][0] == 0) isBlack = true;
        else isBlack = false;

    bool foundedFirstWhiteColor = false;

    for(int y = 0; y < COLS_MAX_SIZE; y++)
    for(int x = 0; x < ROWS_MAX_SIZE; x++)
    {
        if (arr[x][y] == 1 && !foundedFirstWhiteColor)
        {
            foundedFirstWhiteColor = true;
            point.x = y+1;
            point.y = x+1;
            continue;
        }
        if (foundedFirstWhiteColor && arr[x][y] == 1)
        {
            // Расчет ширины
            point.wide = COLS_MAX_SIZE - (point.x - 1) - (COLS_MAX_SIZE - y-1);
        }
    }

    for(int x = 0; x < ROWS_MAX_SIZE; x++)
    for(int y = 0; y < COLS_MAX_SIZE; y++)
    {
        if (arr[x][y] == 1)
        {
            tmp.x = x+1;
            tmp.y = y+1;
        }
    }

    for(int x = ROWS_MAX_SIZE - 1; x != 0; x--)
    for(int y = COLS_MAX_SIZE - 1; y != 0; y--)
    {
        if (arr[x][y] == 1)
        {
            tmp2.x = x+1;
            tmp2.y = y+1;
        }
    }

    // Расчёт высоты
    point.height = ROWS_MAX_SIZE - (ROWS_MAX_SIZE - tmp.x) - (ROWS_MAX_SIZE - (ROWS_MAX_SIZE - (tmp2.x - 1)));
    // Получание координаты левого верхнего угла
    tmp = getUpperLeftPoint(arr);
    point.x = tmp.x;
    point.y = tmp.y;

    cout << "Point is: x - " << point.x << ", y - " << point.y <<
            ",\n wide - " << point.wide << ", height - " << point.height<< endl;

    return point;
}

void recognizeNumber(bool arr[COLS_MAX_SIZE][ROWS_MAX_SIZE])
{
    getAttr(arr);
    cout << endl << endl;
}

int main()
{
    IplImage * image = cvLoadImage(filename,2);
    IplImage * src   = cvCloneImage(image);
    assert(src != 0);

    Mat M = imread(filename,CV_LOAD_IMAGE_GRAYSCALE);
    M = M > 128;

    for(int x = 0; x < ROWS_MAX_SIZE; x++)
    {
        for(int y = 0; y < COLS_MAX_SIZE; y++)
        {
            if (M.at<bool>(x,y) == true)
                arr[x][y]  = 1;
            else arr[x][y] = 0;
        }
    }
    //cout << "SOURCE = " << endl << " " << M << endl << endl;
    cout << "\n-----------------------------------------------------------\n";
    drawArr(arr);
    recognizeNumber(arr);


    return 0;
}


