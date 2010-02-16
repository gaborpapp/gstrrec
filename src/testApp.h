#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxSimpleGuiToo.h"
#include "ofxOpenCv.h"

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
		void update_source();
		void find_contours();

		// gui
		ofxSimpleGuiContent *gui_video;

		bool source_video;
		bool source_video_last;

		bool camera_prev;
		bool camera_next;

		// video source
		int video_width;
		int video_height;


		ofVideoGrabber video_grabber;
		ofVideoPlayer video_player;

		ofTexture video_texture;

		int camera_id;

		bool new_frame_available;

		// ofxOpenCV
		ofxCvColorImage img_color;
		ofxCvGrayscaleImage img_gray;

		ofxCvContourFinder contour_finder;
};

#endif
