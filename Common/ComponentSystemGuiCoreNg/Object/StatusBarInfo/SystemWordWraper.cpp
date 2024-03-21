#include "SystemWordWraper.h"

SystemWordWraper::SystemWordWraper ()
{}

SystemWordWraper::~SystemWordWraper ()
{}

QString SystemWordWraper::getWrapText (const QString &text, int maxSimbolsInRow, int& rowNum)
{
    rowNum = 1;
    QString tempText;
    if (text.size () > maxSimbolsInRow)
    {
        QStringList strList = text.split(" ");
        for(int i = 0; i < strList.size(); i++)
        {
            if(i == 0)
                tempText = strList.operator [](i);
            else
            {
                QStringList tempRow = tempText.split("\n");
                QString tempName = tempRow.operator [](tempRow.size()-1);
                QString tempStr = strList.operator [](i);
                if((tempName.size() + tempStr.size() + 1) > maxSimbolsInRow)
                {
                    tempText = tempText + "\n" + tempStr;
                    rowNum++;
                }
                else
                    tempText = tempText + " " + tempStr;
            }
        }
    }
    else
        tempText = text;

    return tempText;
}
