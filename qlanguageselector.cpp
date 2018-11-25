#include "qlanguageselector.h"

QLanguageSelector::QLanguageSelector(QString prefix, QString path)
    : QObject(nullptr)
    , m_translator(nullptr)
    , m_reversed(nullptr)
    , m_path(path)
    , m_prefix(prefix)
    , m_forceUseText(false)
    , m_textEditorOpened(false)
    , m_userSelecting(false)
{
    //initialize();
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
        m_translator = nullptr;
    }
    if(!info.TextFile.isEmpty() && (m_forceUseText || info.OpenTextEditor)) {
        QDir translationDir(m_path);
        if(m_reversed == nullptr && !m_reverseFile.isEmpty()) {
            m_reversed = new QTextTranslator(this, translationDir.filePath(m_reverseFile));
        }
        if(m_reversed != nullptr) {
            m_translator = new QTextTranslator(this, translationDir.filePath(info.TextFile), m_reversed);
            qApp->installTranslator(m_translator);
        }
    } else {
        m_translator = new QTranslator;
        bool exist = m_translator->load(QLocale(info.Code),
                                     "", m_prefix, m_path);
        if(exist) {
            qApp->installTranslator(m_translator);
        } else {
            delete m_translator;
            m_translator = nullptr;
        }
    }

    emit languageChanged(info.Caption);
}

void QLanguageSelector::initialize(QString path)
{
    QString inipath = path.isEmpty() ? QString("%1/%2").arg(m_path).arg(LANGUAGES_INI) : path;
    QSettings settings(inipath, QSettings::IniFormat, this);
    settings.setIniCodec(QTextCodec::codecForName("UTF-8"));

    QStringList groups = settings.childGroups();
    foreach(const QString g, groups) {
        settings.beginGroup(g);
        if(g == "QLanguageSelector") {
            m_reverseFile = settings.value("ReverseFile", "").toString();
            m_forceUseText = settings.value("ForceUseText", false).toBool();
        } else {
            LanguageInfo info = {
                settings.value("Code", "").toString(),
                settings.value("Caption", "").toString(),
                settings.value("Qm", "").toString(),
                settings.value("TextFile", "").toString(),
                settings.value("OpenTextEditor", false).toBool(),
            };
            m_languages[info.Caption] = info;
            m_languageList << info.Caption;
        }
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
    bool useText = false;
    foreach(const QString& lang, m_languageList) {
        LanguageInfo info = m_languages[lang];
        if(!info.TextFile.isEmpty())
            useText = true;
        if(!info.OpenTextEditor) {
            QAction* action = parent->addAction(info.Caption);
            action->setCheckable(true);
            if(m_uiLanguage == info.Caption) {
                action->setChecked(true);
            }
            connect(action, &QAction::triggered, this, [=]() {
                clearLanguageMenus();
                action->setChecked(true);
                m_userSelecting = true;
                self->resetTranslator(info.Caption);
                m_userSelecting = false;
            });
            m_actions << action;
            continue;
        }
        parent->addSeparator();
        QAction* action = parent->addAction(info.Caption);
        action->setCheckable(true);
        if(m_uiLanguage == info.Caption) {
            action->setChecked(true);
        }
        connect(action, &QAction::triggered, this, [=]() {
            clearLanguageMenus();
            action->setChecked(true);
            m_userSelecting = true;
            self->resetTranslator(info.Caption);
            m_userSelecting = false;
            if(!m_textEditorOpened && info.OpenTextEditor) {
                emit openTextEditorForLanguage(info);
                m_textEditorOpened = true;
            }
        });
        m_actions << action;
    }
    if(useText) {
        parent->addSeparator();
        QAction* action = parent->addAction(tr("Always use text translation", "Menu text that uses textual translation rather than regular qm format"));
        action->setCheckable(true);
        if(m_forceUseText) {
            action->setChecked(true);
        }
        connect(action, &QAction::triggered, this, [=]() {
            m_forceUseText = !m_forceUseText;
            m_userSelecting = true;
            self->resetTranslator(m_uiLanguage);
            m_userSelecting = false;
        });
    }
}
