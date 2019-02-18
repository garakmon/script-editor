#include "project.h"

#include <QDebug>
#include <QRegularExpression>
#include <QFile>



Project::Project() {
    //
    keywords = new QMap<QString, Keyword>;
    // TODO: DELETE!!
    loadProject("../emerald");
}

Project::~Project() {
    delete keywords;
}

void Project::loadProject(QString dir) {
    //
    qDebug() << "loading project from" << dir;
    this->root = dir;
    loadKeywords();

    // TEST
    for (auto key : keywords->keys())
        qDebug() << key << keywords->value(key).description << keywords->value(key).arguments;
}

QString Project::loadTextFile(QString filepath) {
    //
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << QString("Could not open file '%1' for reading.").arg(filepath) + file.errorString();
        return QString();
    }
    QTextStream in(&file);
    in.setCodec("UTF-8");
    QString text = "";
    while (!in.atEnd()) {
        text += in.readLine() + "\n";
    }
    return text;
}

void Project::saveTextFile(QString filepath, QString text) {
    QFile file(filepath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(text.toUtf8());
    } else {
        qDebug() << QString("Could not open file '%1' for writing.").arg(filepath) + file.errorString();
    }
}

void Project::loadKeywords() {
    //
    QString events_inc = loadTextFile(root + "/asm/macros/event.inc");

    if (events_inc.isEmpty()) return;

    QRegularExpression re("[@]{0,1}\\s+(?<desc>.*)\\s+.macro\\s+(?<macro>[A-Za-z0-9_]+)\\s+(?<args>[A-Za-z0-9:,=_@\\\\ ]*)\\s+[A-Za-z0-9.,_\\s\\\\|&\\(\\)><\'\"=]*endm");//
    QRegularExpressionMatchIterator iter = re.globalMatch(events_inc);

    while (iter.hasNext()) {
        QRegularExpressionMatch match = iter.next();
        QString name = match.captured("macro");
        QString desc = match.captured("desc").remove("@").trimmed();
        QString args = match.captured("args");
        args.remove(QRegularExpression("@.*"));// remove comments captured in args

        QStringList arguments;
        QRegularExpression arg_re("(?<arg>[A-Za-z_]+)[:A-Za-z]*");
        QRegularExpressionMatchIterator arg_iter = arg_re.globalMatch(args);

        while (arg_iter.hasNext()) {
            QRegularExpressionMatch arg_match = arg_iter.next();
            QString arg = arg_match.captured("arg");
            if (arg != "req")
                arguments.append(arg);
        }
        Keyword keyword = {desc, arguments};

        keywords->insert(name, keyword);
    }
}











