#include "document.h"

#include <QPainter>



Document::Document(Project *project_, Theme theme_, QWidget *parent) : QPlainTextEdit(parent), completer_(0) {
    this->project = project_;
    this->theme = theme_;
    QPalette pal = this->palette();
    pal.setColor(QPalette::Base, theme.backgroundColor); // background color
    pal.setColor(QPalette::Text, theme.plaintextColor); // text color
    pal.setColor(QPalette::Highlight, theme.highlightColor);// highlight
    pal.setColor(QPalette::HighlightedText, theme.highlightTextColor);// highlighted text
    setPalette(pal);

    lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth()));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    //connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    //
    QCompleter *completer = new QCompleter(project->keywords->keys(), this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    this->setCompleter(completer);

    updateLineNumberAreaWidth();
    //highlightCurrentLine();
};

int Document::lineNumberAreaWidth() {
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        digits++;
    }

    int space = 10 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}

void Document::updateLineNumberAreaWidth() {
    setViewportMargins(lineNumberAreaWidth() + 1, 0, 0, 0);
}

void Document::updateLineNumberArea(const QRect &rect, int dy) {
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth();
}

void Document::resizeEvent(QResizeEvent *e) {
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth() + 1, cr.height()));
}

// TODO: update this to highlight the lineNumberArea only
void Document::highlightCurrentLine() {
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(theme.highlightColor).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void Document::lineNumberAreaPaintEvent(QPaintEvent *event) {
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), theme.backgroundColor);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(theme.commentColor);
            painter.drawText(0, top, lineNumberArea->width() - 4, fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        blockNumber++;
    }

    painter.drawLine(lineNumberArea->width() - 1, 0, lineNumberArea->width() - 1, lineNumberArea->height());
}

void Document::setCompleter(QCompleter *c) {
    if (completer_)
        QObject::disconnect(completer_, 0, this, 0);

    completer_ = c;

    if (!completer_)
        return;

    completer_->setWidget(this);
    completer_->setCompletionMode(QCompleter::PopupCompletion);
    completer_->setCaseSensitivity(Qt::CaseInsensitive);
    QObject::connect(completer_, SIGNAL(activated(QString)),
                     this, SLOT(insertCompletion(QString)));
}

void Document::insertCompletion(const QString& completion) {
    if (completer_->widget() != this)
        return;
    QTextCursor tc = textCursor();
    int extra = completion.length() - completer_->completionPrefix().length();
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(completion.right(extra));
    setTextCursor(tc);
}

QCompleter *Document::completer() const {
    return completer_;
}























Highlighter::Highlighter(Document *parent) : QSyntaxHighlighter(parent->document()) {
    Theme theme = parent->theme;
    HighlightingRule rule;

    // decimal number values (this is imperfect)
    numberFormat.setForeground(theme.numberColor);
    rule.pattern = QRegularExpression("\\s+[\\d]+");
    rule.format = numberFormat;
    highlightingRules.append(rule);

    // hex number values (also probably imperfect)
    hexadecimalFormat.setForeground(theme.numberColor);
    rule.pattern = QRegularExpression("\\s+0x[\\dA-Fa-f]+");
    rule.format = numberFormat;
    highlightingRules.append(rule);

    // macros / keywords
    keywordFormat.setForeground(theme.keywordColor);
    keywordFormat.setFontWeight(QFont::Bold);
    for (const QString &keyword : parent->project->keywords->keys()) {
        rule.pattern = QRegularExpression(QString("\\b%1\\b").arg(keyword));
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }
    // hover over keyword gives description

    // script label
    scriptLabelFormat.setFontWeight(QFont::Bold);
    scriptLabelFormat.setForeground(theme.labelColor);
    rule.pattern = QRegularExpression("[A-Za-z0-9_]+[:]{1,2}");
    rule.format = scriptLabelFormat;
    highlightingRules.append(rule);

    // @ comments
    singleLineCommentFormat.setForeground(theme.commentColor);
    rule.pattern = QRegularExpression("@[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    // /* */ comments
    multiLineCommentFormat.setForeground(theme.commentColor);
    commentStartExpression = QRegularExpression("/\\*");
    commentEndExpression = QRegularExpression("\\*/");

    // strings surrounded by quotation marks
    quotationFormat.setForeground(theme.quotedStringColor);
    rule.pattern = QRegularExpression("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    // functions (what are examples in this context?)
    functionFormat.setFontItalic(true);
    functionFormat.setForeground(theme.functionColor);
    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);
}

void Highlighter::highlightBlock(const QString &text) {
    for (const HighlightingRule &rule : highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0) {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}

