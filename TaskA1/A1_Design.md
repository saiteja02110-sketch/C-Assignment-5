# TASK A1 – DESIGN THE POLYMORPHIC BILL HIERARCHY

## A1.1 – Common Interface

Every bill represents an amount owed by a patient.  
Common data: patient name / identifier.

| Member       | Purpose                              | Design                                      |
|--------------|--------------------------------------|---------------------------------------------|
| patient      | Stores patient name or ID            | Protected data member in Bill               |
| total()      | Calculates the final bill amount     | Pure virtual: `double total() const = 0`    |
| type()       | Returns the bill category            | Pure virtual: `std::string type() const = 0`|
| print()      | Displays the bill details            | Ordinary member function using virtual calls|
| ~Bill()      | Destroys objects through base pointers | Virtual destructor                        |

## A1.2 – Class Hierarchy and UML (Text Representation)

```
<<abstract>> Bill
-----------------------------------
# patient : string
+ total() : double = 0          {pure virtual}
+ type()  : string = 0          {pure virtual}
+ print() : void
+ ~Bill()                       {virtual}
          |
   -------+-------------------------------+------------------+----------------
   |                |                     |                  |
OutPatientBill   InPatientBill        DayCareBill       EmergencyBill
- consultation   - days               - package         - baseCharge
- tests          - perDay                               - hourlyRate
                 - surgery                              - hours
+ total() override
+ type()  override
```

- Arrow from each derived class to Bill = public inheritance.
- `total()` and `type()` are pure virtual in Bill and overridden in every concrete class.
- Destructor is virtual in the base class.

## A1.3 – Pseudocode to Process All Bills

```
ALGORITHM CalculateDailyTotal
INPUT:  Array Bills[0 ... n-1] of pointers to Bill
OUTPUT: Daily total

BEGIN
    SET dailyTotal ← 0
    FOR each index i from 0 to n-1 DO
        IF Bills[i] is not NULL THEN
            SET amount ← Bills[i].total()
            DISPLAY Bills[i].type(), amount
            SET dailyTotal ← dailyTotal + amount
        END IF
    END FOR
    DISPLAY dailyTotal
END
```

## Alternative Concept and Complexity Analysis

If bills were represented by one `BillRecord` structure with a type field and a `switch` in `total()`:

- Both designs visit n bills once → **O(n)** time.
- Switch design may store unused fields for every record and must be edited when a new type is added.
- Polymorphic design stores only relevant fields per derived type and allows adding a new derived class without changing the processing loop.

| Measure              | Virtual hierarchy                          | One structure + switch                     |
|----------------------|--------------------------------------------|--------------------------------------------|
| Time for n bills     | O(n), one virtual dispatch per bill        | O(n), one switch per bill                  |
| Per-object storage   | Fields of derived object + vptr            | Common record with all possible fields + tag |
| Adding a new type    | Add a derived class; loop unchanged        | Add tag, fields and new switch branch      |
| Main advantage       | Open for extension; clear separation       | Simple layout and straightforward serialization |
