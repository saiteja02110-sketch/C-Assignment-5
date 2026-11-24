// Full polymorphic billing system with multiple bill types and payers
// Demonstrates runtime polymorphism + RAII (unique_ptr)

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>

using namespace std;

// -------------------- Bill hierarchy --------------------
class Bill {
protected:
    string patient;
public:
    explicit Bill(string p) : patient(std::move(p)) {}
    virtual ~Bill() = default;
    virtual double total() const = 0;
    virtual string type() const = 0;
    void print() const {
        cout << type() << " " << patient << ": " << total() << '\n';
    }
    const string& patientName() const { return patient; }
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
};

class InPatient : public Bill {
    int days;
    double perDay, surgery;
public:
    InPatient(string p, int d, double rate, double s)
        : Bill(std::move(p)), days(d), perDay(rate), surgery(s) {
        if (d < 0 || rate < 0 || s < 0) throw invalid_argument("negative charge");
    }
    double total() const override { return days * perDay + surgery; }
    string type() const override { return "IP"; }
};

class DayCareBill : public Bill {
    double package;
public:
    DayCareBill(string p, double amount)
        : Bill(std::move(p)), package(amount) {
        if (amount < 0) throw invalid_argument("negative charge");
    }
    double total() const override { return package; }
    string type() const override { return "DAYCARE"; }
};

class EmergencyBill : public Bill {
    double base, hourly;
    int hours;
public:
    EmergencyBill(string p, double b, double h, int hrs)
        : Bill(std::move(p)), base(b), hourly(h), hours(hrs) {
        if (b < 0 || h < 0 || hrs < 0) throw invalid_argument("negative charge");
    }
    double total() const override { return base + hourly * hours; }
    string type() const override { return "EMERGENCY"; }
};

// -------------------- Payer hierarchy --------------------
class Payer {
public:
    virtual ~Payer() = default;
    virtual double amountCovered(const Bill& bill) const = 0;
    virtual string name() const = 0;
};

class SelfPay : public Payer {
public:
    double amountCovered(const Bill& bill) const override { return bill.total(); }
    string name() const override { return "SelfPay"; }
};

class Insurance : public Payer {
    double coverLimit;
    double coPayPercent;
public:
    Insurance(double limit, double percent)
        : coverLimit(limit), coPayPercent(percent) {}
    double amountCovered(const Bill& bill) const override {
        double eligible = min(bill.total(), coverLimit);
        return eligible * (1.0 - coPayPercent / 100.0);
    }
    string name() const override { return "Insurance"; }
};

class GovernmentScheme : public Payer {
    double packageRate;
public:
    explicit GovernmentScheme(double rate) : packageRate(rate) {}
    double amountCovered(const Bill&) const override { return packageRate; }
    string name() const override { return "GovtScheme"; }
};

// -------------------- main --------------------
int main() {
    vector<unique_ptr<Bill>> bills;
    bills.push_back(make_unique<OutPatient>("Ravi", 500, 1200));
    bills.push_back(make_unique<InPatient>("Meena", 3, 2500, 40000));
    bills.push_back(make_unique<DayCareBill>("Arun", 18000));
    bills.push_back(make_unique<EmergencyBill>("Sara", 3000, 750, 4));

    double sum = 0;
    for (const auto& b : bills) {
        b->print();
        sum += b->total();
    }
    cout << "Daily total: " << sum << '\n';

    // Demonstrate payers
    SelfPay self;
    Insurance ins(50000, 20);
    GovernmentScheme gov(15000);

    cout << "\n--- Payer demonstration ---\n";
    for (const auto& b : bills) {
        cout << b->type() << " " << b->patientName()
             << "  SelfPay covers: " << self.amountCovered(*b)
             << "  Insurance covers: " << ins.amountCovered(*b)
             << "  Govt covers: " << gov.amountCovered(*b) << '\n';
    }
}
