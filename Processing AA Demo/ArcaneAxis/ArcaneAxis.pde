/*
 *
 *  Arcane Axis Demo (main class)
 *  Helpful: http://www.processing.org/tutorials/p3d/
 */
 
 Cube myCube;
 
 
 // Sets up the entire project:
 void setup()
 {
   // sets window size and display type. In this case: 3D
   size(200,200, P3D);
   println("set up");
   
   myCube = new Cube(width/2, height/2, 0);
 }
 
 void draw()
 {
   background(0);
   //camera(mouseX, height/2, (height/2) / tan(PI/6), width/2, height/2, 0, 0, 1, 0);
   
   //lights();
   
   
   println("Main draw called.");
   
   myCube.Display();
 }
 

