// TASK B3.3 – Cost of exceptions vs return codes
// Compile: g++ -std=c++17 -O2 -Wall -Wextra B3_ExceptionCost.cpp -o B3ex
// Measure for 1 000 000 claims with 0 %, 1 % and 50 % invalid.

#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <optional>
#include <stdexcept>

using namespace std;
using Clock = chrono::high_resolution_clock;

// ----- Exception version -----
double approve_ex(double billed, double cover) {
    if (billed < 0 || cover < 0) throw invalid_argument("negative");
    if (billed > cover) throw runtime_error("exceeds");
    return billed;
}

// ----- Optional / return-code version -----
optional<double> approve_opt(double billed, double cover) {
    if (billed < 0 || cover < 0) return nullopt;
    if (billed > cover) return nullopt;
    return billed;
}

template <typename F>
double time_ms(F&& f, int reps = 3) {
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
    const size_t N = 1'000'000;
    vector<double> billed(N), cover(N);

    // Generate data for different invalid rates
    auto run_for_rate = [&](double invalid_rate) {
        mt19937 rng(42);
        uniform_real_distribution<double> dist(1000.0, 100000.0);
        bernoulli_distribution inv(invalid_rate);

        for (size_t i = 0; i < N; ++i) {
            billed[i] = dist(rng);
            cover[i]  = dist(rng);
            if (inv(rng)) billed[i] = cover[i] + 1000;   // force exceed
        }

        // Exception path
        double t_ex = time_ms([&]{
            size_t ok = 0;
            for (size_t i = 0; i < N; ++i) {
                try {
                    approve_ex(billed[i], cover[i]);
                    ++ok;
                } catch (...) {}
            }
            (void)ok;
        });

        // Optional path
        double t_opt = time_ms([&]{
            size_t ok = 0;
            for (size_t i = 0; i < N; ++i) {
                if (approve_opt(billed[i], cover[i])) ++ok;
            }
            (void)ok;
        });

        cout << "Invalid rate " << (invalid_rate*100) << "%\n";
        cout << "  Exceptions : " << t_ex  << " ms\n";
        cout << "  Optional   : " << t_opt << " ms\n\n";
    };

    run_for_rate(0.00);
    run_for_rate(0.01);
    run_for_rate(0.50);
}
