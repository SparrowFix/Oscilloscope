#include "PlayerOverlay.h"
#include <Poco/Delegate.h>
#include "sounddevices.h"
#include "ofxFontAwesome.h"
#include "ofApp.h"
#include "MuiL.h"


static string timestring(double t);

PlayerOverlay::PlayerOverlay( float x_, float y_, float width_, float height_)
: mui::Container( x_, y_, width_, height_ ){
	float x = 10, y = 10, w = 400, h = 30;
	bg = ofColor(125,50);
	opaque = true;
	micMenu = NULL;

	string xx = ofxFontAwesome::play; 
	
		stopButton = new FaButton( ofxFontAwesome::cogs, x, y, h, h );
	ofAddListener( stopButton->onPress, this, &PlayerOverlay::buttonPressed );
	y += stopButton->height + 10;
	add( stopButton );
	
	scaleLabel = addLabel( "Scale" );
	scaleSlider = new mui::SliderWithLabel( x, y, w, h, 0.1, 2, 1, 2 );
	scaleSlider->label->fg = ofColor(255);
	ofAddListener( scaleSlider->slider->onChange, this, &PlayerOverlay::sliderChanged );
	y += scaleSlider->height + 10;
	add( scaleSlider );
	
	
	w = 90;
	flipXY = new FaToggleButton( ofxFontAwesome::repeat, ofxFontAwesome::repeat, x, y, h, h );
	ofAddListener( flipXY->onPress, this, &PlayerOverlay::buttonPressed );
	add( flipXY );
	x += 100;
	
	invertX = new FaToggleButton( ofxFontAwesome::arrows_h, ofxFontAwesome::arrows_h, x, y, h, h );
	ofAddListener( invertX->onPress, this, &PlayerOverlay::buttonPressed );
	add( invertX );
	x += 100;
	
	invertY = new FaToggleButton( ofxFontAwesome::arrows_v, ofxFontAwesome::arrows_v, x, y, h, h );
	ofAddListener( invertY->onPress, this, &PlayerOverlay::buttonPressed );
	add( invertY );
	x += 100;
	
	fullscreenButton = new FaToggleButton( ofxFontAwesome::expand, ofxFontAwesome::compress, x, y, h, h );
	ofAddListener( fullscreenButton->onPress, this, &PlayerOverlay::buttonPressed );
	add( fullscreenButton );
	
	loadFileButton = new FaButton( ofxFontAwesome::folder_open, x, y, h, h );
	ofAddListener( loadFileButton->onPress, this, &PlayerOverlay::buttonPressed );
	add( loadFileButton );
	
	useMicButton = new FaToggleButton( ofxFontAwesome::microphone, ofxFontAwesome::microphone_slash, x, y, h, h );
	ofAddListener( useMicButton->onPress, this, &PlayerOverlay::buttonPressed );
	add( useMicButton );
	
	playButton = new FaToggleButton( ofxFontAwesome::play, ofxFontAwesome::pause, x, y, h, h );
	ofAddListener( playButton->onPress, this, &PlayerOverlay::buttonPressed );
	add( playButton );
	
	sideBySide = new FaToggleButton(ofxFontAwesome::cube, ofxFontAwesome::cube, 10, 1, h, h);
	ofAddListener(sideBySide->onPress, this, &PlayerOverlay::buttonPressed);
	add(sideBySide);

	flip3d = new FaToggleButton(ofxFontAwesome::exchange, ofxFontAwesome::exchange, 10, 1, h, h);
	ofAddListener(flip3d->onPress, this, &PlayerOverlay::buttonPressed);
	add(flip3d);



	x = 10;
	y += invertY->height + 10;
	
	timeSlider = new mui::Slider( 0, 0, w, h );
	add( timeSlider );
	
	timeLabelButton = new mui::Button("-00:00:00", 0,0, 30,30);
	timeLabelButton->fitWidthToLabel(3);
	ofAddListener(timeLabelButton->onPress, this, &PlayerOverlay::buttonPressed);
	add(timeLabelButton);
	
	outputVolumeLabel = addLabel( "Volume" );
	outputVolumeSlider = new mui::SliderWithLabel(0, 0, 100, h, 0, 1, 0.8, 2 );
	ofAddListener( outputVolumeSlider->slider->onChange, this, &PlayerOverlay::sliderChanged );
	outputVolumeSlider->label->fg = ofColor(255);
	add(outputVolumeSlider);
	
	strokeWeightLabel = addLabel( "Stroke Weight" );
	strokeWeightSlider = new mui::SliderWithLabel(0, 0, 100, h, 1, 50, 4, 1 );
	ofAddListener( strokeWeightSlider->slider->onChange, this, &PlayerOverlay::sliderChanged );
	strokeWeightSlider->label->fg = ofColor(255);
	add( strokeWeightSlider );
	
	timeStretchLabel = addLabel( "Time Stretch" );
	timeStretchSlider = new mui::SliderWithLabel(0, 0, 100, h, 0.25, 100, 1, 2 );
	timeStretchSlider->slider->valueMapper = make_shared<mui::Slider::MapperLog>(6000);
	ofAddListener( timeStretchSlider->slider->onChange, this, &PlayerOverlay::sliderChanged );
	timeStretchSlider->label->fg = ofColor(255);
	add( timeStretchSlider );
	
	blurLabel = addLabel( "Blur" );
	blurSlider = new mui::SliderWithLabel(0,0, 100, h, 0, 255, 30, 0);
	ofAddListener( blurSlider->slider->onChange, this, &PlayerOverlay::sliderChanged );
	blurSlider->label->fg = ofColor(255);
	add(blurSlider);
	
	numPtsLabel = addLabel( "Points" );
	numPtsSlider = new mui::SliderWithLabel(0,0,100,h, 1, 100, 20, 0);
	ofAddListener( numPtsSlider->slider->onChange, this, &PlayerOverlay::sliderChanged );
	numPtsSlider->label->fg = ofColor(255);
	add(numPtsSlider);

	hueLabel = addLabel( "Hue" );
	hueSlider = new mui::SliderWithLabel(0,0,100,h,0,360, 127, 0);
	ofAddListener( hueSlider->slider->onChange, this, &PlayerOverlay::sliderChanged );
	hueSlider->label->fg = ofColor(255);
	add(hueSlider);
	
	intensityLabel = addLabel( "Intensity" );
	intensitySlider = new mui::SliderWithLabel(0,0,100,h,0,1, 0.5, 2);
	ofAddListener( intensitySlider->slider->onChange, this, &PlayerOverlay::sliderChanged );
	intensitySlider->label->fg = ofColor(255);
	add(intensitySlider);
	
	afterglowLabel = addLabel( "Afterglow" );
	afterglowSlider = new mui::SliderWithLabel(0,0,100,h,0,1, 0.3, 2);
	ofAddListener( afterglowSlider->slider->onChange, this, &PlayerOverlay::sliderChanged );
	afterglowSlider->label->fg = ofColor(255);
	add(afterglowSlider);
	
	
	

	// make labels the same width
	outputVolumeSlider->label->width = timeStretchSlider->label->width;
	
	layout();
}

