//
// Created by Sviatoslav Prylutsky on 2/5/19.
//

#ifndef BOMBERMAN_BOMB_H
#define BOMBERMAN_BOMB_H

#include "object.h"

void explose(Object *explosion, Object *bombOwner);
void * playerPlaintTheBomb(void *args);
#endif //BOMBERMAN_BOMB_H
