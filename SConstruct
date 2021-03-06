################################################################################
# gstrrec top level scons script

TARGET = 'gstrrec'

DEBUG = 1
PROFILE = 0
STATIC = 0

env = Environment()

platform = env['PLATFORM']

BuildDir('build', 'src', duplicate = 0)
TARGET = 'build/' + TARGET

APP_SOURCES  = ['src/main.cpp', 'src/testApp.cpp']

if platform == 'posix':
	PKG_CONFIG = ARGUMENTS.get('PKGConfig', 'pkg-config')
elif platform  == 'darwin':
	PKG_CONFIG = ARGUMENTS.get('PKGConfig', '/opt/local/bin/pkg-config')

OFLIB = ['libs/openFrameworks/utils/ofMath.cpp',
	'libs/openFrameworks/utils/ofUtils.cpp',
	'libs/openFrameworks/graphics/ofBitmapFont.cpp',
	'libs/openFrameworks/graphics/ofGraphics.cpp',
	'libs/openFrameworks/graphics/ofImage.cpp',
	'libs/openFrameworks/graphics/ofTexture.cpp',
	'libs/openFrameworks/graphics/ofTrueTypeFont.cpp',
	'libs/openFrameworks/app/ofAppGlutWindow.cpp',
	'libs/openFrameworks/app/ofAppRunner.cpp',
	'libs/openFrameworks/video/ofVideoPlayer.cpp',
	'libs/openFrameworks/video/ofVideoGrabber.cpp'
]
#'libs/openFrameworks/sound/ofSoundPlayer.cpp',
#'libs/openFrameworks/sound/ofSoundStream.cpp'
# 'libs/openFrameworks/video/ofUCUtils.cpp',
# 'libs/openFrameworks/communication/ofSerial.cpp'

OFINC = ['libs/openFrameworks']

GLEELIB = ['libs/GLee/GLee.c']
GLEEINC = ['libs/GLee']

OFXMLSETTINGSLIB = ['libs/ofxXmlSettings/libs/tinyxml.cpp',
	'libs/ofxXmlSettings/libs/tinyxmlerror.cpp',
	'libs/ofxXmlSettings/libs/tinyxmlparser.cpp',
	'libs/ofxXmlSettings/src/ofxXmlSettings.cpp']
OFXMLSETTINGSINC = ['libs/ofxXmlSettings/src',
	'libs/ofxXmlSettings/libs']

OFXMSAIOLIB = ['libs/ofxMSAInteractiveObject/src/ofxMSAInteractiveObject.cpp']
OFXMSAIOINC = ['libs/ofxMSAInteractiveObject/src']

OFXSIMPLEGUILIB = ['libs/ofxSimpleGuiToo/src/ofxSimpleGuiConfig.cpp',
	'libs/ofxSimpleGuiToo/src/ofxSimpleGuiControl.cpp',
	'libs/ofxSimpleGuiToo/src/ofxSimpleGuiPage.cpp',
	'libs/ofxSimpleGuiToo/src/ofxSimpleGuiToo.cpp',
	'libs/ofxSimpleGuiToo/src/Controls/ofxSimpleGuiButton.cpp',
	'libs/ofxSimpleGuiToo/src/Controls/ofxSimpleGuiColorPicker.cpp',
	'libs/ofxSimpleGuiToo/src/Controls/ofxSimpleGuiContent.cpp',
	'libs/ofxSimpleGuiToo/src/Controls/ofxSimpleGuiFPSCounter.cpp',
	'libs/ofxSimpleGuiToo/src/Controls/ofxSimpleGuiMovieSlider.cpp',
	'libs/ofxSimpleGuiToo/src/Controls/ofxSimpleGuiOption.cpp',
	'libs/ofxSimpleGuiToo/src/Controls/ofxSimpleGuiOptionGroup.cpp',
	'libs/ofxSimpleGuiToo/src/Controls/ofxSimpleGuiQuadWarp.cpp',
	'libs/ofxSimpleGuiToo/src/Controls/ofxSimpleGuiSlider2d.cpp',
	'libs/ofxSimpleGuiToo/src/Controls/ofxSimpleGuiTitle.cpp',
	'libs/ofxSimpleGuiToo/src/Controls/ofxSimpleGuiToggle.cpp']
OFXSIMPLEGUIINC = ['libs/ofxSimpleGuiToo/src/',
	'libs/ofxSimpleGuiToo/src/Controls/']

OFXOPENCVLIB = ['libs/ofxOpenCv/src/ofxCvColorImage.cpp',
	'libs/ofxOpenCv/src/ofxCvContourFinder.cpp',
	'libs/ofxOpenCv/src/ofxCvFloatImage.cpp',
	'libs/ofxOpenCv/src/ofxCvGrayscaleImage.cpp',
	'libs/ofxOpenCv/src/ofxCvImage.cpp',
	'libs/ofxOpenCv/src/ofxCvShortImage.cpp']
OFXOPENCVINC = ['libs/ofxOpenCv/src']

OFXTHREADLIB = ['libs/ofxThread/src/ofxThread.cpp']
OFXTHREADINC = ['libs/ofxThread/src/']

