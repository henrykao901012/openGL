#include <windows.h>
#define GLUT_DISABLE_ATEXIT_HACK
#ifdef __GGGG__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>
#include <iostream>

using namespace std;

#define WHITE 1
#define RED 2
#define GREEN 3
#define BLUE 4

#define LINE 1
#define TRIANGLE 2
#define RECTANGLE 3
#define CIRCLE 4

#define LOAD 1
#define SAVE 2
#define CLEAR 3

#define Pi atan(1) * 4
struct point
{
    long x;
    long y;
};
int count1 = 0;
point point1[20000];
int count2 = 0;
point point2[20000];
int count3 = 0;
point point3[20000];
int count4 = 0;
point point4[20000];

int n = 1000;
int shape = 1;
int color = WHITE;
int color_arr[5][20000]; // using only 1 ~ 4 for poly

int width_arr[5][20000]; // ..

int screenWidth = 640;
int screenHeight = 480;

char text_arr[100][100];
int text_col;
int text_size[1000];

int width = 1;
int mouse_x;
int mouse_y;

void drawtext()
{
    glClear(GL_COLOR_BUFFER_BIT);

    float x = 10;
    float y = 460;

    for (int col = 0; col <= text_col; col++)
    {
        glRasterPos2f(x, y - col * 15);
        // for (char *c = text_arr[col]; *c != '\0'; c++)
        //{
        //     // if (*c != 13)
        //     glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
        // }
        for (int row = 0; row <= text_size[col]; row++)
        {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, text_arr[col][row]);
        }
    }
    glutBitmapCharacter(GLUT_BITMAP_8_BY_13, ' ');
    glutBitmapCharacter(GLUT_BITMAP_8_BY_13, ' ');
    glutBitmapCharacter(GLUT_BITMAP_8_BY_13, '|');
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == 13)
    {
        // text_row = 0;
        text_col++;
    }
    else
    {
        text_arr[text_col][text_size[text_col]++] = key;
    }

    // cout << text_col << " " << text_row << "\n";
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 640.0, 0.0, 480.0);
}

void drawThread(int x1, int y1, int x2, int y2)
{
    // glLineWidth(10);

    glBegin(GL_LINE_STRIP);
    glVertex2i(x1, y1);
    glVertex2i(x2, y2);
    glEnd();
}

void drawRectangle(int x1, int y1, int x2, int y2)
{
    glBegin(GL_LINE_LOOP);
    glVertex2i(x1, y1);
    glVertex2i(x1, y2);
    glVertex2i(x2, y2);
    glVertex2i(x2, y1);
    glEnd();
}

void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
    glBegin(GL_LINE_LOOP);
    glVertex2i(x1, y1);
    glVertex2i(x2, y2);
    glVertex2i(x3, y3);
    glEnd();
}

void drawCircle(int x1, int y1, int x2, int y2)
{
    float R = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) / 2; // 半徑
    float x = (x1 + x2) / 2;                                           // 圓心橫坐標
    float y = (y1 + y2) / 2;                                           // 圓心粽坐標
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < n; ++i)
        glVertex2f(x + R * cos(2 * Pi / n * i), y + R * sin(2 * Pi / n * i));
    glEnd();
}

void color_func(int getColor)
{
    switch (getColor)
    {
    case WHITE:
        glColor3f(1.0, 1.0, 1.0);
        break;
    case RED:
        glColor3f(1.0, 0.0, 0.0);
        break;
    case GREEN:
        glColor3f(0.0, 1.0, 0.0);
        break;
    case BLUE:
        glColor3f(0.0, 0.0, 1.0);
        break;

    default:
        break;
    }
}

void myDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLineWidth(width);

    drawtext();
    color_func(color);
    drawCircle(mouse_x - 10, mouse_y - 10, mouse_x + 10, mouse_y + 10);

    if (count1 > 1)
    {
        for (int i = 0; i < count1 - 1; i = i + 2)
        {
            color_func(color_arr[1][i]);
            glLineWidth(width_arr[1][i]);
            drawThread(point1[i].x, point1[i].y, point1[i + 1].x, point1[i + 1].y);
        }
    }
    if (count2 % 3 == 0 || count2 % 3 == 1)
    {
        for (int i = 0; i + 2 <= count2 - 1; i = i + 3)
        {
            color_func(color_arr[2][i]);
            glLineWidth(width_arr[2][i]);
            drawTriangle(point2[i].x, point2[i].y, point2[i + 1].x, point2[i + 1].y, point2[i + 2].x, point2[i + 2].y);
        }
    }
    if (count2 % 3 == 2)
    {
        for (int i = 0; i + 2 <= count2 - 1; i = i + 3)
        {
            color_func(color_arr[2][i]);
            glLineWidth(width_arr[2][i]);
            drawTriangle(point2[i].x, point2[i].y, point2[i + 1].x, point2[i + 1].y, point2[i + 2].x, point2[i + 2].y);
        }
        drawThread(point2[count2 - 2].x, point2[count2 - 2].y, point2[count2 - 1].x, point2[count2 - 1].y);
    }
    if (count3 > 1)
    {
        for (int i = 0; i < count3 - 1; i = i + 2)
        {
            color_func(color_arr[3][i]);
            glLineWidth(width_arr[3][i]);
            drawRectangle(point3[i].x, point3[i].y, point3[i + 1].x, point3[i + 1].y);
        }
    }
    if (count4 > 1)
    {
        for (int i = 0; i < count4 - 1; i = i + 2)
        {
            color_func(color_arr[4][i]);
            glLineWidth(width_arr[4][i]);
            drawCircle(point4[i].x, point4[i].y, point4[i + 1].x, point4[i + 1].y);
        }
    }
    glFlush();
}

void itDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLineWidth(width);

    drawtext();

    for (int i = 0; i <= count1 - 1; i = i + 2)
    {
        color_func(color_arr[1][i]);
        glLineWidth(width_arr[1][i]);
        drawThread(point1[i].x, point1[i].y, point1[i + 1].x, point1[i + 1].y);
    }
    for (int i = 0; i + 2 <= count2; i = i + 3)
    {
        color_func(color_arr[2][i]);
        glLineWidth(width_arr[2][i]);
        drawTriangle(point2[i].x, point2[i].y, point2[i + 1].x, point2[i + 1].y, point2[i + 2].x, point2[i + 2].y);
    }
    if ((count2 + 1) % 3 == 2)
    {
        color_func(color_arr[2][count2]);
        glLineWidth(width_arr[3][count2]);
        drawThread(point2[count2 - 1].x, point2[count2 - 1].y, point2[count2].x, point2[count2].y);
    }
    for (int i = 0; i <= count3 - 1; i = i + 2)
    {
        color_func(color_arr[3][i]);
        glLineWidth(width_arr[3][i]);
        drawRectangle(point3[i].x, point3[i].y, point3[i + 1].x, point3[i + 1].y);
    }
    for (int i = 0; i <= count4 - 1; i = i + 2)
    {
        color_func(color_arr[4][i]);
        glLineWidth(width_arr[4][i]);
        drawCircle(point4[i].x, point4[i].y, point4[i + 1].x, point4[i + 1].y);
    }
    glFlush();
}

void myMouse(int button, int state, int x, int y)
{
    mouse_x = x;
    mouse_y = screenHeight - y;

    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN || state == GLUT_UP)
        {
            switch (shape)
            {
            case LINE:
                point1[count1].x = x;
                point1[count1].y = screenHeight - y;

                color_arr[1][count1] = color;
                width_arr[1][count1] = width;
                count1++;
                myDisplay();
                break;
            case RECTANGLE:
                point3[count3].x = x;
                point3[count3].y = screenHeight - y;

                color_arr[3][count3] = color;
                width_arr[3][count3] = width;
                count3++;
                myDisplay();
                break;
            case CIRCLE:
                point4[count4].x = x;
                point4[count4].y = screenHeight - y;

                color_arr[4][count4] = color;
                width_arr[4][count4] = width;
                count4++;
                myDisplay();
                break;
            }
        }
    }
    if (button == GLUT_LEFT_BUTTON) // TRIANGLE
    {
        if (state == GLUT_DOWN && shape == 2)
        {
            point2[count2].x = x;
            point2[count2].y = screenHeight - y;

            color_arr[2][count2] = color;
            width_arr[2][count2] = width;
            count2++;
            myDisplay();
        }
    }
}

