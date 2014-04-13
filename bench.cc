#include <map>
#include <unordered_map>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <functional>
#include <vector>
#include <utility>
#include <chrono>
#include <random>

#include <timer.hh>
#include <macros.hh>

using namespace std;

static inline pair<double, double>
stats(const vector<double> &v)
{
  double s = 0.;
  for (auto e : v)
    s += e;
  const double mu = s / double(v.size());

  double s1 = 0.;
  for (auto e : v)
    s1 += (e - mu) * (e - mu);
  const double var = s1 / double(v.size());

  return make_pair(mu, sqrt(var));
}

static pair<double, double>
timeit(function<void()> f, size_t n)
{
  vector<double> rts(n);
  timer t;
  for (size_t i = 0; i < n; i++) {
    f();
    rts[i] = t.lap_ms();
  }
  return stats(rts);
}

template <typename T>
static inline void
insert(T &t, size_t n)
{
  for (size_t i = 0; i < n; i++)
    t[i] = i;
}

template <typename T, typename PRNG>
static inline void
lookup(const T &t, PRNG &rng, size_t n)
{
  uniform_int_distribution<size_t> dist(0, t.size()-1);
  size_t s = 0;
  for (size_t i = 0; i < n; i++) {
    const auto it = t.find(i);
    if (it == t.end())
      continue;
    ALWAYS_ASSERT(it->second == i);
    s++;
  }
  ALWAYS_ASSERT(s == n);
}

template <typename K, typename V>
static inline ostream &
operator<<(ostream &o, const pair<K, V> &kv)
{
  o << kv.first << " " << kv.second;
  return o;
}

typedef default_random_engine PRNG;

template <typename T>
static void
benchinsert(const vector<size_t> &ns)
{
  for (auto n : ns) {
    const auto insert_stats = timeit([n](void) {
        T m;
        insert(m, n);
    }, 50);
    cout << insert_stats << endl;
  }
}

template <typename T, typename PRNG>
static void
benchlookup(const vector<size_t> &ns, PRNG &rng)
{
  for (auto n : ns) {
    T m;
    insert(m, n);
    const auto lookup_stats = timeit([&m, &rng, n](void) {
        lookup(m, rng, n);
    }, 50);
    cout << lookup_stats << endl;
  }
}

typedef map<size_t, size_t> map_type;
typedef unordered_map<size_t, size_t> unordered_map_type;

int
main(int argc, char **argv)
{
  const vector<size_t> ns({1000, 10000, 100000, 1000000, 10000000});
  const unsigned seed =
    chrono::system_clock::now().time_since_epoch().count();
  PRNG rng(seed);
  benchinsert<map_type>(ns);
  benchlookup<map_type>(ns, rng);
  benchinsert<unordered_map_type>(ns);
  benchlookup<unordered_map_type>(ns, rng);
  return 0;
}
