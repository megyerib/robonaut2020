////////////////////////////////////////////////////////////////////////////////
//
//  NaviMath2D v1.0.0
//  by Balazs Megyeri
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <math.h>

#define WORLD nullptr
//#define DOUBLE_PRECISION


#ifdef DOUBLE_PRECISION

typedef double fp_t;

#define COS(x) cos(x)
#define SIN(x) sin(x)
#define ABS(x) abs(x)
#define SQRT(x) sqrt(x)

#define DEG_TO_RAD(x) (x*3.1415926535/180.0)
#define RAD_TO_DEG(x) (x*180.0/3.1415926535)

#else // SINGLE_PRECISION

typedef float fp_t;

#define COS(x) cosf(x)
#define SIN(x) sinf(x)
#define ABS(x) fabs(x)
#define SQRT(x) sqrtf(x)

#define DEG_TO_RAD(x) (x*3.1415926535f/180.0f)
#define RAD_TO_DEG(x) (x*180.0f/3.1415926535f)

#endif

class CoordinateSystem
{

};

class CartesianCS : public CoordinateSystem
{
public:
    fp_t a[2]; // First axe base vector
    fp_t b[2]; // Second axe base vector
    fp_t a_len;
    fp_t b_len;

    fp_t center_x;
    fp_t center_y;
    fp_t alpha;    // [rad] angle of world X and positive x of this CS
    fp_t x_res;    // X resolution (negative: reverse axe)
    fp_t y_res;    // Y resolution (negative: reverse axe)

    CartesianCS(
        fp_t center_x,
        fp_t center_y,
        fp_t alpha,
        fp_t x_res,
        fp_t y_res
    );

    void RecalcBaseVectors();
};

class Location
{
protected:
    CoordinateSystem* cs;
    fp_t world_x;
    fp_t world_y;

    virtual void CalcWorldCoordinates() = 0; // 0: pure virtual
    virtual void CalcOwnCoordinates() = 0;

public:
    fp_t getWorldX();
    fp_t getWorldY();



    virtual ~Location() = default;
    virtual void TransformTo(CoordinateSystem* new_cs) = 0;
};

class CartesianLoc : public Location
{
    fp_t x;
    fp_t y;

    void CalcWorldCoordinates() override;
    void CalcOwnCoordinates() override;

public:
    CartesianLoc(CartesianCS* cs, fp_t x, fp_t y);
    CartesianLoc(Location& l);
    CartesianLoc(CartesianCS* cs, CartesianLoc& l); // Does no transformation

    ~CartesianLoc() override;

    fp_t GetX();
    fp_t GetY();

    void SetX(fp_t x);
    void SetY(fp_t y);

    void TransformTo(CoordinateSystem* cs) override;
};
