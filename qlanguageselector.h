#ifndef QLANGUAGESELECTOR_H
#define QLANGUAGESELECTOR_H

#include <QtGui>
#if QT_VERSION_MAJOR >= 5
#  include <QtWidgets>
#endif

#include "qtexttranslator.h"

#ifndef LANGUAGES_INI
  #define LANGUAGES_INI "languages.ini"
#endif
#ifndef LANGUAGE_DEFUALT
  #define LANGUAGE_DEFUALT "English"
#endif

struct LanguageInfo
{
    /**
     * @brief Code
     * Code page for the language. Usually, e.g. it may be 'en', or 'en_US'
     */
    QString Code;
    /**
     * @brief Caption
     * The language name which is human-readarable. e.g. 'English', 'Chinese(Simplified)'
     */
    QString Caption;
    /**
     * @brief Qm
     * The translation file name which is Qt standard(qm format).
     * If Qm is empty, and ForseUseText=false, this section will be default language for the application.
     */
    QString Qm;
    /**
     * @brief TextFile
     * The translation file name which is text(Internally, ini) format.
     * Users can edit the files.
     * And QLanguageSelector will load and change all 'tr' words every time when the menu is selected.
     */
    QString TextFile;
    /**
     * @brief OpenTextEditor
     * When user select the menu, open the 'TextFile' with a text editor, if the flag is true.
     */
    bool OpenTextEditor;
};


/**
 * @brief The QLanguageSelector class
 *
 * This object provides a mechanism for applications to make language changes by users.
 * Each language is registered as a child QAction menu of QMenu specified.
 * Each language is described in the ini file (usually "translations/languages.ini").
 * Allow users of the application to add new languages without changing the source code.
 * When the application user selects the Language Menu,
 *  the QLanguageSelector automatically sets the new QTranslator
 *  to qApp and replaces the language resources of the application.
 * Eventually the languageChanged signal is emitted and you can handle additional change events.
 */
class QLanguageSelector : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief QLanguageSelector
     * @param prefix: prefix of *.qm files  e.g. [prefix][langageId].qm
     * @param path: The path of the directory where the qm file is stored
     */
    QLanguageSelector(QString prefix, QString path="translations/");

    QString language() { return m_uiLanguage; }

    /**
     * @brief initialize
     *
     * Load language.ini and recognize the list of languages this application can switch.
     * Besides the usual binary(.qm) format, it supports text format (TransConf) converted from TS(.ts) format.
     */
    void initialize(QString path="");

    /**
     * @brief initializeMenu
     * @param parent
     *
     * Create a menu that allows you to change the display language.
     * For each specified parent menu, add each language registered in language.ini as a submenu.
     * Each menu is checkable, and the currently selected menu is checked.
     * When the user selects a menu, the display language of the application is changed.
     */
    void initializeMenu(QMenu* parent);

    /**
     * @brief getLanguageInfo
     * @param languageId
     *
     * Returns an instance of the language information specified by languageId.
     * This is the information set in languages.ini.
     */
    LanguageInfo getLanguageInfo(QString languageId);

    /**
     * @brief getSystemLanguageInfo
     *
     * Returns language information that matches the language resource
     *  of the environment in which the application is executed.
     * If it does not match, it returns the language information specified by LANGUAGE_DEFUALT.
     */
    LanguageInfo getSystemLanguageInfo();

    void setForceUseText(bool force) { m_forceUseText = force; }
    bool ForceUseText() const { return m_forceUseText; }
    void copyLanguages(QMap<QString, LanguageInfo>& languages) { m_languages = languages; }
    QMap<QString, LanguageInfo>& Languages() { return m_languages; }


signals:
    /**
     * @brief languageChanged
     * @param languageId
     *
     * Called after the language resource of the application has been replaced.
     * To change the language of Widgets other than QMainWindow
     *  you should receive this signal in your slot.
     */
    void languageChanged(QString languageId);
    /**
     * @brief openTextEditorForLanguage
     * @param info
     *
     * Called before the text translation file selected.
     */
    void openTextEditorForLanguage(LanguageInfo info);

public slots:
    /**
     * @brief resetTranslator
     * @param languageId: language name in English
     *
     * Replace the language resources of the application.
     */
    void resetTranslator(QString languageId);

private:
    QString m_uiLanguage;
    QTranslator *m_translator;
    QTextTranslator *m_reversed;
    QString m_path;
    QString m_prefix;
    QString m_reverseFile;
    bool m_forceUseText;
    bool m_textEditorOpened;
    bool m_userSelecting;

    QMap<QString, LanguageInfo> m_languages;
    QList<QAction*> m_actions;
    QStringList m_languageList;

    void clearLanguageMenus();
};

#endif // QLANGUAGESELECTOR_H
