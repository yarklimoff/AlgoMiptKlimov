#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#define EPS 0.000000001
enum Type{Left = 0, Right = 1};

class Point {
public:
    long long x, y;
    Point();
    Point(long long _x, long long _y);
    friend std::istream& operator>>(std::istream& is, Point& p) {
        int x, y;
        is >> x >> y;
        p = Point(x, y);
        return is;
    }
};

Point::Point() : x(0LL), y(0LL) {}

Point::Point(long long _x, long long _y) : x(_x), y(_y) {}

bool operator<(Point a, Point b) {
    return (a.x < b.x) || ((a.x == b.x) && (a.y < b.y));
}

bool operator>(Point a, Point b) {
    return b < a;
}

bool operator==(Point a, Point b) {
    return ((a.x - b.x) < EPS && (a.y - b.y) < EPS);
}

bool operator!=(Point a, Point b) {
    return !(a == b);
}

class Segment{
public:
    Point p1, p2;
    Segment();
    Segment(Point _p1, Point _p2);
    friend std::istream& operator>>(std::istream& is, Segment& s) {
        Point a, b;
        is >> a >> b;
        s = Segment(a, b);
        return is;
    }
};

struct EndPoint {
    Point p;
    Point q;
    long long side;
    long long id;
    EndPoint() : p(Point(0,0)), q(Point(0,0)), side(0), id(-1){}
    EndPoint(Point _p, Point _q, bool t, long long _id) : p(_p), q(_q), side(t), id(_id){}
    long double getY(long long x) {
        if (p.x == q.x) {
            return p.y;
        }
        return p.y + (q.y - p.y) * (x - p.x) / ((long double)q.x - p.x);
    }
};

bool Comp(const EndPoint& e, const EndPoint& w) {
    return ((e.p.x < w.p.x) || ((e.p.x == w.p.x) && (e.side < w.side)) || (((e.p.x == w.p.x) && (e.side == w.side) && (e.p.y < w.p.y))));
}

Segment::Segment(): p1(0, 0), p2(0, 0){}

Segment::Segment(Point _p1, Point _p2): p1(_p1), p2(_p2){}

bool operator==(const Segment& a, const Segment& b) {
    return a.p1 == b.p1 && a.p2 == b.p2;
}

bool operator!=(const Segment& a, const Segment& b) {
    return !(a==b);
}

long long Det(long long a, long long b, long long c, long long d) {
    return a * d - b * c;
}

bool Between(long long a, long long b, long long c) {
    return std::min(a, b) <= c + EPS && c <= std::max(a, b) + EPS;
}

bool IntersectParallels(long long a, long long b, long long c, long long d) {
    if (a > b)  {
        std::swap(a, b);
    }
    if (c > d) {
        std::swap(c, d);
    }
    return std::max(a, c) <= std::min(b, d);
}


bool Intersect(const Segment& s1, const Segment& s2) {
    long long A1 = s1.p1.y - s1.p2.y;
    long long B1 = s1.p2.x - s1.p1.x;
    long long C1 = -A1 * s1.p1.x - B1 * s1.p1.y;
    long long A2 = s2.p1.y - s2.p2.y;
    long long B2 = s2.p2.x - s2.p1.x;
    long long C2 = -A2 * s2.p1.x - B2 * s2.p1.y;
    long long d = Det(A1, B1, A2, B2);
    if (std::abs(d) >= EPS) {
        long double x = - Det(C1, B1, C2, B2) * 1.0 / d;
        long double y = - Det(A1, C1, A2, C2) * 1.0 / d;
        return Between(s1.p1.x, s1.p2.x, x) && Between(s1.p1.y, s1.p2.y, y)
               && Between(s2.p1.x, s2.p2.x, x) && Between(s2.p1.y, s2.p2.y, y);
    } else {
        return (Det(A1, C1, A2, C2) == 0 && Det(B1, C1, B2, C2) == 0) &&
               (IntersectParallels(s1.p1.x, s1.p2.x, s2.p1.x, s2.p2.x) &&
                IntersectParallels(s1.p1.y, s1.p2.y, s2.p1.y, s2.p2.y));
    }
}

struct Comparator {
    bool operator () (EndPoint e1, EndPoint e2) {
        long long x = std::max(std::min(e1.p.x, e1.q.x), std::min(e2.p.x, e2.q.x));
        long double diff = e1.getY(x) - e2.getY(x);
        if (std::abs(diff) < EPS) {
            return e1.id < e2.id;
        }
        return diff < 0;
    }
};

void Solve(std::vector <Segment>& s) {
    long long n = s.size();
    std::vector<EndPoint> list_end_points(2 * n);
    for (long long i = 0; i < n; ++i) {
        if (s[i].p1 > s[i].p2)
            std::swap(s[i].p1, s[i].p2);
        list_end_points[2 * i] = EndPoint(s[i].p1, s[i].p2, Left, i);
        list_end_points[2 * i + 1] = EndPoint(s[i].p2, s[i].p1, Right, i);
    }
    sort(list_end_points.begin(), list_end_points.end(), Comp);
    std::set<EndPoint, Comparator> Tree;
    for (long long i = 0; i < 2 * n; ++i) {
        if (list_end_points[i].side == Left) {
            auto ins = Tree.insert(list_end_points[i]);
            if (ins.second) {
                if (++ins.first != Tree.end()) {
                    auto successor = ins.first;
                    if (Intersect(Segment(list_end_points[i].p, list_end_points[i].q), Segment((*successor).p, (*successor).q))) {
                        std::cout << "YES\n" << std::min(list_end_points[i].id, (*successor).id) + 1 << " "
                             << std::max(list_end_points[i].id, (*successor).id) + 1;
                        return;
                    }
                }
                if (--ins.first != Tree.begin()) {
                    auto predecessor = ins.first;
                    --predecessor;
                    if (Intersect(Segment(list_end_points[i].p, list_end_points[i].q), Segment((*predecessor).p, (*predecessor).q))) {
                        std::cout << "YES\n" << std::min(list_end_points[i].id, (*predecessor).id) + 1 << " "
                             << std::max(list_end_points[i].id, (*predecessor).id) + 1;
                        return;
                    }
                }
            }
        } else {
            EndPoint exist = list_end_points[i];
            std::swap(exist.q, exist.p);
            exist.side = 0;
            auto place = Tree.insert(exist);
            if (place.first != Tree.begin() && ++place.first != Tree.end()) {
                --place.first;
                auto successor = place.first;
                ++successor;
                auto predecessor = place.first;
                --predecessor;
                if (Intersect(Segment((*successor).p, (*successor).q), Segment((*predecessor).p, (*predecessor).q))) {
                    std::cout << "YES\n" << std::min((*successor).id, (*predecessor).id) + 1 << " "
                         << std::max((*successor).id, (*predecessor).id) + 1;
                    return;
                }
            }
            Tree.erase(exist);
        }
    }
    std::cout << "NO\n";
}

int main() {
    long long N;
    std::cin >> N;
    std::vector <Segment> s(N);
    for (long long i = 0; i < N; ++i) {
        std::cin >> s[i];
    }
    Solve(s);
    return 0;
}
