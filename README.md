# ETA-GESTEMAS

## Build

```bash
sudo apt-get install cmake libx11-dev libxext-dev libxi-dev 
cd path/to/this/project
mkdir build && cd build
cmake .. -DBUILD_MAN=ON -DBUILD_DOC=ON
make
make test # To run automated tests
```

