#pragma once
#include <QString>
#include <algorithm>
#include <QVector>

class orQStrLine
{
public:
    static QString clean(QString str);//去掉\0后的字符
	static QString Trim(QString linestr);
    static bool isCommentLine(QString linestr);
    static int AnayStringLine(QString linestr, QVector<QString>& word);
};

