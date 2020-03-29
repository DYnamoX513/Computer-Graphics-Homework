// 使用的库文件为freeglut-3.2.1，为glut的完全替代版本
// 参考API：https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/
// http://freeglut.sourceforge.net/docs/api.php
// https://www.opengl.org/resources/libraries/glut/spec3/spec3.html
// 从源代码编译而来的freeglut为Release版本，需要添加预处理器定义
#define NDEBUG

// std_image.h是一个单头文件图像加载库，它能够加载大部分流行的文件格式。用于添加纹理
// 下载地址 https://github.com/nothings/stb/blob/master/stb_image.h
// 通过定义STB_IMAGE_IMPLEMENTATION，预处理器会修改头文件，让其只包含相关的函数定义源码
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <gl/glut.h>
#include <iostream>
#include <windows.h>

#define MAX_CHAR 128

using namespace std;   

// 旋转角度
float rotateX = 20.0;
float rotateY = 20.0;
float rotateZ = 0;
// 平移向量
float translateX = 0;
float translateY = 0;
float translateZ = 0;
// 缩放比例
float ratio = 1;

// 8个顶点坐标
GLfloat vertex_list[][3] = {
    -0.5f, -0.5f, 0.5f, //左前下
    -0.5f, 0.5f, 0.5f,  //左前上
    -0.5f, -0.5f, -0.5f,//左后下
    -0.5f, 0.5f, -0.5f, //左后上
    0.5f, -0.5f, 0.5f,  //右前下
    0.5f, 0.5f, 0.5f,   //右前上
    0.5f, -0.5f, -0.5f, //右后下
    0.5f, 0.5f, -0.5f,  //右后上
};

GLint index_list[][4] = {
    0, 1, 5, 4, //前
    0, 1, 3, 2, //左
    4, 5, 7, 6, //右
    2, 3, 7, 6, //后
    1, 3, 7, 5, //上
    2, 6, 4, 0, //下
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
    // 绑定纹理对象
    glBindTexture(GL_TEXTURE_2D, texture);
    // 设置环绕方式：重复
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // 纹理放大缩小时使用线性过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 加载并生成纹理
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

    // 初始化显示列表
    if (isFirstCall) { 
        isFirstCall = 0;
        // 申请MAX_CHAR个连续的显示列表编号
        lists = glGenLists(MAX_CHAR);
        // 把每个字符的绘制命令都装到对应的显示列表中
        wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
    }
    // 调用每个字符对应的显示列表，绘制每个字符
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
    // 绘制四边形
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

// 当窗口被拉伸时，保持图形不被拉伸
void reshape(int width, int height) {
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // 纵横比与窗口一致
    gluPerspective(45, 1.0 * width/height, 1, 1000);
    // eye(0,0,3),center(0,0,0),up(0,1,0)
    gluLookAt(0, 0, 3, 0, 0, 0, 0, 1, 0);
}

// ASCII字符回调
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

// 创建菜单
void createMenu() {
    int submenu = glutCreateMenu(menuProcess);
    glutAddMenuEntry(" image cube",2);
    glutAddMenuEntry(" color cube", 3);
    int menu = glutCreateMenu(menuProcess);
    glutAddMenuEntry(" Reset ", 1);
    glutAddSubMenu(" Change Cube ", submenu);
    glutAddMenuEntry(" open help window ", 4);
    glutAddMenuEntry(" close help window ", 5);
    // 绑定右键打开菜单
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}


int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(800, 800);
    mainWindow = glutCreateWindow("OpenGL 1");
    
    // 启用深度检测
    glEnable(GL_DEPTH_TEST);
    // 绑定回调函数
    glutDisplayFunc(display);
    glutKeyboardFunc(normalKeys);
    glutReshapeFunc(reshape);
    // 启用纹理贴图
    glEnable(GL_TEXTURE_2D);
    generateTexture();
    // 创建菜单
    createMenu();

    // 创建帮助子窗口
    helpWindow = glutCreateSubWindow(mainWindow, 0, 0, 200, 200);
    glutDisplayFunc(helpDisplay);
    
    glutMainLoop();
    return 0;
}