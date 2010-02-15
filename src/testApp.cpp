#include "testApp.h"

testApp::testApp() :
	source_video(true),
	source_video_last(false),
	camera_prev(false),
	camera_next(false),
	camera_id(0)
{
}

testApp::~testApp()
{
	video_player.close();
	video_grabber.close();
	video_texture.clear();
}

void testApp::setup()
{
	ofBackground(100, 100, 100);

	ofSetDataPathRoot("data/");
	video_player.loadMovie("test-01.mov");

	video_grabber.setVerbose(false);
	video_grabber.listDevices();
	video_grabber.setDeviceID(camera_id);
	video_grabber.initGrabber(CAMERA_WIDTH, CAMERA_HEIGHT);

	video_width = max(video_player.getWidth(), video_grabber.getWidth());
	video_height = max(video_player.getHeight(), video_grabber.getHeight());

	video_texture.allocate(video_width, video_height, GL_RGB);

	gui_video = &gui.addContent("Camera feed", video_texture, 320);

	gui.addToggle("Use video", source_video).setSize(128, 20);
	gui.addButton("Next camera", camera_next).setSize(128, 20);
	gui.addButton("Prev camera", camera_prev).setSize(128, 20);
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

void testApp::update()
{
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

		video_grabber.close();
		video_grabber.setDeviceID(camera_id);
		video_grabber.initGrabber(CAMERA_WIDTH, CAMERA_HEIGHT);

		camera_next = camera_prev = false;
	}

	if (source_video != source_video_last)
	{
		if (source_video)
		{
			video_player.play();
			gui_video->setName("Video Feed");
		}
		else
		{
			video_player.stop();
			gui_video->setName("Camera Feed");
		}
	}

	if (source_video)
	{
		video_player.update();
		video_texture.loadData(video_player.getPixels(),
				video_player.getWidth(), video_player.getHeight(), GL_RGB);
	}
	else
	{
		video_grabber.update();
		video_texture.loadData(video_grabber.getPixels(),
				video_grabber.getWidth(), video_grabber.getHeight(), GL_RGB);
	}

	source_video_last = source_video;
}

void testApp::draw()
{
	gui.draw();
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

