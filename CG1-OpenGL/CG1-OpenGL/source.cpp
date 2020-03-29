// ʹ�õĿ��ļ�Ϊfreeglut-3.2.1��Ϊglut����ȫ����汾
// �ο�API��https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/
// http://freeglut.sourceforge.net/docs/api.php
// https://www.opengl.org/resources/libraries/glut/spec3/spec3.html
// ��Դ������������freeglutΪRelease�汾����Ҫ���Ԥ����������
#define NDEBUG

// std_image.h��һ����ͷ�ļ�ͼ����ؿ⣬���ܹ����ش󲿷����е��ļ���ʽ�������������
// ���ص�ַ https://github.com/nothings/stb/blob/master/stb_image.h
// ͨ������STB_IMAGE_IMPLEMENTATION��Ԥ���������޸�ͷ�ļ�������ֻ������صĺ�������Դ��
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <gl/glut.h>
#include <iostream>
#include <windows.h>

#define MAX_CHAR 128

using namespace std;   

// ��ת�Ƕ�
float rotateX = 20.0;
float rotateY = 20.0;
float rotateZ = 0;
// ƽ������
float translateX = 0;
float translateY = 0;
float translateZ = 0;
// ���ű���
float ratio = 1;

// 8����������
GLfloat vertex_list[][3] = {
    -0.5f, -0.5f, 0.5f, //��ǰ��
    -0.5f, 0.5f, 0.5f,  //��ǰ��
    -0.5f, -0.5f, -0.5f,//�����
    -0.5f, 0.5f, -0.5f, //�����
    0.5f, -0.5f, 0.5f,  //��ǰ��
    0.5f, 0.5f, 0.5f,   //��ǰ��
    0.5f, -0.5f, -0.5f, //�Һ���
    0.5f, 0.5f, -0.5f,  //�Һ���
};

GLint index_list[][4] = {
    0, 1, 5, 4, //ǰ
    0, 1, 3, 2, //��
    4, 5, 7, 6, //��
    2, 3, 7, 6, //��
    1, 3, 7, 5, //��
    2, 6, 4, 0, //��
};

GLfloat texture_coord[][2] = {
    0.0, 1.0,
    0.0, 0.0,
    1.0, 0.0,
    1.0, 1.0,
};

bool useColor = false;
 GLfloat colors[][3] = { 
     0.0, 0.0, 1.0 , 
     0.0, 1.0, 0.0 , 
     1.0, 0.0, 0.0 ,
     1.0, 0.0, 1.0 ,  
     1.0, 1.0, 0.0 , 
     0.0, 1.0, 1.0 ,
 };

unsigned int texture;

void generateTexture() {
    glGenTextures(1, &texture);
    // ���������
    glBindTexture(GL_TEXTURE_2D, texture);
    // ���û��Ʒ�ʽ���ظ�
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // ����Ŵ���Сʱʹ�����Թ���
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // ���ز���������
    int width, height, nrChannels;
    unsigned char* data = stbi_load("sq.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {      
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
        cout << "Load texture complete" << endl;
    }
    else
    {
        cout << "Failed to load texture" << endl;
    }
    stbi_image_free(data);
}


void drawString(const char* str) {
    static int isFirstCall = 1;
    static GLuint lists;

    // ��ʼ����ʾ�б�
    if (isFirstCall) { 
        isFirstCall = 0;
        // ����MAX_CHAR����������ʾ�б���
        lists = glGenLists(MAX_CHAR);
        // ��ÿ���ַ��Ļ������װ����Ӧ����ʾ�б���
        wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
    }
    // ����ÿ���ַ���Ӧ����ʾ�б�����ÿ���ַ�
    for (; *str != '\0'; ++str)
        glCallList(lists + *str);
}

void display() {
    glClearColor(0.3, 0.4, 0.9, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(translateX, translateY, translateZ);

    glRotatef(rotateX, 1.0, 0.0, 0.0);
    glRotatef(rotateY, 0.0, 1.0, 0.0);
    glRotatef(rotateZ, 0.0, 0.0, 1.0);
    
    glScalef(ratio, ratio, ratio);

    int i, j;
    // �����ı���
    glBegin(GL_QUADS); 
    for (i = 0; i < 6; ++i)
    {
        if (useColor)
            glColor3f(colors[i][0], colors[i][1], colors[i][2]);
        else
            glColor3f(1.0, 1.0, 1.0);
        for (j = 0; j < 4; ++j)
        {
            if (!useColor)
                glTexCoord2f(texture_coord[j][0], texture_coord[j][1]);
            glVertex3fv(vertex_list[index_list[i][j]]);
        }
    }
    glEnd();

    glutSwapBuffers();
}

void helpDisplay() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(0.0f, 0.0f, 0.0f);

    glRasterPos2f(-0.9f, 0.7f);
    drawString("ROTATE:  x-axis ( I / K )");
    glRasterPos2f(-0.9f, 0.5f);
    drawString("                y-axis ( J / L )");
    glRasterPos2f(-0.9f, 0.3f);
    drawString("                z-axis ( N / M )");

    glRasterPos2f(-0.9f, 0.0f);
    drawString("MOVE:   x-axis ( A / D )");
    glRasterPos2f(-0.9f, -0.2f);
    drawString("                y-axis ( W / S )");
    glRasterPos2f(-0.9f, -0.4f);
    drawString("                z-axis ( Z / X )");

    glRasterPos2f(-0.9f, -0.7f);
    drawString("Scale:   up ( Q )");
    glRasterPos2f(-0.9f, -0.9f);
    drawString("                down ( E )");
    
    glutSwapBuffers();
}

