#include <cmath>
#include <ctime>
#include <iostream>
#include <map>
#include <random>

#include <GL/glut.h>

#define Pi atan(1) * 4

#define floor_x 5
#define floor_z 5

#define set_floor_color 1
#define set_head_color 2
#define set_torso_color 3
#define set_back_arm_color 4
#define set_front_arm_color 5
#define set_thigh_color 6
#define set_calf_color 7

using namespace std;

int screen_width = 500, screen_height = 500;
int mouse_x, mouse_y;
float camera_pos[3] = {6.0, 6.0, 6.0};
float robot_pos[3] = {0.0, 0.0, 0.0};

float walking_thigh_degree;
float walking_calf_degree;
float walking_back_arm_degree;
float walking_front_arm_degree;
float running_body_tilt_degree = 10;
bool isWalking;
bool isRunning;
bool isStuck;
bool isSwinging;
bool isRight;
int running_f;
int f_per_idle = 100;
int frame_counter;

float head_radius;
float torso_length;
float front_arm_length;
float back_arm_length;
float hand_radius;
float thigh_length;
float calf_length;

// float thigh_length = 5;
// float calf_length = 3;

float floor_color[floor_x][floor_z][3];
float head_color[6][3];
float torso_color[6][3];
float front_arm_color[6][3];
float back_arm_color[6][3];
float hand_color[6][3];
float thigh_color[6][3];
float calf_color[6][3];

map<char, int> dic = {{'x', 0}, {'y', 1}, {'z', 2}, {'X', 0}, {'Y', 1}, {'Z', 2}};
float points[8][3] = {
    {0, 0, 0},
    {1, 0, 0},
    {1, 0, 1},
    {0, 0, 1},
    {0, 1, 0},
    {1, 1, 0},
    {1, 1, 1},
    {0, 1, 1},
};
int face[6][4] = {
    {0, 1, 2, 3},
    {4, 5, 6, 7},
    {0, 3, 7, 4},
    {1, 2, 6, 5},
    {0, 1, 5, 4},
    {3, 2, 6, 7},
}; // 6 faces,each consist of 4 index of points

/*************************************************************************************************************************/

void display();
void reshape(int, int);
void keyboard(unsigned char, int, int);
void mouse(int, int, int, int);
void PassiveMotion(int, int);
void idle();

void init();
void set_init_color();
void set_random_color(float, float);
void set_parameter();

void cube(float ***);
void ball(float ***);

void floor();
void robot();

// void head();
// void troso();
// void front_arm();
// void back_arm();
// void hand();
/*************************************************************************************************************************/

void set_parameter()
{
    head_radius = 0.8;
    torso_length = 4;
    front_arm_length = 1.5;
    back_arm_length = 2;
    hand_radius = 0.2;
    thigh_length = 1.5;
    calf_length = 1;

    isStuck = 0;

    if (isRunning)
    {
        walking_thigh_degree = 50;
        walking_calf_degree = 25;
        walking_back_arm_degree = 75;
        walking_front_arm_degree = 15;
        running_f = 2;
    }
    else
    {
        walking_thigh_degree = 25;
        walking_calf_degree = 15;
        walking_back_arm_degree = 30;
        walking_front_arm_degree = 12;
        running_f = 1;
    }
}