void PlayerOverlay::layout(){
	mui::L(fullscreenButton).pos(width-30, 0);
	mui::L(stopButton).leftOf(fullscreenButton,1);
	mui::L(sideBySide).leftOf(stopButton, 10);
	mui::L(flip3d).leftOf(sideBySide, 1);


	mui::L(playButton).pos(10,40);
	mui::L(timeLabelButton).pos(width-10-timeLabelButton->width,playButton->y);
	mui::L(timeSlider).rightOf(playButton, 10).stretchToRightEdgeOfParent(width-(timeLabelButton->x-10));
	
	mui::L(loadFileButton).below(playButton, 10);
	mui::L(useMicButton).rightOf(loadFileButton, 10 );
	mui::L(outputVolumeLabel).rightOf(useMicButton,20);
	mui::L(outputVolumeSlider).rightOf(outputVolumeLabel,5).stretchToRightEdgeOfParent(10);
	
	mui::L(timeStretchLabel).below(outputVolumeLabel).alignRightEdgeTo(outputVolumeLabel);
	mui::L(timeStretchSlider).rightOf(timeStretchLabel,5).stretchToRightEdgeOfParent(10);
	
	mui::L(invertX).below(timeStretchLabel, 20).alignLeftEdgeTo(loadFileButton);
	mui::L(invertY).rightOf(invertX, 10);
	mui::L(flipXY).rightOf(invertY,10);
	mui::L(scaleLabel).rightOf(flipXY,20);
	mui::L(scaleSlider).rightOf(scaleLabel,5).stretchToRightEdgeOfParent(10);
	
	mui::L(strokeWeightLabel).below(scaleLabel).alignRightEdgeTo(scaleLabel);
	mui::L(strokeWeightSlider).rightOf(strokeWeightLabel,5).stretchToRightEdgeOfParent(10);
	
	/*mui::L(blurLabel).below(strokeWeightLabel).alignRightEdgeTo(strokeWeightLabel);
	mui::L(blurSlider).rightOf(blurLabel,5).stretchToRightEdgeOf(this,10);
	
	mui::L(numPtsLabel).below(blurLabel).alignRightEdgeTo(blurLabel);
	mui::L(numPtsSlider).rightOf(numPtsLabel,5).stretchToRightEdgeOf(this,10);*/
	blurLabel->visible = false;
	blurSlider->visible = false;
	numPtsLabel->visible = false;
	numPtsSlider->visible = false;
	
	mui::L(hueLabel).below(strokeWeightLabel).alignRightEdgeTo(strokeWeightLabel);
	mui::L(hueSlider).rightOf(hueLabel,5).stretchToRightEdgeOfParent(10);
	
	mui::L(intensityLabel).below(hueLabel).alignRightEdgeTo(hueLabel);
	mui::L(intensitySlider).rightOf(intensityLabel,5).stretchToRightEdgeOfParent(10);
	
	mui::L(afterglowLabel).below(intensityLabel).alignRightEdgeTo(intensityLabel);
	mui::L(afterglowSlider).rightOf(afterglowLabel,5).stretchToRightEdgeOfParent(10);
	
	height = afterglowSlider->y + afterglowSlider->height;
}


