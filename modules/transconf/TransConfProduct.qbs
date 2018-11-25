import qbs
import qbs.FileInfo
import qbs.ModUtils
import qbs.PropertyList
import qbs.TextFile
import qbs.Utilities
import qbs.Xml

Product {
//    Depends { name: "qbsbuildconfig" }
    Depends { name: "transconf" }
    name: "Generating TransConfs"
    version: "1.1.2" // your application version
    

    type: ["Conf type transrations"]
    property string confExt: ".lang" // file extent of your conf type translation files, usually ".lang", ".lng", ".txt" or ".ini"

    // if you want to change the file name of conf type translations, define the mapping
    property var tsFileNameMap /*: { 
        return {
            "qlanguageseletor_ja.ts": "aaa.txt",
        }
    }*/
    property string reverseConfName: "Reverse" // the reverse translation conf file name
    property var template: {
//        console.info(["qbsbuildconfig.appInstallDir",qbsbuildconfig.appInstallDir])
        console.info(["buildDirectory",buildDirectory])
        return {
            file_header: [
                // File header is the top text of the generated translation confs
                "; Language file was generated at "+Generator.toMailDateString(new Date(Date.now())),
                "; === COMMENTS ===",
                "; This is a language file for "+project.name+". Lines beginning with ; are comments.",
                "; Each language should have one language file in UTF-8 character set.",
                "; When you change the text of this file, the notation of the application will be changed.",
                "; ",
                "; Note 1: Use \\n (backslash+n) instead of new line for each text, if you need.",
                "; Note 2: 'hint' is a hint comment for each 'text'.",
                "; ",
            ],
            // first section name (default: [FileInfo])
            section_name: "FileInfo",
            // keys in the first section
            entries: {
                TranslatorName: "QLanguageSelector Developer Team",
                TranslatorEmail: "k.kanryu@gmail.com",
                Caption: "language caption",
                LanguageName: "language name(e.g. English)",
                LanguageCode: "language code(e.g. en)",
                TargetVersion: "your application version",
            }
        }
    }
    Group {
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: product.installDir ? product.installDir : "translations"
        qbs.installSourceBase: buildDirectory
    }
}
