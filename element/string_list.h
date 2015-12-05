#ifndef STRING_H
#define STRING_H

#include <QObject>
#include <QStringList>

class String : public QObject
{
public:
    explicit String(QObject *parent);
    void clear();
    QStringList getList() const;
    void appendCapabilities();
    void appendCGroups();
    void appendShred();
    void appendDPI(int);
    void appendSecurityLevel(QString&);
    void appendWM(QString&);
    void appendWindowSize(int, int);
    void appendIncludes(QString&);
    void appendMountDirs();
    void appendGuiApp();
    void appendHomeDir(QString&);
    void appendTempDir(QString&);
    void appendSandboxType(QString&);
    void appendCommand(QString&);
    void appendSession();

private:
    QStringList list;

};

#endif //STRING_H
