# TASK B4 – Final Engineering Decision

## B4.1 Practical Extension
- Added `RehabilitationBill` (new bill type) by deriving from `Bill`.
- Added `CorporatePayer` (new payer) by deriving from `Payer`.
- No existing class was modified.
- All earlier tests continue to pass.

## B4.2 Resource Safety
Compile and run with AddressSanitizer:

```bash
g++ -std=c++17 -Wall -Wextra -g -O1 \
    -fsanitize=address,undefined -fno-omit-frame-pointer \
    B4_ExtensionAndSafety.cpp -o B4
./B4
```

Expected clean report (no ASan / UBSan diagnostics) together with destructor messages proving RAII cleanup on the exception path.

## B4.3 Production Decision
The polymorphic design with exceptions is suitable for a hospital group that:
- Adds new bill types and payers regularly,
- Processes ~200 000 bills per day,
- Needs clear, maintainable extension points.

Virtual dispatch cost is O(1) per call; overall processing remains O(n).  
Exceptions are appropriate for exceptional conditions (invalid configuration, missing records, allocation failure).  
For high-frequency claim rejections an explicit result type (`optional` / status code) can be preferred for predictable performance.

Production checklist:
- Input validation at boundaries
- Audit logging of rejected claims
- Transaction / batch boundaries
- Automated tests + sanitizer runs
- Performance measurements on representative workloads

## Alternative: Configuration-driven plug-ins
Rule files can store rates and simple formulas.  
Interpretation cost is higher than a virtual call, but business rates can be changed without recompilation.  
Requires rigorous validation, versioning and security of the configuration.
