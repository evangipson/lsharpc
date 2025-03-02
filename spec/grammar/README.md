# Grammar

## General
This section of the L# specification describes how a source file is interpreted as a sequence of instructions.

Conceptually speaking, an L# program is compiled using three steps:
1. Transformation, which converts an L# source code file into a series of [lexemes](lexemes.md)
1. Lexeme analysis, which translates a series of lexemes into [statements](statements.md)
1. Statement analysis, which translates the statements into executable instructions