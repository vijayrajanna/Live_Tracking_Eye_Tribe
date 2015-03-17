#include <iostream>
#include <string>
#include <gazeapi.h>
#include<stdlib.h>
#include<stdio.h>

#include <dos.h>
#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "graphics.h"

#define ESC     0x1b                    /* Define the escape key        */
#define TRUE    1                       /* Define some handy constants  */
#define FALSE   0                       /* Define some handy constants  */
#define PI      3.14159                 /* Define a value for PI        */
#define ON      1                       /* Define some handy constants  */
#define OFF     0                       /* Define some handy constants  */

#define NFONTS 11

char *Fonts[NFONTS] = {
	"DefaultFont", "TriplexFont", "SmallFont",
	"SansSerifFont", "GothicFont", "ScriptFont", "SimplexFont", "TriplexScriptFont",
	"ComplexFont", "EuropeanFont", "BoldFont"
};

char *LineStyles[] = {
	"SolidLn", "DottedLn", "CenterLn", "DashedLn", "UserBitLn"
};

char *FillStyles[] = {
	"EmptyFill", "SolidFill", "LineFill", "LtSlashFill",
	"SlashFill", "BkSlashFill", "LtBkSlashFill", "HatchFill",
	"XHatchFill", "InterleaveFill", "WideDotFill", "CloseDotFill"
};

char *TextDirect[] = {
	"HorizDir", "VertDir"
};

char *HorizJust[] = {
	"LeftText", "CenterText", "RightText"
};

char *VertJust[] = {
	"BottomText", "CenterText", "TopText"
};

struct PTS {
	int x, y;
};      /* Structure to hold vertex points      */

int    GraphDriver;             /* The Graphics device driver           */
int    GraphMode;               /* The Graphics mode value              */
double AspectRatio;             /* Aspect ratio of a pixel on the screen*/
int    MaxX, MaxY;              /* The maximum resolution of the screen */
int    MaxColors;               /* The maximum # of colors available    */
int    ErrorCode;               /* Reports any graphics errors          */
struct palettetype palette;             /* Used to read palette info    */


using namespace std;



// --- MyGaze definition
class MyGaze : public gtl::IGazeListener
{
public:
	MyGaze();
	~MyGaze();
private:
	// IGazeListener
	void on_gaze_data(gtl::GazeData const & gaze_data);
private:
	gtl::GazeApi m_api;
};

// --- MyGaze implementation
MyGaze::MyGaze()
{
	// Connect to the server in push mode on the default TCP port (6555)
	if (m_api.connect(true))
	{
		// Enable GazeData notifications
		m_api.add_listener(*this);
	}
	else
	{
		cout << "Server is not active";
	}
}

MyGaze::~MyGaze()
{
	m_api.remove_listener(*this);
	m_api.disconnect();
}

void MyGaze::on_gaze_data(gtl::GazeData const & gaze_data)
{
	if (gaze_data.state & gtl::GazeData::GD_STATE_TRACKING_GAZE)
	{
		gtl::Point2D const & smoothedCoordinatesLeftEye = gaze_data.lefteye.avg; // smoothed data from left eye
		gtl::Point2D const & smoothedCoordinatesRightEye = gaze_data.righteye.avg; // smoothed data from right eye

		float LeftEyeX = smoothedCoordinatesLeftEye.x;
		float LeftEyeY = smoothedCoordinatesLeftEye.y;

		float RightEyeX = smoothedCoordinatesRightEye.x;
		float RightEyeY = smoothedCoordinatesRightEye.y;

		// Move GUI point, do hit-testing, log coordinates, etc.
		cout << "x = " << (LeftEyeX + RightEyeX) / 2 << " y = " << (LeftEyeY + RightEyeY) / 2 << endl; //center values for left and right eyes, respectively.
		//m_GraphcsObject.DrawEllipse()

		circle((LeftEyeX + RightEyeX) / 2, (LeftEyeY + RightEyeY) / 2, gaze_data.lefteye.psize);
	}
}

void Initialize(void)
{
	int xasp, yasp;                       /* Used to read the aspect ratio*/

	GraphDriver = DETECT;                 /* Request auto-detection       */
	initgraph(&GraphDriver, &GraphMode, "");
	
	ErrorCode = graphresult();            /* Read result of initialization*/
	if (ErrorCode != grOk){              /* Error occured during init    */
		printf(" Graphics System Error: %s\n", grapherrormsg(ErrorCode));
		exit(1);
	}

	getpalette(&palette);               /* Read the palette from board  */
	MaxColors = getmaxcolor() + 1;        /* Read maximum number of colors*/

	MaxX = getmaxx();
	MaxY = getmaxy();                     /* Read size of screen          */

	getaspectratio(&xasp, &yasp);       /* read the hardware aspect     */
	AspectRatio = (double)xasp / (double)yasp; /* Get correction factor   */

}

void PaletteDemo(void)
{
	int i, j, x, y, color;
	struct viewporttype vp;
	int height, width;

	//MainWindow("Palette Demonstration");
	//StatusLine("Press any key to continue, ESC to Abort");

	getviewsettings(&vp);
	width = (vp.right - vp.left) / 15;   /* get width of the box         */
	height = (vp.bottom - vp.top) / 10;   /* Get the height of the box    */

	x = y = 0;                            /* Start in upper corner        */
	color = 1;                            /* Begin at 1st color           */

	for (j = 0; j<10; ++j){              /* For 10 rows of boxes         */
		for (i = 0; i<15; ++i){            /* For 15 columns of boxes      */
			setfillstyle(SOLID_FILL, color++);      /* Set the color of box */
			bar(x, y, x + width, y + height);           /* Draw the box         */
			x += width + 1;                           /* Advance to next col  */
			color = 1 + (color % (MaxColors - 2));    /* Set new color        */
		}                           /* End of COLUMN loop           */
		x = 0;                              /* Goto 1st column              */
		y += height + 1;                    /* Goto next row                */
	}                                     /* End of ROW loop              */

	while (!kbhit()){                    /* Until user enters a key...   */
		setpalette(1 + random(MaxColors - 2), random(65));
	}

	setallpalette(&palette);

	//Pause();                              /* Wait for user's response     */

}

int main()
{
	MyGaze gazeReceiver;
	Initialize();                 
	while (true)
	{
	
	}
	return	0;
}