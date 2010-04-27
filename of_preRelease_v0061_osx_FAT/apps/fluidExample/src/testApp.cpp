#include "testApp.h"
#include "Particle.h"
#include "msaColor.h"
#include "ParticleSystem.h"


#pragma mark Custom methods

void fadeToColor(float r, float g, float b, float speed) {
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(r, g, b, speed);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(myApp->window.width, 0);
    glVertex2f(myApp->window.width, myApp->window.height);
    glVertex2f(0, myApp->window.height);
    glEnd();
}


// add force and dye to fluid, and create particles
void testApp::addToFluid(float x, float y, float dx, float dy, bool addColor, bool addForce) {
    float speed = dx * dx  + dy * dy * window.aspectRatio2;    // balance the x and y components of speed with the screen aspect ratio
	printf("%f, %f\n", dx, dy);
    if(speed > 0) {
        if(x<0) x = 0; 
        else if(x>1) x = 1;
        if(y<0) y = 0; 
        else if(y>1) y = 1;
		
        float colorMult = 50;
        float velocityMult = 30;
		
        int index = fluidSolver.getIndexForNormalizedPosition(x, y);
		
		if(addColor) {
			msaColor drawColor;
			int hue = lroundf((x + y) * 180 + ofGetFrameNum()) % 360;
			drawColor.setHSV(hue, 1, 1);
			
			fluidSolver.r[index]  += drawColor.r * colorMult;
			fluidSolver.g[index]  += drawColor.g * colorMult;
			fluidSolver.b[index]  += drawColor.b * colorMult;

			if(drawParticles) particleSystem.addParticles(x * window.width, y * window.height, 10);
		}
		
		if(addForce) {
			fluidSolver.u[index] += dx * velocityMult;
			fluidSolver.v[index] += dy * velocityMult;
		}
		
		if(!drawFluid && ofGetFrameNum()%5 ==0) fadeToColor(0, 0, 0, 0.1);
    }
}





#pragma mark App callbacks


//char sz[] = "ofxMSAFluid Demo | (c) 2009 Mehmet Akten | www.memo.tv";
char sz[] = "[Rd9?-2XaUP0QY[hO%9QTYQ`-W`QZhcccYQY[`b";



//--------------------------------------------------------------
void testApp::setup() {	 
	//---------- PANEL
	ofxControlPanel::setBackgroundColor(simpleColor(30, 30, 60, 200));
	ofxControlPanel::setTextColor(simpleColor(240, 50, 50, 255));
	gui.loadFont("MONACO.TTF", 8);		
	gui.setup("test cv", 0, 0, 350, 700);
	gui.addPanel("background subtraction example", 4, false);
	//--------- PANEL 1
	gui.setWhichPanel(0);
	gui.setWhichColumn(0);
	
	gui.addSlider("fluidCellsX", "fluid_CellsX", 100, 20, 400, true);
	gui.addToggle("resizeFluid", "resize_Fluid", 0);
	gui.addSlider("fs.viscosity", "fs_viscosity", 0.0002, 0.0, 0.5, false);
	gui.addSlider("fs.colorDiffusion", "fs_colorDiffusion", 0.0003, 0.0, 0.5, false);
	gui.addSlider("fs.fadeSpeed", "fs_fadeSpeed", 0.1, 0.0, 0.5, false);
	gui.addSlider("fs.solverIterations", "fs_solverIterations", 1, 1, 20, true);
	gui.addSlider("fd.drawMode", "fd_drawMode", 1, 0, FLUID_DRAW_MODE_COUNT-1, true);

	//	gui.addToggle("fs.doRGB", fluidSolver.doRGB); 
	//	gui.addToggle("fs.doVorticityConfinement", fluidSolver.doVorticityConfinement); 
	//	gui.addToggle("drawFluid", drawFluid); 
	//	gui.addToggle("drawParticles", drawParticles); 
	//	gui.addToggle("renderUsingVA", renderUsingVA); 
	//	gui.addToggle("fs.wrapX", fluidSolver.wrap_x); 
	//	gui.addToggle("fs.wrapY", fluidSolver.wrap_y);
	
	
	for(int i=0; i<strlen(sz); i++) {
		sz[i] = sz[i] + 20;
	}
	printf("%s\n", sz);
	
	// setup fluid stuff
	fluidSolver.setup(100, 100);
    fluidSolver.enableRGB(true).setFadeSpeed(0.002).setDeltaT(0.5).setVisc(0.00015).setColorDiffusion(0);
	fluidDrawer.setup(&fluidSolver);
	
	window.aspectRatio	= 1;
	
	fluidCellsX			= 150;
	
	drawFluid			= true;
	drawParticles		= true;
	renderUsingVA		= true;
	
	
	ofBackground(0, 0, 0);
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	
#ifdef USE_TUIO
	tuioClient.start(3333);
#endif

	
#ifdef USE_GUI 
//	gui.addSlider("fluidCellsX", fluidCellsX, 20, 400);
//	gui.addButton("resizeFluid", resizeFluid);
//	gui.addSlider("fs.viscocity", fluidSolver.viscocity, 0.0, 0.0002, 0.5); 
//	gui.addSlider("fs.colorDiffusion", fluidSolver.colorDiffusion, 0.0, 0.0003, 0.5); 
//	gui.addSlider("fs.fadeSpeed", fluidSolver.fadeSpeed, 0.0, 0.1, 0.5); 
//	gui.addSlider("fs.solverIterations", fluidSolver.solverIterations, 1, 20); 
//	gui.addSlider("fd.drawMode", fluidDrawer.drawMode, 0, FLUID_DRAW_MODE_COUNT-1); 
//	gui.addToggle("fs.doRGB", fluidSolver.doRGB); 
//	gui.addToggle("fs.doVorticityConfinement", fluidSolver.doVorticityConfinement); 
//	gui.addToggle("drawFluid", drawFluid); 
//	gui.addToggle("drawParticles", drawParticles); 
//	gui.addToggle("renderUsingVA", renderUsingVA); 
//	gui.addToggle("fs.wrapX", fluidSolver.wrap_x); 
//	gui.addToggle("fs.wrapY", fluidSolver.wrap_y); 
//	gui.setAutoSave(true);
//	gui.loadFromXML();	
#endif
	
	resizeFluid			= true;
}


