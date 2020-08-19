#include "quantum.h"
#include "layers.h"

int mod_counts[8] = { 0 };
int layer_counts[NUM_LAYERS] = { 0 };

void register_mod(int mod) {
    // all mods are contiguous and LCTRL is the first one
    int index = mod - KC_LCTRL;
    if(index < 0 || index >= 8) {
        // this isn't a keycode for a real modifier key
        return;
    }

    if(mod_counts[index] == 0) {
        register_code(mod);
    }

    mod_counts[index] += 1;
}

void unregister_mod(int mod) {
    int index = mod - KC_LCTRL;
    if(index < 0 || index >= 8) {
        // this isn't a keycode for a real modifier key
        return;
    }

    mod_counts[index] -= 1;

    if(mod_counts[index] == 0) {
        unregister_code(mod);
    }
}


void register_layer(int layer) {
    if(layer >= NUM_LAYERS) {
        // This isn't a real layer
        return;
    }

    if(layer_counts[layer] == 0) {
        layer_on(layer);
    }

    layer_counts[layer] += 1;
}

void unregister_layer(int layer) {
    if(layer >= NUM_LAYERS) {
        // This isn't a real layer
        return;
    }

    layer_counts[layer] -= 1;

    if(layer_counts[layer] == 0) {
        layer_off(layer);
    }
}
