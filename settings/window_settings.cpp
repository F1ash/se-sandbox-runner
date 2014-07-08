#include "window_settings.h"

/* [ -d DPI ] [ -W windowmanager ] [ -w windowsize ]
for -X parameter ONLY */

WindowSet::WindowSet(QWidget *parent=0) :
    QWidget(parent)
{
    setContentsMargins(1,1,1,1);
    commonLayout = new QGridLayout(this);
    commonLayout->setAlignment(Qt::AlignJustify);

    dpiLabel = new QLabel("DPI", this);
    DPI = new QSpinBox(this);
    DPI->setMaximum(256);
    DPI->setMinimum(96);
    DPI->setSingleStep(16);

    wmLabel = new QLabel("Window Manager", this);
    WM = new QLineEdit(this);
    WM->setPlaceholderText("Enter WindowManager");

    initSizeWidgets();

    commonLayout->addWidget(dpiLabel, 0, 0);
    commonLayout->addWidget(DPI, 0, 2, Qt::AlignRight);
    commonLayout->addWidget(wmLabel, 1, 0);
    commonLayout->addWidget(WM, 1, 2, Qt::AlignRight);
    commonLayout->addWidget(sizeTitle, 3, 0);
    commonLayout->addWidget(windowWidth, 3, 2, Qt::AlignLeft);
    commonLayout->addWidget(windowHeight, 3, 2, Qt::AlignRight);

    setLayout(commonLayout);
    setEnabled(false);
}
WindowSet::~WindowSet()
{
    delete DPI;
    DPI = 0;
    delete WM;
    WM = 0;
    delete dpiLabel;
    dpiLabel = 0;
    delete wmLabel;
    wmLabel = 0;
    delete sizeTitle;
    sizeTitle = 0;
    delete windowWidth;
    windowWidth = 0;
    delete windowHeight;
    windowHeight = 0;
    delete commonLayout;
    commonLayout = 0;
}
void WindowSet::initSizeWidgets()
{
    sizeTitle = new QLabel("Window Size", this);

    windowWidth = new QSpinBox(this);
    windowWidth->setToolTip("Width");
    windowWidth->setMaximum(10000);
    windowWidth->setMinimum(400);
    windowWidth->setSingleStep(20);
    windowWidth->setValue(1000);

    windowHeight = new QSpinBox(this);
    windowHeight->setToolTip("Height");
    windowHeight->setMaximum(10000);
    windowHeight->setMinimum(400);
    windowHeight->setSingleStep(20);
    windowHeight->setValue(700);
}
