#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_image.h"
#include "background.h"
#include "world.h"
#include "solid.h"
#include "controllable.h"
#include "physics.h"

#include <map>
#include <iostream>
using namespace std;

bool init(int width, int height);

const int SCREEN_WIDTH = 1024, SCREEN_HEIGHT= 768;
const int FPS = 100;

int main( int argc, char* args[] )
{
	if (init(SCREEN_WIDTH, SCREEN_HEIGHT) == false)
		return 1;
	
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();

	Physics physics;
	
	Drawable::setScreenSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	Controllable robot(202,100);

	vector<Drawable*> drawables;
	
	drawables.push_back(new Background("media/bakgrunn_PH.png", 1.0, 1.0));
	drawables.push_back(new World("data/world.txt"));
	drawables.push_back(&robot);

	for (unsigned int i = 0; i < drawables.size(); ++i)
	{
		drawables[i]->generateList();
	}
	
	clock_t start = clock();
	bool done = false;
	SDL_Event action;
	while( !done )
	{
		while( SDL_PollEvent(&action) )
		{
			if( action.type == SDL_QUIT )
				done = true;
			else if( action.type == SDL_KEYDOWN )
			{
				if( action.key.keysym.sym == SDLK_ESCAPE )
					done = true;
				if( robot.keyBind.find( action.key.keysym.sym ) != robot.keyBind.end() )
					robot.key[ robot.keyBind[ action.key.keysym.sym ] ] = true;
			}
			else if( action.type == SDL_KEYUP )
			{
				if( robot.keyBind.find( action.key.keysym.sym ) != robot.keyBind.end() )
					robot.key[ robot.keyBind[ action.key.keysym.sym ] ] = false;
			}
		}
		
		physics.calculateNewPositions(&drawables);

		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glLoadIdentity();
		glTranslated( SCREEN_WIDTH/2.0-(robot.position.x + robot.hitboxSize.x/2), SCREEN_HEIGHT/2.0-(robot.position.y + robot.hitboxSize.y/2), 0 );

		for (unsigned int i = 0; i < drawables.size(); ++i)
		{
			drawables[i]->draw(Vector2D(), Vector2D(1.0,1.0));
		}

		Drawable::showAllElements();

		SDL_GL_SwapBuffers();

		if (clock() - start < 1000/FPS)
			SDL_Delay(1000/FPS - clock() + start);
		
		start = clock();
	}
	return 0;
}

bool init(int width, int height)
{
	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
	{
		return false;
	}
    
	if( SDL_SetVideoMode( width, height, 32, SDL_OPENGL ) == NULL )
	{
		return false;
	}
	//if (loadTexture(texfiles, texture) == false)
	//{
	//	return false;
	//}

    glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glShadeModel( GL_SMOOTH );
	glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);							// The Type Of Depth Testing To Do
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    
	//vi vil ikke rendere for mye, dette fjerner ting bak "kamera"
	//glCullFace( GL_BACK );
	//glFrontFace( GL_CCW );
	//glEnable( GL_CULL_FACE );
	

	glViewport( 0, 0, width, height );
    
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0, width, height, 0, -1, 1 );
    
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
    
	if( glGetError() != GL_NO_ERROR )
	{
		return false;    
	}
    
	SDL_WM_SetCaption("Tank and Spank", NULL);

	return true;
}