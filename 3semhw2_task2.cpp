#include <math.h>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <vector>
#define INF 1000000000LL

class Point {
public:
    long long x, y;
    Point();
    Point(long long _x, long long _y);
    friend std::istream& operator >> (std::istream& is, Point& p) {
        long long x, y;
        is >> x >> y;
        p = Point(x, y);
        return is;
    }
};

Point::Point():x(0LL), y(0LL) {}

Point::Point(long long _x, long long _y) : x(_x), y(_y) {}

bool operator<(const Point& a, const Point& b) {
    return (a.x < b.x) || ((a.x == b.x) && (a.y < b.y));
}

bool operator>(const Point& a, const Point& b) {
    return (a.x > b.x) || ((a.x == b.x) && (a.y > b.y));
}

bool operator==(const Point& a, const Point& b) {
    return ((a.x == b.x) && (a.y == b.y));
}

bool operator!=(const Point& a, const Point& b) {
    return !(a == b);
}

double dist(const Point& a, const Point& b) {
    return sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}

class Vector {
public:
    long long x, y;
    Vector();
    Vector(long long a, long long b);
    Vector(const Point& a, const Point& b);
    Vector operator+(const Vector& v);
    Vector operator-(const Vector& v);
};

Vector::Vector() : x(0), y(0) {}

Vector::Vector(long long a, long long b) : x(a), y(b) {}

Vector::Vector(const Point& a, const Point& b) : x(b.x - a.x), y(b.y - a.y) {}

Vector Vector::operator+(const Vector& v) {
    return Vector(this->x + v.x, this->y + v.y);
}

Vector Vector::operator-(const Vector& v) {
    return Vector(this->x - v.x, this->y - v.y);
}

Vector operator*(const long long k, const Vector& v) {
    return Vector(v.x * k, v.y * k);
}

Vector operator*(const Vector& v, const long long k) {
    return Vector(v.x * k, v.y * k);
}

Vector operator/(const long long k, const Vector& v) {
    return (1 / k) * v;
}

Vector operator/(const Vector& v, const long long k) {
    return (1 / k) * v;
}

Point operator+(const Point& p, const Vector& v) {
    return Point(p.x + v.x, p.y + v.y);
}

long long PseudoscalarDot(const Vector& a, const Vector& b) {
    return a.x * b.y - a.y * b.x;
}

std::vector<Point> ConvexHull(Point left, Point right, std::vector<Point> points) {
    int N = points.size();
    std::vector<Point> up, down;
    up.push_back(left);
    down.push_back(left);
    sort(points.begin(), points.end());
    for (int i = 1; i < N; ++i) {
        if (points[i] == right || PseudoscalarDot(Vector(left, right),
                                                  Vector(left, points[i])) > 0) {
            while (up.size() >= 2 && PseudoscalarDot(Vector(up[up.size() - 2], points[i]),
                                                     Vector(up[up.size() - 2], up[up.size() - 1])) <= 0) {
                up.pop_back();
            }
            up.push_back(points[i]);
        }
        if (points[i] == right || PseudoscalarDot(Vector(left, right),
                                                  Vector(left, points[i])) < 0) {
            while (down.size() >= 2 && PseudoscalarDot(Vector(down[down.size() - 2], points[i]),
                                                       Vector(down[down.size() - 2], down[down.size() - 1])) >= 0) {
                down.pop_back();
            }
            down.push_back(points[i]);
        }
    }
    for (int i = up.size() - 1; i >= 0; --i) {
        down.push_back(up[i]);
    }
    return down;
}

double CountPerimetr(Point left, Point right, std::vector<Point> points) {
    std::vector<Point> res = ConvexHull(left, right, points);
    double ans = 0;
    for (int i = 0; i < res.size() - 1; ++i) {
        ans += dist(res[i], res[i + 1]);
    }
    return ans;
}
int main() {
    int N;
    std::cin >> N;
    std::vector<Point> points(N);
    Point left = Point(INF, INF), right = Point(-INF, -INF);
    for (int i = 0; i < N; ++i) {
        std::cin >> points[i];
        if (points[i] < left) {
            left = points[i];
        }
        if (right < points[i]) {
            right = points[i];
        }
    }
    std::cout << std::fixed << std::setprecision(10) << CountPerimetr(left, right, points);
    return 0;
}
