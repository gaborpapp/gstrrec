#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxSimpleGuiToo.h"
#include "ofxOpenCv.h"
#include "ofxVideoGrabber.h"

#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 480

class testApp : public ofSimpleApp
{
	public:
		testApp();
		~testApp();

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased();

	private:
		enum {
			SOURCE_VIDEO,
			SOURCE_CAMERA,
			SOURCE_CAMERA_DC1394
		};

		void update_source();
		void find_contours();
		void apply_filters();

		// gui
		ofxSimpleGuiContent *gui_video;

		bool source_video;
		bool source_video_last;

		int source;
		int source_last;

		bool camera_prev;
		bool camera_next;
		bool camera_dc1394;
		//bool camera_settings;

		bool draw_bbox;
		bool draw_blob;
		bool draw_hull;
		bool draw_defects;

		// video source
		int video_width;
		int video_height;

		ofVideoGrabber video_grabber;
		ofVideoPlayer video_player;
		ofxVideoGrabber video_grabber_dc1394;
		Libdc1394Grabber *dc1394_sdk;

		ofTexture video_texture;

		int camera_id;

		bool new_frame_available;

		// ofxOpenCV
		ofxCvColorImage img_color;
		ofxCvGrayscaleImage img_gray;
		ofxCvGrayscaleImage img_blur;
		ofxCvGrayscaleImage img_highpass;
		ofxCvGrayscaleImage img_amplify;

		ofxCvContourFinder contour_finder;

		float contour_min_area;
		float contour_max_area;

		// filter
		bool filter_blur;
		int filter_blur_amount;

		bool filter_highpass;
		int filter_highpass_blur_amount;
		int filter_highpass_noise_amount;

		bool filter_amplify;
		float filter_amplify_amount;
};

#endif
