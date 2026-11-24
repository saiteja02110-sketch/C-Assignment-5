# TASK A5 – DESIGN RECOVERY AND RETHROWING

## A5.1 – Where to Catch, Log and Rethrow

```
User input / UI layer
        │
        ▼
Billing service  ── catches only if it can add useful context
        │
        ├── log audit details
        └── throw;   (rethrow the same exception)
        │
        ▼
Application boundary
        │
        ├── catch ClaimError / BillingError
        ├── display a clear message
        └── continue with next independent bill when safe
```

Catch an exception at the layer that can take a meaningful action.  
Low-level calculation should normally let the exception propagate.  
Service layer may log and rethrow.  
Application boundary reports a safe message and decides whether to continue.

## A5.2 – Stack Unwinding and RAII

When an exception leaves a scope, C++ destroys fully-constructed local objects in reverse order of construction.  
RAII classes (`std::vector`, `std::string`, `std::unique_ptr`, file streams, …) release their resources inside their destructors.  
This guarantees cleanup even when an exception is thrown, eliminating the need for manual `delete` in every catch block.

## Predict the Output (write BEFORE running)

Program:

```cpp
#include <iostream>
#include <string>

struct Guard {
    std::string n;
    explicit Guard(const std::string& s) : n(s) {}
    ~Guard() { std::cout << '~' << n << ' '; }
};

void f() {
    Guard g("f");
    throw 42;
}

int main() {
    try {
        Guard m("main");
        f();
        std::cout << "not printed ";
    } catch (int x) {
        std::cout << "caught " << x;
    } catch (...) {
        std::cout << "other";
    }
    std::cout << '\n';
}
```

**My predicted output (written before running):**  
```
~f ~main caught 42
```

### Actual output after running
```
~f ~main caught 42
```

### Explanation of order
1. `Guard g("f")` is constructed inside `f`.
2. `f` throws 42 → stack unwinding destroys `g` → prints `~f`.
3. Control returns to `main`; the `try` block is left → `m` is destroyed → prints `~main`.
4. The matching `catch (int)` executes → prints `caught 42`.
5. The statement `"not printed "` is never reached because control transferred to the handler.

## Alternative: Manual delete in catch blocks

Every catch path must release every resource acquired before the error.  
Runtime cost is still O(1) per resource, but code size grows with the number of resources and error paths.  
Adding a new exception path creates another opportunity for leaks or double-deletion.  
RAII centralises cleanup in object lifetime and is far safer.
