//
//  parser.cpp
//  lab3
//
//  modified by tarek abdelrahman on 2020-10-04.
//  created by tarek abdelrahman on 2018-08-25.
//  copyright © 2018-2020 tarek abdelrahman.
//
//  permission is hereby granted to use this code in ece244 at
//  the university of toronto. it is prohibited to distribute
//  this code, either publicly or to third parties.

#include <iostream>
#include <sstream>
#include <string>

using namespace std

#include "globals.h"
#include "Shape.h"

// this is the shape array, to be dynamically allocated
Shape **shapesArray;

// the number of shapes in the database, to be incremented
int shapeCount = 0;

// the value of the argument to the maxshapes command
int max_shapes;

// ece244 student: you may want to add the prototype of
// helper functions you write here
int readInt(stringstream &linestream, int min);
int readInt(stringstream &linestream, int min, int max);
string readStr(stringstream &linestream);
bool endOfLine(stringstream &lineStream);
bool nameValid(string name);
bool typeValid(string type);
bool sizeValid(string type, int sizex, int sizey);
bool arrayFull();
Shape **findShape(string name);

void maxShapes(int max);
void create(string name, string type, int locx, int locy, int sizex, int sizey);
void move(Shape *name, int loc, int locy);
void rotate(Shape *name, int angle);
void draw(Shape *shape);
void drawAll();
void deleteShape(Shape **shape);
void deleteAll();

int main()
{

    string line;
    string command;

    cout << "> ";       // Prompt for input
    getline(cin, line); // Get a line from standard input

    while (!cin.eof())
    {
        // Put the line in a linestream for parsing
        // Making a new sstream for each line so flags etc. are cleared
        stringstream lineStream(line);

        // Read from string stream into the command
        // The only way this can fail is if the eof is encountered
        lineStream >> command;

        // Check for the command and act accordingly
        // ECE244 Student: Insert your code here
        if (command == keyWordsList[1]) // maxShapes
        {
            int newMax;
            if ((newMax = readInt(lineStream, 0)) != -1 &&
                    endOfLine(lineStream))
                maxShapes(newMax);
        }
        else if (command == keyWordsList[2]) // create
        {
            // invalid shape name
            // shape name exists
            // invalid shape type
            // invalid argument
            // invalid value (*squares and circles)
            // too many arguments
            // too few arguments
            // shape array is full

            string name, type;
            int locx, locy, sizex, sizey;

            if (!(name = readStr(lineStream)).empty() &&
                    nameValid(name) &&
                    !(type = readStr(lineStream)).empty() &&
                    typeValid(type) &&
                    (locx = readInt(lineStream, 0)) != -1 &&
                    (locy = readInt(lineStream, 0)) != -1 &&
                    (sizex = readInt(lineStream, 0)) != -1 &&
                    (sizey = readInt(lineStream, 0)) != -1 &&
                    sizeValid(type, sizex, sizey) &&
                    endOfLine(lineStream) &&
                    !arrayFull())
            {
                create(name, type, locx, locy, sizex, sizey);
            }
        }
        else if (command == keyWordsList[3]) // move
        {
            // invalid argument
            // shape name not found
            // invalid value
            // too many arguments
            // too few arguments
            string name;
            int locx, locy;
            Shape **pshape;
            if (!(name = readStr(lineStream)).empty() &&
                    (pshape = findShape(name)) &&
                    (locx = readInt(lineStream, 0)) != -1 &&
                    (locy = readInt(lineStream, 0)) != -1 &&
                    endOfLine(lineStream))
                move(*pshape, locx, locy);
        }
        else if (command == keyWordsList[4]) // rotate
        {
            // invalid argument
            // shape name not found
            // invalid value
            // too many arguments
            // too few arguments
            string name;
            int angle;
            Shape **pshape;
            if (!(name = readStr(lineStream)).empty() &&
                    (pshape = findShape(name)) &&
                    (angle = readInt(lineStream, 0, 360)) != -1 &&
                    endOfLine(lineStream))
                rotate(*pshape, angle);
        }
        else if (command == keyWordsList[5]) // draw
        {
            /// shape name not found
            /// too many arguments
            /// too few arguments
            string name;
            if (!(name = readStr(lineStream)).empty()) 
            {
                if (name == "all")
                { if (endOfLine(lineStream))
                    drawAll();
                }
                else
                {
                    Shape **pshape;
                    if ((pshape = findShape(name)) &&
                            endOfLine(lineStream))
                        draw(*pshape);
                }
            }
        }
        else if (command == keyWordsList[6]) // delete
        {
            /// shape name not found
            /// too many arguments
            /// too few arguments
            string name;
            if (!(name = readStr(lineStream)).empty())
            {
                if (name == "all")
                { if (endOfLine(lineStream))
                    deleteAll();
                }
                else
                {
                    Shape **pshape;
                    if ((pshape = findShape(name)) &&
                            endOfLine(lineStream))
                        deleteShape(pshape);
                }
            }
        }
        else
        {
            cout << "Error: invalid command" << endl;
        }

        // once the command has been processed, prompt for the
        // next command
        cout << "> "; // prompt for input
        getline(cin, line);

    } // end input loop until eof.

    if (shapeCount != 0) // clear database
    {
        for (int i = 0; i < max_shapes; i++)
            delete shapesArray[i];
    }
    delete[] shapesArray;

    return 0;
}

/* reads and errortraps for an integer that is at least as big as min. 
 * if no input was received, output the "too few arguments" error.
 */
