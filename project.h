#ifndef PROJECT_H
#define PROJECT_H

#include <QStringList>
#include <QVector>
#include <QMap>

class Project {

public:
    //
    Project();
    ~Project();

    QString root;

    struct Keyword {
        //
        QString name;
        QString description;
        QStringList arguments;
    };

    QVector<Keyword> *keywords = nullptr;

    void loadProject(QString dir);
    QString loadTextFile(QString filename);
    void saveTextFile(QString filename, QString text);

    void loadKeywords();

    //QMap
};

#endif // PROJECT_H
