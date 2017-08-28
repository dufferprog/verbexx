 ___

# verbexx
verbexx is a toy verb-oriented scripting language.  There are no control or definition statements, and there is no distinction between operators and functions.  Verbs do almost everything in verbexx.  The only non-comment syntax element is the expression, consisting of verbs and their parameters, along with parenthesis, braces, and other punctuation.

## Main verbexx features

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

## Sample verbexx code
#### Hello world 

```
@OUT "Hello, World!";
```

#### Fizzbuzz 

```        
@VAR i value:1;
@VAR t3 t5; 

@LOOP while:(i <= 100)
{
  t3 = (i % 3 == 0); 
  t5 = (i % 5 == 0);

  @SAY ( @CASE when:(t3 && t5) {"FizzBuzz"}
               when: t3        {"Fizz"    }
               when: t5        {"Buzz"    }
               else:           {i         }           
       );
 
  i++;
};
```

#### Fibonacci (recursive) 

```
fib1 @FCN [n] 
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

#### Fibonacci (using tail call)

```
fib2 @FCN [n]
{
   @VAR fibx value:
   (
      @FCN [a b n] 
      {
         @IF (n <= 0) then:{ @RETURN a };
         
         // @RETURN @fibx b (a+b) (n-1);
         @XCTL "fibx" right:[b (a+b) (n-1)];
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
( @FCN [f] 
  {
    @RETURN ( @(                         // @ sigil causes expression output to be treated as a verb
                 @FCN [x]
                 { 
                   @RETURN ( @f ( @FCN [y] { @RETURN (@(@x x) y); } ) );
                 }
               )
               (                         // This is the argument for the above function
                 @FCN [x] 
                 {
                   @RETURN ( @f ( @FCN [y] { @RETURN (@(@x x) y); } ) );
                 }
               )
            );
  }
);


// Function to generate an anonymous factorial function as the return value
// ------------------------------------------------------------------------

@VAR fact_gen value:
( @FCN [fact]
  { 
    @RETURN ( @FCN [n]                // Simple (non tail-recursive) factorial function
              {
                @IF (n <= 0)
                  then:{ @RETURN 1;                     } 
                  else:{ @RETURN (n * (@fact (n - 1))); }
                ; 
              }
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

___
___
