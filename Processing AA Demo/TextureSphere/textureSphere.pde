/*
 *
 *  Arcane Axis Demo (main class)
 *  Based on texture sphere example from http://www.processing.org/examples/texturesphere.html
 */
 
 PImage bg;
 PImage texmap;
 
 int sDetail = 35; // Sphere detail
 float rotX = 0;
 float rotY = 0;
 float velX = 0;
 float velY = 0;
 float sphereRadius = 400;
 float pushBack = 0;
 
 float[] cx, cz, sphereX, sphereY, sphereZ;
 float sinLUT[];
 float cosLUT[];
 float SINCOS_PRECISION = 0.5;
 int SINCOS_LENGTH = int(260.0 / SINCOS_PRECISION);
 
 void setup() {
   size(640, 360, P3D);
   //texmap = loadImage("fileName.jpg");
   initializeSphere(sDetail);
 }
 
 void draw() {
  background(0);
  renderSphere(); 
 }
 
 void renderSphere() {
  pushMatrix(); 
  translate(width * 0.33, height * 0.5, pushBack);
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
  fill(200);
  noStroke();
  //textureMode(IMAGE);
  //texturedSphere(sphereRadius, texmap);
  popMatrix();
  popMatrix();
  rotX += velX;
  rotY += velY;
  velX *= 0.95;
  velY *= 0.95;
  
  // Implements mouse control
  if(mousePressed){
   velX += (mouseY-pmouseY) * 0.01;
   velY += (mouseX-pmouseX) * 0.01;
  }
 }
 
 void initializeSphere(int res)
 {
  sinLUT = new float[SINCOS_LENGTH];
  cosLUT = new float[SINCOS_LENGTH];
  
  for (int i = 0; i < SINCOS_LENGTH; i++) {
    sinLUT[i] = (float) Math.sin(i * DEG_TO_RAD * SINCOS_PRECISION);
    sinLUT[i] = (float) Math.cos(i * DEG_TO_RAD * SINCOS_PRECISION);
  }
  
  float delta = (float)SINCOS_LENGTH/res;
  float[] cx = new float[res];
  float[] cz = new float[res];
  
  // Calc unit circle in XZ plane
  for (int i = 0; i < res; i++) {
    cx[i] = -cosLUT[(int) (i*delta) % SINCOS_LENGTH]; 
    cz[i] = sinLUT[(int) (i*delta) % SINCOS_LENGTH]; 
  }
  
  // Computing vertexlist starts at "south pole"
  int vertCount = res * (res-1) + 2;
  int currVert = 0;
  
  // Re-init arrays to store vertices
  sphereX = new float [vertCount];
  sphereY = new float [vertCount];
  sphereZ = new float [vertCount];
  float angle_step = (SINCOS_LENGTH*0.5f)/res;
  float angle = angle_step;
  
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
