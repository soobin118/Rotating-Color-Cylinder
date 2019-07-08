#pragma once

#include<vgl.h>
#include<InitShader.h>
#include<vec.h>

vec4 * vertex_pos;
vec4 * vertex_col;
class MyColorCylinder {
public:
	int numVertices;
	GLuint vao,buffer,prog;
	vec4 * points;
	vec4 * colors;
	int ind = 0;
	int sizeofPoints,sizeofColors;

	MyColorCylinder() {
		points = NULL;
		colors = NULL;
	}
	~MyColorCylinder() {
		delete[] points;
		delete[] colors;
		delete[] vertex_pos;
		delete[] vertex_col;
	}
	void makeQuad(int a, int b, int c, int d)
	{
		points[ind] = vertex_pos[a];	colors[ind] = vertex_col[a];	ind++;
		points[ind] = vertex_pos[b];	colors[ind] = vertex_col[a];	ind++;
		points[ind] = vertex_pos[c];	colors[ind] = vertex_col[a];	ind++;

		points[ind] = vertex_pos[c];	colors[ind] = vertex_col[a];	ind++;
		points[ind] = vertex_pos[d];	colors[ind] = vertex_col[a];	ind++;
		points[ind] = vertex_pos[a];	colors[ind] = vertex_col[a];	ind++;
	}
	void makeCylinder(int polygonNum) {
		int index = 6 * polygonNum;
		ind = 0;

		for (int i = 0; i < polygonNum; i++)	//side
		{
			makeQuad(i,(i+1)%polygonNum, (i + 1) % polygonNum+polygonNum,i+polygonNum);
		}

		for (int i = 0; i < polygonNum; i++)	//top
		{
			points[index] = vertex_pos[polygonNum * 2];	//top center point
			colors[index] = vertex_col[polygonNum * 2];
			index++;
			points[index] = vertex_pos[i];
			colors[index] = vertex_col[i];
			index++;
			points[index] = vertex_pos[(i + 1) % polygonNum];
			colors[index] = vertex_col[(i + 1) % polygonNum];
			index++;
		}

		for (int i = 0; i < polygonNum; i++)	//bottom
		{
			points[index] = vertex_pos[polygonNum * 2 + 1];	//bottom center point
			colors[index] = vertex_col[polygonNum * 2 + 1];
			index++;
			points[index] = vertex_pos[i+polygonNum];
			colors[index] = vertex_col[i + polygonNum];
			index++;
			points[index] = vertex_pos[(i + 1) % polygonNum + polygonNum];
			colors[index] = vertex_col[(i + 1) % polygonNum + polygonNum];
			index++;
		}
	}

	void initPosition(int polygonNum) {

		numVertices = polygonNum * 12;

		points = new vec4[numVertices];
		colors = new vec4[numVertices];

		sizeofPoints = sizeof(vec4)*numVertices;
		sizeofColors = sizeof(vec4)*numVertices;

		vertex_pos = new vec4[polygonNum*2+2];
		vertex_col = new vec4[polygonNum*2+2];

		float r = 1.0;
		float g = 0.0;
		float b = 0.0;
		//make vertex_col array
		for (int i = 0; i < polygonNum; i++)
		{
			if (i == polygonNum - 1)	//end color = blue
			{
				vertex_col[i] = vec4(0.0, 0.0, 1.0, 1.0);
				vertex_col[i + polygonNum] = vec4(0.0, 0.0, 1.0, 1.0);
			}
			else {
				vertex_col[i] = vec4(r, g, b, 1.0);
				vertex_col[i + polygonNum] = vec4(r, g, b, 1.0);
				r -= 1 / (float)polygonNum;
				b += 1 / (float)polygonNum;
			}
		}
		vertex_col[polygonNum * 2] = vec4(1.0, 0.0, 0.0, 1.0);	//center point = red
		vertex_col[polygonNum*2+1] = vec4(1.0, 0.0, 0.0, 1.0);

		//make vertex_pos array
		float theta = ((360 / (float)polygonNum) / 180.0)*3.141592;
		for (int i = 0; i < polygonNum; i++)
		{
			vertex_pos[i].x = 0.5*cos(theta);
			vertex_pos[i].y = 0.5*sin(theta);
			vertex_pos[i].z = -0.5;
			vertex_pos[i].w = 1.0;
			theta+= ((360 / (float)polygonNum) / 180.0)*3.141592;
		}
		theta = ((360 / (float)polygonNum) / 180.0)*3.141592;
		for (int i = polygonNum; i < polygonNum*2; i++)
		{
			vertex_pos[i].x = 0.5*cos(theta);
			vertex_pos[i].y = 0.5*sin(theta);
			vertex_pos[i].z = 0.5;
			vertex_pos[i].w = 1.0;
			theta += ((360 / (float)polygonNum) / 180.0)*3.141592;
		}
		vertex_pos[polygonNum * 2] = vec4(0.0, 0.0, -0.5, 1.0);	//center point
		vertex_pos[polygonNum * 2 + 1] = vec4(0.0, 0.0, 0.5, 1.0);
		
		makeCylinder(polygonNum);
	}

	void initCylinder(int polygonNum) {
		initPosition(polygonNum);

		// send data to GPU
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeofPoints + sizeofColors, NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeofPoints, points);
		glBufferSubData(GL_ARRAY_BUFFER, sizeofPoints, sizeofColors, colors);

		// upload shaders to GPU
		prog = InitShader("vshader.glsl", "fshader.glsl");
		glUseProgram(prog);

		// link the data to the vshader
		GLuint vPosition = glGetAttribLocation(prog, "vPosition");
		glEnableVertexAttribArray(vPosition);
		glVertexAttribPointer(vPosition, 4, GL_FLOAT, false, 0, BUFFER_OFFSET(0));

		GLuint vColor = glGetAttribLocation(prog, "vColor");
		glEnableVertexAttribArray(vColor);
		glVertexAttribPointer(vColor, 4, GL_FLOAT, false, 0, BUFFER_OFFSET(sizeofPoints));
	}

	void redraw(int polygonNum)
	{
		ind = 0;
		initPosition(polygonNum);
		// send data to GPU
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeofPoints + sizeofColors, NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeofPoints, points);
		glBufferSubData(GL_ARRAY_BUFFER, sizeofPoints, sizeofColors, colors);

		// upload shaders to GPU
		glUseProgram(prog);
		
		// link the data to the vshader
		GLuint vPosition = glGetAttribLocation(prog, "vPosition");
		glEnableVertexAttribArray(vPosition);
		glVertexAttribPointer(vPosition, 4, GL_FLOAT, false, 0, BUFFER_OFFSET(0));

		GLuint vColor = glGetAttribLocation(prog, "vColor");
		glEnableVertexAttribArray(vColor);
		glVertexAttribPointer(vColor, 4, GL_FLOAT, false, 0, BUFFER_OFFSET(sizeofPoints));
		
		glDrawArrays(GL_TRIANGLES, 0, numVertices);
	}

	void draw(float time) {
		glBindVertexArray(vao);
		glUseProgram(prog);
		
		GLuint uTime = glGetUniformLocation(prog, "uTime");
		glUniform1f(uTime, time);

		glDrawArrays(GL_TRIANGLES, 0, numVertices);
	}
};