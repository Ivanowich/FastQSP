#include "qsp_callback.h"
#include "fastqspwindow.h"


FastQSPWindow::FastQSPWindow(QWidget *parent) :
    QMainWindow(parent),
    gameWidth(800),
    gameHeight(600),
    aspectRatio(qreal(gameWidth) / qreal(gameHeight)),
    scaleFactor(1),
    media(new Phonon::MediaObject(this))
{
    // Init audio
    audioOutput = new Phonon::AudioOutput(Phonon::VideoCategory, this);
    Phonon::createPath(media, audioOutput);

    // Creating main view
    //mainView = new FastQSPView(this);
    //QGraphicsWidget
    mainView = new QWebView(this);
    mainView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    mainView->setContextMenuPolicy(Qt::NoContextMenu);
    mainView->installEventFilter(this);
    mainView->setRenderHints(
                QPainter::Antialiasing |
                QPainter::HighQualityAntialiasing |
                QPainter::NonCosmeticDefaultPen |
                QPainter::SmoothPixmapTransform |
                QPainter::TextAntialiasing);

    connect(mainView,
            SIGNAL(linkClicked(const QUrl&)),
            SLOT(linkClicked(const QUrl&)));
    setCentralWidget(mainView);

    // TODO: That must be optional
    mainView->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
    mainView->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    //mainView->show();

    // Creating menu
    QMenu* fileMenu = new QMenu("File");
    fileMenu->addAction("Open file",
                    this,
                    SLOT(openFileDialog()),
                    Qt::CTRL + Qt::Key_O);
    fileMenu->addAction("Exit",
                    this,
                    SLOT(close()),
                    Qt::CTRL + Qt::Key_Q);
    menuBar()->addMenu(fileMenu);

    gameMenu = new QMenu("Game");
    gameMenu->addAction("Save",
                    this,
                    SLOT(saveGame()),
                    Qt::CTRL + Qt::Key_S);
    gameMenu->addAction("Load",
                    this,
                    SLOT(loadGame()),
                    Qt::CTRL + Qt::Key_L);
    menuBar()->addMenu(gameMenu);
    gameMenu->setDisabled(true);


    QMenu* debugMenu = new QMenu("Debug");
    debugMenu->addAction("Show html",
                    this,
                    SLOT(showHtml()));
    debugMenu->addAction("Show css",
                    this,
                    SLOT(showCss()));

    QMenu* helpMenu = new QMenu("Help");
    helpMenu->addAction("About",
                    this,
                    SLOT(about()));

    menuBar()->addMenu(helpMenu);

    // Initializing QSP
    QSPInit();
    QSPCallback::QSPCallback();
}

void FastQSPWindow::loadFonts()
{
    QDir fontsDir(gameDirectory + "fonts");
    fontsDir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    fontsDir.setSorting(QDir::Size | QDir::Reversed);
    QFileInfoList fontList = fontsDir.entryInfoList();
    for (int i = 0; i < fontList.size(); ++i) {
        QFileInfo fileInfo = fontList.at(i);
        QFontDatabase::addApplicationFont(fileInfo.absoluteFilePath());
    }
}

FastQSPWindow::~FastQSPWindow()
{
    QSPDeInit();
}

bool FastQSPWindow::eventFilter(QObject * obj, QEvent *e)
{
    QMainWindow::eventFilter(obj, e);
    if(e->type() == QEvent::MouseButtonPress)
    {
        if(((QMouseEvent *)e)->button() != Qt::LeftButton)
            return true;
    }
    if(e->type() == QEvent::DragEnter ||
       e->type() == QEvent::DragMove ||
       e->type() == QEvent::DragLeave ||
       e->type() == QEvent::DragResponse ||
       e->type() == QEvent::Drop)
    {
        return true;
    }
    return false;
}

void FastQSPWindow::keyPressEvent(QKeyEvent *e)
{
    if ((e->key()==Qt::Key_Return) && (e->modifiers()==Qt::AltModifier))
    {

        if(isFullScreen())
        {
            menuBar()->show();
            showNormal();
            qDebug() << "normal mode";
        }
        else
        {
            menuBar()->hide();
            showFullScreen();
            qDebug() << "fullscreen";
        }
    }
}

void FastQSPWindow::about()
{
    QLabel *about = new QLabel;
    about->setText("<h2>FastQSP player v0.3</h2><table><tr><td>Author:</td> <td>Graylor[graylor@yandex.ru]</td></tr><tr><td>Page:</td> <td><a href='https://github.com/graylor/FastQSP'>https://github.com/graylor/FastQSP</a></td></tr><tr><td>License:</td> <td>GPL v3</td></tr>");
    about->setFixedSize(250,90);
    about->show();
}

void FastQSPWindow::openFileDialog()
{
    QFileDialog dlg;
    QString filename = dlg.getOpenFileName(
                this,
                "Open File",
                NULL,
                "QSP game (*.qsp)");
    openFile(filename);
}

void FastQSPWindow::saveGame()
{
    QFileDialog dlg;
    QString filename = dlg.getSaveFileName(
                this,
                "Save Game",
                gameDirectory,
                "QSP save-game (*.sav)");
    if(!filename.isEmpty())
        QSPSaveGame(filename.toStdWString().c_str(), true);
}

