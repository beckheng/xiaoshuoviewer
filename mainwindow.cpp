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
    this->webPage->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

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

        QString url = this->ui->webView->url().toString();
        // 关闭妨碍阅读的广告窗口
        // epzw.com: ED_CloseIt()
        QWebFrame *frame = this->webPage->mainFrame();
        if (url.contains("epzw.com"))
        {
            frame->evaluateJavaScript(QString("ED_CloseIt();"));
        }
        else if (url.contains("to59.com"))
        {
            // TODO
        }
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
    }
    else
    {
        qDebug() << reply->errorString();
    }
}

void MainWindow::on_epzwSite_clicked()
{
    this->ui->urlText->setText(QString("http://www.epzw.com/"));

    this->on_pushButton_open_clicked();
}

void MainWindow::on__59toSite_clicked()
{
    this->ui->urlText->setText(QString("http://www.59to.com/"));

    this->on_pushButton_open_clicked();
}

void MainWindow::on_webView_linkClicked(const QUrl &url)
{
    QString oldUrl = this->ui->urlText->text();

    bool isOpen = true;

    if (!oldUrl.isEmpty() && !oldUrl.contains(url.host()))
    {
        // 咨询是否打开外链
        isOpen = QMessageBox::Yes == QMessageBox::question(this, "外链咨询", QString("是否打开外链: %1 ?").arg(url.toString()), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    }

    if (isOpen)
    {
        this->ui->urlText->setText(url.toString());
        this->on_pushButton_open_clicked();
    }
}
