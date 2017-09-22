#include <bits/stdc++.h>

using namespace std;

struct fraction {
  using T = ll;

  T x = 0, y = 1;

  fraction() {
    x = 0;
    y = 1;
  }

  fraction(T a) {
    x = a;
    y = 1;
  }

  fraction(T a, T b) {
    x = a;
    y = b;
    reduce();
  }

  void reduce() {
    auto g = __gcd(abs(x), abs(y));
    x /= g;
    y /= g;

    if(y < 0)
      x = -x, y = -y;
  }

  T lcm(T x, T y) const {
    return x / __gcd(x, y) * y;
  }

  fraction flip() const {
    if(x == 0)
      return *this;
    return fraction{y, x};
  }

  operator T() {
    return x/y;
  }

  fraction& operator=(T a) {
    x = a;
    y = 1;
    return *this;
  }

  fraction operator-() const {
    return fraction(-x, y);
  }

  fraction operator+(fraction const& b) const {
    auto m = lcm(y, b.y);

    T x2 = (m/y)*x + (m/b.y)*b.x;
    T y2 = m;

    return fraction(x2, y2);
  }

  fraction operator-(fraction const& b) const {
    auto f = -b;
    return *this+f;
  }

  fraction operator*(fraction const& b) const {
    return fraction(x*b.x, y*b.y);
  }

  fraction operator/(fraction const& b) const {
    return *this * b.flip();
  }

  bool operator==(fraction const& b) const {
    return (x==b.x && y==b.y);
  }

  bool operator<(fraction const& b) const {
    return x*b.y < y*b.x;
  }

  bool operator>(fraction const& b) const {
    return x*b.y > y*b.x;
  }

  bool operator<=(fraction const& b) const {
    return x*b.y <= y*b.x;
  }

  bool operator>=(fraction const& b) const {
    return x*b.y >= y*b.x;
  }

  friend istream& operator>>(istream& in, fraction& me) {
    return in >> me.x;
  }

  friend ostream& operator<<(ostream& out, fraction& me) {
    return out << me.x << "/" << me.y;
  }
};

int main() {
  fraction a = 2;
  fraction b = 6;
  fraction c = a/b;
  
  cout << c << endl;
}