// �����ڱ�����ʱ������ͼ�β�������
void reshape(int width, int height) {
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // �ݺ���봰��һ��
    gluPerspective(45, 1.0 * width/height, 1, 1000);
    // eye(0,0,3),center(0,0,0),up(0,1,0)
    gluLookAt(0, 0, 3, 0, 0, 0, 0, 1, 0);
}

// ASCII�ַ��ص�
void normalKeys(unsigned char key, int x, int y) {
    switch (key)
    {
    case 'w':
        translateY += 0.03;
        break;
    case 'a':
        translateX -= 0.03;
        break;
    case 's':
        translateY -= 0.03;
        break;
    case 'd':
        translateX += 0.03;
        break;
    case 'z':
        translateZ -= 0.03;
        break;
    case 'x':
        translateZ += 0.03;
        break;
    case 'q':
        ratio *= 1.05;
        break;
    case 'e':
        ratio /= 1.05;
        break;
    case 'i':
        rotateX -= 5.0f;
        if (rotateX < 0.0f)  rotateX = 355.0f;
        break;
    case 'k':
        rotateX += 5.0f;
        if (rotateX > 355.0f)  rotateX = 0.0f;
        break;
    case 'j':
        rotateY -= 5.0f;
        if (rotateY < 0.0f)  rotateY = 355.0f;
        break;
    case 'l':
        rotateY += 5.0f;
        if (rotateY > 355.0f)  rotateY = 0.0f;
        break;
    case 'n':
        rotateZ -= 5.0f;
        if (rotateZ < 0.0f)  rotateZ = 355.0f;
        break;
    case 'm':
        rotateZ += 5.0f;
        if (rotateZ > 355.0f)  rotateZ = 0.0f;
        break;
    default:
        break;
    }

    glutPostRedisplay();
}

int mainWindow, helpWindow;

void menuProcess(int option) {
    switch (option) {
    case 1:
        rotateX = rotateY = 20.0;
        translateX = translateY = translateZ = rotateZ = 0.0;
        ratio = 1;
        glutPostRedisplay();
        break;
    case 2:
        useColor = false;
        glEnable(GL_TEXTURE_2D);
        glutPostRedisplay();
        break;
    case 3:
        useColor = true;
        glDisable(GL_TEXTURE_2D);
        glutPostRedisplay();
        break;
    case 4:
        glutSetWindow(helpWindow);
        glutShowWindow();
        glutSetWindow(mainWindow);
        break;
    case 5:
        glutSetWindow(helpWindow);
        glutHideWindow();
        glutSetWindow(mainWindow);
        break;
    }
}

// �����˵�
void createMenu() {
    int submenu = glutCreateMenu(menuProcess);
    glutAddMenuEntry(" image cube",2);
    glutAddMenuEntry(" color cube", 3);
    int menu = glutCreateMenu(menuProcess);
    glutAddMenuEntry(" Reset ", 1);
    glutAddSubMenu(" Change Cube ", submenu);
    glutAddMenuEntry(" open help window ", 4);
    glutAddMenuEntry(" close help window ", 5);
    // ���Ҽ��򿪲˵�
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}


int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(800, 800);
    mainWindow = glutCreateWindow("OpenGL 1");
    
    // ������ȼ��
    glEnable(GL_DEPTH_TEST);
    // �󶨻ص�����
    glutDisplayFunc(display);
    glutKeyboardFunc(normalKeys);
    glutReshapeFunc(reshape);
    // ����������ͼ
    glEnable(GL_TEXTURE_2D);
    generateTexture();
    // �����˵�
    createMenu();

    // ���������Ӵ���
    helpWindow = glutCreateSubWindow(mainWindow, 0, 0, 200, 200);
    glutDisplayFunc(helpDisplay);
    
    glutMainLoop();
    return 0;
}