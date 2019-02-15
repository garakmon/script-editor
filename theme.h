#ifndef THEME_H
#define THEME_H

#include <QColor>
#include <QMap>

class Theme {
public:
    Theme();
    Theme(QColor, QColor, QColor, QColor, QColor, QColor, QColor, QColor, QColor, QColor, QColor);

    QColor backgroundColor;
    QColor plaintextColor;
    QColor keywordColor;
    QColor functionColor;
    QColor labelColor;
    QColor quotedStringColor;
    QColor commentColor;
    QColor invalidColor;
    QColor numberColor;
    QColor highlightColor;
    QColor highlightTextColor;
};

extern QMap<QString, Theme> GlobalThemes;

#endif // THEME_H
