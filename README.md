___

# verbexx

verbexx is a toy verb-oriented scripting language.  There are no control or definition statements, and there is no distinction between operators and functions.  Verbs do almost everything in verbexx.  The only non-comment syntax element is the expression, consisting of verbs and their parameters, along with parenthesis, braces, and other punctuation.

## Sample verbexx code:

#### Hello world: 

```
@OUT "Hello, World!";
```

#### Fizzbuzz: 


#### Y-combinator:

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
