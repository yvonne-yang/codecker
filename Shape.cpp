//
//  Shape.cpp
//  lab3
//
//  Modified by Tarek Abdelrahman on 2020-10-04.
//  Created by Tarek Abdelrahman on 2018-08-25.
//  Copyright © 2018-2020 Tarek Abdelrahman.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.

//  ECE244 Student:
//  Write the implementation (and only the implementation) of the Shape class below
#include "Shape.h"

// Build a Shape object with its properties
Shape::Shape(string n, string t, int x_loc, int x_sz, int y_loc, int y_sz)
{
    name = n;
    type = t;
    x_location = x_loc;
    x_size = x_sz;
    y_location = y_loc;
    y_size = y_sz;
}

// Accessors
string Shape::getType() // Returns the type
{
    return type;
}
string Shape::getName() // Returns the name of the shape
{
    return name;
}
int Shape::getXlocation() // Returns location of the shape on the x-axis
{
    return x_location;
}
int Shape::getYlocation() // Returns location of the shape on the y-axis
{
    return y_location;
}
int Shape::getXsize() // Returns the size of the shape in the x-dimension
{
    return x_size;
}
int Shape::getYsize() // Returns the size of the shape in the y-dimension
{
    return y_size;
}

// Mutators
void Shape::setType(string t) // Sets the type (see globals.h)
{
    type = t;
}
void Shape::setName(string n) // Sets the name of the shape
{
    name = n;
}
void Shape::setXlocation(int x_loc) // Sets location of the shape on the x-axis
{
    x_location = x_loc;
}
void Shape::setYlocation(int y_loc) // Sets location of the shape on the y-axis
{
    y_location = y_loc;
}
void Shape::setXsize(int x_sz) // Sets size of the shape in the x-dimension
{
    x_size = x_sz;
}
void Shape::setYsize(int y_sz) // Sets size of the shape in the y-dimension
{
    y_size = y_sz;
}
void Shape::setRotate(int angle) // sets the rotation of the shape
{
    rotation = angle;
}

// Utility methods
void Shape::draw() // Draws a shape; for this assignment it
{
}
// only prints the information of the shape
