#include "Scene.h"


MyScene::MyScene()
{
	cout << "Scene Constructor Called." << endl;
}


MyScene::~MyScene()
{
	cout << "Scene Destructor Called." << endl;
}

void MyScene::initialize()
{
	checkOpenGLErrors("MyScene::initialize0");

	// Initialize OpenGL 
	glEnable(GL_DEPTH_TEST); // Turn depth testing
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f); // Set the window clear color

	// Build shader proram
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "vertexShaderPerPixel.vs.glsl" },
		{ GL_FRAGMENT_SHADER, "fragmentShaderPerPixel.fs.glsl" },
		{ GL_NONE, NULL } // signals that there are no more shaders 
	};

	perPixelShaderProgram = BuildShaderProgram(shaders);
	checkOpenGLErrors("MyScene::initialize1");

	// Set up the uniform blocks for this shader
	SharedProjectionAndViewing::setUniformBlockForShader(perPixelShaderProgram);
	SharedMaterialProperties::setUniformBlockForShader(perPixelShaderProgram);
	SharedGeneralLighting::setUniformBlockForShader(perPixelShaderProgram);

	sun = new Sphere(1, 32, 32);
	sun->material.setAmbientAndDiffuseMat(glm::vec4(1.0f, 1.0f, 0.1f, 1.0f));
	sun->material.setupTexture("preview_sun.jpg", DECAL);
	sun->setShader(perPixelShaderProgram);
	addChild(sun);

	earth = new Sphere(0.5);
	earth->material.setAmbientAndDiffuseMat(glm::vec4(0.0f, 0.5f, 0.0f, 1.0f));
	earth->material.setupTexture("earth.bmp", DECAL);
	earth->setShader(perPixelShaderProgram);
	addChild(earth);

	ship = new Model("Space/dark_fighter_6.obj");
	ship->setModelTransformation( glm::scale(glm::vec3(0.25f, 0.25f, 0.25f) ));
	ship->setShader(perPixelShaderProgram);
	addChild(ship);

	cube = new Cube();
	cube->material.setAmbientAndDiffuseMat(glm::vec4(0.1f, 0.1f, 1.0f, 1.0f));
	cube->material.setupTexture("Brick.bmp", DECAL);
	cube->setShader(perPixelShaderProgram);
	addChild(cube);

	nanoSuitModel = new Model("model/nanosuit.obj");
	nanoSuitModel->material.setAmbientAndDiffuseMat(glm::vec4(0.1f, 1.0f, .1f, 0.9f));
	nanoSuitModel->material.setTextureMode(DECAL);
	nanoSuitModel->setShader(perPixelShaderProgram);
	addChild(nanoSuitModel);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	cube2 = new Cube();
	cube2->material.setAmbientAndDiffuseMat(glm::vec4(0.1f, 1.0f, 1.0f, 0.3f));
	cube2->setShader(perPixelShaderProgram);
	addChild(cube2);

	VisibleObject::initialize();

	//SharedProjectionAndViewing::setViewMatrix(glm::translate(glm::vec3(0.0f, 0.0f, -52.0f)));
	SharedProjectionAndViewing::setViewMatrix(glm::translate(glm::vec3(0.0f, 0.0f, -50.0f))  * glm::rotate(M_PI/2.0f, glm::vec3(1.0f, 0.0f, 0.0f)));
	checkOpenGLErrors("MyScene::initialize3");

	// Light 1
	glm::vec4 light1AmbColor(0.15f, 0.15f, 0.15f, 1.0f);
	glm::vec4 light1DifColor(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 light1SpecColor(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 light1PositionOfDirection(5.0f, 5.0f, 5.0f, 1.0f);
	bool light1IsSpot = false;
	bool light1Enabled = true;

	SharedGeneralLighting::setAmbientColor(GL_LIGHT_ZERO, light1AmbColor);
	SharedGeneralLighting::setDiffuseColor(GL_LIGHT_ZERO, light1DifColor);
	SharedGeneralLighting::setSpecularColor(GL_LIGHT_ZERO, light1SpecColor);
	SharedGeneralLighting::setPositionOrDirection(GL_LIGHT_ZERO, light1PositionOfDirection);
	SharedGeneralLighting::setIsSpot(GL_LIGHT_ZERO, light1IsSpot);
	SharedGeneralLighting::setEnabled(GL_LIGHT_ZERO, light1Enabled);

	checkOpenGLErrors("MyScene::initialize4");
	// Light 2
	glm::vec4 light2AmbColor(0.15f, 0.15f, 0.15, 1.0f);
	glm::vec4 light2DifColor(0.75f, 0.75f, 0.75f, 1.0f);
	glm::vec4 light2SpecColor(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 light2PositionOfDirection(1.0f, 1.0f, 1.0f, 0.0f);
	bool light2IsSpot = false;
	bool light2Enabled = true;

	SharedGeneralLighting::setAmbientColor(GL_LIGHT_ONE, light2AmbColor);
	SharedGeneralLighting::setDiffuseColor(GL_LIGHT_ONE, light2DifColor);
	SharedGeneralLighting::setSpecularColor(GL_LIGHT_ONE, light2SpecColor);
	SharedGeneralLighting::setPositionOrDirection(GL_LIGHT_ONE, light2PositionOfDirection);
	SharedGeneralLighting::setIsSpot(GL_LIGHT_ONE, light2IsSpot);
	SharedGeneralLighting::setEnabled(GL_LIGHT_ONE, light2Enabled);

	checkOpenGLErrors("MyScene::initialize5");
	// Light 3
	glm::vec4 light3AmbColor(0.15f, 0.15f, 0.15, 1.0f);
	glm::vec4 light3DifColor(0.9f, 0.9f, 0.9f, 1.0f);
	glm::vec4 light3SpecColor(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 light3PositionOfDirection(0.0f, 0.0f, 12.0f, 1.0f);
	glm::vec3 light3SpotDirection(0.0f, 0.0f, -1.0f);
	bool light3IsSpot = true;
	GLfloat light3SpotCutOff = glm::cos(glm::radians(10.0f));
	GLfloat light3SpotExponent = 0.9f;
	bool light3Enabled = true;

	SharedGeneralLighting::setAmbientColor(GL_LIGHT_TWO, light3AmbColor);
	SharedGeneralLighting::setDiffuseColor(GL_LIGHT_TWO, light3DifColor);
	SharedGeneralLighting::setSpecularColor(GL_LIGHT_TWO, light3SpecColor);
	SharedGeneralLighting::setPositionOrDirection(GL_LIGHT_TWO, light3PositionOfDirection);
	SharedGeneralLighting::setSpotDirection(GL_LIGHT_TWO, light3SpotDirection);
	SharedGeneralLighting::setIsSpot(GL_LIGHT_TWO, light3IsSpot);
	SharedGeneralLighting::setSpotCutoffCos(GL_LIGHT_TWO, light3SpotCutOff);
	SharedGeneralLighting::setSpotExponent(GL_LIGHT_TWO, light3SpotExponent);
	SharedGeneralLighting::setEnabled(GL_LIGHT_TWO, light3Enabled);

	checkOpenGLErrors("MyScene::initialize6");

}


void MyScene::resize(int windowWidth, int windowHeight)
{
	glm::mat4 projectionTransformation = glm::perspective(M_PI / 4.0f, (GLfloat)windowWidth / windowHeight, 1.0f, 500.0f);

	SharedProjectionAndViewing::setProjectionMatrix(projectionTransformation);
}


void MyScene::toggleLights(int light)
{
	switch (light)
	{
		case 0:
			SharedGeneralLighting::setEnabled(GL_LIGHT_ZERO, !SharedGeneralLighting::getEnabled(GL_LIGHT_ZERO));
			cout << "toggle lights" << endl;
			break;
		case 1:
			SharedGeneralLighting::setEnabled(GL_LIGHT_ONE, !SharedGeneralLighting::getEnabled(GL_LIGHT_ONE));
			cout << "toggle lights" << endl;
			break;
		case 2:
			SharedGeneralLighting::setEnabled(GL_LIGHT_TWO, !SharedGeneralLighting::getEnabled(GL_LIGHT_TWO));
			cout << "toggle lights" << endl;
			break;
		default:
			cout << "Illegal Light" << endl;
			break;
	}
}


void MyScene::setTextureMode(int mode)
{
	switch (mode)
	{
	case 0:

		VisibleObject::setTextureMode(NO_TEXTURE);

		cout << "No Texture" << endl;
		break;
	case 1:

		VisibleObject::setTextureMode(DECAL);

		cout << "Decal" << endl;
		break;
	case 2:

		VisibleObject::setTextureMode(REPLACE_AMBIENT_DIFFUSE);

		cout << "Replace ambient and diffuse" << endl;
		break;

	default:
		cout << "Illegal shader" << endl;
		break;
	}
}


bool MyScene::update(float deltaTime)
{
	// Update the modeling transformations of the objects in the scene
	//sun stuff
	static float sunAngle = 0.0f;
	sunAngle += glm::radians(20.0) * deltaTime;
	glm::mat4 sunMod = glm::rotate(sunAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	sunMod *= glm::rotate(M_PI / 2, glm::vec3(1.0f, 0.0f, 0.0f)); // rotate so it is upright
	sun->setModelTransformation(sunMod);

	//Earth stuff (rotate->translate->rotate)
	static float earthOrbitAngle = 0.0f;
	earthOrbitAngle += glm::radians(40.0) * deltaTime;
	static float earthSpinAngle = 0.0f;
	earthSpinAngle += glm::radians(70.0) * deltaTime;
	glm::mat4 earthMod = glm::rotate(earthOrbitAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	earthMod *= glm::translate(glm::vec3(20.0f, 0.0f, 0.0f));
	earthMod *= glm::rotate(earthOrbitAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	earthMod *= glm::rotate(-M_PI / 2, glm::vec3(1.0f, 0.0f, 0.0f));
	earth->setModelTransformation(earthMod);

	//ship stuff
	static float shipOrbitAngle = -0.1f;
	shipOrbitAngle += glm::radians(40.0) * deltaTime;
	glm::mat4 shipMod = glm::rotate(shipOrbitAngle, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::translate(glm::vec3(20.0f, 0.0f, 0.0f)) * glm::rotate(-M_PI/2.0f, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(glm::vec3(.012f, .012f, .012f));
	ship->setModelTransformation(shipMod);
	
	//cube stuff
	glm::mat4 cubeMod = glm::rotate(earthOrbitAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	cubeMod *= glm::translate(glm::vec3(20.0f, 0.0f, 0.0f));
	cubeMod *= glm::rotate(earthOrbitAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	cubeMod *= glm::rotate(-M_PI / 2, glm::vec3(1.0f, 0.0f, 0.0f));
	cubeMod *= glm::translate(glm::vec3(0.5f, 0.0f, 0.0f));
	cubeMod *= glm::scale(glm::vec3(.2f, .2f, .2f));
	cube->setModelTransformation(cubeMod);

	//cube2 stuff
	glm::mat4 cube2mod = glm::translate(glm::vec3(20.0f, 0.0f, 0.0f));
	cube2mod *= glm::scale(glm::vec3(3.0f, 3.0f, 3.0f));
	cube2->setModelTransformation(cube2mod);
	
	//nanosuit stuff
	glm::mat4 suitMod = glm::translate(glm::vec3(20.0f, 0.0f, 0.0f)) * glm::scale(glm::vec3(.15, .15, .15));
	nanoSuitModel->setModelTransformation(suitMod);

	//SCALE IS ALWAYS FURTHEST TO THE RIGHT

	checkOpenGLErrors("MyScene::update");

	VisibleObject::update(deltaTime);

	return true;

}

void MyScene::draw() {
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	glm::mat4 projectionTransformation;
	switch (proj) {
	case(0) :
		//ortho
		glViewport(0, 0, width, height);
		projectionTransformation = glm::ortho((GLfloat)-width / 60, (GLfloat)width / 60, (GLfloat)-height / 60, (GLfloat)height / 60, 1.0f, 100.0f);
		SharedProjectionAndViewing::setProjectionMatrix(projectionTransformation);
		break;
	case(1) :
		//perspective
		glViewport(0, 0, width, height);
		projectionTransformation = glm::perspective(M_PI / 6.0f, (GLfloat)width / height , 1.0f, 100.0f);
		SharedProjectionAndViewing::setProjectionMatrix(projectionTransformation);
		break;
	case(2) :
		//splitting
		projectionTransformation = glm::ortho((GLfloat)-width / 60, (GLfloat)width / 60, (GLfloat)-height / 120, (GLfloat)height / 120, 1.0f, 100.0f);
		SharedProjectionAndViewing::setProjectionMatrix(projectionTransformation);
		glViewport(0, height / 2, width, height / 2);
		VisibleObject::draw();

		glm::mat4 projectionSplit = glm::perspective(M_PI / 4.0f, (GLfloat)width / (height / 2), 1.0f, 500.0f);
		SharedProjectionAndViewing::setProjectionMatrix(projectionSplit);
		glViewport(0, 0, width, height / 2);
		break;
	}
	VisibleObject::draw();

}

void MyScene::setProj(int i) {
	proj = i;
}



