#include "settings/settings.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle("<noname>");
    restoreGeometry(settings.value("SetDlgGeometry").toByteArray());
    setModal(false);
    setContentsMargins(1,1,1,1);
    initTabWidget();
    initButtons();
    initParameters();
    fullCommandWdg = new QTextEdit(this);
    fullCommandWdg->setReadOnly(true);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(tabWidget);
    commonLayout->addWidget(fullCommandWdg);
    commonLayout->addWidget(buttons);
    setLayout(commonLayout);
    commandLine = new String(this);
    timerId = startTimer(1000);
}

void SettingsDialog::initTabWidget()
{
    /*
   sandbox [-C] [-c] [-s] [ -d DPI ] [-l level ] [[-M | -X]  -H homedir -T tempdir ] [-I includefile ] [
   -W windowmanager ] [ -w windowsize ] [[-i file ]...] [ -t type ] cmd
    * TABs *
    [-C] [-c] [-s] [-l level ] [-X] [ -t type ] [cmd || -S ]
    [ -d DPI ] [ -W windowmanager ] [ -w windowsize ]
    [-I includefile ] [[-i file ]...]
    [[-M | -X]  -H homedir -T tempdir ]
   */
    tabWidget = new QTabWidget(this);
    tabWidget->setContentsMargins(1,1,1,1);
    tabWidget->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
    w1 = new CommonSet(this);
    QIcon i1 = QIcon::fromTheme("preferences-other");
    QString n1 = QString("Common");
    w2 = new WindowSet(this);
    QIcon i2 = QIcon::fromTheme("preferences-desktop");
    QString n2 = QString("Window");
    w3 = new IncludeSet(this);
    QIcon i3 = QIcon::fromTheme("document-export");
    QString n3 = QString("Include files");
    w4 = new DirectorySet(this);
    QIcon i4 = QIcon::fromTheme("folder-sync");
    QString n4 = QString("Work directories");
    tabWidget->addTab(w1, i1, n1);
    tabWidget->addTab(w2, i2, n2);
    tabWidget->addTab(w3, i3, n3);
    tabWidget->addTab(w4, i4, n4);
    connect(w1->guiApp, SIGNAL(stateChanged(int)),
            this, SLOT(windowSetsEnable(int)));
    connect(w1->guiApp, SIGNAL(stateChanged(int)),
            w4, SLOT(setGuiCheckState(int)));
    connect(w1->nameEdit, SIGNAL(textChanged(QString)),
            this, SLOT(set_Title_Name(QString)));
    connect(w4, SIGNAL(guiStateChanged(bool)),
            w1->guiApp, SLOT(setChecked(bool)));
    connect(w1, SIGNAL(sessionUsed(bool)),
            this, SLOT(sessionStateChanged(bool)));
}
void SettingsDialog::initButtons()
{
    ok = new QPushButton(QIcon::fromTheme("dialog-ok"),"Ok");
    cancel = new QPushButton(QIcon::fromTheme("dialog-cancel"),"Cancel");
    buttonsLayout = new QHBoxLayout(this);
    buttonsLayout->addWidget(ok);
    buttonsLayout->addWidget(cancel);
    buttons = new QWidget(this);
    buttons->setLayout(buttonsLayout);
    connect(ok, SIGNAL(clicked()),
            this, SLOT(saveJob()));
    connect(cancel, SIGNAL(clicked()),
            this, SLOT(cancelJob()));
}
QString SettingsDialog::includesFileName(QString s) const
{
    QStringList fullPath = settings.fileName().split(QDir::separator());
    fullPath.removeLast();
    fullPath.append(QString("%1.included").arg(s));
    return fullPath.join(QDir::separator());
}
void SettingsDialog::setJobItem(JobItemIndex *idx)
{
    own_index = idx;
    name = idx->getName();
    previousName = name;
    setWindowTitle(name);
    if ( name!=QString("<noname>") ) {
        w1->set_Job_Name(name);
        w3->set_FileName(includesFileName(name));
        initParameters();
    };
}
void SettingsDialog::saveJob()
{
    name = w1->get_Job_Name();
    w3->set_FileName(includesFileName(name));
    if ( w4->mountDirs->isChecked() && w3->enabled->isChecked() ) {
        QString s;
        s.append("Note: A permanent home directory and included files \
can be used together only once --\nwhen the home directory is empty.");
        QMessageBox::information(this, "Info", s);
    };
    // make specified directories
    if ( w4->mountDirs->isChecked() ) {
        make_SpecifiedDirectories();
    };
    // check Session parameters
    if ( w1->session->isChecked() ) {
        if ( w4->selinuxLabel->text().isEmpty() || !w4->securityLevel->isChecked() ||
             w4->tempDir->text().isEmpty() || w4->homeDir->text().isEmpty() ) {
            QString s;
            s.append("You must to specify\na Security Level,\n\
HOME and TEMP directories.");
            QMessageBox::information(this, "Info", s);
            return;
        };
    };
    // set specified SELinux context for directories
    if ( w4->securityLevel->isChecked() && !w4->selinuxLabel->text().isEmpty() ) {
        if ( !w4->tempDir->text().isEmpty() && !set_SpecifiedLabel(w4->tempDir->text()) )
            return;
        if ( !w4->homeDir->text().isEmpty() && !set_SpecifiedLabel(w4->homeDir->text()) )
            return;
    };

    // check unique Job name & Settings Node
    QStringList groups = settings.childGroups();
    if ( name.isEmpty() )
        QMessageBox::information(this, "Info", "JobName is empty.");
    else if ( groups.contains(name) && !newbe && name==previousName ) {
        saveParameters();
        close();
    } else if ( groups.contains(name) && newbe ) {
        QMessageBox::information(this, "Info", "Same JobName is exist.");
    } else if ( groups.contains(name) && !newbe && name!=previousName ) {
        QMessageBox::information(this, "Info", "Same JobName is exist.");
    } else if ( !groups.contains(name) && newbe ) {
        saveParameters();
        close();
    } else if ( !groups.contains(name) && !newbe ) {
        settings.remove(previousName);
        QFile f;
        f.setFileName(includesFileName(previousName));
        f.remove();
        saveParameters();
        close();
    };
}
void SettingsDialog::cancelJob()
{
    //qDebug()<<"cancel"<<name<< newbe;
    if ( newbe ) {
        settings.remove(name);
        emit creatingJobCancelled();
    };
    close();
}
void SettingsDialog::initParameters()
{
    int c;
    QStringList groups = settings.childGroups();
    newbe = !groups.contains(name);
    //qDebug()<<"init"<<name<< newbe;
    settings.beginGroup(name);
    w1->autoRun->setChecked(settings.value("AutoRun", QVariant()).toBool() );
    w1->cgroups->setChecked( settings.value("CGroups", QVariant()).toBool() );
    w1->guiApp->setChecked( settings.value("GuiApp", QVariant()).toBool() );
    w1->runInTerm->setChecked( settings.value("RunInTerm", QVariant()).toBool() );
    w1->defaultTerminal->setChecked( settings.value("XDG-Utility-Term", QVariant()).toBool() );
    w1->customTerminal->setChecked( settings.value("CustomTerm", QVariant()).toBool() );
    w1->termCommand->setText( settings.value("TermCommand", QVariant()).toString() );
    w1->capabilities->setChecked( settings.value("Capabilities", QVariant()).toBool() );
    w1->shred->setChecked( settings.value("Shred", QVariant()).toBool() );
    w1->copy_paste->setChecked( settings.value("CopyPaste", QVariant()).toBool() );
    c = w1->sandboxType->findText( settings.value("SType", QVariant()).toString() );
    w1->sandboxType->setCurrentIndex(c);
    w1->command->setText( settings.value("Command", QVariant()).toString() );
    w1->checkTimeout->setValue( settings.value("TimeOut", QVariant(TIMEOUT)).toInt() );
    w2->WM->setText( settings.value("WM", QVariant()).toString() );
    w2->DPI->setValue( settings.value("DPI", QVariant()).toInt() );
    w2->windowHeight->setValue( settings.value("wHeight", QVariant()).toInt() );
    w2->windowWidth->setValue( settings.value("wWidth", QVariant()).toInt() );
    w3->enabled->setChecked( settings.value("EnableIncluded", QVariant(false)).toBool() );
    w3->setIncludesList( settings.value("Includes", QVariant()).toString() );
    w4->mountDirs->setChecked( settings.value("Mount", QVariant()).toBool() );
    w4->securityLevel->setChecked( settings.value("SLevel", QVariant()).toBool() );
    w4->selinuxLabel->setText( settings.value("SELinuxLabel", QVariant()).toString() );
    w4->tempDir->setText( settings.value("TempDir", QVariant()).toString() );
    w4->homeDir->setText( settings.value("HomeDir", QVariant()).toString() );
    // it last settings because it (session key) define the directory_settings
    w1->execute->setChecked( settings.value("Execute", QVariant()).toBool() );
    w1->session->setChecked( settings.value("Session", QVariant()).toBool() );
    settings.endGroup();
}
void SettingsDialog::saveParameters()
{
    settings.beginGroup(name);
    settings.setValue("AutoRun", QVariant(w1->autoRun->isChecked()));
    settings.setValue("CGroups", QVariant(w1->cgroups->isChecked()));
    settings.setValue("GuiApp", QVariant(w1->guiApp->isChecked()));
    settings.setValue("RunInTerm", QVariant(w1->runInTerm->isChecked()));
    settings.setValue("XDG-Utility-Term", QVariant(w1->defaultTerminal->isChecked()));
    settings.setValue("CustomTerm", QVariant(w1->customTerminal->isChecked()));
    settings.setValue("TermCommand", QVariant(w1->termCommand->text()));
    settings.setValue("Capabilities", QVariant(w1->capabilities->isChecked()));
    settings.setValue("Shred", QVariant(w1->shred->isChecked()));
    settings.setValue("CopyPaste", QVariant(w1->copy_paste->isChecked()));
    settings.setValue("SType", QVariant(w1->sandboxType->currentText()));
    settings.setValue("Execute", QVariant(w1->execute->isChecked()));
    settings.setValue("Session", QVariant(w1->session->isChecked()));
    settings.setValue("Command", QVariant(w1->command->text()));
    settings.setValue("TimeOut", QVariant(w1->checkTimeout->value()));
    settings.setValue("DPI", QVariant(w2->DPI->value()));
    settings.setValue("WM", QVariant(w2->WM->text()));
    settings.setValue("wHeight", QVariant(w2->windowHeight->value()));
    settings.setValue("wWidth", QVariant(w2->windowWidth->value()));
    settings.setValue("EnableIncluded", QVariant(w3->enabled->isChecked()));
    settings.setValue("Includes", QVariant(w3->get_FileName()));
    settings.setValue("SLevel", QVariant(w4->securityLevel->isChecked()));
    settings.setValue("SELinuxLabel", QVariant(w4->selinuxLabel->text()));
    settings.setValue("Mount", QVariant(w4->mountDirs->isChecked()));
    settings.setValue("TempDir", QVariant(w4->tempDir->text()));
    settings.setValue("HomeDir", QVariant(w4->homeDir->text()));
    settings.endGroup();
    own_index->setName(name);
    if ( own_index->getData().value("isRunning").toBool() )
        QMessageBox::information(
                    this,
                    "Info",
                    "New settings apply\nat next job start.");
}

