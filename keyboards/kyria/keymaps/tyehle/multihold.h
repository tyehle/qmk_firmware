typedef struct {
    uint16_t timer;
    uint16_t count;
} multihold_state_t;

void layer_mod_event(keyrecord_t *record, int layer, uint16_t mod, multihold_state_t *state);
void layer_layer_event(keyrecord_t *record, int layer1, int layer2, multihold_state_t *state);
void mod_mod_event(keyrecord_t *record, uint16_t mod1, uint16_t mod2, multihold_state_t *state);
void quad_mod_event(keyrecord_t *record, uint16_t mod1, uint16_t mod2, uint16_t mod3, uint16_t mod4, multihold_state_t *state);