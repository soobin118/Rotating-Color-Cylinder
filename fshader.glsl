#version 330

in vec4 color;
in vec4 position;
out vec4 fColor;

void main()
{
	fColor = color;

	if(position.z==0.5 && (position.x)*(position.x)+(position.y)*(position.y)<=0.25*0.25)
		fColor = vec4(1.0,0.0,0.0,1.0);
	else if(position.z==-0.5 && (position.x)*(position.x)+(position.y)*(position.y)<=0.25*0.25)
		fColor = vec4(1.0,0.0,0.0,1.0);
	else if(position.z==-0.5 &&(position.x)*(position.x)+(position.y)*(position.y)<=0.4*0.4)
		fColor = vec4(1.0,1.0,1.0,1.0);
	else if(position.z==0.5 &&(position.x)*(position.x)+(position.y)*(position.y)<=0.4*0.4)
		fColor = vec4(1.0,1.0,0.0,1.0);
	else if((position.z>0.47 || position.z<-0.47 )&& (position.x)*(position.x)+(position.y)*(position.y)>0.4*0.4)
		fColor = vec4(0.0,1.0,0.0,1.0);

}