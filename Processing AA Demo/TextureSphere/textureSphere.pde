/*
 *
 *  Arcane Axis Demo (main class)
 *  Based on texture sphere example from http://www.processing.org/examples/texturesphere.html
 */
 
 PImage bg;
 PImage texmap;
 
 int sDetail = 15; // Sphere detail
 float rotX = 0;
 float rotY = 0;
 float rotZ = 0;
 float velZ = 0;
 float velX = 0;
 float velY = 0;
 float sphereRadius = 200;
 float pushBack = 0;
 PVector position;
 float[] cx, cz, sphereX, sphereY, sphereZ;
 float sinLUT[];
 float cosLUT[];
 float SINCOS_PRECISION = 0.5;
 int SINCOS_LENGTH = int(360.0 / SINCOS_PRECISION);
 boolean isZ = false;
 void setup() {
   size(640, 360, P3D);
   texmap = loadImage("Data/moon.jpg");
   initializeSphere(sDetail);
   position =  new PVector(width * .5, height*.5, -5);
 }
 
 void draw() {
  background(200,200,200);
  //if(isZ) background(255, 100,100);
  
  renderSphere(); 
 }
 
 void renderSphere() {
  pushMatrix(); 
  translate(position.x, position.y, position.z);
  pushMatrix();
  noFill();
  stroke(255,200);
  strokeWeight(2);
  smooth();
  popMatrix();
  lights();
  pushMatrix();
  rotateX( radians(-rotX));
  rotateY( radians(270 - rotY));
  rotateZ(radians(-rotZ));
  fill(200);
  noStroke();
  textureMode(IMAGE);
  stroke(255);
  texturedSphere(sphereRadius, texmap);
  popMatrix();
  popMatrix();
  rotX += velX;
  rotY += velY;
  rotZ += velZ;
  velZ *= .95;
  velX *= 0.95;
  velY *= 0.95;
  
  // Implements mouse control
  if(mousePressed){
   /*if(( rotX > 80 && rotX < 110) || (rotX < -80 && rotX > -110))
   {*/
    // println("STUFF");
     //velY -= (mouseY-pmouseY) * 0.05;
    // velZ += (mouseX-pmouseX) * 0.05;
    // println(velZ);
  /* }
   else
   {*/
     velX += (mouseY-pmouseY) * 0.05;
     velY -= (mouseX-pmouseX) * 0.05;
   //}
   
   
  }
  if(rotX > 360)
  {
     rotX -= 360;
  }else if(rotX < -360)
  {
     rotX += 360;
  }
  if(rotY > 360)
  {
     rotY -= 360;
  }else if(rotY < -360)
  {
     rotY += 360;
  }

  
  drawInfo();
 }
 
 void drawInfo()
 {
   fill(0);
   textSize(20);
   text("X Rotation: " + (int)rotX +"\nY Rotation: " + (int)rotY +"\nZ Rotation: " + (int)rotZ, 10,20);
 }
 
 void initializeSphere(int res)
 {
    sinLUT = new float[SINCOS_LENGTH];
  cosLUT = new float[SINCOS_LENGTH];

  for (int i = 0; i < SINCOS_LENGTH; i++) {
    sinLUT[i] = (float) Math.sin(i * DEG_TO_RAD * SINCOS_PRECISION);
    cosLUT[i] = (float) Math.cos(i * DEG_TO_RAD * SINCOS_PRECISION);
  }

  float delta = (float)SINCOS_LENGTH/res;
  float[] cx = new float[res];
  float[] cz = new float[res];
  
  // Calc unit circle in XZ plane
  for (int i = 0; i < res; i++) {
    cx[i] = -cosLUT[(int) (i*delta) % SINCOS_LENGTH];
    cz[i] = sinLUT[(int) (i*delta) % SINCOS_LENGTH];
  }
  
  // Computing vertexlist vertexlist starts at south pole
  int vertCount = res * (res-1) + 2;
  int currVert = 0;
  
  // Re-init arrays to store vertices
  sphereX = new float[vertCount];
  sphereY = new float[vertCount];
  sphereZ = new float[vertCount];
  float angle_step = (SINCOS_LENGTH*0.5f)/res;
  float angle = angle_step;
  
  // Step along Y axis
  for (int i = 1; i < res; i++) {
    float curradius = sinLUT[(int) angle % SINCOS_LENGTH];
    float currY = -cosLUT[(int) angle % SINCOS_LENGTH];
    for (int j = 0; j < res; j++) {
      sphereX[currVert] = cx[j] * curradius;
      sphereY[currVert] = currY;
      sphereZ[currVert++] = cz[j] * curradius;
    }
    angle += angle_step;
  }
  sDetail = res;
}


