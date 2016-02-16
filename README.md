# ETA-GESTEMAS

## Introduction

This project is yet another attempt to bring global multitouch gesture reconizers
to GNU/Linux desktop environments. In those desktop environments, there are few
applications that has native touch event support, but a large number of
applications written for keyboard and mouse interactions only. For touch enabled
devices, there is currently no support for multitouch gestures, such as pinch and
rotate, on those legacy applications.

One approach to control legacy applications with multitouch gestures is that we
first recognize those gestures and send keyboard-mouse event combinations to
those legacy applications. There are a number of programs doing this quite well
on laptops with high-quality touchscreens or touchpads, but they did not do very
well on low-cost, non-hid optical touch interactive white boards. User experience
on these interactive white boards with existing multitouch global gesture
recognizer programs, such as [Touchegg] and [ginn], was not quite satisfactory.
It is probably due to the high amount of noise factor. We observed that even a
small pinch with these programs ended up with a huge zoom on the target client
window.

One of our requirements is to have a long press gesture triggering a right click
event on the target window. Existing programs do not meet this requirement
either. They are based on [libgeis], which does not support long press gesture
recognition, and neither does its backend [libgrail].

Another drawback with existing programs is that it is not possible to have a kind
of private window which is not affected by global gestures, so that only the
local gestures for that specific window are in effect. Applications with native
touch support are good candidates to be private windows. We do not want their
native gestures to get blocked by our global gestures.

eta-gestemas is designed all of these requirements in mind. Having spent some
time on existing projects' source code, cheifly [Touchegg], [ginn] and
[Gestouch], eta-gestemas adopted the way [Gestouch] implements the gesture
recognizers.

## Features

### Sampling Raw Touch Events

To address above-mentioned noise problem with low-cost, non-hid optical touch
interactive white boards, eta-gestemas samples the touch events before gesture
recognition process. This prevents client windows from receiving too many
keyboard-mouse fake events, thus resulting a saner user experience.

### Target Types

We can define a global target, public targets and private targets. A public
target (i.e. a window with gesture recognizers attached) is affected by both its
local gesture recognizers and the ones defined in global targets. On the other
hand, gesture recognizers of global target have no effect on private targets.
Private targets may or may not define their local recognizers while remaining
unaffected by the recognizers in global target.

### Implemented Gesture Recognizers

Currently following gesture recognizers are implemented

- LongPressGestureRecognizer
- PanGestureRecognizer
- TwoTouchPinchGestureRecognizer
- SwipegestureRecognizer
- TapGestureRecognizer

### Implemented Gesture Listeners

Currently following gesture listeners are implemented

- XTestMove
- XTestScroll
- XTestZoom
- XTestSimple
- DBusVirtualKeyboard

XTestSimple is a way to define a single press/release keyboard-mouse combination
in configuration file. Please see [the default configuration file][config-file]
for a reference usage.

DBusVirtualKeyboard toggles the visibility of a D-Bus enabled [keyboard][etak].
Besides sending fake keyboard-mouse events, gesture listeners can also control
applications through D-Bus.

### Configuration

Each user can adjust the default configuration for his/her taste. Configuration
file is installed at `/etc/eta/eta-gestemas/recognizers.xml`. When eta-gestemas
first runs, user configuration file is copied to
`~/.config/eta/eta-gestemas/recognizers.xml` Please see
[the default configuration file][config-file].

## Building

Run the following to build eta-gestemas on a debian based distro.

```bash
sudo apt-get install cmake libx11-dev libxext-dev libxi-dev libxtst-dev \
qt5-default libframe-dev
cd path/to/this/project
mkdir build && cd build
cmake .. -DBUILD_MAN=ON -DBUILD_DOC=ON
make
make test # To run automated tests
# if you don't want to install
cd build
./eta_gestemas recognizers.xml
# to install it
sudo make install
eta-gestemas & # Run it or restart for auto run
```

## Workflow

Create a topic branch off the master. Before merging it into master you should
rebase (preferably interactive rebase) it onto master. After rebasing, update
`ChangeLog` to reflect your commit messages. You can use `git2cl` tool.

Here is how you can do that.

```bash
git checkout -b topic/your-branch master
git commit -am "Add your changes"
git commit -am "Add your another change"

# Get remote changes on master
git checkout master
git pull --rebase
git checkout topic/your-branch # back to your branch
git rebase -i master # rebase onto master

# Update ChangeLog
sudo apt-get install git2cl # in case you don't have it.
git log topic/your-branch --pretty --numstat --summary --no-merges | git2cl > ChangeLog
git commit add ChangeLog
git commit --amend # Amend ChangeLog changes, but don't change last commit message

# Now we can merge topic/your-branch into master
git checkout master
git merge topic/your-branch # maybe with --no-ff
```


## Packaging

`debian/sid` branch is dedicated to debian packaging. `git-buildpackage` is used
for debian packaging. For a release build, tag the head of master branch with the
pattern vx.y.z and merge the tag into debian/sid branch. Edit debian/changelog
and build debian package.

Here is how you could create a debian package.

```bash
git checkout master
git pull
git checkout -b debian/sid
git merge vx.y.z # merge new tag
sudo apt-get install git-buildpackage # if you don't have it
gbp dch --release --auto # edit debian/changelog
git commit -am "New release x.y.z-d"
gbp buildpackage -us -uc --git-tag
```

## Testing

If your distro has built-in gestures, you should first disable them. Run the
following to use eta-gestemas with your touchpad, if you are using synaptics.

```bash
  synclient TapButton2=0
  synclient TapButton3=0
  synclient ClickFinger2=0
  synclient ClickFinger3=0
  synclient HorizTwoFingerScroll=0
  synclient VertTwoFingerScroll=0
```

## Notes

- Works with KDE and LXDE.
- Failed to work with GNOME, Unity, XFCE for now.

[Touchegg]: https://github.com/JoseExposito/touchegg
[ginn]: https://wiki.ubuntu.com/Multitouch/Ginn
[Gestouch]: https://github.com/fljot/Gestouch
[config-file]: ./recognizers.xml
[etak]: https://github.com/Pardus-Kurumsal/etak
[libgeis]: https://launchpad.net/geis
[libgrail]: https://launchpad.net/grail
