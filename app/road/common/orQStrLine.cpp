#include "orQStrLine.h"

//去掉\0后的字符
QString orQStrLine::clean(QString str)
{
    int size = str.size();
    int pos = size;
    for (int i = 0; i < size; i++)
        if (str[i] == '\0')
        {
            pos = i;
            break;
        }

    return str.left(pos);
}

QString orQStrLine::Trim(QString linestr)
{
    return linestr.trimmed();

	if (linestr.isEmpty()) return linestr;

	QString s = linestr;
	int istart = 0;
	for (int i = 0; i< linestr.length(); i++)
	{
		if (s[i] != ' ')
		{
			istart = i;
			break;
		}
	}

	if (istart = linestr.length() - 1) return "";//都是空格

	int len = linestr.length();
	int iend = len - 1;
	for (int i = 0; i < len; i++)
	{
		if (s[len - 1 - i] != ' ')
		{
			iend = i;
			break;
		}
	}
	QString substr = s.mid(istart, iend - istart);

	return substr;
}

bool orQStrLine::isCommentLine(QString linestr)
{
	if (linestr.length() < 2) return false;

    if (linestr[0] == '-' && linestr[1] == '-') return true;

	return false;
 }

//空串 -1
//注释 0
//1
int orQStrLine::AnayStringLine(QString linestr, QVector<QString>& words)
{
    if (linestr == "\r") return 0;

    //int index = linestr.indexOf('\r');

    //while (linestr.find("\r") != std::string::npos)
    //{
    //    linestr = linestr.replace(linestr.find("\r"), 1, " ");
    //}

    QString str = Trim(linestr);
    //tab替换为空格
    for (int i = 0; i < str.length(); i++)
        // while (str.find("\t") != std::string::npos)
    {
        if (str[i] == '\t') str[i] = ' ';
        // str = str.replace(str.find("\t"), 1, " ");
    }
    //空串
    if (str.isEmpty()) return 0;
    //注释行
    if (isCommentLine(str)) return -2;
    /*if (str.length() >= 2)
        if (str.substr(0, 1) == "-" && str.substr(1, 1) == "-") return -2;*/

        //去掉最后一个/后面的字符串
      //  int lastP = (int)(str.find_last_of("/"));
     //   if (lastP >= 0) str = str.substr(0, lastP + 1);
    int lastP = str.lastIndexOf("/");

    if (lastP != -1)
    {
        str = str.left(lastP + 1);
    }
    //提取单词
    words.clear();

    QString strword = "";
    bool bQuote = false;
    while (str.length() > 0)
    {
        QString substr = str.left(1);
        if (substr == " ")//空格
        {
            if (!bQuote)//非引号内的空格
            {
                if (strword.length() > 0)//一个单词结束
                {
                    words.push_back(strword);
                    strword = "";
                }
                //  str.erase(0, 1);//删除空格
            }
            else strword += substr;
            str.erase(str.begin());//删除空格
        }
        else if (substr == "'")//
        {
            if (!bQuote) bQuote = true;//字符串开始
            else//字符串结束
            {
                words.push_back(strword);
                strword = "";
                bQuote = false;
            }
            //str.erase(0, 1);//删除'
            str.erase(str.begin());
        }
        else if (substr == "/")//
        {
            if (strword.length() > 0)  words.push_back(strword);
            words.push_back("/");
            //str.erase(0, 1);//删除/
            str.erase(str.begin());
            if (str.length() == 0) return 1;//行结束

           // int sindex1 = (int)(str.find_first_of("/"));
            int sindex1 = str.indexOf("/");
            if (sindex1 == -1)
            {
                words.push_back(str);
                strword = "";
            }
        }
        else if (substr == "-" && !bQuote)//判断后面是非-
        {
            if (str.length() > 1)
            {
                //std::string substr1 = str.substr(0, 2);
                QString substr1 = str.left(2);
                if (substr1 == "--")
                {
                    words.push_back(str);
                    strword = "";

                    if (words.size() > 1) return 1;
                    return 0;
                }
                else//-号
                {
                    strword += substr;
                    //str.erase(0, 1);//删除
                    str.erase(str.begin());
                }
            }
        }
        else
        {
            strword += substr;
            //str.erase(0, 1);//删除
            str.erase(str.begin());
        }
    }
    if (strword.length() > 0)
        words.push_back(strword);

    return words.size();
    //if (words.size() > 0) return 1;

    //return 0;
}