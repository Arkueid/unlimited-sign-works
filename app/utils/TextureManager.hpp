#pragma once

#include <QObject>
#include <QMap>
#include <QOpenGLTexture>

class TextureSignals : public QObject
{
    Q_OBJECT

    friend class TextureManager;

    TextureSignals() = default;;

signals:
    void texturesUpdated();
};

class TextureManager
{
    static TextureSignals *texturesSignals;

public:
    static void loadTexturesPaths();
    static QOpenGLTexture *loadTexture(const QString &path);
    static QOpenGLTexture *getTexture(int ref);
    static const QMap<int, QString> &getTexturePaths();
    static void releaseTextures();

    // 数据库操作

    /// @brief 在纹理库中注册纹理
    /// @param externalPath 外部纹理路径，会复制到 ../../assets/textures/ 下
    /// @return
    static int registerTexture(const QString &externalPath, const QString &textureName);

    /// @brief 注销纹理
    /// @param textureName
    static bool unregisterTexture(const QString &textureName);

    static TextureSignals *getTexturesSignals()
    {
        return texturesSignals;
    }

    static void initSignals();

    static void destroySignals();
};