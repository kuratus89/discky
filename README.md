# DISCKY

discky is a terminal graphics library to draw colored 2D preimitives in terminal window.

It provides API to:
- create drawable objects
- rendering them into a frame buffer
- and display frames continuously for static scenes or animation.

---

## Features

### Terminal lifecycle

#### iniTerminal()

This initializes the terminal , so discky can render graphics correctly.
Call this once before creating and using Discky

#### endDiscky()

This restore the terminal sate when the program exits.
Call this before returning from main()

---

### Drawing models

Each draw calls returns an objId , which can be used to identify them.

#### Rectangle

``` cpp
objId drawRectangle(const Coordinate& a , const Coordinate& b , const objColor& color , const double& border = 1.0 , const double& opacity = 1.0);
```
Draws a rectangle using two corner coordinates.

example :
 ```cpp  
 discky.drawRectangle(NOR_COORD(  -0.5 , -0.5) , NOR_COORD( 0.5 ,0.5) , DISCKY_COLOR_RED); 
```
---

#### Triangle

```cpp
objId drawTriangle(const Coordinate& a , const Coordinate& b , const Coordinate& c , const objColor& color , const double& border = 1.0 , const double& opacity = 1.0);
```
Draw a triangle using 3 coordinates of its vertices.

example :
```cpp
discky.drawTriangle(NOR_COORD(0 , -0.5) , NOR_COORD(0.5 , 0.5) , NOR_COORD(-0.5 , 0.5) , DISCKY_COLOR_GREEN);
```
---

#### Circle

```cpp
 objId drawCircle(const Coordinate& a , const Coordinate& b , const objColor& color , const double& border = 1.0 , const double& opacity = 1.0);
```

Draw a circle using 1 coordinate of its center and a radius(using radius style coordinate).

example:
```cpp
discky.drawCircle(NOR_COORD(0.0 , 0.0) , NOR_LEN_MONO(0.2) , DISCKY_COLOR_RED);
```
---

#### Polygon

```cpp
objId drawPoly(const std::vector<Coordinate> &ver ,const objColor &color , const double& border =1.0 , const double& opacity = 1.0);
```

Draw a polygon from a vertex list

example:
```cpp
discky.drawPoly({NOR_COORD(0 , -0.5) , NOR_COORD(0.5 , 0) , NOR_COORD(0 , 0.5) , NOR_COORD(-0.5 , 0)} , DISCKY_COLOR_RED);
```
---

#### Raw text
```cpp
objId drawRawTxt(const Coordinate& a ,const std::string& txt , const objColor& color);
```
Draw text at given coordinate

example:
```cpp
discky.drawRawTxt(NOR_COORD(0.0 , 0.0) , "HELLO WORLD" , DISCKY_COLOR_RED);
```
---

### Coordinate system

It support two coordinate modes , for both position and lengths

#### Position coordinate

- `NOR_COORD(x, y)` normalized coordinate where x,y ∈ [-1,1]
- `SCR_COORD(x , y)` exact pixel coordinate where x ∈ [0 , terminal maximum horizontal length] , y ∈[0,terminal maximum vertical length]

---

#### Length coordinate

- `NOR_LEN_MONO(x)` normalized length (depends on min(terminal max horizontal length , terminal max vertical length)) where x ∈ [-1 , 1]
- `SCR_LEN_MONO(x)` exact pixel length (depends on min(terminal max horizontal length , terminal max vertical length)) where x ∈ [0 , min(terminal max horizontal length , terminal max vertical length)]
- `NOR_LEN_DI(x , y)` normalized length (diffrent length in diffrent axis) where x,y ∈ [-1 , 1]
- `SCR_LEN_DI(x , y)` exact pixel length (diffrent length in diffrent axis) where x ∈ [0 , max terminal horizontal length] , y ∈ [0 , max terminal vertical length]

---

### Color system

you can create custom color from 0-225 values usiing : ```cpp DISCKY_COLOR_RGB(r , g ,b)``` where r,g,b ∈ [0 , 255]

discky also has some predefined colors in `colors.h` which is included by `discky.h`
examples : `DISCKY_COLOR_BLUE` , `DISCKY_COLOR_RED` and many more

---

### Antialiasing modes

```cpp
void setAntiAliasing(const antiAliasing& mode);
```

Modes:
- `AA_NONE` : No antialiasing
- `AA_LOW` : 2x2 sampling
- `AA_MEDIUM` : 4x4 sampling
- `AA_HIGH` : 8x8 sampling (not recommended)

---

### Error Handling
```cpp
void onDisckyError(ERRORS e, const std::string& msg) {
    // custom logging / recovery
}
...
d.setErrorHandleFunc(onDisckyError);
```
Use this to centralize diagnostics for invalid input, terminal info failures, and internal/runtime errors.

---

### Render pipeline

Discky use internal buffers ( backBuffer and frontBuffer)

the program should use this pipeline:
1. Initilize terminal using `iniTerminal()`
2. Declare Discky
3. [optional] set error handle function example: `discky.setErrorHandleFunc(handler)` , where discky is Discky datatype and handler is: 
`
void handleDisckyError(ERRORS error ,const std::string& msg){
    std::cerr<<msg;
}`
4. [optional , default = black background] set background color example : `discky.setBackground(DISCKY_COLOR_BLUE)`
5. [optional , default : AA_NONE] set anti-aliasing mode , example : `discky.setAntiAliasing(antiAliasing::AA_LOW)` 
6. Draw/update objects
7. render the frame according to current terminal size using example : `discky.render()`
8. display the rendered frame to terminal example : `discky.display()`
9. [optional] refresh draw list , example : `discky.refresh();`

---

### Usefull functions

- `int getTerminalSizeX()` returns max terminal horizontal length as int value
- `int getTerminalSizeY()` returns max terminal vertical length as int value


- `bool isObjectTouchingBoundryX(objId obj)` returns true if the provided objId's object is touching terminal horizontal boundry
- `bool isObjectTouchingBoundryY(objId obj)` return true if the provided objId's object is touching terminal vertical boundry

- `bool isTxtTouchingBoundryX(objId obj)` return true if the provided objId's text is touching terminal horizontal boundry
- `bool isTxtTouchingBoundryY(objId obj)` return true if the provided objId's text is touching terminal vertical boundry

- `bool checkOverlap(objId a , objId b)` return true if objects of provided objId are overlaping each others

---

## Examples

There are examples in "example" folder , you can observe the code and learn from them for better experience

---

## Build

Build:
```bash
mkdir -p build
cd build
cmake ..
cmake --build .
```
