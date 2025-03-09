# Grammar

## General
This section of the L# specification describes how a source file is interpreted as a sequence of instructions.

Conceptually speaking, an L# program is compiled using three steps:
1. Transformation, which converts an L# source code file into a series of [tokens](tokens.md)
1. Token analysis, which translates a series of tokens into an [abstract syntax tree](abstract-syntax-tree.md)
1. Syntax analysis, which translates the abstract syntax tree into bytecode instructions

## Extended Backus-Naur Form
This is a representation of the grammar of L# in EBNF form.
```
program = { statement } ;

statement = import_statement
          | variable_declaration
          | assignment_statement
          | contract_declaration
          | implementation_declaration
          | function_call ;

import_statement = "grab" identifier ;

variable_declaration = type identifier "=" expression ;

assignment_statement = identifier "=" expression ;

contract_declaration = "contract" identifier contract_body ;

contract_body = { contract_member } ;

contract_member = type identifier
                | type identifier "()" ;

implementation_declaration = identifier "is" identifier implementation_body ;

implementation_body = constructor_definition { function_definition } ;

constructor_definition = identifier "(" [ parameter_list ] ")" constructor_body ;

constructor_body = { assignment_statement } ;

function_definition = type identifier "()" function_body ;

function_body = "return" expression ;

function_call = identifier "." identifier "(" [ argument_list ] ")" ;

expression = literal
           | identifier
           | binary_expression
           | constructor_call ;

constructor_call = "[" argument_list "]" ;

binary_expression = expression operator expression ;

parameter_list = type identifier { "," type identifier } ;

argument_list = expression { "," expression } ;

type = "number" | "text" | "bit" | "numbers" | "texts" | "bits" | identifier ;

literal = number_literal | text_literal | bit_literal | collection_literal ;

number_literal = digit { digit } [ "." digit { digit } ] ;

text_literal = "'" { character } "'" ;

bit_literal = "on" | "off" ;

collection_literal = "[" literal { "," literal } "]" ;

operator = "+" | "-" | "*" | "/" ;

identifier = letter { letter | digit } ;

digit = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;

letter = "a" | "b" | ... | "z" | "A" | "B" | ... | "Z" ;

character = <any character except '> ;
```