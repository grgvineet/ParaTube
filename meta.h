#ifndef META_H
#define META_H

#include <QString>

class Meta
{
    int itag;
    QString url;
    QString extension;
    QString resolution;
    QString size;
public:
    Meta();

    void findVideoSize();

    int getItag() const;
    void setItag(const int &value);
    QString getUrl() const;
    void setUrl(const QString &value);
    QString getExtension() const;
    void setExtension(const QString &value);
    QString getResolution() const;
    void setResolution(const QString &value);
    QString getSize() const;
    void setSize(const QString &value);
};

#endif // META_H