void set_random_color(int state)
{
    static default_random_engine e(time(0));
    uniform_real_distribution<float> heavy(0.0, 0.2);
    uniform_real_distribution<float> light(0.8, 1.0);
    uniform_real_distribution<float> u(0.0, 1.0);
    FILE *fp;
    switch (state)
    {
    case set_floor_color:
        fp = fopen("Floor_Color_Data.txt", "w");
        fprintf(fp, "i j (R, G, B)\n\n");
        for (int i = 0; i < floor_x; i++)
            for (int j = 0; j < floor_z; j++)
            {
                for (int k = 0; k < 3; k++)
                    floor_color[i][j][k] = (i + j) % 2 == 0 ? heavy(e) : light(e);
                // floor_color[i][j][k] = u(e);
                fprintf(fp, "%d %d (%.3f, %.3f, %.3f)\n", i, j, floor_color[i][j][0], floor_color[i][j][1], floor_color[i][j][2]);
            }
        fclose(fp);
        break;
    case set_head_color:
        fp = fopen("Head_Color_Data.txt", "w");
        fprintf(fp, "face (R, G, B)\n\n");
        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                head_color[i][j] = u(e);
            }
            fprintf(fp, "%d (%.3f, %.3f, %.3f)\n", i, head_color[i][0], head_color[i][1], head_color[i][2]);
        }
        fclose(fp);
        break;
    case set_torso_color:
        fp = fopen("Torso_Color_Data.txt", "w");
        fprintf(fp, "face (R, G, B)\n\n");
        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                torso_color[i][j] = u(e);
            }
            fprintf(fp, "%d (%.3f, %.3f, %.3f)\n", i, torso_color[i][0], torso_color[i][1], torso_color[i][2]);
        }
        fclose(fp);
        break;
    case set_back_arm_color:
        fp = fopen("Back_arm_Color_Data.txt", "w");
        fprintf(fp, "face (R, G, B)\n\n");
        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                back_arm_color[i][j] = u(e);
            }
            fprintf(fp, "%d (%.3f, %.3f, %.3f)\n", i, back_arm_color[i][0], back_arm_color[i][1], back_arm_color[i][2]);
        }
        fclose(fp);
        break;
    case set_front_arm_color:
        fp = fopen("Front_arm_Color_Data.txt", "w");
        fprintf(fp, "face (R, G, B)\n\n");
        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                front_arm_color[i][j] = u(e);
            }
            fprintf(fp, "%d (%.3f, %.3f, %.3f)\n", i, front_arm_color[i][0], front_arm_color[i][1], front_arm_color[i][2]);
        }
        fclose(fp);
        break;
    case set_thigh_color:
        fp = fopen("Thigh_Color_Data.txt", "w");
        fprintf(fp, "face (R, G, B)\n\n");
        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                thigh_color[i][j] = u(e);
            }
            fprintf(fp, "%d (%.3f, %.3f, %.3f)\n", i, thigh_color[i][0], thigh_color[i][1], thigh_color[i][2]);
        }
        fclose(fp);
        break;
    case set_calf_color:
        fp = fopen("Calf_Color_Data.txt", "w");
        fprintf(fp, "face (R, G, B)\n\n");
        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                calf_color[i][j] = u(e);
            }
            fprintf(fp, "%d (%.3f, %.3f, %.3f)\n", i, calf_color[i][0], calf_color[i][1], calf_color[i][2]);
        }
        fclose(fp);
        break;
    default:
        break;
    }
}

void set_init_color()
{
    set_random_color(set_floor_color);
    set_random_color(set_head_color);
    set_random_color(set_torso_color);
    set_random_color(set_back_arm_color);
    set_random_color(set_front_arm_color);
    set_random_color(set_thigh_color);
    set_random_color(set_calf_color);
}

void ball(float color[6][3])
{
    glPolygonMode(GL_FRONT, GL_FILL);
    glColor3fv(color[0]);
    glutSolidSphere(head_radius, 50, 50);
}

void cube(float color[6][3])
{
    for (int i = 0; i < 6; i++)
    {
        glColor3fv(color[i]);
        glBegin(GL_POLYGON);
        for (int j = 0; j < 4; j++)
            glVertex3fv(points[face[i][j]]);
        glEnd();
    }
}

