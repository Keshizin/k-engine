// // ****************************************************************************
// //  GEModel Class Definition - Constructors and Destructors
// // ****************************************************************************
// GEEntity::GEEntity()
// {
// 	translate.x = 0.0f;
// 	translate.y = 0.0f;
// 	translate.z = 0.0f;

// 	rotate.angle = 0.0f;
// 	rotate.x = 0.0f;
// 	rotate.y = 0.0f;
// 	rotate.z = 0.0f;

// 	scale.x  = 1.0f;
// 	scale.y  = 1.0f;
// 	scale.z  = 1.0f;

// 	speed.x = 0.0f;
// 	speed.y = 0.0f;
// 	speed.z = 0.0f;

// 	bounding.left = 0.0f;
// 	bounding.right = 0.0f;
// 	bounding.top = 0.0f;
// 	bounding.bottom = 0.0f;
// }

// GEEntity::GEEntity(MODEL *modelParam)
// {
// 	this->model = modelParam;
	
// 	translate.x = 0.0f;
// 	translate.y = 0.0f;
// 	translate.z = 0.0f;

// 	rotate.angle = 0.0f;
// 	rotate.x = 0.0f;
// 	rotate.y = 0.0f;
// 	rotate.z = 0.0f;

// 	scale.x  = 1.0f;
// 	scale.y  = 1.0f;
// 	scale.z  = 1.0f;

// 	speed.x = 0.0f;
// 	speed.y = 0.0f;
// 	speed.z = 0.0f;

// 	bounding.left = 0.0f;
// 	bounding.right = 0.0f;
// 	bounding.top = 0.0f;
// 	bounding.bottom = 0.0f;
// }

// GEEntity::~GEEntity()
// {
// 	// releaseFromMemory();
// 	delete model->vertices;
// 	delete model->colors;
// 	delete model->indices;
// 	delete model;
// }

// void GEEntity::update(double frameTime)
// {
// 	glPushMatrix();
// 	glMatrixMode(GL_MODELVIEW);
// 	glLoadIdentity();

// 	translate.x += speed.x * frameTime;

// 	if(translate.x - 0.5f > bounding.right)
// 		translate.x = bounding.left - 0.5f;

// 	rotate.angle += 500 * speed.x * frameTime;

// 	if(rotate.angle > 360.0f)
// 		rotate.angle = 0.0f;

// 	glTranslatef(translate.x, translate.y, translate.z);
// 	glRotatef(rotate.angle, rotate.x, rotate.y, rotate.z);
// 	glScalef(scale.x, scale.y, scale.z);
// }

// void GEEntity::draw()
// {
// 	// This code will be replaced for VAO, VBO, vertex/fragment shader techniques!
// 	glBegin(GL_TRIANGLES);

// 	for(int i = 0; i < model->total_indices; i++)
// 	{
// 		glColor3f(
// 			model->colors[model->indices[i]].r,
// 			model->colors[model->indices[i]].g,
// 			model->colors[model->indices[i]].b);

// 		glVertex3f(
// 			model->vertices[model->indices[i]].x,
// 			model->vertices[model->indices[i]].y,
// 			model->vertices[model->indices[i]].z);
// 	}

// 	glEnd();

// 	glPopMatrix();

// 	// ------------------------------------------------------------------------
// 	// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
// 	// glDrawArrays(GL_TRIANGLES, 0, 6);
// }

// void GEEntity::setTranslate(float x, float y, float z)
// {
// 	translate.x = x;
// 	translate.y = y;
// 	translate.z = z;
// }

// void GEEntity::setRotate(float angle, float x, float y, float z)
// {
// 	rotate.angle = angle;
// 	rotate.x = x;
// 	rotate.y = y;
// 	rotate.z = z;
// }

// void GEEntity::setScale(float x, float y, float z)
// {
// 	scale.x = x;
// 	scale.y = y;
// 	scale.z = z;
// }

// void GEEntity::setSpeed(float x, float y, float z)
// {
// 	speed.x = x;
// 	speed.y = y;
// 	speed.z = z;
// }

// void GEEntity::setBounding(double left, double right, double top, double bottom)
// {
// 	bounding.left = left;
// 	bounding.right = right;
// 	bounding.top = top;
// 	bounding.bottom = bottom;
// }

// // void GEEntity::loadToMemory()
// // {
// 	// GLuint vao;
// 	// GLuint bo[3] = {0};

// 	// glGenVertexArrays(1, &vao);
// 	// glBindVertexArray(vao);

// 	// if(model->vertices)
// 	// {
// 	// 	glGenBuffers(1, &bo[0]);
// 	// 	glBindBuffer(GL_ARRAY_BUFFER, bo[0]);
// 	// 	glBufferData(GL_ARRAY_BUFFER, model->total_vertex * sizeof(VERTEX), model->vertices, GL_STATIC_DRAW);

// 	// 	glEnableClientState(GL_VERTEX_ARRAY); // (remove)
// 	// 	glVertexPointer(3, GL_FLOAT, 0, 0); // (remove)
// 	// }

// 	// if(model->colors)
// 	// {
// 	// 	glGenBuffers(1, &bo[1]);
// 	// 	glBindBuffer(GL_ARRAY_BUFFER, bo[1]);
// 	// 	glBufferData(GL_ARRAY_BUFFER, model->total_vertex * sizeof(COLOR), model->colors, GL_STATIC_DRAW);

// 	// 	glEnableClientState(GL_COLOR_ARRAY); // (remove)
// 	// 	glColorPointer(3, GL_FLOAT, 0, 0); // (remove)
// 	// }

// 	// if(model->indices)
// 	// {
// 	// 	glGenBuffers(1, &bo[2]);
// 	// 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bo[2]);
// 	// 	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->total_indices * sizeof(unsigned int), model->indices, GL_STATIC_DRAW);
// 	// }
// // }

// // void GEEntity::releaseFromMemory()
// // {
// // }

// ****************************************************************************
//  Getters and Setters
// ****************************************************************************

// GERECT GERenderingSystem::getRenderingWindow()
// {
// 	GERECT window;

// 	if(windowAspectCorrectionState)
// 	{
// 		if(viewportWidth <= viewportHeight)
// 		{
// 			windowAspectCorrection = static_cast<GLdouble>(viewportHeight) / static_cast<double>(viewportWidth);
// 			window.bottom *= windowAspectCorrection;
// 			window.top *= windowAspectCorrection;
// 		}
// 		else
// 		{
// 			windowAspectCorrection = static_cast<GLdouble>(viewportWidth) / static_cast<double>(viewportHeight);

// 			window.left *= windowAspectCorrection;
// 			window.right *= windowAspectCorrection;
// 		}
// 	}

// 	window.left += renderingWindowOffsetX + zoom;
// 	window.right += renderingWindowOffsetX - zoom;
// 	window.top += renderingWindowOffsetX - zoom;
// 	window.bottom += renderingWindowOffsetX + zoom;

// 	return window;
// }