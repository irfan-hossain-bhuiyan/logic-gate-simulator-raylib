#pragma once
#include <functional>
#include <queue>
#include <raylib.h>
#include <raymath.h>
#include <string>
const std::string TRUE = "TRUE";
const std::string FALSE = "FalSE";
using i32 = int;
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;
using u8 = char;
#define pii pair<int, int>
#define pb push_back
#define mp make_pair
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
Rectangle operator+(const Rectangle &r1, const Vector2 pos) {
  return Rectangle{r1.x + pos.x, r1.y + pos.y, r1.width, r1.height};
}
struct Circle {
  Vector2 center;
  float radius;
};
void DrawCircleCir(Circle cir, Color color) {
  DrawCircle(cir.center.x, cir.center.y, cir.radius, color);
}
bool CheckCollisionPointCircle(Vector2 point, Circle cir) {
  return CheckCollisionPointCircle(point, cir.center, cir.radius);
}
std::string vectorToString(const std::vector<std::string> &vec) {
  std::string result;
  for (size_t i = 0; i < vec.size(); ++i) {
    result += vec[i];
    if (i < vec.size() - 1) {
      result += "\n";
    }
  }
  return result;
}

int levenshteinDistance(const std::string &s1, const std::string &s2) {
  // Convert both strings to lowercase
  std::string lowerS1 = s1;
  std::string lowerS2 = s2;
  std::transform(lowerS1.begin(), lowerS1.end(), lowerS1.begin(), ::tolower);
  std::transform(lowerS2.begin(), lowerS2.end(), lowerS2.begin(), ::tolower);

  int len1 = lowerS1.length();
  int len2 = lowerS2.length();

  std::vector<std::vector<int>> matrix(len1 + 1, std::vector<int>(len2 + 1));

  for (int i = 0; i <= len1; ++i) {
    matrix[i][0] = i;
  }

  for (int j = 0; j <= len2; ++j) {
    matrix[0][j] = j;
  }

  for (int i = 1; i <= len1; ++i) {
    for (int j = 1; j <= len2; ++j) {
      int cost = (lowerS1[i - 1] == lowerS2[j - 1]) ? 0 : 1;
      matrix[i][j] = std::min({matrix[i - 1][j] + 1, matrix[i][j - 1] + 1,
                               matrix[i - 1][j - 1] + cost});
    }
  }

  return matrix[len1][len2];
}

// Function to perform fuzzy search in a vector of strings


// Function to perform fuzzy search in a vector of strings
std::vector<std::string> fuzzySearch(const std::string& query, const std::vector<std::string>& vec) {
    const int threshold = 2; // Adjust threshold for fuzzy search
    std::vector<std::string> ans;
    
    // Define a priority queue to store pairs of (distance, string)
    std::priority_queue<std::pair<int, std::string>> pq;

    // Iterate through each word in the vector
    for (const std::string& word : vec) {
        int distance = levenshteinDistance(query, word);
        if (distance <= threshold) {
            // Invert the distance before inserting into the priority queue
            pq.push(std::make_pair(-distance, word));
        }
    }
    
    // Extract strings from the priority queue in ascending order of distance
    while (!pq.empty()) {
        ans.push_back(pq.top().second);
        pq.pop();
    }
    
    return ans;
}

