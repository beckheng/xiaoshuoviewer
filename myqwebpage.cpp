#include "myqwebpage.h"

MyQWebPage::MyQWebPage(QObject *parent): QWebPage(parent)
{
}

bool MyQWebPage::acceptNavigationRequest(QWebFrame *frame, const QNetworkRequest &request, QWebPage::NavigationType type)
{
    if (type == NavigationTypeFormSubmitted)
    {
        emit formSubmitted(request);
        return false;
    }

    return QWebPage::acceptNavigationRequest(frame,request,type);
}
