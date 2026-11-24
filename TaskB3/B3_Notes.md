# TASK B3 – Notes for Measurements

## B3.1 Correctness
Both virtual and switch implementations produce the same totals for the same data (verified on small sample).

## B3.2 Timing (fill after running on your machine)

| N bills     | Virtual (ms) | Switch (ms) |
|-------------|--------------|-------------|
| 1 000       |              |             |
| 1 000 000   |              |             |
| 10 000 000  |              |             |

Command:
```bash
g++ -std=c++17 -O2 -Wall -Wextra B3_VirtualVsSwitch.cpp -o B3
./B3
```

## B3.3 Exception cost (fill after running)

| Invalid % | Exceptions (ms) | Optional (ms) |
|-----------|-----------------|---------------|
| 0 %       |                 |               |
| 1 %       |                 |               |
| 50 %      |                 |               |

Command:
```bash
g++ -std=c++17 -O2 -Wall -Wextra B3_ExceptionCost.cpp -o B3ex
./B3ex
```

## B3.4 Engineering Trade-off
- Use **virtual functions** when the set of types grows and you need open extension.
- Use a **switch** when the set of types is small and stable and cache locality matters.
- Use **exceptions** for rare failures; use **optional / return codes** for frequent expected validation failures.
