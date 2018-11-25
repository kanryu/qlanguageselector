import qbs
import qbs.FileInfo
import qbs.ModUtils
import qbs.PropertyList
import qbs.TextFile
import qbs.Utilities
import qbs.Xml
import "./main.js" as Main


Module {
    name: "transconf"

    property var Generator: {
        function traceNode(node, cnt) {
            if(!cnt) cnt = 0;
            console.error([cnt+0, "node: ", node]);
            console.error([cnt+1, "isCDATASection: ", node.isCDATASection()]);
            console.error([cnt+2, "isElement: ", node.isElement()]);
            console.error([cnt+3, "isText: ", node.isText()]);
            if(node.isElement())
                console.error([cnt+4, "tag: <" + node.tagName()+ ">"]);
        }

        function toISOString(date) {
            function format2(num) { return num < 10 ? "0" + num : num;}
            return (1900+date.getYear())
                    + "-" + format2(date.getUTCMonth())
                    + "-" + format2(date.getUTCDate())
                    + "T" + format2(date.getUTCHours())
                    + ":" + format2(date.getUTCMinutes())
                    + ":" + format2(date.getUTCSeconds())
                    + "Z"
            ;
        }
        function toMailDateString(date) {
            function format2(num) { return num < 10 ? "0" + num : num;}
            function timezone(tz) {
                if(tz < 0) {
                    tz = -tz;
                    return "+" +format2(tz / 60) + format2(tz%60);
                }
                return "-" +format2(tz / 60) + format2(tz%60);
            }

            return ["Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"][date.getDay()]
                    + ", " + format2(date.getUTCDate())
                    + " " + ["Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"][date.getUTCMonth()]
                    + " " + (1900+date.getYear())
                    + " " + format2(date.getHours())
                    + ":" + format2(date.getMinutes())
                    + ":" + format2(date.getSeconds())
                    + " " + timezone(date.getTimezoneOffset())
            ;
        }
        return {
            "toMailDateString": toMailDateString,
            "toISOString": toISOString,
        }
    }

    property var Languages: {
        return {
            af: 'Afrikaans',
            am: 'Amharic',
            ar: 'Arabic',
            ar_AE: 'Arabic(UnitedArabEmirates)',
            ar_BH: 'Arabic(Bahrain)',
            ar_DZ: 'Arabic(Algeria)',
            ar_EG: 'Arabic(Egypt)',
            ar_IQ: 'Arabic(Iraq)',
            ar_JO: 'Arabic(Jordan)',
            ar_KW: 'Arabic(Kuwait)',
            ar_LB: 'Arabic(Lebanon)',
            ar_LY: 'Arabic(Libya)',
            ar_MA: 'Arabic(Morocco)',
            ar_OM: 'Arabic(Oman)',
            ar_QA: 'Arabic(Qatar)',
            ar_SA: 'Arabic(SaudiArabia)',
            ar_SY: 'Arabic(Syria)',
            ar_TN: 'Arabic(Tunisia)',
            ar_YE: 'Arabic(Yemen)',
            'as': 'Assamese',
            az: 'Azeri',
            az_AZ: 'Azeri',
            be: 'Belarusian',
            bg: 'Bulgarian',
            bn: 'Bengali(Bangladesh)',
            bo: 'Tibetan',
            bs: 'Bosnian',
            ca: 'Catalan',
            cs: 'Czech',
            cy: 'Welsh',
            da: 'Danish',
            de: 'German',
            de_AT: 'German(Austria)',
            de_CH: 'German(Switzerland)',
            de_DE: 'German(Germany)',
            de_LI: 'German(Liechtenstein)',
            de_LU: 'German(Luxembourg)',
            el: 'Greek',
            el_GR: 'Greek',
            en: 'English',
            en_AU: 'English(Australia)',
            en_BZ: 'English(Belize)',
            en_CA: 'English(Canada)',
            en_CB: 'English(Caribbean)',
            en_GB: 'English(GreatBritain)',
            en_IE: 'English(Ireland)',
            en_IN: 'English(India)',
            en_JM: 'English(Jamaica)',
            en_NZ: 'English(NewZealand)',
            en_PH: 'English(Phillippines)',
            en_TT: 'English(Trinidad)',
            en_US: 'English(UnitedStates)',
            en_ZA: 'English(SouthernAfrica)',
            es: 'Spanish',
            es_AR: 'Spanish(Argentina)',
            es_BO: 'Spanish(Bolivia)',
            es_CL: 'Spanish(Chile)',
            es_CO: 'Spanish(Colombia)',
            es_CR: 'Spanish(CostaRica)',
            es_DO: 'Spanish(DominicanRepublic)',
            es_EC: 'Spanish(Ecuador)',
            es_ES: 'Spanish(Spain)',
            es_GT: 'Spanish(Guatemala)',
            es_HN: 'Spanish(Honduras)',
            es_MX: 'Spanish(Mexico)',
            es_NI: 'Spanish(Nicaragua)',
            es_PA: 'Spanish(Panama)',
            es_PE: 'Spanish(Peru)',
            es_PR: 'Spanish(PuertoRico)',
            es_PY: 'Spanish(Paraguay)',
            es_SV: 'Spanish(ElSalvador)',
            es_UY: 'Spanish(Uruguay)',
            es_VE: 'Spanish(Venezuela)',
            et: 'Estonian',
            eu: 'Basque',
            fa: 'Farsi(Persian)',
            fi: 'Finnish',
            fo: 'Faroese',
            fr: 'French',
            fr_BE: 'French(Belgium)',
            fr_CA: 'French(Canada)',
            fr_CH: 'French(Switzerland)',
            fr_FR: 'French(France)',
            fr_LU: 'French(Luxembourg)',
            gd: 'Gaelic',
            gd_IE: 'Gaelic(Ireland)',
            gl: 'Galician',
            gn: 'Guarani',
            gu: 'Gujarati',
            he: 'Hebrew',
            hi: 'Hindi',
            hr: 'Croatian',
            hu: 'Hungarian',
            hy: 'Armenian',
            id: 'Indonesian',
            is: 'Icelandic',
            it: 'Italian',
            it_CH: 'Italian(Switzerland)',
            it_IT: 'Italian(Italy)',
            ja: 'Japanese',
            ja_JP: 'Japanese',
            ka: 'Georgian',
            kk: 'Kazakh',
            km: 'Khmer',
            kn: 'Kannada',
            ko: 'Korean',
            ks: 'Kashmiri',
            la: 'Latin',
            lo: 'Lao',
            lt: 'Lithuanian',
            lv: 'Latvian',
            mi: 'Maori',
            mk: 'FYROMacedonia',
            ml: 'Malayalam',
            mn: 'Mongolian',
            mr: 'Marathi',
            ms: 'Malay',
            ms_BN: 'Malay(Brunei)',
            ms_MY: 'Malay(Malaysia)',
            mt: 'Maltese',
            my: 'Burmese',
            nb: 'Norwegian(Bokml)',
            ne: 'Nepali',
            nl: 'Dutch(Belgium)',
            nl_BE: 'Dutch(Belgium)',
            nl_NL: 'Dutch(Netherlands)',
            nn: 'Norwegian(Nynorsk)',
            no_NO: 'Norwegian(Bokml)',
            or: 'Oriya',
            pa: 'Punjabi',
            pl: 'Polish',
            pt: 'Portuguese',
            pt_BR: 'Portuguese(Brazil)',
            pt_PT: 'Portuguese(Portugal)',
            rm: 'Raeto-Romance',
            ro: 'Romanian',
            ro_MO: 'Romanian(Moldova)',
            ru: 'Russian',
            ru_MO: 'Russian(Moldova)',
            sa: 'Sanskrit',
            sb: 'Sorbian',
            sd: 'Sindhi',
            sk: 'Slovak',
            sl: 'Slovenian',
            so: 'Somali',
            sq: 'Albanian',
            sr: 'Serbian',
            sr_SP: 'Serbian(Cyrillic)',
            sv: 'Swedish',
            sv_FI: 'Swedish(Finland)',
            sv_SE: 'Swedish(Sweden)',
            sw: 'Swahili',
            ta: 'Tamil',
            te: 'Telugu',
            tg: 'Tajik',
            th: 'Thai',
            tk: 'Turkmen',
            tn: 'Setsuana',
            tr: 'Turkish',
            ts: 'Tsonga',
            tt: 'Tatar',
            uk: 'Ukrainian',
            ur: 'Urdu',
            uz: 'Uzbek',
            uz_UZ: 'Uzbek(Cyrillic)',
            vi: 'Vietnamese',
            xh: 'Xhosa',
            yi: 'Yiddish',
            zh: 'Chinese',
            zh_CN: 'Chinese(Simplified)',
            zh_HK: 'Chinese(HongKong)',
            zh_MO: 'Chinese(Macau)',
            zh_SG: 'Chinese(Singapore)',
            zh_TW: 'Chinese(Traditional)',
            zu: 'Zulu'
        };
    }


    Rule {
        inputsFromDependencies: ["ts_input"]
        Artifact {
            filePath: {
                // input.baseName is e.g. 'yourapp_en_US'
                /// langcode will be 'en_US'
                if(product.TsFileNameMap) {
                    return product.TsFileNameMap[input.fileName];
                }
                var _idx = input.baseName.indexOf('_');
                var langcode = input.baseName.substring(_idx+1);
                var langname = ModUtils.moduleProperty(product, "Languages")[langcode];
                if(!langname)
                    console.err("Can't determine the language from file '"+input.fileName+"'.");
                return langname + product.confExt
            }
            fileTags: ["Conf type translations"]
        }
        prepare: {
            var cmd = new JavaScriptCommand();
            cmd.description = "transconf: " + input.fileName + " -> " + output.fileName;
            cmd.sourceCode = function() {
                var header = product.template;
                var option = {
                    method: "general",
                    header: header,
                    entries: header.entries[input.fileName],
                };
                var tsroot = Main.parseTranslationXml(input.filePath);
                Main.generateTranslationIni(output.filePath, tsroot, option);
            }
            return [cmd];
        }
    }

    Rule {
        inputsFromDependencies: ["ts_default_input"]
        multiplex: true
        Artifact {
            filePath: {
                // input.baseName is e.g. 'yourapp_en_US'
                /// langcode will be 'en_US'
                if(product.tsFileNameMap) {
                    return product.tsFileNameMap[input.fileName];
                }
                var _idx = input.baseName.indexOf('_');
                var langcode = input.baseName.substring(_idx+1);
                var langname = ModUtils.moduleProperty(product, "Languages")[langcode];
                if(!langname)
                    console.err("Can't determine the language from file '"+input.fileName+"'.");
                return langname + product.confExt
            }
            fileTags: ["Default(User Language) translation"]
        }
        prepare: {
            var cmd = new JavaScriptCommand();
            cmd.description = "transconf: " + input.fileName + " -> " + output.fileName;
            cmd.sourceCode = function() {
                var header = product.template;
                var option = {
                    method: "default",
                    header: header,
                    entries: header.entries[input.fileName],
                };
                var tsroot = Main.parseTranslationXml(input.filePath);
                Main.generateTranslationIni(output.filePath, tsroot, option);
            }
            return [cmd];
        }
    }

    Rule {
        inputsFromDependencies: ["ts_reverse_input"]
        multiplex: true
        Artifact {
            filePath: product.reverseConfName + product.confExt
            fileTags: ["Reverse translation"]
        }
        prepare: {
            var cmd = new JavaScriptCommand();
            cmd.description = "transconf: " + input.fileName + " -> " + output.fileName;
            cmd.sourceCode = function() {
                var header = product.template;
                var option = {
                    method: "reverse",
                    header: header,
                    entries: header.entries && header.entries[input.fileName] ? header.entries[input.fileName] : undefined,
                };
                var tsroot = Main.parseTranslationXml(input.filePath);
                Main.generateTranslationIni(output.filePath, tsroot, option);
            }
            return [cmd];
        }
    }
}
