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