int readInt(stringstream &linestream, int min)
{
    if (linestream.peek() == '\n' || linestream.eof())
    {
        cout << "Error: too few arguments" << endl;
        return -1;
    }

    int tmp;

    linestream >> tmp;
    if (linestream.fail() || (linestream.peek() != ' ' && linestream.peek() != '\t' && linestream.peek() != '\n' && linestream.peek() != '\r' && linestream.peek() != '\v' && linestream.peek() != '\f' && !linestream.eof()))
        // check if the token was a string, or the next character is not whitespace (trailing string or float)
    {
        cout << "Error: invalid argument" << endl;
        return -1;
    }

    if (tmp < min) // check if value is in range
    {
        cout << "Error: invalid value" << endl;
        return -1;
    }
    return tmp;
}

/* reads and errortraps for an integer i such that min<= i <= max. 
 * if no input was received, output the "too few arguments" error.
 */
int readInt(stringstream &linestream, int min, int max)
{
    int tmp = readInt(linestream, min);
    if (tmp == -1)
        return -1;
    else
    {
        if (tmp > max)
        {
            cout << "Error: invalid value" << endl;
            return (-1);
        }
    }
    return tmp;
}

/* Returns empty string if no string to be read */
string readStr(stringstream &linestream)
{
    if (linestream.peek() == '\n' || linestream.eof())
    {
        cout << "Error: too few arguments" << endl;
        return "";
    }

    string tmp;
    linestream >> tmp;
    return tmp;
}

/* returns true if the end of the line or eof has been reached */
bool endOfLine(stringstream &lineStream)
{
    string s;
    lineStream >> s;
    if (s!="" || !lineStream.eof())
    {
        cout << "Error: too many arguments" << endl;
        return false;
    }
    return true;
}

bool nameValid(string name)
{
    for (int i = 0; i < max_shapes; i++)
    {
        if (shapesArray[i] && shapesArray[i]->getName() == name)
        {
            cout << "Error: shape " << name << " exists" << endl;
            return false;
        }
    }
    for (int i = 0; i < NUM_KEYWORDS; i++)
    {
        if (keyWordsList[i] == name)
        {
            cout << "Error: invalid shape name" << endl;
            return false;
        }
    }
    for (int i = 0; i < NUM_TYPES; i++)
    {
        if (shapeTypesList[i] == name)
        {
            cout << "Error: invalid shape name" << endl;
            return false;
        }
    }
    return true;
}

bool typeValid(string type)
{
    for (int i = 0; i < NUM_TYPES; i++)
    {
        if (shapeTypesList[i] == type)
            return true;
    }
    cout << "Error: invalid shape type" << endl;
    return false;
}

bool sizeValid(string type, int sizex, int sizey)
{
    if (type == "circle" && sizex != sizey)
    {
        cout << "Error: invalid value" << endl;
        return false;
    }
    return true;
}

bool arrayFull()
{
    if (shapeCount == max_shapes)
    {
        cout << "Error: shape array is full" << endl;
        return true;
    }
    return false;
}

Shape **findShape(string name)
{
    for (int i = 0; i < max_shapes; i++)
    {
        if (shapesArray[i] && shapesArray[i]->getName() == name)
            return &shapesArray[i];
    }
    cout << "Error: shape " << name << " not found" << endl;
    return nullptr;
}

void maxShapes(int max)
{
    if (shapeCount != 0) // clean existing database
    {
        for (int i = 0; i < max_shapes; i++)
            delete shapesArray[i];
        delete[] shapesArray;
        shapesArray = nullptr;
        shapeCount = 0;
    }
    shapesArray = new Shape *[max];
    max_shapes = max;
    for (int i = 0; i < max_shapes; i++)
        shapesArray[i] = NULL;
    cout << "New database: max shapes is " << max << endl;
}

void create(string name, string type, int locx, int locy, int sizex, int sizey)
{
    shapesArray[shapeCount++] = new Shape(name, type, locx, sizex, locy, sizey);
    cout << "Created " << name << ": " << type << " " << locx << " " << locy << " " << sizex << " " << sizey << endl;
}

void move(Shape *name, int locx, int locy)
{
    name->setXlocation(locx);
    name->setYlocation(locy);
    cout << "Moved " << name->getName() << " to " << locx << " " << locy << endl;
}

void rotate(Shape *name, int angle)
{
    name->setRotate(angle);
    cout << "Rotated " << name->getName() << " by " << angle << " degrees" << endl;
}

void draw(Shape *shape)
{
    shape->draw();
    cout << "Drew " << shape->getName() << ": " << shape->getType() << " " << shape->getXlocation() << " " << shape->getYlocation() << " " << shape->getXsize() << " " << shape->getYsize() << endl;
}

void drawAll()
{
    cout << "Drew all shapes" << endl;

    for (int i = 0; i < max_shapes; i++)
    {
        if (shapesArray[i])
        {
            Shape *shape = shapesArray[i];
            shape->draw();
            cout << shape->getName() << ": " << shape->getType() << " " << shape->getXlocation() << " " << shape->getYlocation() << " " << shape->getXsize() << " " << shape->getYsize() << endl;
        }
    }
}

void deleteShape(Shape **shape)
{
    cout << "Deleted shape " << (*shape)->getName() << endl;
    delete (*shape);
    (*shape) = nullptr;
}

void deleteAll()
{
    for (int i = 0; i < max_shapes; i++)
    {
        if (shapesArray[i])
            delete shapesArray[i];
    }
    cout << "Deleted: all shapes" << endl;
}
