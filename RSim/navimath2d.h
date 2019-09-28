////////////////////////////////////////////////////////////////////////////////
//
//  NaviMath2D v1.1.0
//  by Balazs Megyeri
//
////////////////////////////////////////////////////////////////////////////////
//
//  Files
//  - common.h
//  - coordinatesystem.h
//  - cartesiancs.h
//  - location.h
//  - cartesianloc.h
//  - position.h
//  - cartesianpos.h
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <math.h>

#define WORLD nullptr
#define DOUBLE_PRECISION


#ifdef DOUBLE_PRECISION

typedef double fp_t;

#define COS(x) cos(x)
#define SIN(x) sin(x)
#define TAN(x) tan(x)
#define ABS(x) abs(x)
#define SQRT(x) sqrt(x)

#define PI 3.1415926535

#define DEG_TO_RAD(x) (x*PI/180.0)
#define RAD_TO_DEG(x) (x*180.0/PI)

#else // SINGLE_PRECISION

typedef float fp_t;

#define COS(x) cosf(x)
#define SIN(x) sinf(x)
#define TAN(x) tanf(x)
#define ABS(x) fabs(x)
#define SQRT(x) sqrtf(x)

#define PI 3.1415926535f

#define DEG_TO_RAD(x) (x*PI/180.0f)
#define RAD_TO_DEG(x) (x*180.0f/PI)

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
protected:
    fp_t x;
    fp_t y;

    void CalcWorldCoordinates() override;
    void CalcOwnCoordinates() override;

public:
    CartesianLoc(CartesianCS* csys, fp_t x, fp_t y);
    CartesianLoc(Location& l);
    CartesianLoc(CartesianCS* csys, CartesianLoc& l); // Does no transformation

    ~CartesianLoc() override = default;

    fp_t GetX();
    fp_t GetY();

    void SetX(fp_t x);
    void SetY(fp_t y);

    void TransformTo(CoordinateSystem* cs) override;
};

class Position
{
protected:
    CoordinateSystem* cs;

    fp_t world_x;
    fp_t world_y;
    fp_t world_phi;

    virtual void CalcWorldCoordinates() = 0; // 0: pure virtual
    virtual void CalcOwnCoordinates() = 0;

public:
    fp_t getWorldX();
    fp_t getWorldY();
    fp_t getWorldPhi();

    virtual ~Position() = default;
    virtual void TransformTo(CoordinateSystem* new_cs) = 0;
};

class CartesianPos : public Position
{
protected:
    fp_t x;
    fp_t y;
    fp_t phi;

    void CalcWorldCoordinates() override;
    void CalcOwnCoordinates() override;

public:
    CartesianPos(CartesianCS* csys, fp_t x, fp_t y, fp_t phi);
    CartesianPos(Position& p);
    CartesianPos(CartesianCS* csys, CartesianPos& p); // Does no transformation

    ~CartesianPos() override = default;

    fp_t GetX();
    fp_t GetY();
    fp_t GetPhi();

    void SetX(fp_t x);
    void SetY(fp_t y);
    void SetPhi(fp_t phi);

    void TransformTo(CoordinateSystem* cs) override;
};
