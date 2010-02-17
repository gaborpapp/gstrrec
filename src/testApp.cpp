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

	img_color.clear();
	img_gray.clear();
	img_blur.clear();
	img_highpass.clear();
	img_amplify.clear();
}

void testApp::setup()
{
	ofBackground(100, 100, 100);

	ofSetDataPathRoot("data/");
	if (!video_player.loadMovie("test-01.mov"))
	{
		::exit(1);
	}

	video_grabber.setVerbose(false);
	video_grabber.listDevices();
	video_grabber.setDeviceID(camera_id);

	video_width = max(video_player.getWidth(), video_grabber.getWidth());
	video_height = max(video_player.getHeight(), video_grabber.getHeight());

	video_texture.allocate(video_width, video_height, GL_RGB);
	img_color.allocate(video_width, video_height);
	img_gray.allocate(video_width, video_height);
	img_blur.allocate(video_width, video_height);
	img_highpass.allocate(video_width, video_height);
	img_amplify.allocate(video_width, video_height);

	gui_video = &gui.addContent("Camera feed", video_texture, 320);
	gui.page(1).setName("Hand Gesture Classifier");

	ofxSimpleGuiConfig *c = gui.config;
	gui.addToggle("Use video", source_video).setSize(128, 20);
	gui.addButton("Prev camera", camera_prev).setSize(128, 20);
	gui.addButton("Next camera", camera_next).setDeltaPos(128 + c->padding.x,-(c->buttonHeight + c->padding.y)).setSize(128, 20);

	gui.addContent("Tracker", img_gray, 320).setNewColumn(true);
	gui.addSlider("Min area", contour_min_area, 0, 1);
	gui.addSlider("Max area", contour_max_area, 0, 1);

	gui.addContent("", img_blur, 160).setNewColumn(true);
	gui.addToggle("Blur", filter_blur).setDeltaPos(170, -(140 + c->padding.y)).setSize(150, 20);
	gui.addSlider("Value", filter_blur_amount, 0, 15).setDeltaPos(170, -120).setSize(150, 30);

	gui.addContent("", img_highpass, 160);
	gui.addToggle("Highpass", filter_highpass).setDeltaPos(170, -(140 + c->padding.y)).setSize(150, 20);
	gui.addSlider("Blur", filter_highpass_blur_amount, 0, 200).setDeltaPos(170, -120).setSize(150, 30);
	gui.addSlider("Noise", filter_highpass_noise_amount, 0, 30).setDeltaPos(170, -80).setSize(150, 30);

	gui.addContent("", img_amplify, 160);
	gui.addToggle("Amplify", filter_amplify).setDeltaPos(170, -(140 + c->padding.y)).setSize(150, 20);
	gui.addSlider("Value", filter_amplify_amount, 0, 3.).setDeltaPos(170, -120).setSize(150, 30);

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
	int img_area = video_width * video_height;
	contour_finder.findContours(img_gray,
								img_area * contour_min_area,
								img_area * contour_max_area, 10, true);
}

void testApp::apply_filters()
{
	if (filter_blur)
	{
		if (filter_blur_amount > 0)
		{
			img_gray.blur((filter_blur_amount * 2) + 1);
		}
		img_blur = img_gray;
	}
	else
	{
		img_blur.set(0);
	}

	if (filter_highpass)
	{
		img_gray.highpass(filter_highpass_blur_amount, filter_highpass_noise_amount);
		img_highpass = img_gray;
	}
	else
	{
		img_highpass.set(0);
	}

	if (filter_amplify)
	{
		img_gray.amplify(filter_amplify_amount);
		img_amplify = img_gray;
	}
	else
	{
		img_amplify.set(0);
	}
}

void testApp::update()
{
	static int values_inited = 0; // if gui values are loaded

	// before the first update the gui is updated
	if (!values_inited)
	{
		source_video_last = !source_video;
		values_inited = 1;
	}

	update_source();

	img_color.setFromPixels(source_video ? video_player.getPixels() :
										   video_grabber.getPixels(),
							video_width, video_height);

	// convert to grayscale with operator overloading
	img_gray = img_color;

	// enhance image quality
	apply_filters();

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
		new_frame_available = video_grabber.isFrameNew();
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

