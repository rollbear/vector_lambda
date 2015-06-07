This is the source code used to collect timing data for the blog post
[Performance observations on a C++vector of lambdas](http://playfulprogramming.blogspot.com/2015/06/performance-observations-on-c-vector-of.html)

The sources uses a set of macros to enable during compilation, e.g. using the
-D compiler flag to g++ or clang++.

These macros are:

- **`PUSH_FUNC`** uses `push_back()` on the `std::vector<std::function<int(int)>>`
- **`EMPLACE_FUNC`** uses `emplace_back()` on the `std::vector<std::function<int(int)>>`
- **`PUSH_TEMPLATE`** uses `push_back()` on the `std::vector<std::unique_ptr<func<int(int)>>>`
- **`EMPLACE_TEMPLATE`** uses `emplace_back()` on the `stc::vector<std::unique_ptr<func<int(int)>>>`
- **`DERIVED`** used with **`PUSH_TEMPLATE`** or **`EMPLACE_TEMPLATE`** to make `std::unique_ptr<func_n<lambda, int, int>>` instead of `std::unique_ptr<func<int(int)>>`

Example:  
`clang++ -DEMPLACE_TEMPLATE -DDERIVED -std=c++14 -O t.cpp`

or:  
`g++ -DPUSH_FUNC -std=c++14 -ggdb t.cpp`
