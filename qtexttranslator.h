#ifndef QTEXTTRANSLATOR_H
#define QTEXTTRANSLATOR_H

#include <QtCore>

/**
 * @brief The QTextTranslator class
 *
 * QTextTranslator enables real-time translation execution
 * with text format (ini) directly editable by end users
 * without using normal ts format (qm as binary).
 *
 * The Text language files used by this class can be automatically generated
 * from the usual ts file using qbs bundled to this library.
 * Please refer to the QuickViewer source code for how to actually do it.
 *
 * This class implements the service by two instances. First create 'Reversed' instance.
 * Next you give the instance of Reversed to the constructor to create a real Translator instance.
 * Each key (eg. 'text_100003') described in the text file is temporarily set by this library,
 * and it is not related to your application.
 */
class QTextTranslator : public QTranslator
{
    Q_OBJECT
public:
    QTextTranslator(QObject* parent, QString path, QTextTranslator* reverse=nullptr);
    virtual ~QTextTranslator() {}

    QString translate(const char *context, const char *sourceText,
                                const char *disambiguation = Q_NULLPTR, int n = -1) const override;

    QString getString(const QString& group, const QString& key) const;

    bool isEmpty() const override;

protected:
    typedef QHash<QString, QString> InnerMap;
    typedef QHash<QString, InnerMap> TransMap;
    TransMap m_trans;
    QTextTranslator* m_reverse;
};


#endif // QTEXTTRANSLATOR_H
