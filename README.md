
# verbexx
verbexx is a toy verb-oriented scripting language.  There are no control or definition statements, and there is no distinction between operators and functions.  Verbs do almost everything in verbexx.  The only non-comment syntax elements are expression and code blocks, consisting of verbs and their parameters, along with parenthesis, semicolons, braces, and other punctuation.

## Main verbexx features

* No distinction between functions and operators -- these are both considered verbs.  Verbs can have positional and/or keyword
parameters on the right and/or left sides.  Verbs can be defined with either lexical scoping (the default), dynamic scoping, no scoping (only local variables and global variables are visible), or same scoping (verb runs in same stack frame as the caller -- this is needed when creating user-defined verbs that function as control statements).

* No control or definition statements -- almost everything is done with verbs.  Most things are first class, including types (primitive for now, and not hooked in yet), functions, and chuncks of code (enclosed in braces).  Passing around chunks of code allows verbs to act like control statements. 

* Extremely simple syntax -- so simple, in fact, that sigils are needed in front of identifiers when these are
to be parsed as verbs ("@" leading sigil) or keyword names (":" trailing sigil).  Similarly, if something that looks
like an operator ("+", "-", etc.) needs to be passed to another verb as a parameter, a leading "$" sigil is needed.

* No automatic tail calls -- these must be coded manually, using the @XCTL verb

* Verbs, expressions, and chunks of code can have multiple (or no) return values.  They may return both positional and keyword values, but any keyword values returned must be acceptable to the receiving verb.

* Many things are missing, including: 
  * No OOP support 
  * Almost no support for functional programming 
  * No lazy evaluation (except for verbs like "&&", "||", @LOOP, etc. ), other than blocks of code passed into verbs. 
  * No pattern matching, immutable data, pure functions, monads, functors, continuations, currying, etc.
  * No support for regular expressions
  * No garbage collection
  * No list comprehensions and generators, ranges, coroutines, etc.
  * No concurrency, multi-threading, green threads, fibers, channels, etc. 
  * No trancendental functions, BigNum support, and other math support (this may happen soon)
  * No file I/O (except very limited I/O to/from stdout/stdin) (this may also happen soon)
  * No fancy general-purpose container structures, iterators, move semantics, etc.
  * No advanced string functions, string interpolation, formatted output, etc.
  * No internationalization and locales, No proper Unicode support (combining characters) etc.
  * No communications or web support -- HTML, sockets, etc.
  * No foreign function APIs 
  * No elaborate type system or type inference, enumerations, etc.
  * No parse-time macros, although it's possible to run user-defined and built-in verbs during the preprocess stage while the AST is being built, and these can cause strings to be inserted into the program source as it's being parsed.
  * No generic programming, templates, metaprogramming, etc.
  * No namespaces
  * No debuggers, IDEs, etc.

## Sample verbexx code

#### Hello world (Rosetta Code version)

```
@STDOUT "Hello world!\n";
```

#### Fizzbuzz 

```        
@VAR i value:1;
@VAR t3 t5; 

@LOOP while:(i <= 100)
{
  t3 = (i % 3 == 0); 
  t5 = (i % 5 == 0);

  @SAY ( @CASE when:(t3 && t5) { 'FizzBuzz }
               when: t3        { 'Fizz     }
               when: t5        { 'Buzz     }
               else:           { i         }           
       );
 
  i++;
};
```

#### Fibonacci (simple recursive) 

```
fib1 @FN [n] 
{
  @IF (n <= 0) then: { @RETURN 0; }; 
  @IF (n == 1) then: { @RETURN 1; };

  @RETURN ( (@fib1 (n-1)) + (@fib1 (n-2)) ); 
};


@VAR i value:-1;
@LOOP while:(i <= 16)
{
  @SAY "@fib1<" i "> = " (@fib1 i);
  i++;
};
```

#### Fibonacci (using manual tail call)

```
fib2 @FN [n]
{
   @VAR fibx value:
   (
      @FN [a b n] 
      {
         @IF (n <= 0) then:{ @RETURN a };
         
         // @RETURN @fibx b (a+b) (n-1);
         @XCTL 'fibx right:[b (a+b) (n-1)];
      }
   );
   @RETURN @fibx 0 1 n;  
};

i = -1; 
@LOOP while:(i <= 90)
{
  @SAY "@fib2" i " = " (@fib2 i);
  i++;
};
```

#### Y-combinator

```

// Y-combinator function (copied from a JavaScript example)
// --------------------------------------------------------

@VAR y_comb value:
( @FN [f] 
  {
    @RETURN ( @(                         // @ sigil causes expression output to be treated as a verb
                 @FN [x]
                 { 
                   @RETURN ( @f ( @FN [y] { @RETURN (@(@x x) y); } close:) );
                 } close:
               )
               (                         // This is the argument for the above function
                 @FN [x] 
                 {
                   @RETURN ( @f ( @FN [y] { @RETURN (@(@x x) y); } close:) );
                 } close:
               )
            );
  } 
);


// Function to generate an anonymous factorial function as the return value
// ------------------------------------------------------------------------

@VAR fact_gen value:
( @FN [fact]
  { 
    @RETURN ( @FN [n]                // Simple (non tail-recursive) factorial function
              {
                @IF (n <= 0)
                  then:{ @RETURN 1;                     } 
                  else:{ @RETURN (n * (@fact (n - 1))); }
                ; 
              } close:
            );
  } 
);


// loop to test the above functions
// --------------------------------

@VAR i value:-1;

@LOOP while:(i <= 20)
{
  @SAY  i "factorial =" ( @(@y_comb fact_gen) i );  
  i++; 
};




```

#### Y-combinator -- compact version

```
// Y-combinator function 
// ---------------------

@VAR y_comb =
( @FN [f] 
  { //1   2 23   4   5 56 7    7  6       43       1
     @(@FN[x]{@f (@FN[y]{@(@x x) y} close:)} close:)       // output of this expression is treated as a verb, because of outer @(     )
      (@FN[x]{@f (@FN[y]{@(@x x) y} close:)} close:)       // this is the argument supplied to the above verb expression
  }
);


// Function to generate an anonymous factorial function as the return value -- Simple (non tail-recursive) factorial function
// ------------------------------------------------------------------------

@VAR fact_gen = ( @FN [fact] { @FN [n]{ @IF (n <= 0) then:{1} else:{n*(@fact (n-1))} } close: } );
                

// loop to test the above functions
// --------------------------------

@VAR i = -1;

@LOOP while:(i <= 20)
{
  @SAY  i "factorial =" ( @(@y_comb fact_gen) i );  
  i++; 
};

```