void FastQSPWindow::loadGame()
{
    QFileDialog dlg;
    QString filename = dlg.getOpenFileName(
                this,
                "Load Game",
                gameDirectory,
                "QSP save-game (*.sav)");
    if(!filename.isEmpty() &&
        QSPOpenSavedGame(filename.toStdWString().c_str(), true))
    {
        loadPage();
    }
}

void FastQSPWindow::showHtml()
{
    static QTextEdit *htmlText;
    if(!htmlText)
        htmlText = new QTextEdit();
    htmlText->setPlainText(mainView->page()->mainFrame()->toHtml());
    htmlText->show();
}

void FastQSPWindow::showCss()
{
    static QTextEdit *cssText;
    if(!cssText)
        cssText = new QTextEdit();
    int numVal;
    wchar_t *strVal;
    QSPGetVarValues(L"STYLESHEET", 0, &numVal, &strVal);
    cssText->setPlainText(QString::fromWCharArray(strVal));
    cssText->show();
}

void FastQSPWindow::linkClicked(const QUrl & url)
{
    if(url.toString().startsWith(QLatin1String("exec:"), Qt::CaseInsensitive))
    {
        QString execStr;
        execStr = url.toString().remove(0, 5).trimmed().replace("%22", "\"");
        QSPExecString(execStr.toStdWString().c_str(), true);
    }
    else
    if(url.toString().startsWith(QLatin1String("obj:"), Qt::CaseInsensitive))
    {
        bool ok = false;
        int number = 0;
        number = url.toString().remove(0, 4).trimmed().toInt(&ok);
        if(!ok)
        {
            qCritical() << "Unknown link format" << url.toString();
            return;
        }
        QSPSetSelObjectIndex(number, true);
    }
    else
    if(url.toString() == QLatin1String("msgclose"))
    {
        builder.hideMessage();
    }
    else
    {
        bool ok = false;
        int number;
        number = url.toString().toInt(&ok);
        if(!ok)
        {
            qCritical() << "Unknown link format" << url.toString();
            return;
        }
        QSPSetSelActionIndex(number - 1, true);
        QSPExecuteSelActionCode(true);
    }
    loadPage();
    qDebug() << "Link clicked" << url.toString();
}

void FastQSPWindow::playAudio(QString filename, int vol)
{
    if(QFile(filename).exists())
    {
        qDebug() << "playing:" << filename << vol;
        audioOutput->setVolume(qreal(vol) / qreal(100));
        media->setCurrentSource(QUrl(filename));
        media->play();
    }
}

void FastQSPWindow::stopAudio()
{
    media->stop();
}

void FastQSPWindow::openFile(const QString &filename)
{
    if(!QSPLoadGameWorld(filename.toStdWString().c_str()))
        qCritical() << QString("Could not open file: ") << filename;
    if(QSPRestartGame(QSP_TRUE))
    {
        gameMenu->setEnabled(true);
        gameDirectory = QFileInfo(filename).absolutePath() + "/";
        builder.setGameDir(gameDirectory);
        loadFonts();
        QFile configFile(gameDirectory + QLatin1String("config.xml"));
        if(configFile.open(QFile::ReadOnly))
        {
            QTextStream stream(&configFile);
            QString config = stream.readLine();
            configFile.close();

            QRegExp re;
            re.setPattern("width=\"(\\d+)\"");
            re.indexIn(config);
            gameWidth = re.cap(1).toInt();

            re.setPattern("height=\"(\\d+)\"");
            re.indexIn(config);
            gameHeight = re.cap(1).toInt();

            re.setPattern("title=\"(.+)\"");
            re.indexIn(config);
            setWindowTitle(re.cap(1));

        }
        aspectRatio = qreal(gameWidth) / qreal(gameHeight);
        mainView->resize(gameWidth, gameHeight);
        resize(gameWidth, gameHeight);
        loadPage();
    }
}

// That function is called by callback if isRefsresh == true
// according to the debug log that never happens. maybe I should remove it?
void FastQSPWindow::refreshView()
{
    qDebug() << "refreshView()";
}

void FastQSPWindow::loadPage()
{
    mainView->setHtml(builder.getHTML());
}

// TODO: maximize doesn't work properly
void FastQSPWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    QSize newSize;
    newSize = mainView->size();

    if(newSize.isValid())
    {
        int viewWidth, viewHeight;
        viewWidth = newSize.width();
        viewHeight = viewWidth / aspectRatio;
        if(viewHeight > newSize.height())
        {
            viewHeight = newSize.height();
            viewWidth = viewHeight * aspectRatio;
            //if(isMaximized() || isFullScreen())
        }
        mainView->page()->setViewportSize(QSize(viewWidth, viewHeight));
        scaleFactor = qreal(viewWidth) / qreal(gameWidth);
        mainView->setZoomFactor(scaleFactor);
    }
}

void FastQSPWindow::timerEvent(QTimerEvent *event)
{
    QMainWindow::timerEvent(event);
    QSPExecCounter(true);
}
