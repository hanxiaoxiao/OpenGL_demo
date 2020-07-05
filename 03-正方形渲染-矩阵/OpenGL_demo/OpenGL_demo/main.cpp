//
//  main.cpp
//  OpenGL_demo
//
//  Created by han xiao on 2020/6/30.
//  Copyright © 2020 han xiao. All rights reserved.
//

#include "GLShaderManager.h"

#include "GLTools.h"
#include <GLUT/GLUT.h>


//定义一个着色器
GLShaderManager shaderManager;

//简单的批次容器，是GLTools的一个简单的容器类
GLBatch triangleBatch;


//blockSize大小
GLfloat blockSize = 0.1f;

//正方形的4个点坐标
GLfloat vVerts[] = {
    -blockSize,-blockSize,0.0f,
    blockSize,-blockSize,0.0f,
    blockSize,blockSize,0.0f,
    -blockSize,blockSize,0.0f
};

GLfloat xPos = 0.0f;
GLfloat yPos = 0.0f;
//窗口大小改变时接受新的宽度和高度，其中0,0代表窗口中视口的左下角坐标，w,h代表像素
void ChangeSize(int w,int h)
{
    glViewport(0,0, w, h);
}

//开始渲染
void RenderScene(void){
    //清除一个或一组特定的缓冲区
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    //设置一组浮点数k来表示红色
    GLfloat vRed[] = {1.0f,0.0f,0.0f,1.0f};
    
    //通过矩阵方式进行渲染
    M3DMatrix44f mTransfromMatrix;
    
    //平移
    m3dTranslationMatrix44(mTransfromMatrix, xPos, yPos, 0.0f);
    
    //将矩阵结果 提交给固定着色器（平面着色器）中绘制
    shaderManager.UseStockShader(GLT_SHADER_FLAT,mTransfromMatrix,vRed);

    triangleBatch.Draw();
    
    //将在后台缓冲区进行渲染，然后在结束时交换到前台
    glutSwapBuffers();
    
}

//为程序作一次性的设置
void SetupRC(){
    //设置背景颜色
    glClearColor(0.98f, 0.40f, 0.7f, 1.0f);
    //初始化着色管理器
    shaderManager.InitializeStockShaders();

    //批次处理
    triangleBatch.Begin(GL_TRIANGLE_FAN, 4);
    triangleBatch.CopyVertexData3f(vVerts);
    triangleBatch.End();
}
void SpecialKeys(int key,int x,int y){
    GLfloat stepSize = 0.025f;
    
    if (key == GLUT_KEY_UP) {
        yPos += stepSize;
    }
    if (key == GLUT_KEY_DOWN) {
        yPos -= stepSize;
    }
    if (key == GLUT_KEY_LEFT) {
        xPos -= stepSize;
    }
    if (key == GLUT_KEY_RIGHT) {
        xPos += stepSize;
    }
    
    //碰撞检测
    if (xPos < (-1.0f + blockSize)) {
        
        xPos = -1.0f + blockSize;
    }
    
    if (xPos > (1.0f - blockSize)) {
        xPos = 1.0f - blockSize;
    }
    
    if (yPos < (-1.0f + blockSize)) {
        yPos = -1.0f + blockSize;
    }
    
    if (yPos > (1.0f - blockSize)) {
        yPos = 1.0f - blockSize;
    }
   
    glutPostRedisplay();
}


int main(int argc,char* argv[]){
    //设置当前工作目录，针对MAC OS X
    gltSetWorkingDirectory(argv[0]);
    //初始化GLUT库
    glutInit(&argc, argv);
    //初始化双缓冲窗口，其中标志GLUT_DOUBLE、GLUT_RGBA、GLUT_DEPTH、GLUT_STENCIL分别指双缓冲窗口、RGBA颜色模式、深度测试、模版缓冲区
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
    
    //GLUT窗口大小，标题窗口
    glutInitWindowSize(800, 600);
    glutCreateWindow("Triangle");
    //注册回调函数
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    
    //注册特殊函数
    glutSpecialFunc(SpecialKeys);
    
    //驱动程序初始化中没有出现任何问题
    GLenum err = glewInit();
    if (GLEW_OK !=err) {
        fprintf(stderr, "glew error:%s\n",glewGetErrorString(err));
        return 1;
    }
    //调用SetupRC
    SetupRC();
    
    glutMainLoop();
    return 0;
}
