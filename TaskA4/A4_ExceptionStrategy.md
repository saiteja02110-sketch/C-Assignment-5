# TASK A4 – DESIGN THE EXCEPTION STRATEGY

## A4.1 – Error Conditions and Hierarchy

| Error condition                        | Exception / handling                  | Reason                              |
|----------------------------------------|---------------------------------------|-------------------------------------|
| Negative consultation / room charge    | std::invalid_argument                 | Precondition violation              |
| Claim exceeds policy cover             | ClaimError (from runtime_error)       | Domain-specific billing rejection   |
| Unknown government package             | UnknownPackageError                   | Configured package not found        |
| Missing patient record                 | PatientNotFoundError                  | Required data absent                |
| Invalid text where number expected     | std::invalid_argument (from stod)     | Conversion failure                  |
| Impossible allocation request          | std::bad_alloc                        | Allocation cannot be satisfied      |

```
std::exception
│
├── std::logic_error
│      └── std::invalid_argument
│
└── std::runtime_error
       └── BillingError
              ├── ClaimError
              ├── UnknownPackageError
              └── PatientNotFoundError
```

```cpp
class BillingError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class ClaimError : public BillingError {
public:
    using BillingError::BillingError;
};
```

## A4.2 – Throwing and noexcept

| Function                     | Specification     | Reason                                      |
|------------------------------|-------------------|---------------------------------------------|
| Bill constructor / validation| May throw         | Reject invalid data                         |
| total()                      | Prefer noexcept   | Calculation should be predictable           |
| parseAmount()                | May throw         | std::stod can throw                         |
| ~Bill()                      | noexcept          | Safe cleanup during stack unwinding         |
| close / release cleanup      | noexcept          | Must not interrupt exception propagation    |

Destructors should never throw: a second exception escaping while another is active causes `std::terminate`.

## Alternative: Return codes

A function can return an enum `Status` (Success / InvalidAmount / ClaimTooLarge) and write the result through an out-parameter.  
Normal path avoids exception machinery.  
Every caller must inspect the status and forward error information.  
Return codes suit frequent expected validation failures; exceptions suit unusual failures that should travel to a higher recovery layer.
