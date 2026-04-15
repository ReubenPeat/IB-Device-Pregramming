#include "mbed.h"

DigitalOut led1(LED1); //green
DigitalOut led2(LED2); //blue
DigitalOut led3(LED3); //red
InterruptIn button(USER_BUTTON);

const int seq_len = 4; //sequence length
int seq[seq_len];
volatile int led_num = 0;

volatile int playing_index = 0; //current number in sequence being played
volatile int record_index =0; //current number in sequence being chosen

volatile bool button_pressed = false;
volatile bool seq_playing = false;

Timeout button_debounce_timeout;
float debounce_time = 0.3;

Ticker led_swap;
int cycle_time = 1; //time between led swaps

void led_selector(int num)
{
    led1 = (num == 0);
    led2 = (num == 1);
    led3 = (num == 2);
}

void move_to_next() //func to move to next LED
{
    if (! seq_playing)
    {
        led_num = (led_num + 1) % 3;
    }
    else 
    {
        playing_index = (playing_index + 1) % seq_len;//move to next le being played
    }
}

void onButtonStopDebouncing();

void onButtonPress(void)
{
    button_pressed = true;
    button.rise(NULL);
    button_debounce_timeout.attach(onButtonStopDebouncing, debounce_time);
}

void onButtonStopDebouncing(void)
{
    button.rise(onButtonPress);
}

int main() 
{
    button.rise(onButtonPress);
    led_swap.attach(move_to_next, cycle_time);

    while(true)
    {
        if (button_pressed)
        {
            button_pressed = false;
            seq[record_index] = led_num;
            record_index += 1;
        }

        if (record_index >= seq_len)
        {
            button.rise(NULL);
            seq_playing = true;
        }

        if (seq_playing)
        {
            led_selector(seq[playing_index]);
        }
        else
        {
            led_selector(led_num);
        }
    }
}