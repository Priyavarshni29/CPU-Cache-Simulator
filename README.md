# CPU Cache Simulator with Write-Back Policy

A CPU cache simulator written in C that demonstrates the **Least Recently Used (LRU)** cache replacement algorithm using a **Doubly Linked List** and **Hash Table**. The simulator implements a **Write-Back Cache Policy** with **Dirty Bit Management** to emulate realistic cache behavior found in modern computer systems.

---

## Features

- LRU (Least Recently Used) cache replacement algorithm
- O(1) average-case cache lookup using a Hash Table
- O(1) LRU updates using a Doubly Linked List
- Write-Back cache policy
- Dirty Bit tracking
- Simulated RAM (Main Memory)
- Configurable cache capacity
- Cache hit, miss, and eviction statistics
- Dynamic memory allocation in C

---

## Technologies Used

- C Programming
- Doubly Linked List
- Hash Table (Separate Chaining)
- Dynamic Memory Allocation
- Pointer Manipulation

---

## Data Structures

### Doubly Linked List

Maintains cache entries in order of usage.

- Head → Most Recently Used (MRU)
- Tail → Least Recently Used (LRU)

### Hash Table

Provides O(1) average lookup of cache blocks using memory addresses as keys.

---

## Cache Workflow

Read Request

- Cache Hit
  - Return data
  - Move block to the front

- Cache Miss
  - Load block from RAM
  - Insert at front
  - Evict LRU block if cache is full

Write Request

- Write Hit
  - Update cache value
  - Mark Dirty Bit = 1
  - Move block to front

- Write Miss
  - Load block into cache
  - Update value
  - Mark Dirty Bit = 1

Eviction

- Dirty = 0 → Discard block
- Dirty = 1 → Write block back to RAM before eviction

---

## Time Complexity

| Operation | Complexity |
|----------|------------|
| Read | O(1) Average |
| Write | O(1) Average |
| Lookup | O(1) Average |
| Insert | O(1) |
| Eviction | O(1) |

---

## Sample Input

```
Cache Capacity : 3

Operations

R 10
R 20
R 30
W 10 500
R 30
R 50
R 60
R 70
```

---

## Sample Output

```
Hits      : 2
Misses    : 6
Evictions : 3

Hit Ratio  : 25%
Miss Ratio : 75%
```

---

## Future Improvements

- Direct Mapped Cache
- Set Associative Cache
- Multi-Level Cache (L1/L2/L3)
- Write-Through Cache Policy
- Cache Line Simulation
- Performance Benchmarking

---

## Author

**Priyavarshni**
