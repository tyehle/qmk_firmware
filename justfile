build:
    make kyria/rev1:tyehle

flash side:
    make kyria/rev1:tyehle:avrdude-split-{{side}}
