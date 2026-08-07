# Oil Tycoon

A small console-based oil-industry management game written in C++17.

## Current Gameplay

The current prototype contains:

- Company money
- Spare parts
- Exploration
- Commercial discoveries
- Dry holes
- Drilling projects
- Reservoirs
- Wells
- Natural-flow production
- Reservoir pressure decline
- Reservoir reserve depletion
- Six maintenance states
- Well degradation
- Well repair
- Maintenance policies
- Transportation network
- Storage facility
- Temporary fixed oil price
- Oil sales
- Technology tree
- Technology prerequisites
- Technology tiers

## Game Loop

Each day is processed in this order:

1. Exploration
2. Drilling
3. Transportation construction
4. Storage construction
5. Maintenance degradation
6. Repairs
7. Production
8. Transportation
9. Storage
10. Sales
11. Status update

## Build

Linux/macOS:

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror \
    -Iinclude \
    src/*.cpp \
    -o oil_tycoon
