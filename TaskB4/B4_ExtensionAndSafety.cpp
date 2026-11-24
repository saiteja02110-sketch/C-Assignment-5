// TASK B4 – Add new bill type + new payer without changing existing classes
// Also demonstrates RAII + exception safety (clean under AddressSanitizer)

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>

using namespace std;

// -------------------- Existing Bill hierarchy (unchanged) --------------------
class Bill {
protected:
    string patient;
public:
    explicit Bill(string p) : patient(std::move(p)) {}
    virtual ~Bill() { cout << "Bill released\n"; }
    virtual double total() const = 0;
    virtual string type() const = 0;
    void print() const {
        cout << type() << " " << patient << ": " << total() << '\n';
    }
};

class OutPatient : public Bill {
    double consultation, tests;
public:
    OutPatient(string p, double c, double t)
        : Bill(std::move(p)), consultation(c), tests(t) {
        if (c < 0 || t < 0) throw invalid_argument("negative charge");
    }
    double total() const override { return consultation + tests; }
    string type() const override { return "OP"; }
    ~OutPatient() override { cout << "OP bill released\n"; }
};

// -------------------- NEW bill type (no change to existing classes) ----------
class RehabilitationBill : public Bill {
    int sessions;
    double ratePerSession;
public:
    RehabilitationBill(const string& p, int count, double rate)
        : Bill(p), sessions(count), ratePerSession(rate) {
        if (count < 0 || rate < 0) throw invalid_argument("negative charge");
    }
    double total() const override { return sessions * ratePerSession; }
    string type() const override { return "REHAB"; }
    ~RehabilitationBill() override { cout << "REHAB bill released\n"; }
};

// -------------------- Existing Payer hierarchy (unchanged) -------------------
class Payer {
public:
    virtual ~Payer() = default;
    virtual double amountCovered(const Bill& bill) const = 0;
};

class SelfPay : public Payer {
public:
    double amountCovered(const Bill& bill) const override { return bill.total(); }
};

// -------------------- NEW payer (no change to existing classes) --------------
class CorporatePayer : public Payer {
    double coverPercent;
public:
    explicit CorporatePayer(double percent) : coverPercent(percent) {
        if (percent < 0 || percent > 100)
            throw invalid_argument("invalid percentage");
    }
    double amountCovered(const Bill& bill) const override {
        return bill.total() * coverPercent / 100.0;
    }
};

// -------------------- Demo --------------------------------------------------
int main() {
    try {
        vector<unique_ptr<Bill>> bills;
        bills.push_back(make_unique<OutPatient>("Ravi", 500, 1200));
        bills.push_back(make_unique<RehabilitationBill>("Kiran", 8, 1500));

        SelfPay self;
        CorporatePayer corp(80);

        for (const auto& b : bills) {
            b->print();
            cout << "  SelfPay covers   : " << self.amountCovered(*b) << '\n';
            cout << "  Corporate covers : " << corp.amountCovered(*b) << '\n';
        }

        // Force an exception to prove RAII cleanup
        bills.push_back(make_unique<OutPatient>("Bad", -5, 100));

    } catch (const invalid_argument& e) {
        cout << "Input error: " << e.what() << '\n';
    } catch (const bad_alloc&) {
        cout << "Memory allocation failed\n";
    } catch (...) {
        cout << "Unexpected billing error\n";
    }

    cout << "Program continues safely\n";
}
