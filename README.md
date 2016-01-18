# ETA-GESTEMAS

## Build

Run the following for building eta-gestemas

```bash
sudo apt-get install cmake libx11-dev libxext-dev libxi-dev libxtst-dev \
qt5-default libframe-dev
cd path/to/this/project
mkdir build && cd build
cmake .. -DBUILD_MAN=ON -DBUILD_DOC=ON
make
make test # To run automated tests
```

## Test

Run the following for touchpad tests

```bash
  synclient TapButton2=0
  synclient TapButton3=0
  synclient ClickFinger2=0
  synclient ClickFinger3=0
  synclient HorizTwoFingerScroll=0
  synclient VertTwoFingerScroll=0
```
