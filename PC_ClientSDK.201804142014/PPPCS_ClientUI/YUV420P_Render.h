#ifndef YUV420P_RENDER_H
#define YUV420P_RENDER_H

#include <QObject>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
class YUV420P_Render: protected QOpenGLFunctions
{
public:
    using Ptr = std::shared_ptr<YUV420P_Render>;
    YUV420P_Render(QObject* pParent);
    ~YUV420P_Render();

    //初始化gl
    void initialize();
    void destory();
    //刷新显示
    void render(uchar* py,uchar* pu,uchar* pv,int width,int height,int type);
    void render(uchar* ptr,int width,int height,int type);
    void renderBorder(int nWidth);
    void renderBK();
protected:
private:
    GLuint textureUniformY; //y纹理数据位置
    GLuint textureUniformU; //u纹理数据位置
    GLuint textureUniformV; //v纹理数据位置
    GLuint id_y; //y纹理对象ID
    GLuint id_u; //u纹理对象ID
    GLuint id_v; //v纹理对象ID
    QOpenGLTexture* m_pTextureY;  //y纹理对象
    QOpenGLTexture* m_pTextureU;  //u纹理对象
    QOpenGLTexture* m_pTextureV;  //v纹理对象
    QOpenGLTexture* m_pTextureBorder;  //边框对象
    QOpenGLShader *m_pVSHader;  //顶点着色器程序对象
    QOpenGLShader *m_pFSHader;  //片段着色器对象
    QOpenGLShaderProgram *m_pShaderProgram; //着色器程序容器
    QObject* m_pParent = nullptr;
};

#endif // YUV420P_RENDER_H