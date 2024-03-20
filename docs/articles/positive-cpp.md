---
layout: default
title: A positive approach to developing in C++
parent: Articles
---

# A positive approach to developing in C++

_March 2024_

A coworker used to joke that most web applications just ferry JSON from one system to another.

There's probably a good amount of truth in that joke.

That got me thinking, could C++ be used for such types of applications? And, is it worthwhile to implement a REST (or something like that) backend in C++ ? ([find some examples here](https://github.com/bignacio/soc/tree/main/docs/articles/code/positivecpp){:target="_blank"}).

The answer is probably yes, but that's not what I want to talk about. I want to explore how C++ is often depicted as a complex and risky language. At times, it is almost feared, but it doesn't have to be.

Developing in C++ can be approached with the same level of care (or concern) as any other programming language or tool.

## C++ as a tool for creating software

The C++ language in its totality is complicated. It is enormous, not unlike other languages that have been around a long time. But we can go a long way just with the simplest, most fundamental concepts.

Let's see how we can approach some of the commonly challenging points differently.

### Data types and structures

C++ and the standard library cover all the data types we could ask for: integers, floating-point numbers, strings, lists, hash tables, sets, trees, and beyond.

These types and data structures behave exactly as we'd anticipate, making it easy for developers to move on to the next topic without much difficulty.

### Memory management

Memory management and safety in C++ have recently garnered significant attention, with [expert grade material](https://github.com/BjarneStroustrup/profiles){:target="_blank"} making compelling arguments for safer C++ programming practices.

Rather than trying to reword the same arguments, I'll offer my straightforward and incremental approach to writing memory-safe code in C++.

Memory-safe access and management primarily revolve around data ownership and lifetime. In C++, data can be represented by value, by reference, or through pointers.

Here's a few things we can do to work with all that safely and correctly:

Here's a revised version of your points:

1. **Avoid Pointers**: Pointers aren't necessary in most of the code we write, if not all.
2. **Use Values**: Assign values, pass everything by value, and return by value (more on that below).
3. **Then pass by reference**: If necessary, consider passing by reference, which might be needed at some point.
4. **And use Smart Pointers**: Only resort to smart pointers (unique pointers, shared pointers, etc.) if absolutely necessary. Make sure to have a good understanding of how they work and when to use them before diving in.

**Notes on working with values only**: C++ developers (and even linters!) will point out that using values for everything can result in significant data copying and affect program performance. That is true, and I'll just note that, like any optimization, don't worry about it until you have to worry about it. Measure first, and if it works for you, move on to the next problem.

All data structures I mentioned above will work just fine with values.

**Notes on smart pointers**: They aren't that hard to use and can be safely used without any deep knowledge about their internals. For those new to C++, you can go a long way without using them, so long as you never use pointers. Sometimes, simplifying the code and avoiding long-living objects and multiple ownership might be a better solution.

Instead of worrying about memory management intricacies, focus on writing clear, expressive code that solves the problem at hand. Let the language features and standard library do the heavy lifting for you.

### Programming paradigms

C++ has great support for object-oriented programming. It comes with all the bells and whistles out of the box, and even includes a few contentious features like class friendship and multiple inheritance.

My suggestion here is simple: stick to object-oriented best practices techniques.

However, C++ also supports first-class high-order functions, even though it's not necessarily considered a functional programming language. Lambdas, in particular, are quite powerful and easy to use.

### Template metaprogramming

We can do a lot without ever using template metaprogramming, while still writing good code . Most people in fact might never have to use it until a class or function need to be made generic. In that case, templates can be used just as we would do generic programming in any other programming language.

It would be correct to note that template metaprogramming isn't the same as generic programming. However, I'd say it's a fair simplification to consider it as a way to implement generic behaviour like we would in another programming language.

### Strings

Strings are a common source of memory management concerns in programming languages. In C++, the standard string class `std::string` handles memory management for us.
When you a string is created, the necessary memory is allocated automatically. We can modify it, increase it, shrink it and concatenate strings without having to worry about memory allocation.
When the string is not in use anymore, the memory is freed up without any extra effort from the developer.


### Build systems, package management and other tooling

Yeah, this part is not particularly great. If there's anything worth investing ahead of time is researching build and package management tools. Starting with a fully automated build + test + deploy pipeline from day zero is extremely important.

Use an IDE. Visual Studio Code, CLion and Visual Studio on Windows are great. Sure, Vi(m) and Emacs work great too :)

As for other tools, C++ has great linters. Use them. Also from day zero.

### Concurrency and networking

The C++ Standard library has sufficiently good support for concurrent programming but nothing for networking.

In any case, these are hard things to do properly even for the most experience C++ developer. Unless it's your area of expertise, I would suggest finding a framework that hides all complexity and does what you need it to do. The simpler the better.

## In the end, after all this

If you're considering starting with C++ or just looking at options for a new project, give C++ a chance.
While it's long been known for its use in high-performance and low-level applications, C++ can also easily be used in other domains.

Programming languages often become associated with specific domains, and usually, there are good reasons for that. Ultimately, it's about finding the best and most suitable tool for the job.

All in all, always be positive.