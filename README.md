# serialize

Pack C++ structs into a compact byte-array without macros or boilerplate code.

* Header-only library
* No external dependencies
* Requires `C++17`
* MIT License

## Simple arithmetic and string types

```cpp
#include <serialize/serialize.h>

struct MyStruct {
  char a;
  int b;
  float c;
  std::string d;
  uint64_t e;
  bool f;
};

int main() {

  MyStruct s{'a', 5, 3.14f, "Hello", 12345, true};

  // Serialize
  auto bytes = serialize(s); // 15 bytes

  // Deserialize
  auto recovered = deserialize<MyStruct>(bytes);
}

// bytes
// {
//   0x61,                           // char 'a'
//   0x05,                           // int 5
//   0xc3, 0xf5, 0x48, 0x40,         // float 3.14f
//   0x05,                           // start of 5-byte string
//   0x48, 0x65, 0x6c, 0x6c, 0x6f    // string "Hello"
//   0xb9, 0x60,                     // 2-byte uint 12345
//   0x01                            // bool true
// }
```

## Sequence containers: arrays and vectors

```cpp
#include <cassert>
#include <serialize/serialize.h>

struct MyStruct {
  std::array<int, 3> a;
  std::vector<std::vector<float>> b;
};

int main() {

  MyStruct s{{1, 2, 3},
	     {{3.14, 1.61}, {2.71, -1}}};

  // Serialize
  auto bytes = serialize(s); // 24 bytes

  // Deserialize
  auto recovered = deserialize<MyStruct>(bytes);
}

// bytes:
// {
//   0x01, 0x02, 0x03,           // array {1, 2, 3}
//   0x02,                       // vector size = 2
//   0x02,                       // start of inner vector (size = 2)
//   0xc3, 0xf5, 0x48, 0x40,     // vector[0][1] = 3.14
//   0x7b, 0x14, 0xce, 0x3f,     // vector[0][1] = 1.61
//   0x02,                       // sub-vector size = 2
//   0xa4, 0x70, 0x2d, 0x40,     // vector[1][0] = 2.71
//   0x00, 0x00, 0x80, 0xbf      // vector[1][0] = -1
// }
```

### Supported Types

```cpp
uint8_t
uint16_t 
uint32_t 
uint64_t
int8_t 
int16_t 
int32_t 
int64_t
enum class
float
double 
bool
char
std::string
std::optional<T>
std_array<T, N>
std::vector<T>
std::tuple<T, U...>
std::pair<T, U>
std::map<T, U>
std::unordered_map<T, U>
struct
```