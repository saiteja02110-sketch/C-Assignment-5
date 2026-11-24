// Predict the Output – Task B2
#include <iostream>
#include <stdexcept>

void validate(double amount) {
    if (amount < 0) throw std::invalid_argument("negative");
    if (amount > 1e6) throw std::out_of_range("too large");
    std::cout << "ok ";
}

void process(double amount) {
    try {
        validate(amount);
    } catch (const std::out_of_range& e) {
        std::cout << "[log " << e.what() << "] ";
        throw;          // rethrow
    }
}

int main() {
    for (double a : {100.0, -5.0, 2e6}) {
        try {
            process(a);
            std::cout << "done ";
        } catch (const std::exception& e) {
            std::cout << "caught " << e.what() << ' ';
        }
    }
    std::cout << '\n';
}

/*
Predicted output (written BEFORE running):
ok done caught negative [log too large] caught too large

Explanation:
- 100.0  → validate prints "ok ", process finishes, main prints "done "
- -5.0   → invalid_argument is not caught inside process, so propagates to main → "caught negative "
- 2e6    → out_of_range is caught inside process, logged, then rethrown → main catches it → "caught too large "
*/
