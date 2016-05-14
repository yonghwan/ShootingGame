#ifndef HITBOX_H
#define HITBOX_H

/**
 * Hitbox - can be compared to another hitbox.
 * Silly, useful structure.
 */

class Hitbox {
      public:
       float xmin;
       float xmax;
       float ymin;
       float ymax;
       Hitbox(float xmin,float xmax,float ymin,float ymax);
       bool overlaps(Hitbox* other);
       
};

#endif
