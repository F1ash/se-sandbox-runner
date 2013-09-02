#ifndef STRING_H
#define STRING_H

#include <QStringList>

class String : public QObject
{
  Q_OBJECT;
public:
    String(QObject *parent);
    ~String();

private:
    QStringList list;

public slots:
    void clear();
    QStringList getList() const;
    void appendCapabilities();
    void appendCGroups();
    void appendShred();
    void appendDPI(int i);
    void appendSecurityLevel(QString &s);
    void appendWM(QString &s);
    void appendWindowSize(int w, int h);
    void appendIncludes(QString &s);
    void appendMountDirs();
    void appendGuiApp();
    void appendHomeDir(QString &s);
    void appendTempDir(QString &s);
    void appendSandboxType(QString &s);
    void appendCommand(QString &);
    void appendSession();

};

#endif