void robot()
{
    set_parameter();
    float frac = (float)frame_counter / f_per_idle;

    /**********************************************************************************/

    float x_trans = isWalking * isRight * frac * running_f * !isStuck;
    float z_trans = isWalking * !isRight * frac * running_f * !isStuck;
    glTranslatef(robot_pos[0], robot_pos[1], robot_pos[2]);
    glTranslatef(x_trans, 0, z_trans);
    if (isRight)
    {
        glTranslatef(0, 0, 1);
        glRotatef(90, 0, 1, 0);
    }
    if (isWalking && isRunning)
    {
        glRotatef(running_body_tilt_degree, 1, 0, 0);
    }

    /**********************************************************************************/
    // head

    glPushMatrix();
    glTranslatef(0.5, head_radius + torso_length + thigh_length + calf_length, 0.5);
    ball(head_color);
    glPopMatrix();

    /**********************************************************************************/
    // torso

    glPushMatrix();
    glTranslatef(0, thigh_length + calf_length, 0);
    glScalef(1.0, torso_length, 1.0);
    cube(torso_color);
    glPopMatrix();

    /**********************************************************************************/
    // back_arm

    glPushMatrix();
    glTranslatef(1, torso_length / 3 + thigh_length + calf_length, 0.25);
    if (isWalking)
    {
        glTranslatef(0, back_arm_length, 0);
        if (frac <= 0.25)
            glRotatef(walking_back_arm_degree * 4 * frac, 1, 0, 0);
        else if (frac > 0.25 && frac <= 0.5)
            glRotatef(walking_back_arm_degree * 4 * (0.5 - frac), 1, 0, 0);
        else if (frac > 0.5 && frac <= 0.75)
            glRotatef(-walking_back_arm_degree * 4 * (frac - 0.5), 1, 0, 0);
        else
            glRotatef(-walking_back_arm_degree * 4 * (1 - frac), 1, 0, 0);
        glTranslatef(0, -back_arm_length, 0);
    }
    glScalef(0.5, back_arm_length, 0.5);
    cube(back_arm_color);
    glPopMatrix();
    /**************************/
    glPushMatrix();
    glTranslatef(-0.5, torso_length / 3 + thigh_length + calf_length, 0.25);
    if (isWalking)
    {
        glTranslatef(0, back_arm_length, 0);
        if (frac <= 0.25)
            glRotatef(-walking_back_arm_degree * 4 * frac, 1, 0, 0);
        else if (frac > 0.25 && frac <= 0.5)
            glRotatef(-walking_back_arm_degree * 4 * (0.5 - frac), 1, 0, 0);
        else if (frac > 0.5 && frac <= 0.75)
            glRotatef(walking_back_arm_degree * 4 * (frac - 0.5), 1, 0, 0);
        else
            glRotatef(walking_back_arm_degree * 4 * (1 - frac), 1, 0, 0);
        glTranslatef(0, -back_arm_length, 0);
    }
    glScalef(0.5, back_arm_length, 0.5);
    cube(back_arm_color);
    glPopMatrix();

    /**********************************************************************************/
    // front_arm

    glPushMatrix();
    glTranslatef(1, torso_length / 3 + thigh_length + calf_length - front_arm_length, 0.25 + 0.125);
    // glTranslatef(0, 0, front_arm_length / 2 - 0.25);

    if (isWalking)
    {
        glTranslatef(0, front_arm_length + back_arm_length, 0);
        if (frac <= 0.25)
            glRotatef(walking_back_arm_degree * 4 * frac, 1, 0, 0);
        else if (frac > 0.25 && frac <= 0.5)
            glRotatef(walking_back_arm_degree * 4 * (0.5 - frac), 1, 0, 0);
        else if (frac > 0.5 && frac <= 0.75)
            glRotatef(-walking_back_arm_degree * 4 * (frac - 0.5), 1, 0, 0);
        else
            glRotatef(-walking_back_arm_degree * 4 * (1 - frac), 1, 0, 0);
        glTranslatef(0, -front_arm_length - back_arm_length, 0);

        glTranslatef(0, front_arm_length, 0);
        if (frac <= 0.25)
            glRotatef(-walking_front_arm_degree * 4 * frac, 1, 0, 0);
        else if (frac > 0.25 && frac <= 0.5)
            glRotatef(-walking_front_arm_degree * 4 * (0.5 - frac), 1, 0, 0);
        else if (frac > 0.5 && frac <= 0.75)
            glRotatef(walking_front_arm_degree * 4 * (frac - 0.5), 1, 0, 0);
        else
            glRotatef(walking_front_arm_degree * 4 * (1 - frac), 1, 0, 0);
        glTranslatef(0, -front_arm_length, 0);
    }
    glScalef(0.3, front_arm_length, 0.3);
    cube(front_arm_color);
    glPopMatrix();
    /**************************/
    glPushMatrix();
    glTranslatef(-0.3, torso_length / 3 + thigh_length + calf_length - front_arm_length, 0.25 + 0.125);
    // glTranslatef(0, 0, front_arm_length / 2 - 0.25);

    if (isWalking)
    {
        glTranslatef(0, front_arm_length + back_arm_length, 0);
        if (frac <= 0.25)
            glRotatef(-walking_back_arm_degree * 4 * frac, 1, 0, 0);
        else if (frac > 0.25 && frac <= 0.5)
            glRotatef(-walking_back_arm_degree * 4 * (0.5 - frac), 1, 0, 0);
        else if (frac > 0.5 && frac <= 0.75)
            glRotatef(walking_back_arm_degree * 4 * (frac - 0.5), 1, 0, 0);
        else
            glRotatef(walking_back_arm_degree * 4 * (1 - frac), 1, 0, 0);
        glTranslatef(0, -front_arm_length - back_arm_length, 0);

        glTranslatef(0, front_arm_length, 0);
        if (frac <= 0.25)
            glRotatef(walking_front_arm_degree * 4 * frac, 1, 0, 0);
        else if (frac > 0.25 && frac <= 0.5)
            glRotatef(walking_front_arm_degree * 4 * (0.5 - frac), 1, 0, 0);
        else if (frac > 0.5 && frac <= 0.75)
            glRotatef(-walking_front_arm_degree * 4 * (frac - 0.5), 1, 0, 0);
        else
            glRotatef(-walking_front_arm_degree * 4 * (1 - frac), 1, 0, 0);
        glTranslatef(0, -front_arm_length, 0);
    }
    glScalef(0.3, front_arm_length, 0.3);
    cube(front_arm_color);
    glPopMatrix();

    /**********************************************************************************/
    // thigh

    glPushMatrix();
    glTranslatef(0, calf_length, 0.6);
    if (isWalking)
    {
        glTranslatef(0, thigh_length, 0);
        if (frac <= 0.25)
            glRotatef(walking_thigh_degree * 4 * frac, 1, 0, 0);
        else if (frac > 0.25 && frac <= 0.5)
            glRotatef(walking_thigh_degree * 4 * (0.5 - frac), 1, 0, 0);
        else if (frac > 0.5 && frac <= 0.75)
            glRotatef(-walking_thigh_degree * 4 * (frac - 0.5), 1, 0, 0);
        else
            glRotatef(-walking_thigh_degree * 4 * (1 - frac), 1, 0, 0);
        glTranslatef(0, -thigh_length, 0);
    }
    glScalef(0.4, thigh_length, 0.4);
    cube(thigh_color);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.6, calf_length, 0.6);
    if (isWalking)
    {
        glTranslatef(0, thigh_length, 0);
        if (frac <= 0.25)
            glRotatef(-walking_thigh_degree * 4 * frac, 1, 0, 0);
        else if (frac > 0.25 && frac <= 0.5)
            glRotatef(-walking_thigh_degree * 4 * (0.5 - frac), 1, 0, 0);
        else if (frac > 0.5 && frac <= 0.75)
            glRotatef(walking_thigh_degree * 4 * (frac - 0.5), 1, 0, 0);
        else
            glRotatef(walking_thigh_degree * 4 * (1 - frac), 1, 0, 0);
        glTranslatef(0, -thigh_length, 0);
    }
    glScalef(0.4, thigh_length, 0.4);
    cube(thigh_color);
    glPopMatrix();

    /**********************************************************************************/
    // calf

    glPushMatrix();
    glTranslatef(0.05, 0, 0.65);
    if (isWalking)
    {
        glTranslatef(0, thigh_length + calf_length, 0);
        if (frac <= 0.25)
            glRotatef(walking_thigh_degree * 4 * frac, 1, 0, 0);
        else if (frac > 0.25 && frac <= 0.5)
            glRotatef(walking_thigh_degree * 4 * (0.5 - frac), 1, 0, 0);
        else if (frac > 0.5 && frac <= 0.75)
            glRotatef(-walking_thigh_degree * 4 * (frac - 0.5), 1, 0, 0);
        else
            glRotatef(-walking_thigh_degree * 4 * (1 - frac), 1, 0, 0);
        glTranslatef(0, -thigh_length - calf_length, 0);
    }
    if (isWalking && frac > 0.5 && frac <= 0.75)
    {
        glTranslatef(0, 0, -calf_length * sin(walking_calf_degree * 4 * (frac - 0.5) * Pi / 180));
        glRotatef(walking_calf_degree * 4 * (frac - 0.5), 1, 0, 0);
    }
    glScalef(0.3, calf_length, 0.3);
    cube(calf_color);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.65, 0, 0.65);
    if (isWalking)
    {
        glTranslatef(0, thigh_length + calf_length, 0);

        if (frac <= 0.25)
            glRotatef(-walking_thigh_degree * 4 * frac, 1, 0, 0);
        else if (frac > 0.25 && frac <= 0.5)
            glRotatef(-walking_thigh_degree * 4 * (0.5 - frac), 1, 0, 0);
        else if (frac > 0.5 && frac <= 0.75)
            glRotatef(walking_thigh_degree * 4 * (frac - 0.5), 1, 0, 0);
        else
            glRotatef(walking_thigh_degree * 4 * (1 - frac), 1, 0, 0);
        glTranslatef(0, -thigh_length - calf_length, 0);
    }
    if (isWalking && frac <= 0.25)
    {
        glTranslatef(0, 0, -calf_length * sin(walking_calf_degree * 4 * frac * Pi / 180));
        glRotatef(walking_calf_degree * 4 * frac, 1, 0, 0);
    }

    glScalef(0.3, calf_length, 0.3);
    cube(calf_color);
    glPopMatrix();

    /**********************************************************************************/
}

