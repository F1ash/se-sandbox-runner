#include "string_list.h"

String::String(QObject *parent=0) :
    QObject(parent)
{
    list.clear();
}

void String::clear()
{
    list.clear();
}
QStringList String::getList() const
{
    return list;
}
void String::appendGuiApp()
{
    list.append("-X");
}
void String::appendCGroups()
{
    list.append("-c");
}
void String::appendShred()
{
    list.append("-s");
}
void String::appendCapabilities()
{
    list.append("-C");
}
void String::appendSecurityLevel(QString &s)
{
    list.append("-l");
    list.append(s);
}
void String::appendSandboxType(QString &s)
{
    list.append("-t");
    QString t;
    if    (s==QString("No Network Access")) t = QString("sandbox_min_t");
    else if (s==QString("Printer Ports")) t = QString("sandbox_x_t");
    else if (s==QString("Ports required for Web")) t = QString("sandbox_web_t");
    else if (s==QString("All network ports")) t = QString("sandbox_net_t");
    else if (s==QString("Default")) t = QString("sandbox_t");
    list.append(t);
}
void String::appendSession()
{
    list.append("-S");
}
void String::appendCommand(QString &s)
{
    list.append(s);
}
void String::appendDPI(int i)
{
    list.append("-d");
    list.append(QString("%1").arg(i));
}
void String::appendWM(QString &s)
{
    list.append("-W");
    list.append(s);
}
void String::appendWindowSize(int w, int h)
{
    list.append("-w");
    list.append(QString("%1x%2").arg(w).arg(h));
}
void String::appendIncludes(QString &s)
{
    list.append("-I");
    list.append(s);
}
void String::appendMountDirs()
{
    list.append("-M");
}
void String::appendTempDir(QString &s)
{
    list.append("-T");
    list.append(s);
}
void String::appendHomeDir(QString &s)
{
    list.append("-H");
    list.append(s);
}
