#include "quantum.h"
#include "persistent_hold.h"
#include "multihold.h"

void update_state(multihold_state_t *state) {
    // reset or update the count
    if(timer_elapsed(state->timer) <= TAPPING_TERM) {
        state->count += 1;
    } else {
        state->count = 1;
    }

    // refresh the timer
    state->timer = timer_read();
}

void layer_mod_event(keyrecord_t *record, int layer, uint16_t mod, multihold_state_t *state) {
    if(record->event.pressed) {
        update_state(state);

        register_layer(layer);
        if(state->count > 1) {
            register_mod(mod);
        }
    } else {
        unregister_layer(layer);
        if(state->count > 1) {
            unregister_mod(mod);
        }
    }
}

void layer_layer_event(keyrecord_t *record, int layer1, int layer2, multihold_state_t *state) {
    if(record->event.pressed) {
        update_state(state);

        if(state->count == 1) {
            register_layer(layer1);
        } else {
            register_layer(layer2);
        }
    } else {
        if(state->count == 1) {
            unregister_layer(layer1);
        } else {
            unregister_layer(layer2);
        }
    }
}

void mod_mod_event(keyrecord_t *record, uint16_t mod1, uint16_t mod2, multihold_state_t *state) {
    if(record->event.pressed) {
        update_state(state);

        if(state->count == 1) {
            register_mod(mod1);
        } else {
            register_mod(mod2);
        }
    } else {
        if(state->count == 1) {
            unregister_mod(mod1);
        } else {
            unregister_mod(mod2);
        }
    }
}

void quad_mod_event(keyrecord_t *record, uint16_t mod1, uint16_t mod2, uint16_t mod3, uint16_t mod4, multihold_state_t *state) {
    if(record->event.pressed) {
        update_state(state);

        switch(state->count) {
            case 1: register_mod(mod1); break;
            case 2: register_mod(mod2); break;
            case 3: register_mod(mod3); break;
            default: register_mod(mod4); break;
        }
    } else {
        switch(state->count) {
            case 1: unregister_mod(mod1); break;
            case 2: unregister_mod(mod2); break;
            case 3: unregister_mod(mod3); break;
            default: unregister_mod(mod4); break;
        }
    }
}