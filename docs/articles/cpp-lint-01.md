---
layout: default
title: Linting away manipulation of raw pointers with C++20
parent: Articles
---

# Linting away manipulation of raw pointers with C++20

_February 2024_


This article could very well have been titled "clang-tidy told me not to never do this", but that sounded too clickbait-y to me.

All I really want to talk about is how awesome clang-tidy and how important it is to pay attention to code linting results.
The various linting rules create and maintain are really awesome to prevent future headaches and migraines.

Let's look at the - for some - rare case of parsing binary data.

Either from a custom protocol for network data transfer or file encoding, we may find ourselves at some point with just a pointer to a memory address and a length.

Putting it in code, crudely, something like this

```c++
void parse(const char* ptr, size_t length) {
    ...
}
```

Before we get in too deep, let me warn you I'm intentionally ignoring differences between hardware platforms and Endianness fun. Just beware those are important things to consider.

I'd also like to note there's a few (or more) different ways to solve the problems I discuss below and I will not advocate for any one of them. So long as it keeps your code safe, that is. Chose wisely.

---

Suppose our data contains a an array of ASCII strings and the protocol we chose to encoded it is as such.

```
- 1 byte for the number of strings
- 2 bytes for N, representing the length of the next string
- N bytes for the string
```

Then we go about implementing our function `parse` and the first thing we do is to parse the first byte, which tells us how many strings we have in the data.

Ok that's not true, the first thing we should do is check `ptr` is valid and there's no chance we'll be reading from invalid memory addresses.

But let's continue with reading the number of strings.

```
uint8_t count = *(uint8_t*)(ptr);
```

Easy one-liner. Done.

As safety-conscious developers, we compile with our favourite warnings dialed up at max and run a linter through the code as well.
Usually `clang-tidy` with a solid choice of checks and **also** run with `--warnings-as-errors='*'`.

And then we get this

```
error: do not use C-style cast to convert between unrelated types [cppcoreguidelines-pro-type-cstyle-cast,-warnings-as-errors]
    uint8_t count = *(uint8_t*)(ptr);
```

The [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/) are pretty solid and not to be ignored. Not to mention my one-liner above is UB.

Thankfully, C++ 20 introduced `bit_cast` (granted, for those who can use C++ 20) which solves this particular problem for us very nicely.

```
auto count = bit_cast<uint8_t>(*ptr);
```

One line of successfully code, yay! On to the next ones. Now that I know how many strings, I just need to read the length of each and jump to the next one until all have been read.

In my read loop, I start with something like this

```
auto stringLen = bit_cast<uint16_t>(*ptr);

std::string value(ptr+sizeof(uint16_t), stringLen);

```

And we try to compile it and compile/lint it and all alarms go off.
First, we can't bit_cast `*(char*  ptr)` to `uint16_t` because `char` and `uint16_t` have different lengths.
I could static cast `char* ptr` to `uint16_t* ptr` and that would probably work but there's another problem in that code.

```
 error: do not use pointer arithmetic [cppcoreguidelines-pro-bounds-pointer-arithmetic,-warnings-as-errors]
    string value(ptr+sizeof(uint16_t), len);
```

Pinter arithmetic is also discouraged. So maybe we can solve both cases with the same approach. For that, I'll use `std::span` (also C++20, sorry) for everything.

It goes like this

```
const std::span<char> input(ptr, length);

auto count = bit_cast<uint8_t>(input.front());

while(count >0){
  uint16_t stringLen;
  std::copy_n(input.data(), sizeof(uint16_t), static_cast<char*>(static_cast<void*>(&stringLen)));

  std::span<char> remainder = input.subspan(sizeof(uint16_t));
  std::string value(remainder.data(), stringLen);

  input = remainder.subspan(stringLen);

  count--;
}
```

Here `span::subspan` and `std::copy_n` let's us safely avoid pointer arithmetic and direct pointer manipulation.

Side note, the whole `static_cast<char*>(static_cast<void*>(` throws me off a bit but given it's a `static_cast` I can lie myself to sleep at night. There's probably a nicer way to do that.

And that's it, that's all we need to work with raw pointers in a nice and safer way.

As mentioned above, there are other thing to consider when it comes to memory safety. This is one more tool in our toolkit.

## A note on performance

Is `std::span` a zero cost abstraction? In my experience, for this case it can be.
Let's look at a more general simple example in Compiler Explorer, comparing classic pointer casting and arithmetic against using `std::span`


```
int classic(const char* ptr) {

    int val = *((int*)ptr);

    return val;
}

int tidyfied(const char* ptr){
    int val;
    std::copy_n(ptr, sizeof(val), static_cast<char*>(static_cast<void*>(&val)));

    return val;
}

int classic_skip(const char *ptr, int pos){

    int val = *(int*)(ptr+pos);

    return val;
}

int tidyfied_skip(const char *ptr, size_t len, int pos){
    int val;
    std::span<const char> data = std::span<const char>(ptr, len).subspan(pos);


    std::copy_n(data.data(), sizeof(val), static_cast<char*>(static_cast<void*>(&val)));

    return val;
}
```

For `x8_64`, both clang (17) and gcc (13) generate assembly outputs like this
```
classic(char const*):
        mov     eax, DWORD PTR [rdi]
        ret
tidyfied(char const*):
        mov     eax, DWORD PTR [rdi]
        ret
classic_skip(char const*, int):
        movsx   rsi, esi
        mov     eax, DWORD PTR [rdi+rsi]
        ret
tidyfied_skip(char const*, unsigned long, int):
        movsx   rdx, edx
        mov     eax, DWORD PTR [rdi+rdx]
        ret
```

Which seem pretty good to me.

Even a generic version, for trivially copiable types, seem to work well (again, on x86_64)

```
template<typename T>
T generic_convert(const char *ptr, size_t len, int pos) {
    T val;
    std::span<const char> data = std::span<const char>(ptr, len).subspan(pos);


    std::copy_n(data.data(), sizeof(val), static_cast<char*>(static_cast<void*>(&val)));

    return val;
}

struct trivially_copiable {
 int a;
 int b;
 char c;
};

trivially_copiable parse_generic(const char *ptr, size_t len, int pos){
    trivially_copiable d = generic_convert<trivially_copiable>(ptr, len, pos);
    return d;
}
```

The code above generates the following assembly
```
parse_generic(char const*, unsigned long, int):
        movsx   rdx, edx
        add     rdi, rdx
        mov     rax, QWORD PTR [rdi]
        mov     edx, DWORD PTR [rdi+8]
        ret
```

Not too bad.



If this post tries to make any point is that linters are great, clang-tidy is great, (even if slow) and that C++ Core Guidelines and other linting rules are something to pay close attention too.

All the best