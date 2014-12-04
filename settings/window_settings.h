#ifndef WINDOW_SETTINGS_H
#define WINDOW_SETTINGS_H

#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QSpinBox>
#include <QtGui/QGridLayout>

/* [ -d DPI ] [ -W windowmanager ] [ -w windowsize ] */

class WindowSet : public QWidget
{
  Q_OBJECT
public:
    WindowSet(QWidget *parent);
    ~WindowSet();
    QSpinBox        *DPI;
    QLineEdit       *WM;
    QSpinBox        *windowWidth;
    QSpinBox        *windowHeight;

signals:

private:
    QGridLayout     *commonLayout;
    QLabel          *dpiLabel;
    QLabel          *wmLabel;
    QLabel          *sizeTitle;

public slots:

private slots:
    void initSizeWidgets();
};

#endif //WINDOW_SETTINGS_H
