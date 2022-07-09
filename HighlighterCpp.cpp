#include "HighlighterCpp.h"

HighlighterCpp::HighlighterCpp(QObject *parent)
    : AbstractHighlighter{parent}
{
    registerKeywords({ "alignas", "alignof", "and", "and_eq",  "asm", "atomic_cancel", "atomic_commit", "atomic_noexcept",
                     "auto", "bitand", "bitor", "bool", "break", "case", "catch", "char", "char8_t", "char16_t", "char32_t",
                     "class", "compl", "concept", "const", "consteval", "constexpr", "constinit", "const_cast", "continue",
                     "co_await", "co_return", "co_yield", "decltype", "default", "delete", "do", "double", "dynamic_cast",
                     "else", "enum", "explicit", "export", "extern", "false", "float", "for", "friend", "goto", "if", "inline",
                     "int", "long", "mutable", "namespace", "new", "noexcept", "not", "not_eq", "nullptr", "operator", "or",
                     "or_eq", "private", "protected", "public", "reflexr", "register", "reinterpret_cast", "requires", "return",
                     "short", "signed", "sizeof", "static", "static_cast", "static_assert", "struct", "switch", "synchronized",
                     "template", "this", "thread_local", "throw", "true", "try", "typedef", "typeid", "typename", "union",
                     "unsigned", "using", "virtual", "void", "volatile", "wchar_t", "while", "xor", "xor_eq"});
}

void HighlighterCpp::setHighlighter(const QString &highlighter)
{
    clearHighlightingRules();

    if (highlighter == "VSCode") {
        // class rules
        registerHighlighterRules(QColor(255, 255, 85), "\\bQ[A-Za-z]+\\b");
        // quotation rules
        registerHighlighterRules(QColor(Qt::darkGreen), "\".*\"");
        // function rules
        registerHighlighterRules(QColor(Qt::white), "\\b[A-Za-z0-9_]+(?=\\()");
        // single line comment rules
        registerHighlighterRules(QColor(Qt::red), "//[^\n]*");

        // multiline comment rules
        setMultiLineCommentRules(QColor(Qt::blue), "/\\*", "\\*/");

        // keywords rules
        for (const QString& pattern : keywords()) {
            registerHighlighterRules(QColor(255, 255, 85), pattern.toStdString().c_str());
        }
    } else {
        qDebug() << "Error! do not support style " << highlighter;
    }
}
