#include "mainwindow.h"
#include "ui_mainwindow.h"

//#include <QMessageBox>

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
    this->downResource = 0;

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

        this->ui->pushButton_save->setText(QString("保存资源(%1)").arg(this->downResource));
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

        if (this->ui->webView->url().path() != path)
        {
            // 判断是cache数据还是网络上的数据
            bool isFromCache = reply->attribute(QNetworkRequest::SourceIsFromCacheAttribute).toBool();
            if (isFromCache)
            {
                qDebug() << "isFromCache = " << path;
            }

            this->okResource++;

            // 保存数据至文件
            QString fileName = QString("d:/testeee%1").arg(path);
            QFileInfo fileInfo(fileName);
            if (!fileInfo.exists())
            {
                fileInfo.dir().mkpath(fileInfo.dir().path());
            }

            QFile file(fileName);
            file.open(QIODevice::WriteOnly);
            file.write((reply->manager()->cache()->data(reply->url()))->readAll());
            file.close();
        }
    }
    else
    {
        qDebug() << reply->errorString();
    }
}

void MainWindow::on_pushButton_save_clicked()
{
    //
}
