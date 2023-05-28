# README

# About

This is Coolang!!, my hobby project. I always dreamt of making my own programming language, so this is it, how I wanted a programming language to be.  
Coolang is an interpreted language written in C++. Currently it supports primitive types like `number`, `boolean` and `str`. It follows the curly braced syntax of C.

# Syntax

## Primitive Types
### number
A number is a 32 bit integer or floating point. I'm still conflicted whether to add separate int and float types

### boolean
A boolean is a 8 bit bool same as C/C++

## str
A string can be declared using single `'` or double `"` quotes. As you please! we don't restrict you

## Variable Declaration
Variables can start with any character from the alphabet or an `_` but not any numeric character.  
Variables can be declared using the `let` keyword followed by the identifier
```
let a = 10
```
### And lines don't end with semi-colons!!!

## Code Block
Code blocks have their own local scope. Pretty useful in functions, loops, `if` statements or to just use on their own.  
Code blocks can be declared using `{}`
```
{
  let a = 50
}
```
Here a will be a local variable to the code block

## If Statements
The if statements are pretty similar to that of C but without the ugly paranthesised conditional statements.  
Let them be free!
```
if a % 15 == 0 {
  FizzzBuzzz
} elif a % 5 == 0 {
  Fizz
} elif a % 3 == 0 {
  Buzz
} else {
  a
}
```

## Function declarations
Ahh yes, the good part. Functions!!  
In Coolang the functions don't have a return type, cause it's 2023!! we don't need them.

Functions can be declared using the `fn` keyword followed by the function identifier. The function identifier follows the same rules as a variable identifier.

```
fn fact(x) {
  if x == 1 {
    return 1
  }
  return x * fact(x - 1)
}
```
Yeah, recursion works.

## Function Call
Functions can be called just like how you're used to, using the good-old `()`

```
let a = foo()
```