void keyboard(unsigned char key, int x, int y)
{
    if (dic.find(key) != dic.end())
    {
        int mod = glutGetModifiers();
        camera_pos[dic[key]] += 0.25 * (mod == GLUT_ACTIVE_ALT ? -1 : 1);
    }

    switch (key)
    {
    case 'r':
    case 'R':
    case 'l':
    case 'L':
        isWalking = 1;
        isRight = key == 'r' || key == 'R';
        break;
    case 's':
    case 'S':
        isSwinging = 1;
        break;
    case '0':
        robot_pos[0] = robot_pos[1] = robot_pos[2] = 0;
        break;
    case '1':
        isRunning = !isRunning;

    default:
        break;
    }
}

void idle()
{
    if (isWalking || isSwinging)
        frame_counter++;
    if (frame_counter >= f_per_idle)
    {
        isStuck = (isWalking & isRight & robot_pos[0] + running_f >= floor_x) | (isWalking & !isRight & robot_pos[2] + running_f >= floor_z);
        if (isWalking)
        {
            if (isRight && !isStuck)
                robot_pos[0] += isRunning ? 2 : 1;
            else if (!isRight && !isStuck)
                robot_pos[2] += isRunning ? 2 : 1;
        }

        frame_counter = 0;
        isWalking = isSwinging = 0;
    }

    display();
}

