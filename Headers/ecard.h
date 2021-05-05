#ifndef ECARD_H
#define ECARD_H
#include <QString>

class ECard
{
public:
    ECard();

    QString getTitle() const;
    void setTitle(const QString &value);

    QString getImage() const;
    void setImage(const QString &value);

    QString getCardInfo() const;
    void setCardInfo(const QString &value);

    QString getDate() const;
    void setDate(const QString &value);

    int getCurrentStep() const;
    void setCurrentStep(int value);

    int getId() const;
    void setId(int value);



private:
    int id;
    QString title;
    QString image;
    QString cardInfo;
    QString date;
    int currentStep;
};

#endif // ECARD_H
