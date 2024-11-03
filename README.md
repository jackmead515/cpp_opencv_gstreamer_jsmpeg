# Example OpenCV Pipeline with GStreamer formatting for JSMPEG compatibility

This example demonstrates how one would setup a C++ OpenCV pipeline utilizing gstreamer.
In addition, the output pipeline is formatted to be compatible with MPEG1 encoding
for use with the [jsmpeg](https://github.com/phoboslab/jsmpeg) library. If you run a
websocket server, you will be able to visualize this stream in a web browser.

## Why?

Several major benefits of using this setup:

- C++ OpenCV is fast and efficient. You can really get creative with your image processing here.

- GStreamer is powerful and flexible. You can format and encode your video stream in a multitude of ways.

- MPEG1 is a simple and freely available codec. GStreamer has a really nice plugin for this. And, JSMPEG can
allow you to view this stream in the web browser for REAL real-time video streaming.

### Example Use Cases

There are many fun and interesting applications that apply for this setup. As you can imagine, avoiding Python we
have a competitive speed advantage so we can do more computationally intensive tasks. This pipeline is made to run
in real-time (on a CPU or even GPU if you add the right plugins). In addition, there is no need for a seperate video
streaming server, as you would need for RTSP or WebRTC. Here are some ideas:

- Compute stereo depth in real-time for robotics or autonomous vehicles.

- Detect and track objects in real-time for security or surveillance.

- Facial detection for those dorky Snapchat filters.

- A combination of all the above!!

The possibilities are endless. Get creative with the GStreamer pipeline to do some heavy lifting for you. Get creative
with the deep learning models out there to detect all kinds of things. And get creative with how you can leverage the browser
to visualize your stream instead of HLS or some expensive media server.

### Documentation

Here are some direct links to documentation for building, running, and using OpenCV, GStreamer, and JSMPEG:

- [GStreamer avenc_mpeg1video plugin](https://gstreamer.freedesktop.org/documentation/libav/avenc_mpeg1video.html?gi-language=c#avenc_mpeg1video-page)
- [Build OpenCV On Linux](https://docs.opencv.org/4.x/d7/d9f/tutorial_linux_install.html)
- [JSMPEG Library](https://github.com/phoboslab/jsmpeg)

## Install, Build, and Run

Need to install OpenCV and GStreamer packages. Here is how I did it on Ubuntu 20.04:

```bash
# install GStreamer. Can get an updated command on the GStreamer website
sudo apt-get install libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev libgstreamer-plugins-bad1.0-dev gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav gstreamer1.0-tools gstreamer1.0-x gstreamer1.0-alsa gstreamer1.0-gl gstreamer1.0-gtk3 gstreamer1.0-qt5 gstreamer1.0-pulseaudio

# install OpenCV from source. GStreamer should be enabled by default
# could also install from apt-get, but I wanted to make sure GStreamer was enabled
git clone https://github.com/opencv/opencv.git

mkdir build

cd build

cmake -D WITH_GSTREAMER=ON ../opencv

cmake --build .

sudo make install
```

Build and run the program:

```bash
cmake build .

make

./example
```