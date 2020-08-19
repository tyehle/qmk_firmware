typedef struct {
    uint16_t timer;
    uint16_t count;
} multihold_state_t;

void layer_mod_event(keyrecord_t *record, int layer, uint16_t mod, multihold_state_t *state);
