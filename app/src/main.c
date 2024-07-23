#include "../include/acc.h"
#include "../include/buzzer.h"
#include "../include/display.h"
#include "../include/joystick.h"
#include "../include/shared_linux.h"
#include "../../hal/include/config_display.h"

static int flag = 1;

int main() {
    config_pins();
    data_init();
    
    joy_init(&flag);
    buzzer_init(&flag);
    acc_init(&flag);
    display_init(&flag);

    joy_cleanup();
    buzzer_cleanup();
    acc_cleanup();
    display_cleanup();
    data_cleanup();

    return 0;
}