# SPARK(&pyspk) Demos
Original Demo source code is https://sourceforge.net/projects/sparkengine/

## Changes from original

 - renamed from 'Tuto1' to 'Fireworks demo'  
 - 'Tuto2' demo is not ready(this is not working in my env...)
 - 'SFML' and 'Irrlicht' demo is not supported(OpenGL only).

## Additional requirements

*Fire Demo* requires additional libraries: [pyglet](https://bitbucket.org/pyglet/pyglet/wiki/Home), [PyWavefront](https://github.com/greenmoss/PyWavefront)  
please run these commands to install them:  
 - pip install pyglet
 - pip install PyWavefront

## Key Assign

Common

| Key | Descriptions |
| --- | ----- |
| Esc | Quits |  
| Del | Reinitialises the particles systems |
| F1  | Changes the text display (full -> fps only -> nothing)|
|Wheel| Zooms in and out (not all demos)|
|Mouse| Rotates around (not all demos)|


openGL Demos

| Key | Descriptions |
| --- | --- |
|Pause| Pauses the particles systems |
| F2  | Displays the bounding boxes for particle groups or not|
| F4  | Changes the renderers (normal -> basic (points) -> none)|
| F5  | Displays or hides the environment|


Fire Demo

| Key | Descriptions |
| --- | --- |
|Space| Generates smoke or not|


Rain Demo

| Key | Descriptions |
| --- | --- |
|Cursor keys(or WASD)| Moves around |
| +/- | Changes the rain intensity(0-100)|


Fountain Demo

| Key | Descriptions |
| --- | --- |
|Space| Switches between high an low graphics|


Flakes Demo

| Key | Descriptions |
| --- | --- |
| +/- | Changes the number of particles|
|Space| Spreads particles|


Writing Demo

| Key | Descriptions |
| --- | --- |
|Left drag | Draws particles |


Fireworks Demo(Tuto1)

| Key | Descriptions |
| --- | --- |
|Space| spawn fireworks |
