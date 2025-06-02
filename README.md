# echoband 🎧

**echoband** is a wearable sound visualizer that turns ambient audio into reactive light. using fft audio analysis on an esp32 with a max9814 microphone, it maps real-world sound into dynamic led patterns with a ws2812b strip.

built as part of [hack club's highway to undercity](https://hackclub.com/highway/), echoband is perfect for concerts, accessibility, and artistic self-expression.

---

## features
- real-time sound reactivity via fft
- esp32 + max9814 mic combo
- wearable, rechargeable wristband form
- reactive led animations based on pitch and volume

---

## parts used
- esp32 dev board
- max9814 microphone module
- ws2812b rgb led strip (8–12 leds)
- tp4056 lipo charger
- 3.7v lipo battery (500–1000mah)
- breadboard, jumper wires, elastic band

---

## setup

1. connect the microphone output to analog pin gpio 34 on the esp32  
2. connect din of led strip to gpio 13  
3. power the leds and mic with 5v and gnd (shared)  
4. upload the `echoband.ino` sketch using the arduino ide  
5. play music, clap, or talk — watch it light up

---

## build journal
check [journal.md](JOURNAL.md) for daily updates

---

## future plans
- add bluetooth control  
- custom pcb for compact form factor  
- pulse & wave led modes
