/* grab the io library for it's logging functions */
grab io

/* A normal logging function */
io.log('A normal call to io.log from L#.')

/* declaring and assigning the text type */
text phrase = 'Hello, world!'

/* declaring and assigning a collection of text types */
texts phrases = [ 'Hello', 'world!' ]

/* declaring and assigning the number type */
number temperature = 98.4

/* declaring and assigning a collection of number types */
numbers temperatures = [ 97, 98.4, 100.001 ]

/* declaring and assigning the bit type */
bit zero = off

/* declaring and assigning a collection of bit types */
bits mask = [ off, on, off, off, on, off ]

/* defining a contract for a user type without implementation */
contract shape
    text id
    number width
    number height
    number area()

/* defining an implementation of a contract */
square is shape
    /* defining what happens when assigning to a user type */
    square(number a, number b)
        width = a
        height = b
        id = 'Square with `a` width and `b` height'

    /* defining an implementation of a function from the contract */
    number area()
        return width * height

/* create a square */
number my_width = 10
number my_height = my_width + 5

/* notice the square brackets to invoke the constructor, which is similar to how plural types (or collections) work */
square mysquare = [my_width, my_height]