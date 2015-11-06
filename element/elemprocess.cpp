#include "element/elemprocess.h"

ElemProcess::ElemProcess(QObject *parent) :
    QProcess(parent)
{
    setWorkingDirectory(QDir::homePath());
    connect(this, SIGNAL(processState(bool)), this, SLOT(setProcessState(bool)));
    connect(this, SIGNAL(readyRead()), this, SLOT(sendMessage()));
    connect(this, SIGNAL(readyReadStandardError()), this, SLOT(sendMessage()));
    connect(this, SIGNAL(readyReadStandardOutput()), this, SLOT(sendMessage()));
    timerId = 0;
    waitTimerId = 0;
    commandLine = new String(this);
    _diff = 0;
    shredder = new ShredThread(this);
    connect(shredder, SIGNAL(finished()), this, SLOT(shreddingFinished()));
    connect(shredder, SIGNAL(stateChanged(uint)), this, SLOT(setShredState(uint)));
}

void ElemProcess::setItemReference(JobItemModel *model, JobItemIndex *idx)
{
    own_model = model;
    own_index = idx;
    name = idx->getName();
    proc_Status.insert("availability", QVariant(AVAILABLE));
    proc_Status.insert("isRunning", QVariant(STOPPED));
    proc_Status.insert("initName", QVariant(name));
    idx->setData(proc_Status);
    settings.beginGroup(name);
    bool autoRun = settings.value("AutoRun", QVariant()).toBool();
    settings.endGroup();
    if ( autoRun ) runJob();
}
QStringList ElemProcess::getCommand()
{
    settings.beginGroup(name);
    bool guiApp = settings.value("GuiApp", QVariant()).toBool();
    bool cgroups = settings.value("CGroups", QVariant()).toBool();
    bool capabilities = settings.value("Capabilities", QVariant()).toBool();
    bool shred = settings.value("Shred", QVariant()).toBool();
    bool securityLevel = settings.value("SLevel", QVariant()).toBool();
    QString selinuxLabel = settings.value("SELinuxLabel", QVariant()).toString();
    QString sandboxType = settings.value("SType", QVariant()).toString();
    bool execute = settings.value("Execute", QVariant()).toBool();
    bool session = settings.value("Session", QVariant()).toBool();
    QString command = settings.value("Command", QVariant()).toString();
    checkTimeout = settings.value("TimeOut", QVariant(10)).toInt();
    int DPI = settings.value("DPI", QVariant()).toInt();
    QString WM = settings.value("WM", QVariant()).toString();
    int windowHeight = settings.value("wHeight", QVariant()).toInt();
    int windowWidth = settings.value("wWidth", QVariant()).toInt();
    bool enableIncluded = settings.value("EnableIncluded", QVariant()).toBool();
    QString includes = settings.value("Includes", QVariant()).toString();
    bool mountDirs = settings.value("Mount", QVariant()).toBool();
    // defined previously in runJob()
    //QString tempDir = settings.value("TempDir", QVariant()).toString();
    //QString homeDir = settings.value("HomeDir", QVariant()).toString();
    settings.endGroup();
    commandLine->clear();
    // command building
    if ( capabilities ) commandLine->appendCapabilities();
    if ( cgroups ) commandLine->appendCGroups();
    if ( shred ) commandLine->appendShred();
    if ( guiApp && DPI ) commandLine->appendDPI(DPI);
    if ( !selinuxLabel.isEmpty() && securityLevel )
        commandLine->appendSecurityLevel(selinuxLabel);
    if ( mountDirs ) commandLine->appendMountDirs();
    if ( guiApp ) commandLine->appendGuiApp();
    if ( ( guiApp || mountDirs ) && !homeDir.isEmpty() )
        commandLine->appendHomeDir(homeDir);
    if ( ( guiApp || mountDirs ) && !tempDir.isEmpty() )
        commandLine->appendTempDir(tempDir);
    if ( enableIncluded && !includes.isEmpty() )
        commandLine->appendIncludes(includes);
    if ( guiApp ) {
    if (!WM.isEmpty()) commandLine->appendWM(WM);
        if ( windowWidth && windowHeight )
            commandLine->appendWindowSize(windowWidth, windowHeight);
    };
    if ( !sandboxType.isEmpty() ) commandLine->appendSandboxType(sandboxType);
    if ( session ) commandLine->appendSession();
    else if ( execute ) commandLine->appendCommand(command);
    return commandLine->getList();
}
void ElemProcess::appendChildren()
{
    children<<PID;
    //qDebug()<<children.join(" ")<<" begin";
    readChildren();
    //qDebug()<<children.join(" ")<<" end";
}
void ElemProcess::readChildren()
{
    children.removeDuplicates();
    QList<QString>::const_iterator i;
    QStringList _children;
    QString filename;
    filename = QString("/proc/%1/task/%1/children").arg(children.last());
    //qDebug()<<filename;
    QFile f;
    f.setFileName(filename);
    if (f.exists()) {
        f.open(QIODevice::ReadOnly|QIODevice::Text);
        f.waitForReadyRead(10000);
        f.setTextModeEnabled(true);
        QByteArray _data;
        _data = f.readAll();
        //qDebug()<<_data<<" ???"<<f.bytesAvailable();
        _children.append((QString().append(_data.data())).split(" "));
    };
    f.close();

    for (i = _children.constBegin(); i != _children.constEnd(); ++i) {
        if ( !children.contains((*i).toUtf8().data()) && !(*i).isEmpty() ) {
            //qDebug()<<(*i).toUtf8().data();
            children<<(*i).toUtf8().data();
            readChildren();
        };
    };
}
void ElemProcess::runJob()
{
    settings.beginGroup(name);
    bool runInTerm = settings.value("RunInTerm", QVariant()).toBool();
    bool customTerminal = settings.value("CustomTerm", QVariant()).toBool();
    shred = settings.value("Shred", QVariant()).toBool();
    mountDirs = settings.value("Mount", QVariant()).toBool();
    QStringList commandString;
    QString _commandString = settings.value("TermCommand", QVariant()).toString();
    commandString = _commandString.split(" ");
    commandString.removeAll("");
    SELabel = settings.value("SELinuxLabel", QVariant()).toString();
    if ( SELabel.isEmpty() ) SELabel.append("s0");
    tempDir = settings.value("TempDir", QVariant()).toString();
    homeDir = settings.value("HomeDir", QVariant()).toString();
    settings.endGroup();
    // check working directory
    QString user = QString("HOME");
    QStringList env = QProcess::systemEnvironment();
    QList<QString>::const_iterator i;
    for ( i=env.constBegin(); i<env.constEnd(); i++ ) {
        QString item = *i;
        if ( (item).startsWith("USER=") ) {
            user = item.split("=")[1];
            break;
        };
    };
    QStringList args;
    QDir d;
    int exitCode;
    d.setPath(homeDir);
    if ( !homeDir.isEmpty() && homeDir!=TMP_FILE && !d.exists() ) {
        d.mkpath(homeDir);
    } else if ( homeDir.isEmpty() || homeDir==TMP_FILE ) {
        homeDir = QString("/tmp/.sandbox_%1_%2").arg(user).arg(QString::number(qrand()));
        d.mkpath(homeDir);
    };
    args = QStringList()<<"-R"<<"-t"<<"sandbox_file_t"<<"-l"<< SELabel<< homeDir;
    exitCode = QProcess::execute(QString("chcon"), args);
    if ( exitCode!=0 )
        QMessageBox::information(NULL, "INFO","Create Home Directory failed.");

    d.setPath(tempDir);
    if ( !tempDir.isEmpty() && tempDir!=TMP_FILE && !d.exists() ) {
        d.mkpath(tempDir);
    } else if ( tempDir.isEmpty() || tempDir==TMP_FILE ) {
        tempDir = QString("/tmp/.sandbox_%1_%2").arg("tmp").arg(QString::number(qrand()));
        d.mkpath(tempDir);
    };
    args = QStringList()<<"-R"<<"-t"<<"sandbox_file_t"<<"-l"<< SELabel<< tempDir;
    exitCode = QProcess::execute(QString("chcon"), args);
    if ( exitCode!=0 )
        QMessageBox::information(NULL, "INFO","Create Temporary Directory failed.");

    proc_Status.insert("availability", QVariant(NOT_AVAILABLE));
    proc_Status.insert("isRunning", QVariant(RUNNING));
    own_index->setData(proc_Status);
    QStringList cmd;
    QString runApp;
    if (!runInTerm) {
        runApp = QString("/usr/bin/sandbox");
        cmd = getCommand();
    } else {
        QStringList _cmd = getCommand();
        _cmd.prepend("/usr/bin/sandbox");
        if ( !customTerminal ) {
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
    start(runApp, cmd);

    bool started = waitForStarted();
    PID = QString::number(pid());
    if ( started ) {
        waitTimerId = startTimer(1000);
    } else emit processState(STOPPED);
}
void ElemProcess::killJob()
{
    proc_Status.insert("availability", QVariant(NOT_AVAILABLE));
    own_index->setData(proc_Status);
    QModelIndex _idx = own_model->index( own_model->jobItemDataList.indexOf( own_index ), 1 );
    own_model->setData(_idx, "Killing", Qt::EditRole);
    if (timerId) {
        killTimer(timerId);
        timerId = 0;
    };
    appendChildren();
    this->kill();
    this->waitForFinished();
    //qDebug()<<children;
    QList<QString>::const_iterator i;
    for (i = children.constBegin(); i != children.constEnd(); ++i) {
        bool converted;
        pid_t _pid = (*i).toInt(&converted);
        if ( converted && ::kill(_pid, SIGZERO)==0 ) {
            //qDebug() <<_pid<< ::kill(_pid, SIGZERO);
            ::kill(_pid, SIGQUIT);
        };
    };
    if ( shred ) {
        shredder->tempDir = tempDir;
        shredder->homeDir = homeDir;
        shredder->start();
        own_model->setData(_idx, "Shredding|0", Qt::EditRole);
    } else emit processState(STOPPED);
}
void ElemProcess::undockJob()
{
    proc_Status.insert("availability", QVariant(NOT_AVAILABLE));
    own_index->setData(proc_Status);
    QModelIndex _idx = own_model->index( own_model->jobItemDataList.indexOf( own_index ), 1 );
    own_model->setData(_idx, "Undocking", Qt::EditRole);
    if (timerId) {
        killTimer(timerId);
        timerId = 0;
    };
    this->kill();
    this->waitForFinished();
    emit processState(STOPPED);
}
void ElemProcess::setProcessState(bool status)
{
    if ( status ) {
        proc_Status.insert("isRunning", QVariant(RUNNING));
    } else {
        children.clear();
        PID.clear();
        PID.append("-");
        proc_Status.insert("isRunning", QVariant(STOPPED));
    };
    proc_Status.insert("availability", QVariant(AVAILABLE));
    own_index->setData(proc_Status);
    int row = own_model->jobItemDataList.indexOf(own_index);
    QString data;
    for (int i=0; i<3; i++) {
        switch (i) {
        case 0:
            data = name;
            break;
        case 1:
            data = (status) ? "Running":"-";
            break;
        case 2:
            data = PID;
            break;
        default:
            break;
        };
        own_model->setData(own_model->index(row, i), data, Qt::EditRole);
    };
}
void ElemProcess::timerEvent(QTimerEvent *event)
{
    int percent = 0;
    int pid = 0;
    bool converted;
    int _timerId = event->timerId();
    //qDebug() << "Timer ID:" << _timerId<< timerId;
    if ( _timerId && timerId==_timerId ) {
        pid = PID.toInt(&converted);
        if ( !converted ) killJob();
        else if ( ::kill(pid, SIGZERO)!=0 ) killJob();
    } else if ( _timerId && waitTimerId==_timerId ) {
        if ( checkTimeout - _diff + 1 ) {
            percent = int ((float(_diff)/checkTimeout)*100.0);
            QModelIndex _idx = own_model->index( own_model->jobItemDataList.indexOf( own_index ), 1 );
            QString _state("Start|");
            _state.append(QString::number(percent));
            own_model->setData(_idx, _state, Qt::EditRole);
            _diff++;
        } else {
            killTimer(waitTimerId);
            waitTimerId = 0;
            _diff = 0;
            emit processState(RUNNING);
            if (!timerId) timerId = startTimer(1000);
        };
    };
}
void ElemProcess::sendMessage()
{
    QByteArray _data;
    _data = readAllStandardOutput();
    if ( !_data.isEmpty() )_data.append("\n");
    _data.append(readAllStandardError());
    QString msg = QTextStream(&_data).readAll();
    emit procMsg(name, msg);
}
void ElemProcess::shreddingFinished()
{
    emit processState(STOPPED);
}
void ElemProcess::setShredState(uint percent)
{
    QModelIndex _idx = own_model->index( own_model->jobItemDataList.indexOf( own_index ), 1 );
    QString _state("Shredding|");
    _state.append(QString::number(percent));
    //qDebug()<<_state;
    own_model->setData(_idx, _state, Qt::EditRole);
}
