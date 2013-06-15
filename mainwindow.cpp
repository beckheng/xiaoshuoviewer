#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    manager = new QNetworkAccessManager(this);
    diskCache = new QNetworkDiskCache(this);
    diskCache->setCacheDirectory("testqtnetworkmanager");
    manager->setCache(diskCache);

    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));

    this->webPage = new QWebPage(this);
    this->webPage->setNetworkAccessManager(manager);

    this->ui->webView->setPage(this->webPage);

#ifdef Q_OS_WIN
    this->ui->saveTo->setText(QString("C:/temp/ud"));
#else
    this->ui->saveTo->setText(QString("/tmp/ud"));
#endif

    this->ui->urlText->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;

    delete this->diskCache;
    delete this->manager;

    delete this->webPage;
}

void MainWindow::on_pushButton_open_clicked()
{
    // 检查保存目录是否已经设定
    if (this->ui->saveTo->text().isEmpty())
    {
        QMessageBox::warning(this, "请先选择保存目录", "请先选择保存目录");
        return;
    }

    this->ui->pushButton_open->setDisabled(true);

    this->diskCache->clear();

    // 先删除保存目录
    QDir dir(this->ui->saveTo->text());
    if (dir.exists())
    {
        dir.rmpath(dir.path());
    }

    this->ui->webView->load(QUrl(this->ui->urlText->text()));
}

void MainWindow::on_webView_loadProgress(int progress)
{
    this->setWindowTitle(QString("加载进度 %1%").arg(progress));
}

void MainWindow::on_webView_loadFinished(bool ok)
{
    if (ok)
    {
        this->setWindowTitle(this->ui->webView->title());

        this->ui->pushButton_open->setDisabled(false);
    }
    else
    {
        this->setWindowTitle("加载页面失败，请检查!");
    }
}

void MainWindow::replyFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QString path = reply->url().path();

        if (path.endsWith(".js") || path.endsWith(".css")
                || path.endsWith(".jpg") || path.endsWith(".gif")
                || path.endsWith(".bmp")
                || path.endsWith(".htm") || path.endsWith(".html")
                || path.endsWith(".pl") || path.endsWith(".php") || path.endsWith(".jsp"))
        {
            // 保存数据至文件
            QString fileName = QString("%1%2").arg(this->ui->saveTo->text()).arg(path);
            QFileInfo fileInfo(fileName);
            if (!fileInfo.exists())
            {
                fileInfo.dir().mkpath(fileInfo.dir().path());
            }

            if (reply->manager()->cache()->data(reply->url()))
            {
                QFile file(fileName);
                file.open(QIODevice::WriteOnly);
                file.write(reply->manager()->cache()->data(reply->url())->readAll());
                file.close();
            }
        }
    }
    else
    {
        qDebug() << reply->errorString();
    }
}

void MainWindow::on_chooseDir_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "选择保存目录");
    if (!dir.isEmpty())
    {
        this->ui->saveTo->setText(dir);
    }
}

void MainWindow::on_viewDir_clicked()
{
    // TODO:
#ifdef Q_OS_WIN
    QDesktopServices::openUrl(QUrl(QString("file:///%1").arg(this->ui->saveTo->text()), QUrl::TolerantMode));
#else
    QDesktopServices::openUrl(QUrl(QString("file://%1").arg(this->ui->saveTo->text()), QUrl::TolerantMode));
#endif
}