void SettingsDialog::closeEvent(QCloseEvent *ev)
{
    settings.setValue("SetDlgGeometry", saveGeometry());
    settings.sync();
    killTimer(timerId);
    ev->accept();
}

void SettingsDialog::windowSetsEnable(int i)
{
    Qt::CheckState _state = (i) ? Qt::Checked : Qt::Unchecked;
    w2->setEnabled(_state==Qt::Checked);
    int current_policy = w1->sandboxType->currentIndex();
    if ( _state==Qt::Checked ) {
        if ( current_policy < 2 ) {
            w1->sandboxType->setCurrentIndex(2);
        };
    } else {
        int c;
        settings.beginGroup(name);
        c = w1->sandboxType->findText( settings.value("SType", QVariant()).toString() );
        settings.endGroup();
        w1->sandboxType->setCurrentIndex(c);
    };
}

void SettingsDialog::set_Title_Name(QString s)
{
    setWindowTitle(s);
}

void SettingsDialog::timerEvent(QTimerEvent *event)
 {
    int _timerId = event->timerId();
    if ( _timerId && timerId==_timerId ) {
        QString runApp;
        QStringList cmd;
        QStringList commandString;
        commandString = w1->termCommand->text().split(" ");
        commandString.removeAll("");

        if ( !w1->runInTerm->isChecked() ) {
            runApp = QString("/usr/bin/sandbox");
            cmd = _commandBuild();
        } else {
            QStringList _cmd = _commandBuild();
            _cmd.prepend("/usr/bin/sandbox");
            if ( !w1->customTerminal->isChecked() ) {
                cmd.append(_cmd.join(" "));
                runApp = QString("/usr/bin/xdg-terminal");
            } else {
                cmd = _cmd;
                while ( commandString.count() > 1 ) {
                    cmd.prepend( commandString.takeLast() );
                };
                if ( !commandString.isEmpty() && !commandString.first().isEmpty() ) {
                    runApp = commandString.first();
                } else {
                    cmd.prepend("-e");
                    runApp = QString("xterm");
                };
            };
        };
        //qDebug()<<runApp<<cmd.join(" ")<<name;
        fullCommandWdg->setText(QString("%1 %2").arg(runApp).arg(cmd.join(" ")));
    };
}