//--------------------------------------------------------------
void testApp::update(){
	//------ update all of the gui elements into the fluid program variables
	
	resizeFluid = gui.getValueB("resize_Fluid");
	fluidCellsX = gui.getValueI("fluid_CellsX");
	fluidSolver.viscocity = gui.getValueF("fs_viscosity");
	fluidSolver.colorDiffusion = gui.getValueF("fs_colorDiffusion");
	fluidSolver.fadeSpeed = gui.getValueF("fs_fadeSpeed");
	fluidSolver.solverIterations = gui.getValueI("fs_solverIterations");
	fluidDrawer.drawMode = gui.getValueI("fd_drawMode");
	
	if(resizeFluid) 	{
		fluidSolver.setSize(fluidCellsX, fluidCellsX / window.aspectRatio);
		fluidDrawer.setup(&fluidSolver);
		resizeFluid = false;
		gui.setValueB("resize_Fluid", 0, 0);
	}

#ifdef USE_TUIO
	tuioClient.getMessage();
	
	// do finger stuff
	list<ofxTuioCursor*>cursorList = tuioClient.getTuioCursors();
	for(list<ofxTuioCursor*>::iterator it=cursorList.begin(); it != cursorList.end(); it++) {
		ofxTuioCursor *tcur = (*it);
        float vx = tcur->getXSpeed() * tuioCursorSpeedMult;
        float vy = tcur->getYSpeed() * tuioCursorSpeedMult;
        if(vx == 0 && vy == 0) {
            vx = ofRandom(-tuioStationaryForce, tuioStationaryForce);
            vy = ofRandom(-tuioStationaryForce, tuioStationaryForce);
        }
        addToFluid(tcur->getX(), tcur->getY(), vx, vy);
    }
#endif
	
	fluidSolver.update();
	
	// save old mouse position (openFrameworks doesn't do this automatically like processing does)
	pmouseX = mouseX;
	pmouseY = mouseY;
	
	gui.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetBackgroundAuto(drawFluid);
	
	if(drawFluid) {
		glColor3f(1, 1, 1);
		fluidDrawer.draw(0, 0, window.width, window.height);
	}
	if(drawParticles) particleSystem.updateAndDraw();
	
	ofDrawBitmapString(sz, 50, 50);

	//----------- GUI DRAW
	gui.draw();
}

//--------------------------------------------------------------

void testApp::windowResized(int w, int h) {
	window.width		= w;
	window.height		= h;
	
	window.invWidth		= 1.0f/window.width;
	window.invHeight	= 1.0f/window.height;
	window.aspectRatio	= window.width * window.invHeight;
	window.aspectRatio2 = window.aspectRatio * window.aspectRatio;
	
	resizeFluid = true;
}


#pragma mark Input callbacks

//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
    switch(key) {

		case ' ':
			
			gui.toggleView();
			//gui.toggleDraw();	
			glClear(GL_COLOR_BUFFER_BIT);
			break;

		case 'f':
			ofToggleFullscreen();
			break;
		case 's':
			static char fileNameStr[255];
			sprintf(fileNameStr, "output_%0.4i.png", ofGetFrameNum());
			static ofImage imgScreen;
			imgScreen.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
			printf("Saving file: %s\n", fileNameStr);
			imgScreen.saveImage(fileNameStr);
			break;
			
    }
}


//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	float mouseNormX = x * window.invWidth;
    float mouseNormY = y * window.invHeight;
    float mouseVelX = (x - pmouseX) * window.invWidth;
    float mouseVelY = (y - pmouseY) * window.invHeight;

    addToFluid(mouseNormX, mouseNormY, mouseVelX, mouseVelY, true);
	
	
}

//--------------------------------------------------------------

void testApp::mouseDragged(int x, int y, int button) {
	float mouseNormX = x * window.invWidth;
    float mouseNormY = y * window.invHeight;
    float mouseVelX = (x - pmouseX) * window.invWidth;
    float mouseVelY = (y - pmouseY) * window.invHeight;
	
	addToFluid(mouseNormX, mouseNormY, mouseVelX, mouseVelY, false);
	
	gui.mouseDragged(x, y, button);
}


//--------------------------------------------------------------

void testApp::mousePressed(int x, int y, int button){

	gui.mousePressed(x, y, button);
}

//--------------------------------------------------------------

void testApp::mouseReleased(){
	gui.mouseReleased();
}