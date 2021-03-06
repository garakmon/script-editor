#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QVector>
#include <QString>
#include <QRegularExpression>

#include <QPlainTextEdit>
#include <QCompleter>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QObject>

#include "theme.h"
#include "project.h"

class LineNumberArea;
class Highlighter;

class Document : public QPlainTextEdit {

    Q_OBJECT

public:
    //
    Document(Project *project_, Theme theme_, QWidget *parent = nullptr);

    Project *project;

    Theme theme;

    QString filepath;
    QSet<QString> labels;// to jump around for labels

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    void setCompleter(QCompleter *c);
    int lineNumberAreaWidth();
    QCompleter *completer() const;

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth();
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);
    void insertCompletion(const QString &completion);

private:
    QWidget *lineNumberArea;
    QCompleter *completer_;

};



class LineNumberArea : public QWidget {

public:
    LineNumberArea(Document *editor) : QWidget(editor) {
        document = editor;
    }

    QSize sizeHint() const override {
        return QSize(document->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        document->lineNumberAreaPaintEvent(event);
    }

private:
    Document *document;
};



class Highlighter : public QSyntaxHighlighter {

    Q_OBJECT

public:
    Highlighter(Document *parent = 0);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    QVector<HighlightingRule> highlightingRules;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat scriptLabelFormat; // functionColor
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat; // quotedStringColor
    QTextCharFormat functionFormat;
    QTextCharFormat numberFormat;
    QTextCharFormat hexadecimalFormat;
};


/*
class TabWidget : public QTabWidget {
    //
public:
    TabWidget(QWidget *parent = 0) : QTabWidget(parent) {}

};
*/

#endif // DOCUMENT_H
