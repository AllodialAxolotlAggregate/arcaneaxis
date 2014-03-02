/*
 *
 *  Arcane Axis Demo (main class)
 *  Helpful: http://www.processing.org/tutorials/p3d/
 */
 
 // Sets up the entire project:
 void setup()
 {
   // sets window size and display type. In this case: 3D
   size(200,200, P3D);
   
   
 }
 
 void draw()
 {
 }
 

class Cube
{
  PVector position; 
  PVector linearAcceleration; 
  PVector rotationalAcceleration;
  PVector colors; // I like having this be a PVector because PVectors can store 3 values and it's easily contained.
  
  
  // Constructor: Initialize with position in the x, y, and z
  Cube(float x_, float y_, float z_)
  {
    println("Cube created.");
  }
  
  
  
}

