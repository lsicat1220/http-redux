# Another another HTTP server

In my hubris and zeal, I thought I would challenge myself to create an HTTP server without using any dynamic memory.
In other words, this entire source code makes no use of `malloc()` or `free()` or any functions that call them.
Every variable is stack allocated. What a mistake this was.

The idea for this project was that I would structure the functions like a factory line, where the buffer is created,
then gets sent through the parser, then that data gets put into the function that creates the response, etc.. Perhaps
it wouldn't be the most performant, but it at least got my mind thinking about project structure. I don't know if the
functions I wrote really qualify as true functions in the functional programming sense, but I tried to keep them pure.

Lesson #1 about C: C sucks at doing that.

## Receiving the request

Aside from this challenge, the second objective was to utilize C-strings as little as possible and avoid copying data. 
`strtok()` and its derivatives were off the table.
Therefore, I created a `Slice` struct that carried a pointer and a length,
as well as an extended `bufState` struct that carried the buffer with which incoming HTTP requests were stored.
This struct would include the maximum length of the buffer (being that it is stack allocated and therefore has a predetermined size),
the number of bytes that actually contain data, and a number containing the last byte that hasn't been read by `TokenizeSlice` (which will be discussed later).

To populate this `bufState`, I created a function `ReadUntil()` that took a delimiter and called `read()` on the socket until that delimiter is found.

At this point in development, I was concerned with keeping this as platform-agnostic as possible. `memmem()` was, as far as I could tell, Linux only,
so I rolled my own version `TheMemmem()`. This version is probably not as performant, since it's just C, but it's functionally the same.
This function comes in handy for the rest of the project.

`TokenizeSlice()` is a function that takes in the buffer state and reads until a certain delimiter is found.
It then populates the `Slice` that was passed in with a pointer containing the start of the segment that was just read and the bytes until the delimiter.

This function would be used to grab the request line first, then the rest of the headers.

Lesson #2 about C: Function signatures can get very messy very quick.

Because I was so hung up on preventing any segfaults from happening, most functions in this project also took in a length parameter for each string passed.
These function signatures would get *long*, and I wasn't too happy writing them. That being said, it made segfaults less likely, so I guess I succeeded
in what I set out to do. 

## Parsing the request

In my previous HTTP server, I was just reading headers line by line and matching strings, but I wanted to be a bit more fancy with this one.
I decided that I would read the headers by putting them into a hash map. Since this is C, I had to write my own logic for hashing and mapping,
and because this is C I decided not to generalize it, instead opting to just make it for slices. Generics/templates aren't a thing here, and I
wasn't really interested in doing any preprocessor voodoo to hack it into my project. 

The map uses the FNV-1a algorithm to assign indices and linear probing to resolve collisions. Buckets were a no go here since that requires 
something akin to a linked-list which would be 10x more work in a project without `malloc()` and it's unlikely that headers would collide that much anyway.

There isn't much to say about actually parsing the lines aside from that I don't really feel comfortable with how I'm actually doing it.
To find keywords, I created a function that would split a slice based on a delimiter and put it in a predefined array of slices.

Again, I had to pass in the array because I can't malloc anything, which made the function signature that much messier.

After the slice is split, I basically just filtered by length and then used `memcmp()` to find keywords. This is the part I'm uncomfortable with,
because there has to be a better way to do this. That being said, it works and filters out all invalid request lines while putting it into a simple
struct for response creation.

## Creating the response

The hard work is basically done after parsing. The only difficult thing after interpreting the request line was validating the file
and cleaning up any possible directory traversal. It was at this point that I gave up on being platform-agnostic. Whatever way Windows
expands directories is not how Linux does it, and I don't really care, so now the project is Linux only until I decide I want to try 
supporting WIndows. 

It's also the point where I'm forced to copy and use C-strings because all file IO functions expect a null-terminated string. 

Aside from that, the rest is standard and not much different from the previous server. 

Lesson #3 about C: There's a reason C++ exists

God, how much I wish I could encapsulate data and create methods tied to structs after this project. It would have made writing the project
so much easier. RAII or ownership would have saved me many lines of initializing arrays to put into structs. 
However, I don't think I regret doing this project in C and without `malloc()`. It taught me how to think about the structure
of my project, where and when data is actually needed. 

Limiting memory to the stack made me think about how I can take advantage of scope to keep data only as long as I need to, 
and in doing so then the structure basically fell into place for the most part. I'm not saying that the project
is perfect. Far from it. But I think I became a better programmer by doing it, and I certainly see why RAII became popular, and why
Rust took it and ran with it. One thing I thought about when writing this project is how convenient it is that memory is deallocated
once the scope ended, and that you don't have to litter the end of a function with `free()` calls. 

That being said, the next time I do a C project, I'll deal with that over initializing every array manually. 
