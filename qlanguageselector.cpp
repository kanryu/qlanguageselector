#include "qlanguageselector.h"

QLanguageSelector::QLanguageSelector(QString prefix, QString path)
    : QObject(nullptr)
    , m_translator(nullptr)
    , m_path(path)
    , m_prefix(prefix)
{
    initialize();
}


LanguageInfo QLanguageSelector::getLanguageInfo(QString languageId)
{
    if(m_languages.contains(languageId))
        return m_languages[languageId];
    return getSystemLanguageInfo();
}

LanguageInfo QLanguageSelector::getSystemLanguageInfo()
{
    auto systemLocale = QLocale::system();
    foreach(const QString& lang, m_languageList) {
        LanguageInfo info = m_languages[lang];
        auto locale = QLocale(info.Code);
        if(locale.name() == systemLocale.name())
            return info;
    }
    return m_languages[LANGUAGE_DEFUALT];
}

void QLanguageSelector::resetTranslator(QString languageId)
{
    LanguageInfo info = getLanguageInfo(languageId);
    m_uiLanguage = info.Caption;
    if(m_translator) {
        qApp->removeTranslator(m_translator);
        delete m_translator;
    }
    m_translator = new QTranslator;
    bool exist = m_translator->load(QLocale(info.Code),
                                 "", m_prefix, m_path);
    if(exist) {
        qApp->installTranslator(m_translator);
    } else {
        delete m_translator;
        m_translator = nullptr;
    }
    emit languageChanged(info.Caption);
}

void QLanguageSelector::initialize()
{
    QString inipath = QString("%1/%2").arg(m_path).arg(LANGUAGES_INI);
    QSettings settings(inipath, QSettings::IniFormat, this);
    settings.setIniCodec(QTextCodec::codecForName("UTF-8"));

    QStringList groups = settings.childGroups();
    foreach(const QString g, groups) {
        settings.beginGroup(g);
        LanguageInfo info = {
            settings.value("code", "").toString(),
            settings.value("caption", "").toString(),
            settings.value("qm", "").toString(),
        };
        m_languages[info.Caption] = info;
        m_languageList << info.Caption;
        settings.endGroup();
    }
}

void QLanguageSelector::clearLanguageMenus()
{
    foreach(QAction *action, m_actions) {
        action->setChecked(false);
    }
}

void QLanguageSelector::initializeMenu(QMenu* parent)
{
    auto self = this;
    foreach(const QString& lang, m_languageList) {
        LanguageInfo info = m_languages[lang];
        QAction* action = parent->addAction(info.Caption);
        action->setCheckable(true);
        if(m_uiLanguage == info.Caption) {
            action->setChecked(true);
        }
        connect(action, &QAction::triggered, this, [=]() {
            clearLanguageMenus();
            action->setChecked(true);
            self->resetTranslator(info.Caption);
        });
        m_actions << action;
    }
}
