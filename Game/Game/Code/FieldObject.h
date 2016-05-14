//Abstract superclass for all field objects.
//E.g., enemies, bosses, etc.

#ifndef FIELDOBJECT_H
#define FIELDOBJECT_H

#include "SDL/SDL.h"
#include <string>
#include "Global.h"
#include "Hitbox.h"
#include <cmath>

//This is a forward declaration of another class.
//It is important to do it like this instead of
//with circular include statements.
class FieldController;

/**
 * General field object
 */
 
class FieldObject {
      protected:
             float x;
             float y;
             SDL_Surface* sprite;
      
      public:
             FieldObject();
             FieldObject* prev;
             FieldObject* next;
             /*FieldObject(float initx,float inity);*/
             virtual ~FieldObject();
             
             //Draw the sprite onto a surface.
             //"virtual" indicates that this function can be overridden by an inheritor class.
             virtual void step() = 0;
             virtual void draw(SDL_Surface* onto);
             static FieldController* fieldControl;
             virtual bool collision(int type) = 0;
             virtual Hitbox hitbox() = 0;
             virtual bool collidable();
             static int constructors;
             static int destructors;
             
};
#endif
