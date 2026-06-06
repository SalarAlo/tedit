; Functions, methods, and constructors

(call_expression
  function: (identifier) @function)

(call_expression
  function: (field_expression
    field: (field_identifier) @function.method))

(call_expression
  function: (qualified_identifier
    name: (identifier) @function))

(template_function
  name: (identifier) @function)

(template_method
  name: (field_identifier) @function.method)

(function_declarator
  declarator: (identifier) @function)

(function_declarator
  declarator: (field_identifier) @function.method)

(function_declarator
  declarator: (qualified_identifier
    name: (identifier) @function))

(function_declarator
  declarator: (operator_name) @function)

(function_declarator
  declarator: (destructor_name) @destructor)

(function_declarator
  declarator: (template_function
    name: (identifier) @function))

(function_declarator
  declarator: (template_method
    name: (field_identifier) @function.method))

(field_declaration
  declarator: (function_declarator
    declarator: (field_identifier) @function.method))

(declaration
  declarator: (function_declarator
    declarator: (identifier) @function))

; Types

(primitive_type) @type.builtin
(sized_type_specifier) @type.builtin
(type_identifier) @type
(template_type
  name: (type_identifier) @type)
(dependent_type
  (type_identifier) @type)
(auto) @type.builtin

((namespace_identifier) @type
 (#match? @type "^[A-Z]"))

; Variables, fields, and parameters

(identifier) @variable
(field_identifier) @property
(field_expression
  field: (field_identifier) @property)

(parameter_declaration
  declarator: (identifier) @variable.parameter)

(optional_parameter_declaration
  declarator: (identifier) @variable.parameter)

; Constants and literals

(this) @variable.builtin
(null "nullptr" @constant.builtin)
(true) @boolean
(false) @boolean
(number_literal) @number
(char_literal) @character
(literal_suffix) @string.special

(enumerator
  name: (identifier) @constant)

; Modules and namespaces

(module_name
  (identifier) @module)

(namespace_identifier) @module

; Strings

(string_literal) @string
(raw_string_literal) @string
(system_lib_string) @string

; Comments

(comment) @comment

; Attributes and labels

(attribute
  name: (identifier) @attribute)

(statement_identifier) @label

; Preprocessor

(["#define" "#elif" "#elifdef" "#else" "#endif" "#if" "#ifdef" "#ifndef" "#include"] @keyword.directive)
(preproc_directive) @keyword.directive
(preproc_include) @preprocessor
(preproc_def
  name: (identifier) @function.macro)
(preproc_function_def
  name: (identifier) @function.macro)
(preproc_call
  directive: (preproc_directive) @keyword.directive)
(preproc_if) @preprocessor
(preproc_ifdef) @preprocessor
(preproc_else) @preprocessor

; Keywords

[
 "if"
 "else"
 "switch"
] @keyword.conditional

[
 "for"
 "while"
 "do"
] @keyword.repeat

[
 "return"
 "co_return"
] @keyword.return

[
 "try"
 "catch"
 "throw"
] @keyword.exception

[
 "alignas"
 "const"
 "consteval"
 "constexpr"
 "constinit"
 "explicit"
 "extern"
 "final"
 "friend"
 "mutable"
 "noexcept"
 "override"
 "static"
 "virtual"
] @keyword.modifier

[
 "class"
 "concept"
 "enum"
 "namespace"
 "struct"
 "template"
 "typedef"
 "typename"
 "union"
 "using"
] @keyword.storage

[
 "operator"
 "sizeof"
 "alignof"
 "decltype"
 "requires"
 "co_await"
] @keyword.operator

[
 "import"
 "export"
 "module"
] @keyword.directive

[
 "asm"
 "break"
 "case"
 "continue"
 "default"
 "delete"
 "goto"
 "new"
 "private"
 "protected"
 "public"
 "static_assert"
 "co_yield"
] @keyword

; Operators and punctuation

[
 "+"
 "-"
 "*"
 "/"
 "%"
 "="
 "+="
 "-="
 "*="
 "/="
 "%="
 "=="
 "!="
 "<"
 ">"
 "<="
 ">="
 "&&"
 "||"
 "!"
 "&"
 "|"
 "^"
 "~"
 "&="
 "|="
 "^="
 "<<"
 ">>"
 "<<="
 ">>="
 "++"
 "--"
 "->"
 "->*"
 "."
 ".*"
 "?"
] @operator

[
 "("
 ")"
 "{"
 "}"
 "["
 "]"
 "<"
 ">"
] @punctuation.bracket

[
 ";"
 ","
 ":"
 "::"
] @punctuation.delimiter
