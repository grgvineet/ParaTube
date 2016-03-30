#include "meta.h"

Meta::Meta()
{

}

void Meta::setItag(QString &itag)
{
    this->itag = itag;
}

void Meta::setUrl(QString &url)
{
    this->url = url;
}

void Meta::setExtension(QString &extension)
{
    this->extension = extension;
}

void Meta::setResolution(QString &resolution)
{
    this->resolution = resolution;
}

QString Meta::getItag()
{
    return this->itag;
}

QString Meta::getUrl()
{
    return this->url;
}

QString Meta::getExtension()
{
    return this->extension;
}

QString Meta::getResolution()
{
    return this->resolution;
}



