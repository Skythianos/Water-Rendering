/**
* \file box.hpp
*
* A kockát reprezentáló osztály forráskódja itt található.
*
*/

#ifndef BOX
#define BOX

class Box {

	Vector position;		//position of cube
	Vector acceleration;	//acceleration of cube
	Vector velocity;		//velocity of cube

	GLfloat cheight[5];		
	Vector cnormal[5];		

	GLboolean anim;			//physics enable
	GLboolean onColl;		//collision enable
	
	Vector crotate;				
	Vector up;					
	GLfloat cang, cangc, cfric; 
	Vector snormal;				

	GLfloat time;
public:

	Box() : position(0, 0, 10, 1), acceleration(0, 0, 0, 0), velocity(0, 0, 0, 0),
			crotate(0, 0, 0), cang(0), up(0, 0, 1), snormal(0, 0, 0), time(0)
	{
		anim = GL_FALSE;
		onColl = GL_FALSE;
		cfric = 1;
		
	}

	void AnimIt(GLboolean animIt) {
		
		anim = animIt;
	}

	void setZ(GLfloat z) {
		if(!anim) {

			anim = GL_FALSE;
			onColl = GL_FALSE;
			cfric = 1;

			for(int i = 0; i<5; i++) {
				cheight[0] = 0;
				cnormal[0] = Vector(0, 0, 0);
			}

			position.z = z;
			crotate = Vector(0, 0, 0);
			cang = 0;
			cangc = 0;
			up = Vector(0, 0, 1);
			snormal = Vector(0, 0, 0);
			acceleration = Vector(0, 0, 0, 0);
			velocity = Vector(0, 0, 0, 0);
		}

	}

	void Collision(Water water) {

		cheight[0] = water.GetHeightAndNormal(position.x, position.y, cnormal[0]);
		cheight[1] = water.GetHeightAndNormal(position.x-1, position.y-1, cnormal[1]);
		cheight[2] = water.GetHeightAndNormal(position.x+1, position.y+1, cnormal[2]);
		cheight[3] = water.GetHeightAndNormal(position.x+1, position.y-1, cnormal[3]);
		cheight[4] = water.GetHeightAndNormal(position.x-1, position.y+1, cnormal[4]);
	}

	void Idle(GLfloat time) {

		GLfloat dt = (time / this->time) * 0.1;
		this->time = time;


		if(anim) {
			
			if (position.z > cheight[0]) { 

				acceleration.z = -0.09 * cfric;
				cfric = 1;

				onColl = GL_FALSE;

				snormal = Vector(0, 0, 0);

			} else { 

				cfric *= 0.99;


				if (!onColl) { 
					onColl = GL_TRUE;
					velocity = velocity * 0.2;

					for(int i = 1; i<4; i++) { 
						snormal += cnormal[i] * (1 - (position.z - cheight[i]));
					}

					acceleration.z = abs(acceleration.z) * abs(1 - pow((position.z - cheight[0]), 2));
				}

				GLfloat corr = (crotate - (up % snormal)).Length();

				if (corr == 0) {
					corr = 0.00000001;
				}

				crotate = crotate + (up % snormal) * corr * 0.001; 
				crotate.Normalize();

				corr = (up - snormal).Length();

				if (corr == 0) {
					corr = 0.00000001;
				}
				Vector xup = up;
				xup.Normalize();
				Vector xsnormal = snormal;
				xsnormal.Normalize();

				if((xup * xsnormal) < 0) { 
					cangc = (acos(-(xup * xsnormal)) * 180.0/M_PI) / (corr * 5);
				} else {
					cangc = (acos(xup * xsnormal) * 180.0/M_PI) / (corr * 5);
				}

			}

			cang += cangc * cfric * dt; 

			position = position + velocity * dt;
			velocity = velocity + acceleration * dt;

		}
	}

	
	void Draw() {

		glPushMatrix();

		glTranslatef(-position.x, -position.y, -position.z);
		glScalef(4, 4, 4);
		glRotatef(cang, crotate.x, crotate.y, crotate.z);

		glBegin(GL_QUADS);

		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(1.0, 1.0, -1.0);
		glVertex3f(-1.0, 1.0, -1.0);
		glVertex3f(-1.0, 1.0, 1.0);
		glVertex3f(1.0, 1.0, 1.0);

		glColor3f(1.0, 0.5, 0.0);
		glVertex3f(1.0, -1.0, 1.0);
		glVertex3f(-1.0, -1.0, 1.0);
		glVertex3f(-1.0, -1.0, -1.0);
		glVertex3f(1.0, -1.0, -1.0);

		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(1.0, 1.0, 1.0);
		glVertex3f(-1.0, 1.0, 1.0);
		glVertex3f(-1.0, -1.0, 1.0);
		glVertex3f( 1.0, -1.0, 1.0);

		glColor3f(1.0, 1.0, 0.0);
		glVertex3f(1.0, -1.0, -1.0);
		glVertex3f(-1.0,-1.0, -1.0);
		glVertex3f(-1.0, 1.0, -1.0);
		glVertex3f(1.0, 1.0, -1.0);

		glColor3f(0.0 ,0.0 ,1.0); 
		glVertex3f(-1.0, 1.0, 1.0);
		glVertex3f(-1.0, 1.0, -1.0);
		glVertex3f(-1.0, -1.0, -1.0);
		glVertex3f(-1.0, -1.0, 1.0);

		glColor3f(1.0, 0.0, 1.0);
		glVertex3f(1.0, 1.0, -1.0);
		glVertex3f(1.0, 1.0, 1.0);
		glVertex3f(1.0, -1.0, 1.0);
		glVertex3f(1.0, -1.0, -1.0);

		glEnd();

		glColor3f(1.0, 1.0, 1.0);

		glPopMatrix();
	
	}

};

#endif
