#ifndef WAVE_H
#define WAVE_H

struct Wave {
    int zombies;
    int npcs;
    float cooldown;

    Wave(int zombies,int npcs, float cooldown) : zombies(zombies), npcs(npcs),cooldown(cooldown) {}
};

#endif