QStringList SettingsDialog::_commandBuild()
{
    commandLine->clear();

    if ( w1->capabilities->isChecked() ) commandLine->appendCapabilities();
    if ( w1->cgroups->isChecked() ) commandLine->appendCGroups();
    if ( w1->shred->isChecked() ) commandLine->appendShred();
    if ( w1->guiApp->isChecked() && w2->DPI->value() ) commandLine->appendDPI( w2->DPI->value() );
    QString str;
    str = w4->selinuxLabel->text();
    if ( !str.isEmpty() && w4->securityLevel->isChecked() )
        commandLine->appendSecurityLevel( str );
    if ( w4->mountDirs->isChecked() ) commandLine->appendMountDirs();
    if ( w1->guiApp->isChecked() ) commandLine->appendGuiApp();
    str = w4->homeDir->text();
    if ( ( w1->guiApp->isChecked() || w4->mountDirs->isChecked() ) && !str.isEmpty() )
        commandLine->appendHomeDir( str );
    str = w4->tempDir->text();
    if ( ( w1->guiApp->isChecked() || w4->mountDirs->isChecked() ) && !str.isEmpty() )
        commandLine->appendTempDir( str );
    str = w3->get_FileName();
    if ( w3->enabled->isChecked() && !str.isEmpty() ) commandLine->appendIncludes( str );
    if ( w1->guiApp->isChecked() ) {
        str = w2->WM->text();
        if ( !str.isEmpty() ) commandLine->appendWM( str );
        if ( w2->windowWidth->value() && w2->windowHeight->value() )
            commandLine->appendWindowSize( w2->windowWidth->value(), w2->windowHeight->value() );
    };
    str = w1->sandboxType->currentText();
    if ( !str.isEmpty() ) commandLine->appendSandboxType( str );
    str = w1->command->text();
    if    ( w1->session->isChecked() ) commandLine->appendSession();
    else if ( w1->execute->isChecked() ) commandLine->appendCommand( str );

    return commandLine->getList();
}

