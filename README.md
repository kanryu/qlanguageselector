# qlanguageselector
It is a portable component that makes it possible to add and switch language resources of an application by users.

## Summary

This object provides a mechanism for applications to make language changes by users.

Each language is registered as a child QAction menu of QMenu specified.

Each language is described in the ini file (usually "translations/languages.ini").

Allow users of the application to add new languages without changing the source code.

When the application user selects the Language Menu,
 the QLanguageSelector automatically sets the new QTranslator
 to qApp and replaces the language resources of the application.

Eventually the languageChanged signal is emitted and you can handle additional change events.


This is a part of [QuickViewer](https://github.com/kanryu/quickviewer).

## License
BSD-2
Copyright (c) 2017, KATO Kanryu


