#include "testApp.h"

testApp::testApp() :
	source_video(true),
	source_video_last(false),
	camera_prev(false),
	camera_next(false),
	camera_id(0)
{
	img_color.allocate(CAMERA_WIDTH, CAMERA_HEIGHT);
	img_gray.allocate(CAMERA_WIDTH, CAMERA_HEIGHT);

}

testApp::~testApp()
{
	video_player.close();
	video_grabber.close();
	video_texture.clear();

	img_color.clear();
	img_gray.clear();
}

void testApp::setup()
{
	ofBackground(100, 100, 100);

	ofSetDataPathRoot("data/");
	video_player.loadMovie("test-01.mov");

	video_grabber.setVerbose(false);
	video_grabber.listDevices();
	video_grabber.setDeviceID(camera_id);

	video_width = max(video_player.getWidth(), video_grabber.getWidth());
	video_height = max(video_player.getHeight(), video_grabber.getHeight());

	video_texture.allocate(video_width, video_height, GL_RGB);


	gui_video = &gui.addContent("Camera feed", video_texture, 320);

	gui.addToggle("Use video", source_video).setSize(128, 20);
	gui.addButton("Next camera", camera_next).setSize(128, 20);
	gui.addButton("Prev camera", camera_prev).setSize(128, 20);

	gui.addContent("Tracker", img_gray, 320).setNewColumn(true);

	gui.page(1).setName("Hand Gesture Classifier");


/*
	// start a new group
	gui.addTitle("Another group");
	gui.addSlider("myFloat2", myFloat2, 0.0, 1);
	gui.addSlider("myInt2", myInt2, 3, 8);
	gui.addToggle("myBool2", myBool2);

	// new group, this time separate into it's own column
	gui.addTitle("Yes one more group").setNewColumn(true);
	gui.addToggle("myBool4", myBool4);
	gui.addToggle("myBool3", myBool3);
	gui.addSlider("myFloat3", myFloat3, 0.0, 1);
	gui.addSlider("myFloat4", myFloat4, 0.0, 20);
	gui.addSlider("myInt6", myInt6, 0, 10);
	gui.addSlider("myInt4", myInt4, 10, 20);
	gui.addContent("Camera feed", vidGrabber, 160);
	gui.addContent("Inverted", videoTexture, 160);


	gui.addPage("A new page");		// use '[' ']' to cycle through pages, or keys 1-9
	gui.addSlider("myInt5", myInt5, 2, 5);
	gui.addSlider("myInt7", myInt7, 0, 100);
	gui.addSlider("myInt8", myInt8, 10, 50);
	gui.addSlider("myInt3", myInt3, 0, 100);

	// by default each page is saved in an xml with the same name as display name
	// you can override this with ofxSimpleGuiPage::setXMLName(string s);
	// ofxSimpleGuiToo::addPage() returns reference to the page, so you can do it directly on one line
	// of save it in a variable for use later
	*/

	gui.loadFromXML();

	gui.show();
}

void testApp::find_contours()
{
	if (source_video)
	{
		img_color.setFromPixels(video_player.getPixels(),
				video_player.getWidth(), video_player.getHeight());
	}
	else
	{
		img_color.setFromPixels(video_grabber.getPixels(),
				video_grabber.getWidth(), video_grabber.getHeight());
	}

	// convert to grayscale with operator overloading
	img_gray = img_color;

	contour_finder.findContours(img_gray, 20, (340*240), 10, true);
}

void testApp::update()
{
	update_source();

	find_contours();
}

void testApp::update_source()
{
	// init previous/next camera
	if (camera_next || camera_prev)
	{
		if (camera_next)
		{
			camera_id++;
		}
		else if (camera_prev)
		{
			camera_id--;
			if (camera_id < 0)
				camera_id = 0;
		}

		if (!source_video)
		{
			video_grabber.close();
			video_grabber.setDeviceID(camera_id);
			video_grabber.initGrabber(CAMERA_WIDTH, CAMERA_HEIGHT);
		}

		camera_next = camera_prev = false;
	}

	// check source change
	if (source_video != source_video_last)
	{
		if (source_video)
		{
			video_player.play();
			gui_video->setName("Video Feed");
			video_grabber.close();
		}
		else
		{
			video_player.stop();
			gui_video->setName("Camera Feed");
			video_grabber.initGrabber(CAMERA_WIDTH, CAMERA_HEIGHT);
		}
	}

	// update video source
	new_frame_available = false;
	if (source_video)
	{
		video_player.update();
		new_frame_available = video_player.isFrameNew();
		if (new_frame_available)
		{
			video_texture.loadData(video_player.getPixels(),
					video_player.getWidth(), video_player.getHeight(), GL_RGB);
		}
	}
	else
	{
		video_grabber.update();
		new_frame_available = video_player.isFrameNew();
		if (new_frame_available)
		{
			video_texture.loadData(video_grabber.getPixels(),
					video_grabber.getWidth(), video_grabber.getHeight(), GL_RGB);
		}
	}

	source_video_last = source_video;
}

void testApp::draw()
{
	gui.draw();

	contour_finder.draw(351, 56, 320, 240);
}

void testApp::keyPressed (int key)
{
}

void testApp::keyReleased(int key)
{
}

void testApp::mouseMoved(int x, int y )
{
}

void testApp::mouseDragged(int x, int y, int button)
{
}

void testApp::mousePressed(int x, int y, int button)
{
}

void testApp::mouseReleased()
{
}