OFXVIDEOGRABBERLIB = [
	'libs/ofxVideoGrabber/ofxVideoGrabber.cpp',
	'libs/ofxVideoGrabber/ofxVideoGrabberFeature.cpp',
	'libs/ofxVideoGrabber/ofxVideoGrabberSettings.cpp',
	'libs/ofxVideoGrabber/Libdc1394Grabber/Libdc1394Grabber.cpp',
	'libs/ofxVideoGrabber/Libdc1394Grabber/Libdc1394GrabberFeatureHelper.cpp',
	'libs/ofxVideoGrabber/Libdc1394Grabber/Libdc1394GrabberFramerateHelper.cpp',
	'libs/ofxVideoGrabber/Libdc1394Grabber/Libdc1394GrabberUtils.cpp',
	'libs/ofxVideoGrabber/Libdc1394Grabber/Libdc1394GrabberVideoFormatHelper.cpp'
]

OFXVIDEOGRABBERINC = [ 'libs/ofxVideoGrabber' ]

SOURCES = OFLIB + GLEELIB + OFXMLSETTINGSLIB + \
	OFXMSAIOLIB + OFXSIMPLEGUILIB + OFXOPENCVLIB + \
	OFXVIDEOGRABBERLIB + OFXTHREADLIB + \
	APP_SOURCES
INCLUDES = OFINC + GLEEINC + OFXMLSETTINGSINC + \
	OFXMSAIOINC + OFXSIMPLEGUIINC + OFXOPENCVINC + \
	OFXVIDEOGRABBERINC + OFXTHREADINC

# add fobs on linux and quicktime otherwise
if platform == 'posix':
	FOBSLIB = ['libs/fobs/Decoder.cpp',
		'libs/fobs/Encoder.cpp',
		'libs/fobs/Error.cpp',
		'libs/fobs/PacketBuffer.cpp',
		'libs/fobs/Transcoder.cpp'
	]
	FOBSINC = ['libs/fobs']
	SOURCES += FOBSLIB
	INCLUDES += FOBSINC 
else:
	SOURCES += ['libs/openFrameworks/video/ofQtUtils.cpp']

# change the environment for building

CCFLAGS = '-Wall -Wno-unknown-pragmas -Wno-long-long '
#'-pedantic ' # because of the ofPoint anonymous struct


LINKFLAGS = ''

if platform != 'cygwin':
	CCFLAGS += '-Wno-variadic-macros '

if PROFILE:
	DEBUG = 1
	CCFLAGS += '-pg '
	LINKFLAGS += '-pg '

if DEBUG:
	CCFLAGS += '-ggdb2 -O0 -DDEBUG=1 '
else:
	CCFLAGS += '-g0 -O3 '

CPPPATH = ['/usr/include', 
	'libs/openFrameworks/app',
	'libs/openFrameworks/communication',
	'libs/openFrameworks/events',
	'libs/openFrameworks/graphics',
	'libs/openFrameworks/utils',
	'libs/openFrameworks/video'
]

CPPPATH += INCLUDES

LIBPATH = []
LIBS = []

if platform == 'darwin':
    LIBPATH += ['/opt/local/lib']

env.Append(CPPPATH = CPPPATH)
env.Append(CCFLAGS = CCFLAGS)
env.Append(LINKFLAGS = LINKFLAGS)
env.Append(LIBPATH = LIBPATH)
env.Append(LIBS = LIBS)

for lib in ['freetype2', 'opencv']:
	env.MergeFlags(['!%s --cflags --libs %s' % (PKG_CONFIG, lib)])

if platform == 'darwin':
    env.Append(FRAMEWORKS = ['GLUT', 'OpenGL', 'QuickTime', 'CoreServices',
	    'ApplicationServices', 'Carbon'])

# platform specific fixes

'''
if platform == 'posix':
	# ffmpeg does not compile with -pedantic
	CCFLAGS_FFMPEG = CCFLAGS.replace('-pedantic ', '')
	for i in range(len(SOURCES)):
		if SOURCES[i] in FOBSLIB:
			SOURCES[i] = env.Object(SOURCES[i], CCFLAGS = CCFLAGS_FFMPEG)
'''

# configure

LIBS = [['m', 'math.h'],
	['pthread', 'pthread.h'],
	['freeimage', 'FreeImage.h'],
	['freetype', 'ft2build.h', False],
	['PocoFoundation', 'Poco/Poco.h'],
	[['cv', 'cxcore'], 'cv.h', False],
	['dc1394', 'dc1394/dc1394.h']]

if platform == 'posix':
	LIBS += [['X11', 'X11/Xlib.h'],
		 ['GL', 'GL/gl.h'],
		 ['GLU', 'GL/glu.h'],
		 ['glut', 'GL/glut.h'],
		 ['avformat', 'ffmpeg/avformat.h'],
		 ['avcodec', 'ffmpeg/avcodec.h']]

if not GetOption('clean'):
	conf = Configure(env)
	for l in LIBS:
		if len(l) == 2:
			(lib, headers) = l
			autoadd = 1
		else:
			(lib, headers, autoadd) = l

		if not conf.CheckLibWithHeader(lib, headers, 'c++', autoadd = autoadd):
			print "error: '%s' must be installed!" % lib
			Exit(1)
	env = conf.Finish()

# build the application

env.Program(source = SOURCES, target = TARGET)

# run

import os
if platform == 'posix':
    env['ENV']['DISPLAY'] = os.environ['DISPLAY']
    if 'LIBGL_DRIVERS_PATH' in os.environ:
        env['ENV']['LIBGL_DRIVERS_PATH'] = os.environ['LIBGL_DRIVERS_PATH']

env.AlwaysBuild(env.Alias('run', [TARGET], '%s' % env.File(TARGET).abspath))

