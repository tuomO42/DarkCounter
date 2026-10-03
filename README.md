# DarkCounter
DarkSouls death counter for linux

Currently supports:
- Dark Souls Remastered
- Dark Souls 2

# Building

Only uses linux provided api's so only dependecies are the basic build tools and cmake.

```
cmake -B build -S .
cmake --build build
```

# Running

App requires sudo access to read the process memory so you need to use sudo.

`sudo ./build/DarkCounter 0`

The app only reads memory so unlikely that this is ban worthy but i am not 100% sure on this.

The app creates an `values` folder into current working directory and outputs the counter values into text files in that folder. The values are updated every half second.
