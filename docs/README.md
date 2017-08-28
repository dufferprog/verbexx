# verbexx

verbexx is a toy verb-oriented scripting language. 
There are no control or definition statements, and there is no distinction between operators and functions. 
erbs do almost everything in verbexx. 
The only non-comment syntax element is the expression, consisting of verbs and their parameters, along with parenthesis, braces, and other punctuation.

### Main verbexx features

* No distinction between functions and operators -- these are both considered verbs.  Verbs can have positional aod/or keyword
parameters on the right and/or left sides.  Verbs can be defined with either lexical or dynamic scoping.

* No control or definition statements -- almost everything is done with verbs.  Most things are first class, including types (primitive for now), functions, and chuncks of code (enclosed in braces).  Passing in chunks of code allows verbs to act like control statements. 

* Extremely simple syntax -- so simple, in fact, that sigils are needed in front of identifiers when these are
to be parsed as verbs ("@" leading sigil) or keyword names (":" trailing sigil).  Similarly, if something that looks
like an operator ("+", "-", etc.) needs to be passed to another verb as a parameter, a leading "$" sigil is needed.

* No automatic tail calls -- these must be coded manually, using the @XCTL verb

* Verbs, expressions, and chunks of code can have multiple (or no) return values 

* Many things are missing, including: 
  * No OOP support (classes, inheritance, mixins, multiple dispatch, polymorphism, aspect-oriented programming, etc.)
  * Almost no support for functional programming 
  * No lazy evaluation (except for verbs like "&&") 
  * No pattern matching, immutable data, pure functions, monads, functors, continuations, currying, etc.
  * No support for regular expressions
  * No list comprehensions and generators, ranges, coroutines, etc.
  * No concurrency, multi-threading, green threads, fibers, channels, etc. 
  * No trancendental functions, BigNum support,and other math support
  * No file I/O (except very limited I/O to/from stdout/stdin)
  * No fancy general-purpose container structures, iterators, etc.
  * No advanced string functions, string interpolation, formatted output, etc.
  * No internationalization and locales, etc.
  * No web support, HTML, JSON, etc.
  * No foreign function APIs 
  * No elaborate type system or type inference, enumerations, etc.
  * No parse-time macros
  * No generic programming, templates, metaprogramming, etc.
  * No namespaces
  * No debuggers, IDEs, etc.

## Sample verbex code

Apparently, code examples included in the normal README.md file do not format properly on the web page, so the examples cannot be included here.  See README.md file in the repository, which includes the code examples.




