#include<vgl.h>
#include"MyColorCylinder.h"
#define spacebar 32

float time=0;
int polygonNum=10;
int stopflag = 0;
MyColorCylinder cylinder;

void myInit()
{
	cylinder.initCylinder(polygonNum);
}
void myDisplay() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	cylinder.draw(time);

	glutSwapBuffers();
}
void myIdle()
{
	if (stopflag == 1)
	{
		time += 0.033;
		Sleep(20);
	}
	glutPostRedisplay();
}

void myKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '1':
		if (polygonNum > 3) {
			polygonNum -= 1;
			cylinder.redraw(polygonNum);
		}
		printf("\nPolygon: %d, Num.of Triangles: %d, Num. of Vertices: %d", polygonNum, polygonNum * 4, polygonNum * 12);
		break;
	case '2':
		polygonNum+=1;
		cylinder.redraw(polygonNum);
		printf("\nPolygon: %d, Num.of Triangles: %d, Num. of Vertices: %d", polygonNum, polygonNum * 4, polygonNum * 12);
		break;
	case spacebar:
		if (stopflag == 0)
			stopflag = 1;
		else
			stopflag = 0;
		break;
	case 'q':case 'Q':
		exit(1);
		break;
	default:
		break;
	}
}
int main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutCreateWindow("16011087 ÀÌ¼öºó CG2 homework1");

	printf("A Rotating Color Cylinder\nProgramming Assignment #1 for Computer Graphics 2\n");
	printf("Department of Digital Contents, Sejong University\n\n");
	printf("----------------------------------------------------------------\n");
	printf("Spacebar: starting/stoping rotation\n");
	printf("'1' key: Decreasing the Number of Edges\n'2' key: Increasing the Number of Edges\n");
	printf("\n'Q' Key: Exit the program.\n");
	printf("----------------------------------------------------------------\n");
	printf("\nPolygon: %d, Num.of Triangles: %d, Num. of Vertices: %d", polygonNum, polygonNum * 4, polygonNum * 12);

	glewExperimental = true;
	glewInit();

	myInit();
	glutKeyboardFunc(myKeyboard);
	glutIdleFunc(myIdle);
	glutDisplayFunc(myDisplay);
	glutMainLoop();

	return 0;
}