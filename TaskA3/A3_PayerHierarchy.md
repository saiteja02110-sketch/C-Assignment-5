# TASK A3 – DESIGN WITH ABSTRACT CLASSES

## A3.1 – Payer Interface

```cpp
class Payer {
public:
    virtual ~Payer() = default;
    virtual double amountCovered(const Bill& bill) const = 0;
};

class SelfPay : public Payer {
public:
    double amountCovered(const Bill& bill) const override {
        return bill.total();          // patient pays the full amount
    }
};

class Insurance : public Payer {
    double coverLimit;
    double coPayPercent;              // patient share, e.g. 20
public:
    Insurance(double limit, double percent)
        : coverLimit(limit), coPayPercent(percent) {}
    double amountCovered(const Bill& bill) const override {
        double eligible = std::min(bill.total(), coverLimit);
        return eligible * (1.0 - coPayPercent / 100.0);
    }
};

class GovernmentScheme : public Payer {
    double packageRate;
public:
    explicit GovernmentScheme(double rate) : packageRate(rate) {}
    double amountCovered(const Bill&) const override {
        return packageRate;
    }
};
```

- SelfPay covers the complete bill.
- Insurance limits to the policy cover then applies co-payment.
- GovernmentScheme pays a fixed package rate.

## A3.2 – Extension Without Modifying Existing Classes

```cpp
class CorporatePayer : public Payer {
    double negotiatedPercentage;
public:
    explicit CorporatePayer(double p) : negotiatedPercentage(p) {}
    double amountCovered(const Bill& bill) const override {
        return bill.total() * negotiatedPercentage / 100.0;
    }
};
```

The new class implements the existing `Payer` interface.  
Any code that accepts a `Payer&` or `Payer*` continues to work unchanged.

## Alternative: Function pointers / std::function

A payer policy can be represented as `std::function<double(const Bill&)>` together with its data.  
Calling the function is O(1), same asymptotic cost as a virtual call.  
`std::function` may incur type-erasure / allocation overhead.  
A named abstract class groups data + behaviour more clearly and supports inheritance hierarchies.
