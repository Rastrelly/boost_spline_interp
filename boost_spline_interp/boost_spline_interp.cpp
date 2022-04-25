#include <iostream>
#include <vector>
#include <random>
#include <ctime>

#include <GL/freeglut.h>

#include <boost/math/interpolators/cubic_b_spline.hpp>
#include <boost/math/interpolators/cardinal_cubic_b_spline.hpp>


// Масив вхідних даних:
std::vector<double> v = {};
std::vector<double> s = {};

// Крок даних:
double step = 10;

double ymin = 0; double ymax = 0;
double xmin = 0; double xmax = 0;


void initGLUTstuff(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(10, 10);
	glutInitWindowSize(800, 800);
	glutCreateWindow("BOOST Uniform Cubic Cardinal Spline");
	glClearColor(0,0,0,1);

	glMatrixMode(GL_PROJECTION);
	glOrtho(-1,1,-1,1,-1,1);
	glMatrixMode(GL_MODELVIEW);
}

double carthtoogl(double coord, double cmin, double cc)
{
	return ((coord - cmin)*2 / cc) - 1;
}

void drawglcross(double x, double y, double size)
{
	
	glLineWidth(3);
	
	glBegin(GL_LINES);

	glColor3d(1, 0, 0);

	glVertex2d(x - size / 2, y - size / 2);
	glVertex2d(x + size / 2, y + size / 2);

	glVertex2d(x + size / 2, y - size / 2);
	glVertex2d(x - size / 2, y + size / 2);

	glEnd();
}

void cb_render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//перерахунок системи координат
	double w = xmax - xmin;
	double h = ymax - ymin;

	double kx = w; double ky = h;

	glLineWidth(1);
	glBegin(GL_LINE_LOOP);

	glColor3d(1, 1, 1);
	glVertex2d(-1, -1);
	glVertex2d( 1, -1);
	glVertex2d( 1,  1);
	glVertex2d(-1,  1);

	glEnd();

	glLineWidth(2);

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < (v.size()) * 10; i++)
	{
		double cx = carthtoogl(i, xmin, kx);
		double cy = carthtoogl(s[i], ymin, ky);
		printf("On render: %f, %f\n",cx,cy);
		glColor3d(0,0.6,0);
		glVertex2d(cx, cy);
	}
	glEnd();

	for (int i = 0; i < v.size(); i++)
	{
		drawglcross(carthtoogl(i*10, xmin, kx), carthtoogl(v[i], ymin, ky), 0.03);
		printf("Cross %d: %f, %f\n", i, carthtoogl(i * 10, xmin, kx), carthtoogl(v[i], ymin, ky));
	}

	glutSwapBuffers();
}

int main(int argc, char **argv)
{

	srand(time(NULL));

	// Генерація набору вхідних даних:
	for (int i = 0; i < 12; i++)
	{
		v.push_back(rand() % 151 - 75);
		printf("%d) %f\n", i, v[i]);
	}

	system("pause");

	boost::math::cubic_b_spline<double> r_point_spl(v.data(), v.size(), 0, step);
	s.clear();

	for (int i = 0; i < (v.size()) * 10; i++)
	{
		double csp = r_point_spl(i);
		s.push_back(csp);

		if (i == 0)
		{
			xmin = 0; xmax = (v.size()-1)*10;
			ymin = csp; ymax = csp;
		}
		if (csp < ymin) ymin = csp;
		if (csp > ymax) ymax = csp;

		printf("X %d    Y %f\n",i, csp);
	}

	system("pause");

	initGLUTstuff(argc, argv);
	
	glutDisplayFunc(cb_render);

	glutMainLoop();
	
	return 0;
}