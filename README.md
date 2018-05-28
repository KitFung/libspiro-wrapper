libspiro wrapper
-----

Wrap the libspiro in c++, let it simpler to use.

libspiro: https://github.com/fontforge/libspiro

```c++
using spiro::SpiroCPType;

std::vector<spiro::SpiroCP> spiro0 = {
    {334, 117, SpiroCPType::CORNER}, {305, 176, SpiroCPType::CORNER},
    {212, 142, SpiroCPType::G2},     {159, 171, SpiroCPType::G2},
    {224, 237, SpiroCPType::G2},     {347, 335, SpiroCPType::G2},
    {202, 467, SpiroCPType::G2},     {81, 429, SpiroCPType::CORNER},
    {114, 368, SpiroCPType::CORNER}, {201, 402, SpiroCPType::G2},
    {276, 369, SpiroCPType::G2},     {218, 308, SpiroCPType::G2},
    {91, 211, SpiroCPType::G2},      {124, 111, SpiroCPType::G2},
    {229, 82, SpiroCPType::G2},      {0, 0, SpiroCPType::END}};
bool isclosed = true;
spiro::SpiroWrapper spiro(spiro0, isclosed);
bool success = spiro.Compute();
std::cout << "Success: " << success << std::endl;
if (success) {
  for (auto result : spiro.Results()) {
    result.Println();
  }
}
```
