#include <iostream>
#include <chrono>

#include <opencv2/opencv.hpp>

#define DEBUG_WINDOW

struct FPS {
    int fps_buffer_len { 32 };

    int fps_index { 0 };

    double value { 0.0 };

    double fps_buffer[32];

    bool ready { false };

    // initialize the FPS buffer. Fills the buffer with 0.0
    FPS() {
        for (int i { 0 }; i < fps_buffer_len; i++) {
            fps_buffer[i] = 0.0;
        }
    }

    // update the FPS value if the fps_index has reached the buffer length
    void update(double duration) {
        fps_buffer[fps_index] = duration;
        fps_index++;
        if (fps_index >= fps_buffer_len) {
            fps_index = 0;
            double sum_sec = 0.0;
            for (int i { 0 }; i < fps_buffer_len; i++) {
                sum_sec += fps_buffer[i];
            }

            value = static_cast<double>(fps_buffer_len) / sum_sec;
        }
    }
};

int main() {

    // This pipeline outputs a test pattern.
    // std::string input_pipe {
    //     "videotestsrc pattern=1 ! "
    //     "videoconvert ! "
    //     "appsink"
    // };

    // This pipeline is for a MJPEG web camera.
    // v4l2-ctl -d /dev/video0 --list-formats-ext
    std::string input_pipe { 
        "v4l2src device=/dev/video0 io-mode=2 do-timestamp=true ! "
        "image/jpeg,framerate=30/1,width=(int)1920,height=(int)1080 ! "
        "jpegdec ! "
        "videoconvert ! "
        "appsink"
    };

    // This pipeline outputs to MPEG1 over UDP. A websocket server can pick this up and stream it to a browser.
    // NOTE: I drop the framerate to 15 fps to reduce the bandwidth. Any MPEG1 compatible FPS should work.
    // NOTE: MPEG1 expects YUV pixel format. The I420 code is the same as YUV420p (as seen in ffmpeg).
    // NOTE: I set the bitrate a bit low and specify the maxrate. Again, this reduces bandwidth.
    std::string output_pipe {
        "appsrc ! "
        "videoconvert ! "
        "videorate ! "
        "video/x-raw,framerate=15/1,format=I420 ! "
        "avenc_mpeg1video bitrate=100000 maxrate=200000 ! "
        "mpegtsmux ! "
        "udpsink host=127.0.0.1 port=3131"
    };

    cv::VideoCapture input(input_pipe, cv::CAP_GSTREAMER);

    if (!input.isOpened()) {
        throw std::runtime_error("Failed to open input device: " + input_pipe);
    }

    std::cout << "Input device: '" << input_pipe  << "' opened" << std::endl;

    cv::VideoWriter output(output_pipe, cv::CAP_GSTREAMER, 0, 30.0, cv::Size(1920, 1080), true);

    if (!output.isOpened()) {
        throw std::runtime_error("Failed to open output device: " + output_pipe);
    }

    std::cout << "Output device: '" << output_pipe  << "' opened" << std::endl;

    cv::Mat frame;
    cv::Mat frame_gray;
    FPS fps;

    while (true) {
        auto start_time = std::chrono::high_resolution_clock::now();
        
        input >> frame;

        if (frame.empty()) {
            break;
        }

        // Do some simple image processing. Edge detection.
        cv::cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);
        cv::Canny(frame_gray, frame_gray, 100, 200);
        cv::cvtColor(frame_gray, frame, cv::COLOR_GRAY2BGR);

        #ifdef DEBUG_WINDOW // display opencv window for debugging
        cv::imshow("debug", frame);
        if (cv::waitKey(1) == 27) {
            break;
        }
        #endif

        output << frame;

        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

        fps.update(static_cast<double>(duration.count()) / 1000.0);
        
        std::cout << "\rFPS: " << fps.value << std::flush;
    }

    input.release();
    output.release();
    cv::destroyAllWindows();

    std::cout << std::endl;

    return 0;
}