// Generic routine to draw textured sphere
void texturedSphere(float r, PImage t) {
  int v1,v11,v2;
  //r = (r + 240 ) * 0.33; // radius stuff... MAGIC NUMBERS ARE BAD
  r /= 2; // just makes the thing fit on the screen
  beginShape(TRIANGLE_STRIP);
  texture(t);
  //stroke(255);
  float iu=(float)(t.width-1)/(sDetail);
  float iv=(float)(t.height-1)/(sDetail);
  float u=0,v=iv;
  for (int i = 0; i < sDetail; i++) {
    // Vertex at x,y,z, u,v where u and v are normals for tex mapping
    vertex(0, -r, 0,u,0);
    
    vertex(sphereX[i]*r, sphereY[i]*r, sphereZ[i]*r, u, v);
    u+=iu;
  }
  vertex(0, -r, 0,u,0);
  vertex(sphereX[0]*r, sphereY[0]*r, sphereZ[0]*r, u, v);
  endShape();   
  
  // Middle rings
  int voff = 0;
  for(int i = 2; i < sDetail; i++) {
    v1=v11=voff;
    voff += sDetail;
    v2=voff;
    u=0;
    beginShape(TRIANGLE_STRIP);
    texture(t);
    for (int j = 0; j < sDetail; j++) {
      vertex(sphereX[v1]*r, sphereY[v1]*r, sphereZ[v1++]*r, u, v);
      vertex(sphereX[v2]*r, sphereY[v2]*r, sphereZ[v2++]*r, u, v+iv);
      u+=iu;
    }
  
    // Close each ring
    v1=v11;
    v2=voff;
    vertex(sphereX[v1]*r, sphereY[v1]*r, sphereZ[v1]*r, u, v);
    vertex(sphereX[v2]*r, sphereY[v2]*r, sphereZ[v2]*r, u, v+iv);
    endShape();
    v+=iv;
  }
  u=0;
  
  // Add the northern cap
  beginShape(TRIANGLE_STRIP);
  texture(t);
  for (int i = 0; i < sDetail; i++) {
    v2 = voff + i;
    vertex(sphereX[v2]*r, sphereY[v2]*r, sphereZ[v2]*r, u, v);
    vertex(0, r, 0,u,v+iv);    
    u+=iu;
  }
  vertex(sphereX[voff]*r, sphereY[voff]*r, sphereZ[voff]*r, u, v);
  endShape();
  
}


void keyPressed()
{
   if(keyCode == LEFT)
   {
     position.x --;
   }
   else if(keyCode == RIGHT)
   {
     position.x++;
   }
   else if(keyCode == UP)
   {
     position.y--;
   }
   else if(keyCode == DOWN)
   {
     position.y++;
   }
   else if(keyCode == 'R')
   {
     rotX = rotY = velX = velY = velZ = rotZ = 0;
     
   }
   else if(keyCode == 'S')
   {
     velX = velY = velZ = 0;
   }
}

void mouseWheel(MouseEvent event)
{
  float e = event.getAmount();
  if(e < 0){ position.z += 2;} 
  else if(e > 0){ position.z -= 2;} 
}