// tiny helper functions.
// these should somehow be in mui, they're so handy
// returns false if the user changed the value
bool updateSlider( mui::Slider * slider, float targetValue, float &lastValue ){
	if( slider->hasFocus() ){
		if( lastValue != slider->value ){
			lastValue = slider->value;
			return false;
		}
		else{
			return true;
		}
	}
	else{
		slider->value = targetValue;
		return true;
	}
	
}

//--------------------------------------------------------------
void PlayerOverlay::update(){
	static float lastTimeVal = -1;
	useMicButton->selected = globals.micActive;
	timeSlider->visible = !globals.micActive;
	outputVolumeLabel->visible = !globals.micActive;
	outputVolumeSlider->visible = !globals.micActive;
	playButton->visible = !globals.micActive;
	
	if( !globals.micActive ){
		if( !updateSlider(timeSlider, globals.player.getPosition(), lastTimeVal ) ){
			globals.player.setPosition(timeSlider->value);
		}

		if( globals.player.isPlaying != playButton->selected ){
			playButton->selected = globals.player.isPlaying;
			playButton->commit();
		}
	}
	
	
	double currentTime = globals.player.getPositionMS()/1000.0;
	double duration = globals.player.duration/1000.0;
	
	if( timeLabelMode == 0 ){
		timeLabelButton->label->text = timestring(currentTime);
		timeLabelButton->bg = ofColor(0,0);
		timeLabelButton->label->fg = ofColor(255);
	}
	else if( timeLabelMode == 1 ){
		timeLabelButton->label->text = "-" + timestring(duration - currentTime);
		timeLabelButton->bg = ofColor(0,0);
		timeLabelButton->label->fg = ofColor(255);
	}
	timeLabelButton->label->commit();
}


//--------------------------------------------------------------
void PlayerOverlay::draw(){
	ofSetColor(150);
	ofDrawLine( 10, flipXY->y - 10, width-10, flipXY->y - 10 );
	ofSetColor(255);
}



//--------------------------------------------------------------
void PlayerOverlay::touchDown( ofTouchEventArgs &touch ){
}


//--------------------------------------------------------------
void PlayerOverlay::touchMoved( ofTouchEventArgs &touch ){
}


//--------------------------------------------------------------
void PlayerOverlay::touchUp( ofTouchEventArgs &touch ){
}


//--------------------------------------------------------------
void PlayerOverlay::touchDoubleTap( ofTouchEventArgs &touch ){
}


void PlayerOverlay::fromGlobals(){
	outputVolumeSlider->slider->value = globals.outputVolume;
	
	if( globals.player.isPlaying != playButton->selected ){
		playButton->selected = globals.player.isPlaying;
		playButton->commit();
	}
	
	scaleSlider->slider->value = globals.scale;
	strokeWeightSlider->slider->value = globals.strokeWeight;
	timeStretchSlider->slider->value = globals.timeStretch;
	blurSlider->slider->value = globals.blur;
	numPtsSlider->slider->value = globals.numPts;
	hueSlider->slider->value = globals.hue;
	intensitySlider->slider->value = globals.intensity;
	afterglowSlider->slider->value = globals.afterglow;
	invertX->selected = globals.invertX;
	invertX->commit();
	invertY->selected = globals.invertY;
	invertY->commit();
	flipXY->selected = globals.flipXY;
	flipXY->commit();
}

