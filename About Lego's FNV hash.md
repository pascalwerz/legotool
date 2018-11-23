Here's a bit of 'theory' about the FNV1 hash used by Lego games

First, consult the [FNV hash wikipedia article](https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function). The hash function used is the 32-bits FNV1 using the usual FNV 32-bits offset basis (0x811c9dc5).

Second, beware that the FNV1 Lego function is case insensitive. No fancy international characters support here, a-z are simply mapped to A-Z, that is, the 26 letters are uppercased: hashed string are ASCII only identifiers. From now on, FNV or fnv stands for the Lego's 32-bits FNV1 uppercased hash function and, except were noted, the initial hash or offset is 0x811c9dc5.

Lego games make extensive use of its FNV1 hash function. Every object's name is remembered by its hash, transforming name comparison from a string compare to an integer compare, much much faster.

Adding a game support or updating **legotool** mainly requires finding the string that yields a known FNV hash. Sometimes, while searching such strings, you may know a hash is built from a compound string, e.g. you know the string starts with "DebugUsed" or you suspect the string ends with "_StoryComplete". Some 'theory' may help you with some hard-to-reverse hash.



Given:

*fnv(seed, string)*

​	the function that computes the hash of uppercased *string* with initial hash (or offset) equal to *seed*.

*&*

​	the string concatenation operator. String concatenation isn't commutative: "HardlyWork" isn't the same as "WorkHardly".



**A - Reversibility**

fnv(seed, str) is only partially reversible:
​	in *hash=fnv(seed, str)* there are 3 variables: *hash*, *seed* and *str*
​		if you know *seed* and *str*, you can find *hash*, easy, that's the fnv(seed, str) function
​		if you know *hash* and *str*, you can find *seed*, easy, that's the fnv'(hash, str) function (see **B** below)
​		if you know *hash* and *seed*, finding *str* must be done using brute force or by trying strings in a dictionary and there are many possible strings, due to hash collisions.

*hash=fnv(seed, str)* is computed with command `hash <seed> <str>`



**B - *seed=fnv'(hash, str)***

In *hash*=*fnv(seed, str)*, if you know both *hash* and *str*, you can compute the initial *seed* using a 32-bits uppercased *FNV1a-like* function, but instead of using the usual prime 16777619 (0x1000193), you have to use 0x359c449b, the modular inverse of 0x1000193 (this means that 0x359c449b × 0x1000193 is 1 modulo 2^32). As the initial seed of this function, we have to use *hash*. The result of this computation will be *seed*. This function is noted *seed=fnv'(hash, str)*.

Hence, *seed=fnv'(fnv(seed, str), str)*

*seed=fnv'(hash, str)* is computed with command `hash -r <hash> <str>`



**C - Computing the hash of concatenated strings**

The hash of concatenated strings can be computed in separate steps, proceding from *left to right*:

*fnv(seed, str1 & str2 & str3) = fnv(fnv(seed, str1), str2 & str3) = fnv(fnv(fnv(seed, str1), str2), str3)*



**D - Computing the seed of concatenated strings**

The initial seed of concatenated string can be computed if you know the final hash, in separate steps, from *right to left*.

As *hash = fnv(seed, str1 & str2 & str3) = fnv(fnv(fnv(seed, str1), str2), str3)*, so *fnv(fnv(seed, str1), str2) = fnv'(hash, str3)* and *fnv(seed, str1) = fnv'(fnv'(hash, str3), str2)* and so:

*seed = fnv'(fnv'(fnv'(hash, str3), str2), str1)*



**E - Finding the middle string**

given *hash = fnv(seed, str1 & str2 & str3)*, if you know *str1* and *str3*, you can simplify the work when trying *str2* candidates.

Given *hash = fnv(seed, str1 & str2 & str3) = fnv(fnv(fnv(seed, str1), str2), str3)*, trying a candidate for *str2* involves computing the hash of the complete concatenated string, in one or three steps, to verify if it matches. Let's simplify this process.

First, as *str1* is known and constant, compute *hash1=fnv(seed, str1)*, so now *hash fnv(fnv(hash1, str2), str3)*.

Second, as *str3* is known and constant, compute *seed3 = fnv'(hash, str3)*. Now you can check if some *str2* candidate matches by checking if *seed3 == fnv(hash1, str2)*. This is much simpler and faster.

This kind of operation is done via `rhash -p <str1> <hash>` or `rhash -P <hash1> <hash>`.



**F - Common left part**

Using similar techniques, if two strings have a common left part, such as "leftPartABCD" and "leftPartXYZ" if you know their hash but don't know the left part string nor the right part strings, you may check for right part candidates that would yield the same hash for an yet unknown left part.

This is done with command `rhash -k <hash1> <hash2>`.

e.g. (this example requires that "Adam" and "Eve" both are in your dictionary):

`hash "leftpart" "leftPartAdam" "leftPartEve"`

will output:

`{ 0xa7e9dc3c, gameAny, -1, 0x0, "leftPartAdam", "", NULL },`

`{ 0x50f7a05d, gameAny, -1, 0x0, "leftPartEve", "", NULL },`

`{ 0xdf220075, gameAny, -1, 0x0, "leftPart", "", NULL },`

and now

`rhash -k 0xa7e9dc3c 0x50f7a05d|sort -u`

will display, among topmost candidates:

 `4003 0xdf220075 is (0xa7e9dc3c, 0x50f7a05d) without trailing ('Adam', 'Eve')`

You see that both "Adam" and "Eve" are found, as is the "leftPart"'s hash 0xdf220075. It's up to you to look or not what 0xdf220075 stands for.