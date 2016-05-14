#include "SDL/SDL.h"
#include <string>
#include "SDL/SDL_image.h"
#include "FieldObject.h"
#include "Global.h"
#include "FieldController.h"


FieldController* FieldObject::fieldControl = NULL;

int FieldObject::constructors = 0;
int FieldObject::destructors = 0;

FieldObject::FieldObject() {
      constructors++;
}

FieldObject::~FieldObject() {
      /*if (prev == NULL) {
            //Redo field controller's pointers
      } else {
            prev->next = this->next;
      }
      if (next != NULL) {
            next->prev = this->prev;
      }*/
      destructors++;
}

void FieldObject::draw(SDL_Surface* onto) {
       apply_surface(int(x),int(y),sprite,onto);
}

//Filler class
void FieldObject::step() {
     //Do nothing.
}

//Returns true if the object can collide with the player's shots.
bool FieldObject::collidable() {
     return false;
}
