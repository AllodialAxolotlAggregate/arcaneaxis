class Cube 
{
  PVector position; 
  PVector linearAcceleration; 
  PVector rotationalAcceleration;
  PVector colors; // I like having this be a PVector because PVectors can store 3 values and it's easily contained.
  
 
  // Constructor: Initialize with position in the x, y, and z
  Cube(float x_, float y_, float z_)
  {
    // Initialize member variables
    this.position = new PVector(x_, y_, z_); 
    this.linearAcceleration = new PVector(0,0,0);
    this.rotationalAcceleration = new PVector(0,0,0);
    
    // Initialize with completely random color
    this.colors = new PVector(random(0,255), random(0,255), random(0,255)); 
    
    println("Cube created.");
    
  }
  
  // Draws the cube
  void Display()
  {
    // The function call is box(float size) so we need to pushMatrix() and popMatrix() and use translation and stuff
    
    pushMatrix();
    translate(position.x, position.y, position.z);
    fill(colors.x, colors.y, colors.z);
    box(100);
    popMatrix();
  }
  
}
