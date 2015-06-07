#include <vector>
#if defined(PUSH_FUNC) || defined(EMPLACE_FUNC)
#include <functional>
#endif
#include <memory>
#include <chrono>
#include <iostream>
using C = std::chrono::steady_clock;
using ns = std::chrono::nanoseconds;
template <typename T>
class func;

template <typename R, typename ... A>
class func<R(A...)>
{
public:
  virtual ~func() {}
  virtual R call(A...) = 0;
};

template <typename T, typename R, typename ... A>
class func_n : public func<R(A...)>
{
public:
  template <typename U>
  func_n(U&& u) : t(std::forward<U>(u)) {}
  R call(A... a) override { return t(a...); }
private:
  T t;
};

template <typename R, typename ... A, typename T>
auto make_func(T&& t)
{
  using type = typename std::decay<T>::type;
#ifdef DERIVED
  return std::unique_ptr<func_n<type, R, A...>>(new func_n<type, R, A...>(std::forward<T>(t)));
#else
  return std::unique_ptr<func<R(A...)> >(new func_n<type, R, A...>(std::forward<T>(t)));
#endif
 }

#ifdef EMPLACE_FUNC
using function = std::function<int(int)>;
#define X(x) v.emplace_back(x)
#endif
#ifdef PUSH_FUNC
using function = std::function<int(int)>;
#define X(x) v.push_back(x)
#endif
#ifdef EMPLACE_TEMPLATE
using function = std::unique_ptr<func<int(int)>>;
#define X(x) v.emplace_back(make_func<int, int>(x))
#endif
#ifdef PUSH_TEMPLATE
using function = std::unique_ptr<func<int(int)>>;
#define X(x) v.push_back(make_func<int, int>(x))
#endif
#define Y(x) X([](int n) { return n + x; });
#define REPEAT10(x) REPEAT9(x); x
#define REPEAT9(x) REPEAT8(x); x
#define REPEAT8(x) REPEAT7(x); x
#define REPEAT7(x) REPEAT6(x); x
#define REPEAT6(x) REPEAT5(x); x
#define REPEAT5(x) REPEAT4(x); x
#define REPEAT4(x) REPEAT3(x); x
#define REPEAT3(x) REPEAT2(x); x
#define REPEAT2(x) REPEAT1(x); x
#define REPEAT1(x) x
#define REPEAT0(x)
#define REPEAT(n, t) REPEAT ## n(t)

template <typename R, typename ... T>
R call(std::function<R(T...)> &f, T... t)
{
  return f(t...);
}

template <typename R, typename ... T>
R call(std::unique_ptr<func<R(T...)>>& r, T... t)
{
  return r->call(t...);
}

void f(std::vector<function>& v)
{
  REPEAT10(REPEAT(10, REPEAT(10, Y(__COUNTER__))));
}

int main()
{
  std::vector<function> v;
  auto before_pop = C::now();
  f(v);
  auto after_pop = C::now();
  int s = 0;
  for (auto& f : v) s+=call(f, s);
  auto after_eval = C::now();

  std::cout << "Populate: " << ns(after_pop - before_pop).count()
            << "\nEvaluate: " << ns(after_eval - after_pop).count() << '\n';
}
