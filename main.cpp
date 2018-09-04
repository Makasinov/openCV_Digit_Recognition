#include <opencv2/imgproc/imgproc.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

using namespace cv;

const char * filename {"C:\\Files\\test.jpg"}; // FILENAME CHANGE HERE (PATH)

struct Point
{
    struct {
        int x_u = 0, x_l = 0; // x upper cross & x lower cross
        int y = 0;            // y mid cross
        char x_uInfo = ' ';   // l - left, r - right, b - both
        char x_lInfo = ' ';
    } cross;
    int x = 0;                // x cord left upper angle
    int y = 0;                // y cord left upper angle
    int wide = 0;
    int height = 0;

    void info()
    {
        printf("\nx: %d, y: %d, wide: %d, height: %d",x,y,wide,height);
        printf("\ncross.y: %d, cross.x_u: %d, cross.x_l: %d",cross.y,cross.x_u,cross.x_l);
        printf("\ncross.x_uInfo: %c, cross.x_lInfo: %c\n",cross.x_uInfo,cross.x_lInfo);
    }
};



const int COLS_MAX_SIZE = 25;   // Matrix columns size
const int ROWS_MAX_SIZE = 25;   // Matrix rows size

bool arr[COLS_MAX_SIZE][ROWS_MAX_SIZE];

::Point getAttr(bool arr[COLS_MAX_SIZE][ROWS_MAX_SIZE]);
::Point getCross(bool arr[COLS_MAX_SIZE][ROWS_MAX_SIZE], ::Point point);
int getResult(bool arr[COLS_MAX_SIZE][ROWS_MAX_SIZE], ::Point p);

void drawArr(bool arr[COLS_MAX_SIZE][ROWS_MAX_SIZE])
{
    for(int x = 0; x < ROWS_MAX_SIZE; x++)
    {
        for(int y = 0; y < COLS_MAX_SIZE; y++)
        {
            std::cout << arr[x][y] << " ";
        }
        std::cout << std::endl;
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
            //printf("arr[%d][%d]",x+1,y+1);
            if (arr[x][y] == 1)
            {
                //printf(" <--\n");
                tmp.x = x;
                tmp.y = y;
                myExit = true;
            }
            //else printf("\n");
            if (myExit) break;
        }
    }

    // Цикл слева на право - сверху вниз
    for(int x = 0; x < ROWS_MAX_SIZE; x++)
    for(int y = tmp.y; y < COLS_MAX_SIZE; y++)
    {
        //printf("arr[%d][%d]",x+1,y+1);
        if (arr[x][y] == 1)
        {
            //printf(" <--\n");
            tmp.x = tmp.y + 1;
            tmp.y = x+1;
            return tmp;
        }
        //else printf("\n");
    }
    return tmp;
}

::Point getAttr(bool arr[COLS_MAX_SIZE][ROWS_MAX_SIZE])
{
    std::cout << std::endl;
    ::Point point,tmp,tmp2;

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

    //std::cout << "Point is: x - " << point.x << ", y - " << point.y << ", wide - " << point.wide << ", height - " << point.height << std::endl;

    return point;
}

