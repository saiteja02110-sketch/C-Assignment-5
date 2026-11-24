// TASK B3 – Virtual functions vs switch – correctness + timing skeleton
// Compile: g++ -std=c++17 -O2 -Wall -Wextra B3_VirtualVsSwitch.cpp -o B3
// Run several times and record median times for 1e3, 1e6, 1e7 bills.

#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <cstdint>
#include <memory>
#include <cmath>

using namespace std;
using Clock = chrono::high_resolution_clock;

// -------------------- Virtual hierarchy --------------------
struct VBill {
    virtual ~VBill() = default;
    virtual double total() const = 0;
};

struct VOut : VBill {
    double c, t;
    VOut(double c, double t) : c(c), t(t) {}
    double total() const override { return c + t; }
};

struct VIn : VBill {
    int days;
    double rate, surgery;
    VIn(int d, double r, double s) : days(d), rate(r), surgery(s) {}
    double total() const override { return days * rate + surgery; }
};

// -------------------- Switch version --------------------
enum class Type { OP, IP };

struct SBill {
    Type type;
    double a, b, c;   // reinterpreted according to type
};

double switch_total(const SBill& b) {
    switch (b.type) {
        case Type::OP: return b.a + b.b;
        case Type::IP: return b.a * b.b + b.c;
    }
    return 0.0;
}

// -------------------- helpers --------------------
template <typename F>
double time_ms(F&& f, int reps = 5) {
    double best = 1e300;
    for (int r = 0; r < reps; ++r) {
        auto t0 = Clock::now();
        f();
        auto t1 = Clock::now();
        double ms = chrono::duration<double, milli>(t1 - t0).count();
        if (ms < best) best = ms;
    }
    return best;
}

int main() {
    const size_t N = 1'000'000;          // change to 1000 / 10'000'000 as required
    mt19937 rng(42);
    uniform_real_distribution<double> dist(100.0, 5000.0);

    // Prepare virtual data
    vector<unique_ptr<VBill>> vdata;
    vdata.reserve(N);
    for (size_t i = 0; i < N; ++i) {
        if (i % 2 == 0)
            vdata.push_back(make_unique<VOut>(dist(rng), dist(rng)));
        else
            vdata.push_back(make_unique<VIn>(3, dist(rng), dist(rng)*10));
    }

    // Prepare switch data
    vector<SBill> sdata(N);
    for (size_t i = 0; i < N; ++i) {
        if (i % 2 == 0) {
            sdata[i] = {Type::OP, dist(rng), dist(rng), 0};
        } else {
            sdata[i] = {Type::IP, 3.0, dist(rng), dist(rng)*10};
        }
    }

    // Correctness check on first 4
    double vsum = 0, ssum = 0;
    for (size_t i = 0; i < 4; ++i) {
        vsum += vdata[i]->total();
        ssum += switch_total(sdata[i]);
    }
    cout << "Correctness (first 4): virtual=" << vsum
         << "  switch=" << ssum
         << (abs(vsum-ssum)<1e-6 ? "  MATCH\n" : "  MISMATCH\n");

    // Timing
    volatile double sink = 0;   // prevent optimising away

    double t_virtual = time_ms([&]{
        double s = 0;
        for (const auto& b : vdata) s += b->total();
        sink = s;
    });

    double t_switch = time_ms([&]{
        double s = 0;
        for (const auto& b : sdata) s += switch_total(b);
        sink = s;
    });

    cout << "N = " << N << '\n';
    cout << "Virtual  : " << t_virtual << " ms\n";
    cout << "Switch   : " << t_switch  << " ms\n";
    cout << "(Record these numbers for 1e3 / 1e6 / 1e7 in your report)\n";
}
