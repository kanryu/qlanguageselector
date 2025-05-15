#include "qtexttranslator.h"


QTextTranslator::QTextTranslator(QObject *parent, QString path, QTextTranslator *reverse)
    : QTranslator (parent)
{
    m_reverse = reverse;
    QSettings settings(path, QSettings::IniFormat, this);

    // when initialized:
    // *this                                    *reverse
    // [Context]                                [Context]
    // text_xxxx=Translated Text    <--   -->   text_xxxx=Source Text
    //
    // when translate:
    // translate("Context", "Source Text") --> "Translated Text"
    //
    QStringList groups = settings.childGroups();
    foreach(const QString g, groups) {
        settings.beginGroup(g);
        InnerMap map;
        foreach(const QString& key, settings.allKeys()) {
            QString reversed = reverse != nullptr ? reverse->getString(g, key) : "";
            QString message = settings.value(key, reversed).toString();
            map.insert(reversed.isEmpty() ? key : reversed, message);
        }
        m_trans.insert(g, map);
        settings.endGroup();
    }
}

QString QTextTranslator::translate(const char *context, const char *sourceText, const char *disambiguation, int n) const
{
    QString con(context);
    QString source(sourceText);
    return getString(con, source);
}

QString QTextTranslator::getString(const QString &group, const QString &key) const
{
    auto itr = m_trans.find(group);
    if(itr == m_trans.end()) {
        return QString();
    }
    auto itr2 = itr.value().find(key);
    if(itr2 == itr.value().end()) {
        return QString();
    }
    return itr2.value();
}

bool QTextTranslator::isEmpty() const
{
    return false;
}
