# DSA01-192525164-Assignment5

**Student Name:** M SAI TEJA  
**Register Number:** 192525164  
**Assignment:** ASSIGNMENT 5 – CO5  
**Topic:** RELIABLE SOFTWARE WITH RUNTIME POLYMORPHISM AND EXCEPTIONS  

**Compiler:** g++ (GCC) 13.x (or later)  
**Compilation flags used:**  
```bash
g++ -std=c++17 -Wall -Wextra -O2 -g
```
For sanitizer runs:
```bash
g++ -std=c++17 -Wall -Wextra -g -O1 -fsanitize=address,undefined -fno-omit-frame-pointer
```

## How to Compile and Run

Each Task folder contains the relevant source files. Navigate into the folder and compile as shown below.

### Example (TaskB1)
```bash
cd TaskB1
g++ -std=c++17 -Wall -Wextra -O2 B1_polymorphic_billing.cpp -o B1
./B1
```

### Example with AddressSanitizer (TaskB2 / TaskB4)
```bash
g++ -std=c++17 -Wall -Wextra -g -O1 -fsanitize=address,undefined -fno-omit-frame-pointer B2_claim_exceptions.cpp -o B2
./B2
```

## Folder Structure

- **TaskA1** – Design of polymorphic Bill hierarchy (interface, UML text, pseudocode)
- **TaskA2** – Runtime binding, vtables, virtual destructor + Predict the Output program
- **TaskA3** – Abstract Payer hierarchy + extension
- **TaskA4** – Exception hierarchy design
- **TaskA5** – Catch / rethrow / stack unwinding + Predict the Output program
- **TaskB1** – Complete the Code (TODOs) + full polymorphic billing implementation
- **TaskB2** – Error conditions, ClaimError, rethrow, Complete the Code
- **TaskB3** – Benchmarks: virtual vs switch, exception cost
- **TaskB4** – Extension (new bill + new payer) + sanitizer-safe demo + production decision

## Notes

- All “Complete the Code” programs have been completed and verified against the expected output.
- Predicted outputs are written **before** running (as required).
- Screenshots of actual runs should be added by the student before final submission.
- Progressive commits: one commit per major task is recommended.

## GitHub Repository

Repository name: `DSA01-192525164-Assignment5`  
(Add the link here after creating the repo)