void SettingsDialog::sessionStateChanged(bool b)
{
    w4->sessionUsed = b;
    w1->command->setEnabled(!b);
    if (b) w4->setSELinuxLabelState(b);
    else {
        settings.beginGroup(name);
        w4->securityLevel->setChecked( settings.value("SLevel", QVariant()).toBool() );
        w4->mountDirs->setChecked( settings.value("Mount", QVariant()).toBool() );
        settings.endGroup();
    };
}

bool SettingsDialog::make_Directory(QString _dirPath, QString Dir)
{
    QString dirPath;
    QString q = QString(" not specified.\nCreate temporary directory?");
    QString p = QString("cleaning not completed.\nTo continue anyway?");
    if ( _dirPath.isEmpty() ) {
        int answer = QMessageBox::question(
                    this,
                    "Settings",
                    QString("%1 %2").arg(Dir).arg(q),
                    QMessageBox::Yes,
                    QMessageBox::No);
        if ( answer == QMessageBox::Yes ) {
            dirPath = TMP_DIR;
        } else return false;
    } else dirPath = _dirPath;

    if ( !clean_Directory(dirPath) ) {
        int answer = QMessageBox::question(
                    this, "Clean Directory",
                    QString("\"%1\" %2").arg(dirPath).arg(p),
                    QMessageBox::Yes,
                    QMessageBox::No);
            if ( answer == QMessageBox::No ) return false;
    };
    QLineEdit *obj = ( Dir == "TempDir" ) ? w4->tempDir : w4->homeDir;
    obj->setText(dirPath);
    if ( !exist_Directory(obj) ) {
        QMessageBox::information(
                    this,
                    "Info",
                    QString("Path\n\"%1\"\nis failed.").arg(dirPath));
        return false;
    } else if ( !set_SpecifiedLabel(dirPath) ) return false;
    return true;
}

