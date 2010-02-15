#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

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
		int video_width;
		int video_height;

		ofVideoGrabber video_grabber;
		ofVideoPlayer video_player;

		ofTexture video_texture;

		bool source_video;
		bool source_video_last;

		bool camera_next;
		bool camera_prev;
};

#endif
