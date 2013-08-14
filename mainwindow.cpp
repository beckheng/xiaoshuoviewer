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

    this->on_pushButton_recommend_clicked();

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

    if (!(oldUrl.compare(QString("http://")) == 0 || oldUrl.compare(QString("")) == 0))
    {
        if (!oldUrl.isEmpty() && !this->isSameDomain(oldUrl, url.host()))
        {
            // 咨询是否打开外链
            isOpen = QMessageBox::Yes == QMessageBox::question(this, "外链咨询", QString("是否打开外链: %1 ?").arg(url.toString()), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        }
    }

    if (isOpen)
    {
        this->ui->urlText->setText(url.toString());
        this->on_pushButton_open_clicked();
    }
}

bool MainWindow::isSameDomain(QString url1, QString url2)
{
    QUrl curUrl = QUrl(url1);

    QStringList list1 = curUrl.host().replace(QString("http://"), "").replace(QString("https://"), "").replace(QString("/"), "").split(QString("."));
    QStringList list2 = url2.replace(QString("http://"), "").replace(QString("https://"), "").replace(QString("/"), "").split(QString("."));

    QVector<QString> v1;
    QVector<QString> v2;

    for (int i = list1.size() - 1; i > 0; i--)
    {
        v1.append(list1.at(i));
    }

    for (int i = list2.size() - 1; i > 0; i--)
    {
        v2.append((list2.at(i)));
    }

    return (v1.at(0).compare(v2.at(0)) == 0) && (v1.at(1).compare(v2.at(1)) == 0);
}

void MainWindow::on_pushButton_qidian_clicked()
{
    this->ui->urlText->setText(QString("http://www.qidian.com/"));

    this->on_pushButton_open_clicked();
}

void MainWindow::on_pushButton_motie_clicked()
{
    this->ui->urlText->setText(QString("http://www.motie.com/"));

    this->on_pushButton_open_clicked();
}

void MainWindow::on_pushButton_recommend_clicked()
{
    this->ui->urlText->setText(QString("http://"));

    this->ui->webView->load(QUrl(QString("http://www.yixinit.com/xiaoshuo.html")));
}