bool SettingsDialog::make_SpecifiedDirectories()
{
    /*
     * if directories don`t exists, then create them
     */
    if ( !exist_Directory(w4->tempDir) )
        if ( !make_Directory(w4->tempDir->text(), "TempDir") ) return false;
    if ( !exist_Directory(w4->homeDir) )
        if ( !make_Directory(w4->homeDir->text(), "HomeDir") ) return false;
    return true;
}

bool SettingsDialog::set_SpecifiedLabel(QString dirPath)
{
    if ( dirPath==TMP_DIR ) return true;
    /*
     * chcon -R -t sandbox_file_t -l s0:c123,c456 /home/user/Example_HOME
     */
    QString SELabel = w4->selinuxLabel->text();
    if ( SELabel.isEmpty() ) return true;
    QStringList args = QStringList()<<"-R"<<"-t"<<"sandbox_file_t"<<"-l"<< SELabel<< dirPath;
    int exitCode = QProcess::execute(QString("chcon"), args);
    if ( exitCode != 0 ) {
        QMessageBox::information(
                    this,
                    "Info",
                    QString("\"%1\"\n%2\n%3\nTry do it manually.")
                            .arg(dirPath)
                            .arg("can`t marked by label:\n")
                            .arg(SELabel));
        return false;
    };
    return true;
}

bool SettingsDialog::clean_Directory(QString dir, void *opaque, void *p)
{
    if ( dir==TMP_DIR ) return true;
    ShredThread *shredder = static_cast<ShredThread*>(opaque);
    PERCENT *P = static_cast<PERCENT*>(p);
    // Shred temporary files created in $HOME and /tmp, before deleting.
    QDir::Filters flags =
            QDir::AllEntries |
            QDir::NoDotAndDotDot |
            QDir::Hidden |
            QDir::System |
            QDir::AllDirs;
    bool result = true;
    QDir d;
    d.setPath(dir);
    if ( d.exists() ) {
        /*
         * unconfined_u:object_r:user_tmp_t:s0 -- policy for deletion
         */
        QStringList args = QStringList()<<"-R"<<"-t"<<"user_tmp_t"<<"-l"<<"s0"<<dir;
        QProcess::execute(QString("chcon"), args);
        QList<QFileInfo> entry = d.entryInfoList(flags);
        if ( !entry.isEmpty() ) {
            bool _result = true;
            QList<QFileInfo>::const_iterator i;
            for ( i=entry.constBegin(); i<entry.constEnd(); i++ ) {
                QFileInfo item = *i;
                if ( !item.exists() ) continue;
                QString path = item.canonicalFilePath();
                bool ret = true;
                if ( path==d.absoluteFilePath(dir)
                     || item.isSymLink() ) continue;
                //qDebug()<<path<<item.size();
                if ( item.isDir() ) {
                    ret = clean_Directory(path, opaque, p);
                    d.rmdir(path);
                } else {
                    QFile f;
                    f.setFileName(path);
                    if ( f.exists() ) {
                        qint64 pos = 0;
                        qint64 collector = 0;
                        uint   add_percent = P->curr_percent;
                        f.open(QIODevice::ReadWrite);
                        qint64 fSize = f.size();
                        while ( pos<fSize ) {
                            f.seek(pos);
                            f.putChar('\0');
                            pos++;
                            collector++;
                            if ( NULL!=shredder && P!=NULL ) {
                                if ( collector>P->one_percent ) {
                                    collector = 0;
                                    ++add_percent;
                                    shredder->shredStateChanged(add_percent);
                                };
                            };
                        };
                        f.close();
                        ret = f.remove();
                        if ( NULL!=shredder && P!=NULL && ret) {
                            P->current += fSize;
                            shredder->shredStateChanged(P->percent());
                            //qDebug()<<path<<P->current;
                        };
                    };
                };
                _result = _result && ret;
            };
            result = result && _result;
        };
        result = result && d.rmdir(dir);
    } else
        result = false;
    return result;
}

bool SettingsDialog::exist_Directory(QLineEdit *obj)
{
    QDir d;
    QString dir = obj->text();
    if ( dir==TMP_DIR ) return true;
    d.setPath(dir);
    if ( !d.exists() ) {
        QMessageBox::information(
                    this,
                    "Info",
                    QString("Directory\n\"%1\"\n is not exist.")
                            .arg(dir));
        obj->clear();
        return false;
    };
    return true;
}