void floor()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    for (int i = 0; i < floor_x; i++)
        for (int j = 0; j < floor_z; j++)
        {
            glColor3fv(floor_color[i][j]);
            glBegin(GL_POLYGON);
            glVertex3f(i, 0.0, j);
            glVertex3f(i, 0.0, j + 1);
            glVertex3f(i + 1, 0.0, j + 1);
            glVertex3f(i + 1, 0.0, j);
            glEnd();
        }
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, 499, 499);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-8.0, 8.0, -8.0, 8.0, 0.0, 20.0);

    glEnable(GL_DEPTH_TEST);

    glFlush();
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (w > h)
        glOrtho(-10.0, 10.0, -10.0 * (float)h / (float)w, 10.0 * (float)h / (float)w, 0.50, 30.0);
    else
        glOrtho(-10.0 * (float)w / (float)h, 10.0 * (float)w / (float)h, -10.0, 10.0, 0.50, 30.0);
    screen_width = w;
    screen_height = h;
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // gluLookAt(6.0, 6.0, 6.0, 3.0, 0.0, 3.0, 0.0, 1.0, 0.0);
    gluLookAt(camera_pos[0], camera_pos[1], camera_pos[2], robot_pos[0] + 0.0, robot_pos[1] + 0.0, robot_pos[2] + 0.0, 0.0, 1.0, 0.0);

    floor();
    robot();
    glutSwapBuffers();
}

void mouse(int button, int state, int x, int y)
{
}

void PassiveMotion(int x, int y)
{
    mouse_x = x;
    mouse_y = screen_height - y;
    // printf("x = %d, y = %d\n", mouse_x, mouse_y);
    //  printf("%f %f %f\n", camera_pos[0], camera_pos[1], camera_pos[2]);
    // printf("%d\n", isStuck ? 1 : 0);
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(screen_width, screen_height);
    glutCreateWindow("robot");

    init();
    set_init_color();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutPassiveMotionFunc(PassiveMotion);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}