//--------------------------------------------------------------
void PlayerOverlay::buttonPressed( const void * sender, ofTouchEventArgs & args ){
	mui::Container * container = (mui::Container*) sender;
	
	if( sender == stopButton ){
		ofBaseApp * app = ofGetAppPtr();
		app->gotMessage( ofMessage( "stop-pressed" ) );
	}
	else if( sender == playButton ){
		if( globals.player.isPlaying ){
			globals.player.stop();
		}
		else{
			globals.player.play();
		}
	}
	else if( sender == invertX ){
		globals.invertX = invertX->selected;
	}
	else if( sender == invertY ){
		globals.invertY = invertY->selected;
	}
	else if( sender == flipXY ){
		globals.flipXY = flipXY->selected;
	}
	else if( sender == fullscreenButton ){
		ofSetFullscreen(fullscreenButton->selected);
		// windows becomes black without this, not sure why...
		//TODO: check if this is still a problem in of0.9
		#ifdef _WIN32
		if(fullscreenButton->selected){
			int w = ofGetScreenWidth(); 
			int h = ofGetScreenHeight(); 
			ofSetWindowShape(w,h); 
		}
		#endif
	}
	else if( sender == loadFileButton ){
		ofFileDialogResult res = ofSystemLoadDialog("Load audio file", false );
		if( res.bSuccess ){
			ofSendMessage("load:" + res.filePath); 
		}
	}
	else if( sender == useMicButton ){
		if( globals.micActive ){
			ofSendMessage("stop-mic");
		}
		else{
			if( micMenu != NULL ){
				MUI_ROOT->safeRemoveAndDelete(micMenu);
			}
			
			micMenu = new FMenu(0,0,400,0);
			mui::Button * cancelButton = micMenu->addButton("Cancel");
			cancelButton->bg = ofColor(100,100);
			vector<ofSoundDevice> infos = ofSoundStream().getDeviceList();
			for( int i = 0; i < infos.size(); i++ ){
				ofSoundDevice &info = infos[i];
				if( info.inputChannels >= 2 ){
					micMenu->addButton(info.name);
					micDeviceIds[info.name] = i;
				}
			}
			ofAddListener(micMenu->onPress, this, &PlayerOverlay::buttonPressed);
			micMenu->autoSize();
			micMenu->bg = ofColor(150);
			micMenu->opaque = true; 
			
			add(micMenu);
		}
	}
	else if( sender == timeLabelButton){
		timeLabelMode = (1+timeLabelMode)%2;
	}
	else if( micMenu != NULL && container->parent->parent == micMenu ){
		map<string,int>::iterator it = micDeviceIds.find(((mui::Button*)sender)->label->text);
		if( it != micDeviceIds.end() ){
			globals.micDeviceId = (*it).second;
			ofSendMessage("start-mic");
		}
		MUI_ROOT->safeRemoveAndDelete(micMenu);
		micMenu = NULL;
	}
}

void PlayerOverlay::sliderChanged( const void * sender, float & value ){
	if( sender == outputVolumeSlider->slider ){
		globals.outputVolume = outputVolumeSlider->slider->value;
	}
	else if( sender == scaleSlider->slider ){
		globals.scale = scaleSlider->slider->value;
	}
	else if( sender == strokeWeightSlider->slider ){
		globals.strokeWeight = strokeWeightSlider->slider->value;
	}
	else if( sender == timeStretchSlider->slider ){
		globals.timeStretch = timeStretchSlider->slider->value;
	}
	else if( sender == blurSlider->slider ){
		globals.blur = blurSlider->slider->value;
	}
	else if( sender == numPtsSlider->slider ){
		globals.numPts = numPtsSlider->slider->value;
	}
	else if( sender == hueSlider->slider ){
		globals.hue = hueSlider->slider->value;
	}
	else if( sender == intensitySlider->slider ){
		globals.intensity = intensitySlider->slider->value;
	}
	else if( sender == afterglowSlider->slider ){
		globals.afterglow = afterglowSlider->slider->value;
	}
}


mui::Label * PlayerOverlay::addLabel( string text ){
	mui::Label * label = new mui::Label( text, 0, 0, 100, 30 );
	label->commit();
	label->width = label->boundingBox.width;
	label->horizontalAlign = mui::Right;
	add( label );
	return label;
}


string timestring( double secs ){
	int64_t t = secs*1000;
	int millis = t%1000;
	t/=1000;
	
	int seconds = t%60;
	t/=60;
	
	int minutes = t%60;
	t/=60;
	
	stringstream str;
	str << setfill('0') << setw(2) << minutes <<
	":" << setfill('0') << setw(2) << seconds <<
	":" << setfill('0') << setw(2) << (millis/10);
	return str.str();
}
