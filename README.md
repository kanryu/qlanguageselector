# QLanguageSelector
It is a portable component that makes it possible to add and switch language resources of an application by users.

![qlanguagemenu](https://user-images.githubusercontent.com/759165/49052552-b3479e00-f22f-11e8-87e3-e1568d29413d.png)

## Summary

This library provides a mechanism for applications to make language changes by users.

Each language is registered as a child QAction menu of QMenu specified.

Each language is described in the ini file (usually "translations/languages.ini").

Allow users of the application to add new languages without changing the source code.

When the application user selects the Language Menu,
 the QLanguageSelector automatically sets the new QTranslator
 to qApp and replaces the language resources of the application.

Eventually the languageChanged signal is emitted and you can handle additional change events.


This is a part of [QuickViewer](https://github.com/kanryu/quickviewer).

## language.ini

A list of translation files of your application is defined in language.ini. It has the following format.

```
[QLanguageSelector]
ForceUseText=false      <-- If you always use text-based translation, set it to true
ReverseFile=Reverse.txt <-- The file name of the reverse translation file is described here
[English]               <-- Language Group
Code=en                 <-- Language Code(e.g. 'en' or 'en_US')
Caption=English         <-- Language name which is used on main menu
Qm=                     <-- Qt based translation file name
[Spanish]
Code=es
Caption=Spanish
Qm=quickviewer_es.qm
TextFile=Spanish.txt    <-- Text based translation
[YourLanguage]
Code=
Caption=YourLanguage
Qm=
TextFile=YourLanguage.txt
OpenTextEditor=true     <-- Set to true if you want to trigger an event to edit a text translation
...
```

Implementation example to see: [languages.ini of QuickViewer](https://github.com/kanryu/quickviewer/blob/master/QuickViewer/translations/languages.ini)

With the exception of the first QLanguageSelector group, the menu is displayed in this file in the order of the groups you described.

The translated file may be either a normal qm format or a text format provided by this library.

Since QLanguageSelector reads language.ini at its initialization and generates QActions into the parent QMenu to switch translation languages, you do not need to rebuild your program when adding and editing translation languages.

## Text based translations

This library supports text based translation. It is an ini file of the following format. (For convenience of the user, it may be better not to have the extension **ini**)

```
; Language file was generated at Sun, 25 Nov 2018 21:09:55 +0900
; === COMMENTS ===
; This is a language file for QuickViewer. Lines beginning with ; are comments.
; Each language should have one language file in UTF-8 character set.
; When you change the text of this file, the notation of the application will be changed.
; If you make a new translation or modify an existing one,
; please post to Issue(https://github.com/kanryu/quickviewer/issues) or send an email to <k.kanryu@gmail.com>.
; 
; Note 1: Use \n (backslash+n) instead of new line for each text, if you need.
; Note 2: 'hint' is a hint comment for each 'text'.
; 

[MainWindow]
text_109001=&Archivo
text_109002=&Ver
text_109003=&Ayuda
;hint109004: End application
text_109004=&Salir
;hint109005: Automatically resize the image according to the size of the window
text_109005=&Agrandamiento o Reducción de imagen automático
text_109006=&Pantalla completa
text_109007=QuickViewer
;hint109008: A menu for displaying a dialog for selecting an image to be displayed
text_109008=&Abrir Carpeta de Imagenes
text_109009=&Historia
...
```

Implementation example to see: [Japanese.txt](https://gist.github.com/kanryu/e935cd60afbd0f7f770755dd1113ff99)

The *text_XXXXXX* part is automatically generated, so you can not convert perfectly with this single translation file. Always requires a **reverse translation** file.

The part of *hintXXXXXX* can be written in the second argument of **tr()** function. However, Qt's translation file is clever, so if the character string of the first argument is the same but the annotation of the second argument is different, it becomes a different translation word, but since this library can **NOT** be distinguished, such description should not be done.

## Generate text based translation

These translation files are automatically converted from the ts file of your application.

The conversion process can be described by qbs. It would be easy if you made the project with qbs.

Implementation example to see: [maketransconf.qbs](https://github.com/kanryu/quickviewer/blob/master/QuickViewer/translations/maketransconf.qbs)


If your project is written in qmake, you need a little twist, but automation is possible. Please add the following settings to your installation process.


    install_translations.path = $${MY_DEFAULT_INSTALL}/translations
    install_translations.commands = $$shell_path($$[QT_INSTALL_BINS]/../../../Tools/QtCreator/bin/qbs) -f $${PWD}/translations/maketransconf.qbs qbs.installRoot:$${MY_DEFAULT_INSTALL}
    install_translations.files = \
        $${PWD}/translations/languages.ini \
        $${PWD}/translations/quickviewer_ja.qm \
        $${PWD}/translations/quickviewer_es.qm \
        $${PWD}/translations/quickviewer_zh.qm \
        $${PWD}/translations/quickviewer_el.qm \
        $${PWD}/translations/qt_el.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qt_zh_CN.qm \

The entire project file to see: [QuickViewer.pro](https://github.com/kanryu/quickviewer/blob/master/QuickViewer/QuickViewer.pro)

## License
BSD-2
Copyright (c) 2017, KATO Kanryu


