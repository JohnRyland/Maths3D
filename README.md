
# cgMaths
### Maths for Computer Graphics

Copyright (c) 2021, John Ryland
All rights reserved.


## Introduction

This code requires a modern C++ compiler but is mostly C and avoids using
classes (with the exception of providing unit type safety discussed below).

No header or library dependencies.

The API attempts to be functional in style, with inputs and a returned
output.

The inputs are never mutated (modified) and the returned value is a newly
constructed object. This is to maintain referential transparency. Some
benefits to this approach are that it is easy to chain together calls to
these APIs. It also means that they can be safely used in multithreaded
code. From a performance point of view it assumes that the compiler will be
able to perform RVO.

Inputs are passed by const reference when the size of the input would be
larger than the size of a pointer.


## Scalars

Changing the scalar type will change the underlying value type used by
vector and matrix.

A scalar value used to represent an angle can sometimes be used incorrectly
when an API is not clear if the units are degrees or radians. To avoid this,
the API itself should document the units with a specific type that encode
the units. The types Degrees and Radians below do this. These are actually
classes and so this is C++, but it is worth the developer time it saves by
ensuring there is no mistakes of this kind. The same could be used for other
types of units as required, such as meters
vs feet if the API were to need this.


## Vector Components

Both the vector and matrix type contain unions with a 'v' member which can
be used for iteration of the components instead of needing to write code
which requires copy paste of the same code for each component. This avoids
mistakes, such as:

```
   Vector4f ret;
   ret.x = a.x + b.x;
   ret.y = a.y + b.y;
   ret.y = a.z + b.z;
```

Do you see the mistake? Happens all the time. Far better is to write code
like this:

```
   Vector4f ret;
   for (int i = 0; i < 3; ++i)
     ret.v[i] = a.v[i] + b.v[i];
```

Less lines of code, less error prone, and easier to update the formula
applied to each component.


## Vector width

The API omits providing a 3 component wide vector and instead only provides
a Vector4f.  This is because when dealing with arrays of these, we will be
able to optimize better the 4 wide version, particularly if it can be
aligned to 128-bits and used with a SIMD optimized function to transform an
array of them. For all the provided functions below, we don't bother
providing SIMD optimizations as most compilers can auto-vectorize this code
just fine and so it only will make the code more error prone, and not able
to be re-compiled with different compiler flags to target different machine
types or without or without different levels of SIMD support.


