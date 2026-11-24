# TASK A2 – UNDERSTAND RUNTIME BINDING

## A2.1 – Early and Late Binding

- **Early binding** resolves a function call at compile time. A non-virtual function called through a base pointer is selected using the *declared* pointer type.
- **Late binding** resolves a virtual call at run time using the *actual* object type.

Therefore, when `Bill* b` points to an `OutPatientBill` object and `total()` is virtual, the call `b->total()` executes `OutPatientBill::total()`.

## A2.2 – Conceptual Vtable Diagram

```
Bill vtable
- [0] Bill::~Bill
- [1] Bill::total   (pure virtual slot)
- [2] Bill::type    (pure virtual slot)

OutPatientBill vtable
- [0] OutPatientBill::~OutPatientBill
- [1] OutPatientBill::total
- [2] OutPatientBill::type

InPatientBill vtable
- [0] InPatientBill::~InPatientBill
- [1] InPatientBill::total
- [2] InPatientBill::type
```

Call path for `b->total()` when `b` points to an OutPatientBill:

```
Bill* b = new OutPatientBill(...);
b->total()
  → object's virtual-table pointer (vptr)
  → OutPatientBill vtable slot for total()
  → OutPatientBill::total()
```

(C++ does not mandate a particular vtable layout; this is the common implementation.)

## A2.3 – Why the Destructor Must Be Virtual

A base pointer may own a derived object.  
Deleting that object through a base pointer is safe **only** when the base destructor is virtual.  
The derived destructor then runs first, followed by the base destructor.  
Without a virtual destructor the behaviour is undefined and derived cleanup may be skipped.

## Predict the Output (write BEFORE running)

Program:

```cpp
#include <iostream>
struct Bill {
  virtual ~Bill() { std::cout << "~Bill "; }
  virtual double total() const = 0;
  void show() const { std::cout << total() << ' '; }
};
struct OPBill : Bill {
  ~OPBill() override { std::cout << "~OPBill "; }
  double total() const override { return 500; }
};
int main() {
  Bill* b = new OPBill;
  b->show();
  delete b;
  std::cout << '\n';
}
```

**My predicted output (written before running):**  
```
500 ~OPBill ~Bill
```

### Actual output after running
```
500 ~OPBill ~Bill
```

### Explanation
- `show()` is non-virtual, but it calls `total()`. Because `total()` is virtual, the call is dynamically dispatched to `OPBill::total()`.
- Destruction order: derived destructor first (`~OPBill`), then base (`~Bill`).

### After removing `virtual` from `~Bill`
Predicted (and observed under AddressSanitizer):  
Only `~Bill` may appear (or AddressSanitizer reports errors). The behaviour is **undefined**.  
Always keep the destructor virtual when deleting through base pointers.

## Alternative: dynamic_cast

If `total()` were non-virtual, code could test the dynamic type with successive `dynamic_cast`s and call the matching method.  
Worst case: k bill types → up to k checks per bill → O(k) work per bill → O(nk) for n bills.  
Virtual dispatch uses one indirect call → O(n).  
A polymorphic object commonly carries a vptr (implementation-dependent extra storage).
