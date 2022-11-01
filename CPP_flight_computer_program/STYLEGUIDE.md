## programming language

- For now we are sticking to core/ embedded C to maintain portability with any future microcontroller changes


## headers

- arrange headers in a descending order, in case of same length use alphabetical order  


## indents

- we will use the Whitesmiths style of indents, [reasoning](http://www.activeclickweb.com/whitesmiths/index.html)  

- in line tab indents, that means you indent after a command block like in Python and then continue in line  

```
    if (condition == true)
        {
        foobar();
        }
```

- use tabs to indent, 1 tab = 4 spaces  


## functions

- leave a new line to separate two function def blocks  

- use snake case, camel case is just horrible to read  
    Example :  
        getRocketIDWithState() vs get_rocket_id_with_state()  


## variables

- every variable has a default value! I don't wanna be dealing undefined behaviour!  

- variables are also snake case  

- constants must be upper case

## print statements

- every debug print statement should have a tag indicating what it is in reference to   
    Example  
        [ROCKET STATE] => indicates the statement is for debugging rocket state  
        [KX134] => indicates debugging for the respective sensor  


## if-else statements

- make them excplicit

```
    if (0){}            // bad
    if (true){}         // bad
    if (state){}        // bad
    if (state == false) // good
```
