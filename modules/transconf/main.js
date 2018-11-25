var TextFile = require("qbs.TextFile");
var Xml = require("qbs.Xml");

function parseTranslationXml(path) {
    var dom = new Xml.DomDocument();
    dom.load(path);
    var node_ts= dom.documentElement();
    var tsroot = {};
    tsroot.version = node_ts.attribute("version");
    tsroot.language = node_ts.attribute("language");
    tsroot.context = [];

    for(var node_context = node_ts.firstChild(); node_context.isElement(); node_context = node_context.nextSibling()) {
        var node_name = node_context.firstChild();
        var context = {};
        context.name = node_name.text();
        context.message = [];
        for(var node_message = node_name.nextSibling(); node_message.isElement(); node_message = node_message.nextSibling()) {
            var message = {};
            for(var child = node_message.firstChild(); child.isElement(); child = child.nextSibling()) {
                //console.error(child.tagName()+ ": " + child.text());
                var tag = child.tagName();
                message[tag] = child.text();
                if(tag == "translation") {
                    var type = child.attribute("type");
                    if(type && type != "unfinished")
                        message = undefined;
                }
            }
            if(message)
                context.message.push(message);
        }
        tsroot.context.push(context);
    }
    return tsroot;
}

function generateTranslationIni(path, trans, option) {
    var txt = new TextFile(path, TextFile.WriteOnly);
    txt.setCodec("UTF-8");
    if(option.header) {
        if(option.header.file_header)
            for(var i = 0; i < option.header.file_header.length; i++)
                txt.writeLine(option.header.file_header[i]);
        if(option.header.section_name && option.entries) {
            txt.writeLine("["+option.header.section_name+"]");
            for(var propertyName in option.entries)
                txt.writeLine(propertyName+"="+option.entries[propertyName]);
            txt.writeLine("");
        }
    }

    for(var i = 0; i < trans.context.length; i++) {
        var context = trans.context[i];
        txt.writeLine("["+context.name+"]");
        for(var j = 0; j < context.message.length; j++) {
            var num = 100000+i*1000+j+1;
            var message = context.message[j];
            var comment = message.comment ? message.comment : message.extracomment;
            switch(option.method)
            {
            case "general":
                if(comment)
                    txt.writeLine(";hint"+num+": "+comment);
                txt.writeLine("text_"+num+"="+(message.translation ? message.translation : message.source));
                break;
            case "default":
                if(comment)
                    txt.writeLine(";hint"+num+": "+comment);
                txt.writeLine("text_"+num+"="+message.source);
                break;
            case "reverse":
                txt.writeLine("text_"+num+"="+message.source);
                break;
            }
        }
        txt.writeLine("");
    }

    txt.close();
}
