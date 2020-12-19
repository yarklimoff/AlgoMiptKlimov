#include <iostream>
#include <iomanip>
#include <cmath>
#define EPS 0.000000000001
struct Point {
    long double x, y, z;
    Point() : x(0.0), y(0.0), z(0.0) {}
    Point(long double _x, long double _y, long double _z) : x(_x), y(_y), z(_z) {}
    friend std::istream& operator>>(std::istream& is, Point& p) {
        long double x, y, z;
        is >> x >> y >> z;
        p = Point(x, y, z);
        return is;
    }
};

struct Segment {
    Point p1, p2;
    Segment() : p1(Point(0.0, 0.0, 0.0)), p2(Point(0.0, 0.0, 0.0)) {}
    Segment(Point _p1, Point _p2) : p1(_p1), p2(_p2) {}
    friend std::istream& operator>>(std::istream& is, Segment& s) {
        Point p1, p2;
        is >> p1 >> p2;
        s = Segment(p1, p2);
        return is;
    }
};

long double Dist(Point a, Point b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
}

long double DistBetweenPointsOnSegments(long double k, const Point& p, const Segment& s) {
    Point m = Point(s.p1.x + (s.p2.x - s.p1.x) * k, s.p1.y + (s.p2.y - s.p1.y) * k, s.p1.z + (s.p2.z - s.p1.z) * k);
    return Dist(p, m);
}

Point FindNearestPointOnSegment(const Point& a, const Segment& s) {
    double l = 0, r = 1;
    while (r - l > EPS) {
        double m1 = l + (r - l) / 3;
        double m2 = r - (r - l) / 3;
        long double q = DistBetweenPointsOnSegments(m1, a, s), w = DistBetweenPointsOnSegments(m2, a, s);
        if (q < w) {
            r = m2;
        } else {
            l = m1;
        }
    }
    return Point(s.p1.x + (s.p2.x - s.p1.x) * l, s.p1.y + (s.p2.y - s.p1.y) * l, s.p1.z + (s.p2.z - s.p1.z) * l);
}
long double Solve(const Segment& a, const Segment& b) {
    Point left = a.p1, right = b.p1;
    for (int i = 0; i < 100; ++i) {
        right = FindNearestPointOnSegment(left, b);
        left = FindNearestPointOnSegment(right, a);
    }
    return Dist(left, right);
}

int main() {
    Segment s1, s2;
    std::cin >> s1 >> s2;
    std::cout << std::fixed << std::setprecision(10) << Solve(s1, s2) << std::endl;
}
