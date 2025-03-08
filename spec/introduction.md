# Introduction
As the definition of L# evolved, the goals used in its design were as follows:

- L# is intended to be easy-to-write, easy-to-read, immutable by default, general-purpose, extensible, platform-agnostic programming language
- The language is intended to interoperate with other systems and languages
- The language does not allow code that would violate any memory safety; memory should not be a concern of the programmer
- The language does not enforce any specific architecture or structure, rather, the programmer decides and implements how they see fit
- L# applications are intended to be economical with regard to memory and processing, the language is intended to compete directly on performance and size with C or assembly language

This specification describes features of L#, which are as follows:

- [Grammar](grammar): how lexical structure is understood in L#
- [Keywords](keywords): what words are reserved in L#
- [Types](types): what data models L# understands
- [Chunks](chunks): how L# is loaded and packaged
- [Standard Library](standard-library): what out-of-the-box functionality is available with L#

The name L# is pronounced "L Sharp".

The name L# is written as the LATIN CAPITAL LETTER L (U+004C) followed by the NUMBER SIGN # (U+0023).