#ifndef MYQWEBPAGE_H
#define MYQWEBPAGE_H

#include <QObject>
#include <QWebPage>

class MyQWebPage : public QWebPage
{
    Q_OBJECT

public:
    MyQWebPage(QObject *parent = 0);

protected:
    bool acceptNavigationRequest(QWebFrame *, const QNetworkRequest &, NavigationType);

signals:
    void formSubmitted(const QNetworkRequest &);
};

#endif // MYQWEBPAGE_H
