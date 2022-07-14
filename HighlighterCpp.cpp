#include "HighlighterCpp.h"
#include <initializer_list>

HighlighterCpp::HighlighterCpp(QObject *parent)
    : AbstractHighlighter{parent}
{

    language_t lang;
    lang.name = "C++";
    lang.keywords = { "alignas", "alignof", "and", "and_eq",  "asm", "atomicCancel", "atomicCommit", "atomic_noexcept",
                              "auto", "bitand", "bitor", "bool", "break", "case", "catch", "char", "char8_t", "char16_t", "char32_t",
                              "class", "compl", "concept", "const", "consteval", "constexpr", "constinit", "constCast", "continue",
                              "co_await", "co_return", "co_yield", "decltype", "default", "delete", "do", "double", "dynamicCast",
                              "else", "enum", "explicit", "export", "extern", "false", "float", "for", "friend", "goto", "if", "inline",
                              "int", "long", "mutable", "namespace", "new", "noexcept", "not", "not_eq", "nullptr", "operator", "or",
                              "or_eq", "private", "protected", "public", "reflexr", "register", "reinterpretCast", "requires", "return",
                              "short", "signed", "sizeof", "static", "staticCast", "static_assert", "struct", "switch", "synchronized",
                              "template", "this", "thread_local", "throw", "true", "try", "typedef", "typeid", "typename", "union",
                              "unsigned", "using", "virtual", "void", "volatile", "wchar_t", "while", "xor", "xor_eq" };
    lang.syntax.functionPattern = "\\b[A-Za-z0-9_]+(?=\\()";
    lang.syntax.quotationPattern = "\".*\"\\w*";
    lang.syntax.singlelineCommentPattern = "//[^\n]*";
    lang.syntax.multilineCommentPattern = { "/\\*", "\\*/" };
    lang.syntax.attributesPattern = "\\[\\[.*\\]\\]\\s";
    lang.syntax.includePattern = "^#include\\s?[<\"][A-Za-z][\\w\\.]+[>\"]";
    lang.syntax.numberPattern = "\\d+\\w*(\\.\\d*\\w*)?";
    lang.operators = { "::", "++", "--", ".", "->", "+", "-", "*", "!", "~", "&", ".*", "->*", "<<", ">>", "<", ">",
                       "=", "<=>", "<=", ">=", "==", "!=", "^", "|", "&&", "||", "?", ":", "throw", "+=", "-=", "*=",
                       "/=", "%=", "<<=", ">>=", "&=", "^=", "|=", "," };

    appendLanguages(lang);

    highlighter_t style;
    style.name = "VSCode";
    style.keywordsColor = "#ffff55";
    style.quotationColor = "#61a33b";
    style.functionColor = "#00ff00";
    style.commentColor = "#41535b";
    style.attributesColor = "#ff0000";
    style.includeColor = "#00ffff";
    style.numberColor = "#ffff00";
    style.operatorColor = "#ff00ff";

    appendHighlighters(style);
}
