#ifndef META_H
#define META_H

#include <QString>

class Meta
{
    QString itag;
    QString url;
    QString extension;
    QString resolution;
public:
    Meta();

    void setItag(QString& itag);
    void setUrl(QString& url);
    void setExtension(QString& extension);
    void setResolution(QString& resolution);

    QString getItag();
    QString getUrl();
    QString getExtension();
    QString getResolution();

};

#endif // META_H