::Point getCross(bool arr[COLS_MAX_SIZE][ROWS_MAX_SIZE], ::Point p)
{
    int pos = 0;
    int crossedTimes = 0;
    bool isCrossed = false;
    std::cout << std::endl;
    //Get y
    for(int i = 0; i < COLS_MAX_SIZE; i++ )
    {
        pos = (p.x - 1) + (p.wide / 2);
        //std::cout << "    " << arr[i][pos] << std::endl;      // DEBUG
        if (!isCrossed && arr[i][pos] == 1)
        {
            isCrossed = true;
            crossedTimes++;
        }
        if (isCrossed && arr[i][pos] == 0)
            isCrossed = false;
    }
    p.cross.y = crossedTimes;
    crossedTimes = 0;
    isCrossed = false;
    std::cout << std::endl;
    // Get x_u
    for(int i = 0; i < ROWS_MAX_SIZE; i++ )
    {
        pos = (p.y - 1) + (p.height / 4);
        //std::cout << arr[pos][i];             // DEBUG
        if (!isCrossed && arr[pos][i] == 1)
        {
            isCrossed = true;
            crossedTimes++;
            if (i <= COLS_MAX_SIZE / 2)
                p.cross.x_uInfo = 'l';
            else
            {
                if (p.cross.x_uInfo == 'l')
                    p.cross.x_uInfo = 'b';
                else p.cross.x_uInfo = 'r';
            }
        }
        if (isCrossed && arr[pos][i] == 0)
            isCrossed = false;
    }
    p.cross.x_u = crossedTimes;
    crossedTimes = 0;
    isCrossed = false;
    //std::cout << std::endl;
    // Get x_l
    for(int i = 0; i < ROWS_MAX_SIZE; i++ )
    {
        pos = (p.y - 1) + (p.height / 1.5);
        //std::cout << arr[pos][i];             // DEBUG
        if (!isCrossed && arr[pos][i] == 1)
        {
            isCrossed = true;
            crossedTimes++;
            if (i <= COLS_MAX_SIZE / 2)
                p.cross.x_lInfo = 'l';
            else
            {
                if (p.cross.x_lInfo == 'l')
                    p.cross.x_lInfo = 'b';
                else p.cross.x_lInfo = 'r';
            }
        }
        if (isCrossed && arr[pos][i] == 0)
            isCrossed = false;
    }
    p.cross.x_l = crossedTimes;

    return p;
}

int getResult(bool arr[COLS_MAX_SIZE][ROWS_MAX_SIZE], ::Point p)
{
    if (p.wide >= COLS_MAX_SIZE || p.height >= ROWS_MAX_SIZE)
        return -1;
    if (p.cross.y == 2 && p.cross.x_l == 2 && p.cross.x_u == 2 && p.cross.x_uInfo == 'b' && p.cross.x_lInfo == 'b')
        return 0;
    if (p.cross.y == 3 && p.cross.x_l == 1 && (p.cross.x_u == 1 || p.cross.x_u == 2) &&
            (p.cross.x_uInfo == 'b' || p.cross.x_uInfo == 'r') && p.cross.x_lInfo == 'l')
        return 2;
    if (p.cross.y >= 3 && p.cross.x_l == 1 && p.cross.x_u == 1 && p.cross.x_uInfo == 'r' && p.cross.x_lInfo == 'r')
        return 3;
    if (p.cross.y == 1 && p.cross.x_l == 1 && p.cross.x_u == 2 && p.cross.x_uInfo == 'b' && p.cross.x_lInfo == 'r')
        return 4;
    if (p.cross.y == 3 && p.cross.x_l == 1 && p.cross.x_u == 1 && p.cross.x_uInfo == 'l' && p.cross.x_lInfo == 'r')
        return 5;
    if (p.cross.y == 3 && p.cross.x_l == 2 && p.cross.x_u == 1 && p.cross.x_uInfo == 'l' && p.cross.x_lInfo == 'b')
        return 6;
    if (p.cross.y == 2 && p.cross.x_l == 1 && p.cross.x_u == 1 && p.cross.x_uInfo == 'r' && (p.cross.x_lInfo == 'r' || p.cross.x_lInfo == 'l'))
        return 7;
    if (p.cross.y == 3 && p.cross.x_l == 2 && p.cross.x_u == 2 && p.cross.x_uInfo == 'b' && p.cross.x_lInfo == 'b')
        return 8;
    if (p.cross.y == 3 && p.cross.x_l == 1 && p.cross.x_u == 2 && p.cross.x_uInfo == 'b' && p.cross.x_lInfo == 'r')
        return 9;
    if (p.cross.y == 1 && (p.cross.x_u == 1 || p.cross.x_u == 2) && p.cross.x_l == 1) // ? i think else statement w'd be better solution
        return 1;

    return -1;
}

void recognizeNumber(bool arr[COLS_MAX_SIZE][ROWS_MAX_SIZE])
{
    ::Point point = getAttr(arr);
    point = getCross(arr,point);
    point.info();
    int result = getResult(arr,point);
    if (result != -1)
    printf("\n----------------------------------\n"
           "Result:\t%d",result);
    else printf("\nWtf? Can't recognize the symbol o_O\n");
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
    std::cout << "\n-----------------------------------------------------------\n";
    drawArr(arr);
    recognizeNumber(arr);


    return 0;
}


