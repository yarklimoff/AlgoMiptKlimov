// https://contest.yandex.ru/contest/20642/run-report/41612074/

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

Point::Point(long long _x, long long _y):x(_x), y(_y) {}

bool operator < (Point a, Point b) {
    return (a.x < b.x) || ((a.x == b.x) && (a.y < b.y));
}

bool operator > (Point a, Point b) {
    return (a.x > b.x) || ((a.x == b.x) && (a.y > b.y));
}

bool operator == (Point a, Point b) {
    return ((a.x == b.x) && (a.y == b.y));
}

bool operator != (Point a, Point b) {
    return !(a == b);
}

double dist(Point a, Point b) {
    return sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}

class Vector {
public:
    long long x, y;
    Vector();
    Vector(long long a, long long b);
    Vector(Point a, Point b);
    Vector operator +(const Vector& v);
    Vector operator -(const Vector& v);
};

Vector::Vector(): x(0), y(0) {}

Vector::Vector(long long a, long long b): x(a), y(b) {}

Vector::Vector(Point a, Point b): x(b.x - a.x), y(b.y - a.y) {}

Vector Vector::operator +(const Vector& v) {
    return Vector(this -> x + v.x, this -> y + v.y);
}

Vector Vector::operator -(const Vector& v) {
    return Vector(this -> x - v.x, this -> y - v.y);
}

Vector operator *(const long long k, Vector v) {
    return Vector(v.x * k, v.y * k);
}

Vector operator *(Vector v, const long long k) {
    return Vector(v.x*k, v.y*k);
}

Vector operator /(const long long k, Vector v) {
    return (1 / k) * v;
}

Vector operator /(Vector v, const long long k) {
    return (1 / k) * v;
}

Point operator +(Point p, Vector v) {
    return Point(p.x + v.x, p.y + v.y);
}

long long PseudoscalarDot(Vector a, Vector b) {
    return a.x * b.y - a.y * b.x;
}

long long ScalarDot(Vector a, Vector b) {
    return a.x * b.x + a.y * b.y;
}

void ConvexHull(Point left, Point right, std::vector<Point>& points) {
    int N = points.size();
    double ans = 0;
    std::vector<Point> up, down;
    up.push_back(left);
    down.push_back(left);
    sort(points.begin(), points.end());
    for (int i = 1; i < N; ++i) {
        if (points[i] == right || PseudoscalarDot(Vector(left, right),
                                                  Vector(left, points[i])) > 0) {
            while (up.size() >= 2 && PseudoscalarDot(Vector(up[up.size()-2], points[i]),
                                                     Vector(up[up.size()-2], up[up.size()-1])) <= 0) {
                up.pop_back();
            }
            up.push_back(points[i]);
        }
        if (points[i] == right || PseudoscalarDot(Vector(left, right),
                                                  Vector(left, points[i])) < 0) {
            while (down.size() >= 2 && PseudoscalarDot(Vector(down[down.size()-2], points[i]),
                                                       Vector(down[down.size()-2], down[down.size()-1])) >= 0) {
                down.pop_back();
            }
            down.push_back(points[i]);
        }
    }
    for (int i = 0; i < up.size() - 1; ++i) {
        ans += dist(up[i], up[i + 1]);
    }
    for (int i = 0; i < down.size() - 1; ++i) {
        ans += dist(down[i], down[i + 1]);
    }
    std::cout << std::fixed << std::setprecision(10) << ans;
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
    ConvexHull(left, right, points);
    return 0;
}
