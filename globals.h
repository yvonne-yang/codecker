//
//  globals.h
//  lab3
//
//  Modified by Tarek Abdelrahman on 2020-10-04.
//  Created by Tarek Abdelrahman on 2018-08-25.
//  Copyright © 2018-2020 Tarek Abdelrahman.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.


#ifndef globals_h
#define globals_h
#include<string>

// These arrays should make it easier to check that a name is not a reserved word
// and that an entered shape type is valid
#define NUM_KEYWORDS 7
string keyWordsList[7]={"all", "maxShapes", "create", "move", "rotate", "draw", "delete"};

#define NUM_TYPES 4
string shapeTypesList[4]={"circle", "ellipse", "rectangle","triangle"};

#endif /* globals_h */
