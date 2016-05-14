#include "Hitbox.h"


Hitbox::Hitbox(float xmin,float xmax,float ymin,float ymax) {
    this->xmin = xmin; //Left
    this->ymin = ymin; //Top
    this->xmax = xmax; //Right
    this->ymax = ymax; //Bottom
}

bool Hitbox::overlaps(Hitbox* other) {
    if (this->xmin > other->xmax) return false;
    if (other->xmin > this->xmax) return false;
    if (this->ymin > other->ymax) return false;
    if (other->ymin > this->ymax) return false;
    return true;
}
