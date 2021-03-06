// The MIT License (MIT)
//
// Copyright (c) 2018 Mateusz Pusz
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "units/bits/external/hacks.h"
#include "units/physical/dimensions/length.h"
#include "units/physical/dimensions/speed.h"
#include "units/physical/si/base/length.h"
#include "units/physical/si/base/mass.h"
#include "units/physical/si/base/time.h"
#include "units/physical/si/derived/area.h"
#include "units/physical/si/derived/speed.h"
#include <cassert>
#include <cstdint>
#include <type_traits>

#if defined(COMP_MSVC)
#pragma warning( disable : 4459 ) // Disable shadowing warning.
#endif

using namespace units;
using namespace units::physical;
using namespace units::physical::si;
using namespace units::physical::si::unit_constants;

static_assert(2 * m == 2_q_m);
static_assert(2 * s == 2_q_s);
#if !defined(COMP_MSVC) || defined(NDEBUG)
static_assert([]<auto& s = ::s>() {
  assert(!requires { s / 2; });
  assert(!requires { s * 2; });
  assert(!requires { s + 2; });
  assert(!requires { s + s; });
  return 1_q_s + s == 2_q_s;
}());
#endif

constexpr auto m_per_s = m / s;

static_assert(2_q_m / s == 2_q_m_per_s);
static_assert(2 * m / s == 2_q_m_per_s);
static_assert(2 / s * m == 2_q_m_per_s);
static_assert(2 * (m / s) == 2_q_m_per_s);
static_assert(2 * ::m_per_s == 2_q_m_per_s);
static_assert(is_same_v<decltype(::m_per_s)::rep, decltype(m)::rep>);

static_assert(120 * km / (2 * h) == 60_q_km_per_h);
static_assert(120 * km / 2 / h == 60_q_km_per_h);
static_assert([] {
  const auto length{120};
  const auto duration{2};
  return length * km / (duration * h);
}() == 60_q_km_per_h);
static_assert(is_same_v<decltype(std::int64_t{120} * km / (2 * h)), decltype(60_q_km_per_h)>);
static_assert(is_same_v<decltype(std::int64_t{120} * km / 2 / h), decltype(60_q_km_per_h)>);
static_assert(is_same_v<decltype(120.L * km / 2 / h), decltype(60._q_km_per_h)>);

static_assert(1. / 4 * m2 == 1._q_m2 / 4);

static_assert([] {
  Speed auto v0 = 10 * m / s;
  signed s = 2;  // hides  ^
  Length auto v = 20 * m / s;
  /*      */ v0 = 10 * m / ::s;
  return !is_same_v<decltype(v0), decltype(v)>;
}());

int main() {}
