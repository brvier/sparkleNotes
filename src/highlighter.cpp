#include <QtGui>

#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{

}

void Highlighter::updateRules()
{
    HighlightingRule rule;

    highlightingRules.clear();

    italicFormat.setFontItalic(true);
    rule.pattern = QRegExp("\\*([^\\\\]+)\\*");
    rule.pattern.setMinimal(true);
    rule.format = italicFormat;
    highlightingRules.append(rule);

    boldFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegExp("\\*\\*([^\\\\]+)\\*\\*");
    rule.pattern.setMinimal(true);
    rule.format = boldFormat;
    highlightingRules.append(rule);

    boldItalicFormat.setFontWeight(QFont::Bold);
    boldItalicFormat.setFontItalic(true);
    rule.pattern = QRegExp("\\*\\*\\*([^\\\\]+)\\*\\*\\*");
    rule.pattern.setMinimal(true);
    rule.format = boldItalicFormat;
    highlightingRules.append(rule);




    header6Format.setForeground(QColor(m_secondaryHighlightColor));
    header6Format.setFontPointSize(m_baseFontPointSize*0.6);
    rule.pattern = QRegExp("^######\\s(.+)");
    rule.format = header6Format;
    highlightingRules.append(rule);

    header5Format.setForeground(QColor(m_secondaryHighlightColor));
    header5Format.setFontPointSize(m_baseFontPointSize*0.7);
    rule.pattern = QRegExp("^#####\\s(.+)");
    rule.format = header5Format;
    highlightingRules.append(rule);

    header4Format.setForeground(QColor(m_secondaryHighlightColor));
    header4Format.setFontPointSize(m_baseFontPointSize*0.8);
    rule.pattern = QRegExp("^####\\s(.+)");
    rule.format = header4Format;
    highlightingRules.append(rule);

    header3Format.setForeground(QColor(m_secondaryHighlightColor));
    header3Format.setFontPointSize(m_baseFontPointSize*0.9);
    rule.pattern = QRegExp("^###\\s(.+)");
    rule.format = header3Format;
    highlightingRules.append(rule);

    header2Format.setForeground(QColor(m_secondaryHighlightColor));
    header2Format.setFontPointSize(m_baseFontPointSize*1.1);
    rule.pattern = QRegExp("^##\\s(.+)");
    rule.format = header2Format;
    highlightingRules.append(rule);

    header1Format.setForeground(QColor(m_secondaryHighlightColor));
    header1Format.setFontPointSize(m_baseFontPointSize*1.2);
    rule.pattern = QRegExp("^#\\s(.+)");
    rule.format = header1Format;
    highlightingRules.append(rule);

    imageFormat.setForeground(QColor(m_highlightColor));
    rule.pattern = QRegExp("!\\[(.*)\\]\\((.*)\\)");
    rule.pattern.setMinimal(true);
    rule.format = imageFormat;
    highlightingRules.append(rule);

    linkFormat.setForeground(QColor(m_highlightColor));
    rule.pattern = QRegExp("\\[(.*)\\]\\((.*)\\)");
    rule.pattern.setMinimal(true);
    rule.format = linkFormat;
    highlightingRules.append(rule);

    rule.pattern = QRegExp("^--*$");
    rule.format = header2Format;
    highlightingRules.append(rule);

    rule.pattern = QRegExp("^==*$");
    rule.format = header1Format;
    highlightingRules.append(rule);

}

void Highlighter::highlightBlock(const QString &text)
{
    if (m_baseFontPointSize == 0.0)
            return;

    if (this->currentBlock().blockNumber() == 0)
        setFormat(0, this->currentBlock().length(), header1Format);
    else {
        QRegExp title("^==*$");
        QRegExp subtitle("^--*$");

        //qDebug() << this->currentBlock().next().text();

        if (title.indexIn(this->currentBlock().next().text()) >= 0) {
            setFormat(0, this->currentBlock().length(), header1Format);
        }
        if (subtitle.indexIn(this->currentBlock().next().text()) >= 0) {
            setFormat(0, this->currentBlock().length(), header1Format);
        }

    }

    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
}

void Highlighter::setStyle(QString primaryColor,
                           QString secondaryColor,
                           QString highlightColor,
                           QString secondaryHighlightColor,
                           qreal baseFontPointSize)
{
    m_primaryColor = QString(primaryColor);
    m_secondaryColor = QString(secondaryColor);
    m_highlightColor = QString(highlightColor);
    m_secondaryHighlightColor = QString(secondaryHighlightColor);
    m_baseFontPointSize = baseFontPointSize;
    this->updateRules();
    this->rehighlight();
}