void myMotion(int x, int y)
{
    printf("Press x = %d, y = %d\n", x, screenHeight - y);
    switch (shape)
    {
    case LINE:
        point1[count1].x = x;
        point1[count1].y = screenHeight - y;
        itDisplay();
        break;
    case RECTANGLE:
        point3[count3].x = x;
        point3[count3].y = screenHeight - y;
        itDisplay();
        break;
    case CIRCLE:
        point4[count4].x = x;
        point4[count4].y = screenHeight - y;
        itDisplay();
        break;
    }
}

void myPassiveMotion(int x, int y) // TRIANGLE
{
    point2[count2].x = x;
    point2[count2].y = screenHeight - y;
    printf("x = %d, y = %d\n", x, screenHeight - y);
    itDisplay();
}

void LoadPoints()
{
    FILE *fp;
    fp = fopen("dbtext.txt", "r");
    if (fp == NULL)
    {
        printf("fail to open file\n");
        exit(0);
    }
    fscanf(fp, "%d\n", &text_col);
    for (int i = 0; i <= text_col; i++)
    {
        fscanf(fp, "%d\n", &text_size[i]);
        for (int j = 0; j <= text_size[i]; j++)
            fscanf(fp, "%c", &text_arr[i][j]);
        fscanf(fp, "\n");
    }
    fclose(fp);

    fp = fopen("db1.txt", "r");
    if (fp == NULL)
    {
        printf("fail to open file\n");
        exit(0);
    }
    fscanf(fp, "%d\n", &count1);
    for (int i = 0; i < count1; i++)
    {
        fscanf(fp, "%d %d %d %d\n", &point1[i].x, &point1[i].y, &color_arr[1][i], &width_arr[1][i]);
    }
    fclose(fp);

    fp = fopen("db2.txt", "r");
    if (fp == NULL)
    {
        printf("fail to open file\n");
        exit(0);
    }
    fscanf(fp, "%d\n", &count2);
    for (int i = 0; i < count2; i++)
    {
        fscanf(fp, "%d %d %d %d\n", &point2[i].x, &point2[i].y, &color_arr[2][i], &width_arr[2][i]);
    }
    fclose(fp);

    fp = fopen("db3.txt", "r");
    if (fp == NULL)
    {
        printf("fail to open file\n");
        exit(0);
    }
    fscanf(fp, "%d\n", &count3);
    for (int i = 0; i < count3; i++)
    {
        fscanf(fp, "%d %d %d %d\n", &point3[i].x, &point3[i].y, &color_arr[3][i], &width_arr[3][i]);
    }
    fclose(fp);

    fp = fopen("db4.txt", "r");
    if (fp == NULL)
    {
        printf("fail to open file\n");
        exit(0);
    }
    fscanf(fp, "%d\n", &count4);
    for (int i = 0; i < count4; i++)
    {
        fscanf(fp, "%d %d %d %d\n", &point4[i].x, &point4[i].y, &color_arr[4][i], &width_arr[4][i]);
    }
    fclose(fp);
}

