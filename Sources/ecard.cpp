#include "ecard.h"

ECard::ECard()
{
    id = 0;
}

QString ECard::getTitle() const
{
    return title;
}

void ECard::setTitle(const QString &value)
{
    title = value;
}

QString ECard::getImage() const
{
    return image;
}

void ECard::setImage(const QString &value)
{
    image = value;
}

QString ECard::getCardInfo() const
{
    return cardInfo;
}

void ECard::setCardInfo(const QString &value)
{
    cardInfo = value;
}

QString ECard::getDate() const
{
    return date;
}

void ECard::setDate(const QString &value)
{
    date = value;
}

int ECard::getCurrentStep() const
{
    return currentStep;
}

void ECard::setCurrentStep(int value)
{
    currentStep = value;
}

int ECard::getId() const
{
    return id;
}

void ECard::setId(int value)
{
    id = value;
}
