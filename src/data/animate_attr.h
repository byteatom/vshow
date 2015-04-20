#ifndef ANIMATEATTR_H
#define ANIMATEATTR_H

class AnimateAttr
{
public:
    enum {
        MIN_SPEED = 0,  //same as UI index for convenience
        BASE_SPEED = 10,    //step == 1 && pulses == 1
        MAX_SPEED = 20,
        MAX_STAY = 31536000 //one year
    };

    typedef enum {
        RANDOM,
        MOTIONLESS,
        MOVE_UP,
        MOVE_DOWN,
        MOVE_LEFT,
        MOVE_RIGHT,
        OVERLAY_UP,
        OVERLAY_DOWN,
        OVERLAY_LEFT,
        OVERLAY_RIGHT,
        HORIZONTAL_INAUGURATE,
        HORIZONTAL_CONCLUDE,
        VERTICAL_INAUGURATE,
        VERTICAL_CONCLUDE,
        SHUTTER_UP,
        SHUTTER_DOWN,
        SHUTTER_LEFT,
        SHUTTER_RIGHT,
        BLINK,
        BRUSH_UP,
        BRUSH_DOWN,
        BRUSH_LEFT,
        BRUSH_RIGHT,
        PAGE_LEFT,
        PAGE_RIGHT,
        FAN_INAUGURATE,
        FAN_CONCLUDE,
        SPIN_LEFT,
        SPIN_RIGHT,
        RECTANGLE_INAUGURATE,
        RECTANGLE_CONCLUDE,
        CURTAIN_DOWN,
        CURTAIN_RIGHT,
        RECTANGLE_TOP_LEFT,
        RECTANGLE_TOP_RIGHT,
        RECTANGLE_BOTTOM_LEFT,
        RECTANGLE_BOTTOM_RIGHT,
        DIAGONAL_TOP_LEFT,
        DIAGONAL_TOP_RIGHT,
        DIAGONAL_BOTTOM_LEFT,
        DIAGONAL_BOTTOM_RIGHT,
        CROSS_EXPAND,
        CROSS_SHRINKAGE,
        MOVE_LEFT_CONTINUOUS,
        MOVE_RIGHT_CONTINUOUS,
        STYLE_MAX,
    }Style;

    AnimateAttr();
    virtual ~AnimateAttr();

    void setSpeed(unsigned int speed);
    unsigned int getSpeed();

    Style style;
    unsigned int pulses;
    unsigned int step;
    unsigned int stay;   //seconds
};

#endif // ANIMATEATTR_H
