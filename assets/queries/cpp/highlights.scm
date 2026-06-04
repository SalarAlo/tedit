; Functions

(call_expression
  function: (identifier) @function)

(call_expression
  function: (qualified_identifier
    name: (identifier) @function))

(template_function
  name: (identifier) @function)

(template_method
  name: (field_identifier) @function)

(function_declarator
  declarator: (identifier) @function)

(function_declarator
  declarator: (qualified_identifier
    name: (identifier) @function))

(function_declarator
  declarator: (field_identifier) @function)

; Types

(primitive_type) @type
(type_identifier) @type
(sized_type_specifier) @type
(auto) @type

((namespace_identifier) @type
 (#match? @type "^[A-Z]"))

; Variables and fields

(identifier) @variable
(field_identifier) @property

(parameter_declaration
  declarator: (identifier) @parameter)

(optional_parameter_declaration
  declarator: (identifier) @parameter)

; Constants

(this) @variable.builtin
(null "nullptr" @constant)
(true) @constant
(false) @constant

; Modules

(module_name
  (identifier) @module)

; Numbers

(number_literal) @number

; Strings

(string_literal) @string
(raw_string_literal) @string
(char_literal) @string
(system_lib_string) @string

; Comments

(comment) @comment

; Preprocessor

(preproc_include) @preprocessor
(preproc_def) @preprocessor
(preproc_function_def) @preprocessor
(preproc_call) @preprocessor
(preproc_if) @preprocessor
(preproc_ifdef) @preprocessor
(preproc_else) @preprocessor

; Keywords

[
 "alignas"
 "alignof"
 "asm"
 "break"
 "case"
 "catch"
 "class"
 "co_await"
 "co_return"
 "co_yield"
 "concept"
 "const"
 "consteval"
 "constexpr"
 "constinit"
 "continue"
 "decltype"
 "default"
 "delete"
 "do"
 "else"
 "enum"
 "explicit"
 "export"
 "extern"
 "final"
 "for"
 "friend"
 "goto"
 "if"
 "import"
 "mutable"
 "namespace"
 "new"
 "noexcept"
 "operator"
 "override"
 "private"
 "protected"
 "public"
 "requires"
 "return"
 "sizeof"
 "static"
 "static_assert"
 "struct"
 "switch"
 "template"
 "throw"
 "try"
 "typedef"
 "typename"
 "union"
 "using"
 "virtual"
 "while"
] @keyword

; Operators and punctuation

[
 "+"
 "-"
 "*"
 "/"
 "%"
 "="
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
 "<<"
 ">>"
 "->"
 "."
] @operator

[
 "("
 ")"
 "{"
 "}"
 "["
 "]"
 ";"
 ","
 ":"
] @punctuation
