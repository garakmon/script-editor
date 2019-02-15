#include "theme.h"

Theme::Theme() {
    this->backgroundColor = Qt::white;
    this->plaintextColor = Qt::black;
    this->keywordColor = Qt::darkBlue;
    this->functionColor = Qt::blue;
    this->labelColor = Qt::darkMagenta;
    this->quotedStringColor = Qt::darkGreen;
    this->commentColor = Qt::lightGray;
    this->invalidColor = Qt::red;
    this->numberColor = Qt::green;
    this->highlightColor = Qt::lightGray;
    this->highlightTextColor = Qt::black;
}

Theme::Theme(QColor background_, QColor plaintext_, QColor keyword_, 
             QColor function_, QColor label_, QColor quotedString_,
             QColor comment_, QColor invalid_, QColor number_,
             QColor highlight_, QColor highlightText_) {
    this->backgroundColor = background_;
    this->plaintextColor = plaintext_;
    this->keywordColor = keyword_;
    this->functionColor = function_;
    this->labelColor = label_;
    this->quotedStringColor = quotedString_;
    this->commentColor = comment_;
    this->invalidColor = invalid_;
    this->numberColor = number_;
    this->highlightColor = highlight_;
    this->highlightTextColor = highlightText_;
}

/*
        Theme(
            QColor(), // background
            QColor(), // plain text
            QColor(), // keyword
            QColor(), // function
            QColor(), // label
            QColor(), // quoted string
            QColor(), // comment
            QColor(), // invalid
            QColor(), // number
            QColor(), // highlight background
            QColor()  // highlighted text
        )
*/

QMap<QString, Theme> GlobalThemes = {
    //
    {
        "default", Theme()
    },
    {
        "monokai", Theme(
            QColor("#272822"), // background
            QColor("#F8F8F2"), // plain text
            QColor("#F92672"), // keyword
            QColor("#A6E22E"), // function
            QColor("#A6E22E"), // label
            QColor("#E6DB74"), // quoted string
            QColor("#75715E"), // comment
            QColor("#F92672"), // invalid highlight
            QColor("#AE81FF"), // number
            QColor("#3E3D32"), // highlight background
            QColor("#F8F8F2")  // highlighted text
        )
    }
};
