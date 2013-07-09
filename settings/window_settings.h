#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QGridLayout>

/* [ -d DPI ] [ -W windowmanager ] [ -w windowsize ] */

class WindowSet : public QWidget
{
  Q_OBJECT;
public:
    WindowSet(QWidget *parent=0);
    ~WindowSet();
    QSpinBox *DPI;
    QLineEdit *WM;
    QSpinBox *windowWidth;
    QSpinBox *windowHeight;

signals:

private:
    QGridLayout *commonLayout;
    QLabel *dpiLabel;
    QLabel *wmLabel;
    QLabel *sizeTitle;

public slots:

private slots:
    void initSizeWidgets();

};