void SavePoints()
{
    FILE *fp;
    fp = fopen("dbtext.txt", "w");
    if (fp == NULL)
    {
        printf("fail to open file\n");
        exit(0);
    }
    fprintf(fp, "%d\n", text_col);
    for (int i = 0; i <= text_col; i++)
    {
        fprintf(fp, "%d\n", text_size[i]);
        for (int j = 0; j <= text_size[i]; j++)
            fprintf(fp, "%c", text_arr[i][j]);
        fprintf(fp, "\n");
    }
    fclose(fp);

    fp = fopen("db1.txt", "w");
    if (fp == NULL)
    {
        printf("fail to open file\n");
        exit(0);
    }
    fprintf(fp, "%d\n", count1);
    for (int i = 0; i < count1; i++)
    {
        fprintf(fp, "%d %d %d %d\n", point1[i].x, point1[i].y, color_arr[1][i], width_arr[1][i]);
    }
    fclose(fp);

    fp = fopen("db2.txt", "w");
    if (fp == NULL)
    {
        printf("fail to open file\n");
        exit(0);
    }
    fprintf(fp, "%d\n", count2);
    for (int i = 0; i < count2; i++)
    {
        fprintf(fp, "%d %d %d %d\n", point2[i].x, point2[i].y, color_arr[2][i], width_arr[2][i]);
    }
    fclose(fp);

    fp = fopen("db3.txt", "w");
    if (fp == NULL)
    {
        printf("fail to open file\n");
        exit(0);
    }
    fprintf(fp, "%d\n", count3);
    for (int i = 0; i < count3; i++)
    {
        fprintf(fp, "%d %d %d %d\n", point3[i].x, point3[i].y, color_arr[3][i], width_arr[3][i]);
    }
    fclose(fp);

    fp = fopen("db4.txt", "w");
    if (fp == NULL)
    {
        printf("fail to open file\n");
        exit(0);
    }
    fprintf(fp, "%d\n", count4);
    for (int i = 0; i < count4; i++)
    {
        fprintf(fp, "%d %d %d %d\n", point4[i].x, point4[i].y, color_arr[4][i], width_arr[4][i]);
    }

    fclose(fp);
}

void colorSubMenu(GLint colorOption)
{
    color = colorOption;
}

void shapeSubMenu(GLint shapeOption)
{
    shape = shapeOption;
}

void widthSubMenu(GLint widthOption)
{
    width = widthOption;
}

void mainMenu(GLint renderingOption)
{
    switch (renderingOption)
    {
    case LOAD:
        count1 = 0;
        count2 = 0;
        count3 = 0;
        count4 = 0;
        text_col = 0;
        memset(text_size, 0, 4000);
        memset(text_arr, 0, 1e+4);

        LoadPoints();
        myDisplay();
        break;
    case SAVE:
        SavePoints();
        break;
    case CLEAR:
        count1 = 0;
        count2 = 0;
        count3 = 0;
        count4 = 0;
        text_col = 0;
        memset(text_size, 0, 4000);
        memset(text_arr, 0, 1e+4);

        myDisplay();
        break;
    }
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("draw system");
    init();

    GLint subMenu1;
    subMenu1 = glutCreateMenu(shapeSubMenu);
    glutAddMenuEntry("line", LINE);
    glutAddMenuEntry("triangle", TRIANGLE);
    glutAddMenuEntry("rectangle", RECTANGLE);
    glutAddMenuEntry("circle", CIRCLE);

    GLint subMenu2;
    subMenu2 = glutCreateMenu(colorSubMenu);
    glutAddMenuEntry("white", WHITE);
    glutAddMenuEntry("red", RED);
    glutAddMenuEntry("green", GREEN);
    glutAddMenuEntry("blue", BLUE);

    GLint subMenu3;
    subMenu3 = glutCreateMenu(widthSubMenu);
    glutAddMenuEntry("1", 1);
    glutAddMenuEntry("2", 2);
    glutAddMenuEntry("3", 3);
    glutAddMenuEntry("4", 4);
    glutAddMenuEntry("5", 5);
    glutAddMenuEntry("6", 6);
    glutAddMenuEntry("7", 7);
    glutAddMenuEntry("8", 8);
    glutAddMenuEntry("9", 9);
    glutAddMenuEntry("10", 10);

    glutCreateMenu(mainMenu);
    glutAddSubMenu("shape", subMenu1);
    glutAddSubMenu("color", subMenu2);
    glutAddSubMenu("width", subMenu3);
    glutAddMenuEntry("load", LOAD);
    glutAddMenuEntry("save", SAVE);
    glutAddMenuEntry("clear", CLEAR);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutMouseFunc(myMouse);
    glutMotionFunc(myMotion);
    glutPassiveMotionFunc(myPassiveMotion);
    glutDisplayFunc(myDisplay);

    glutKeyboardFunc(keyboard);

    glutMainLoop();
}