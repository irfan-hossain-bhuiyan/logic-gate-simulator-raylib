#include <raylib.h>
#include <raymath.h>

using i32 = int;
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;
using u8 = char;
#define pii pair<int, int>
#define pb push_back
#define mp make_pair
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
// Loop macros for brevity
#define range(i, a, b) for (int i = (a); i < (b); ++i)
#define rangeb(i, a, b, s) for (i64 i = (a); i < (b) - (s) + 1; i += (s))
#define ranges(i, a, b, s) for (i64 i = (a); i < (b); i += (s))
#define rangerev(i, a, b) for (int i = (b)-1; i >= (a); --i)
#define rangerevs(i, a, b, s) for (i64 i = (b)-1; i >= (a); i -= s)
Vector2 operator+(const Vector2 &v1, const Vector2 &v2) {
  return Vector2Add(v1, v2);
}

Vector2 operator-(const Vector2 &v1, const Vector2 &v2) {
  return Vector2Subtract(v1, v2);
}

Vector2 operator*(const Vector2 &v, float scalar) {
  return Vector2Scale(v, scalar);
}

Vector2 operator/(const Vector2 &v, float scalar) {
  return Vector2Divide(v, {scalar, scalar});
}

struct Circle {
  Vector2 center;
  float radius;
};
void DrawCircleCir(Circle cir, Color color) {
  DrawCircle(cir.center.x, cir.center.y, cir.radius, color);
}
