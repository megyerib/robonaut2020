////////////////////////////////////////////////////////////////////////////////
//
//  NaviMath2D v1.1.0
//  by Balazs Megyeri
//
////////////////////////////////////////////////////////////////////////////////
//
//  Files
//  - (coordinatesystem.cpp)
//  - cartesiancs.cpp
//  - location.cpp
//  - cartesianloc.cpp
//  - position.cpp
//  - cartesianpos.cpp
//
////////////////////////////////////////////////////////////////////////////////

#include "navimath2d.h"

CartesianCS::CartesianCS(
        fp_t center_x,
        fp_t center_y,
        fp_t alpha,
        fp_t x_res,
        fp_t y_res
    ) :
    center_x(center_x),
    center_y(center_y),
    alpha(alpha),
    x_res(x_res),
    y_res(y_res)
{
    RecalcBaseVectors();
}

CartesianCS::CartesianCS(CartesianCS* c)
{
    if (c == nullptr)
    {
        center_x = 0;
        center_y = 0;
        alpha    = 0;
        x_res    = 1;
        y_res    = 1;
    }
    else
    {
        center_x = c->center_x;
        center_y = c->center_y;
        alpha    = c->alpha;
        x_res    = c->x_res;
        y_res    = c->y_res;
    }

    RecalcBaseVectors();
}

void CartesianCS::RecalcBaseVectors()
{
    a[0] = x_res * COS(alpha);
    a[1] = x_res * SIN(alpha);
    b[0] = y_res * COS(alpha + DEG_TO_RAD(90));
    b[1] = y_res * SIN(alpha + DEG_TO_RAD(90));

    a_len = SQRT(a[0]*a[0]+a[1]*a[1]);
    b_len = SQRT(b[0]*b[0]+b[1]*b[1]);
}

fp_t Location::getWorldX()
{
    CalcWorldCoordinates();
    return world_x;
}

fp_t Location::getWorldY()
{
    CalcWorldCoordinates();
    return world_y;
}

CartesianLoc::CartesianLoc(CartesianCS* csys, fp_t x, fp_t y) :
    c_x(x),
    c_y(y)
{
    Location::cs = csys;
}

CartesianLoc::CartesianLoc(Location& l) :
    Location(l)
{
    CalcOwnCoordinates();
}

CartesianLoc::CartesianLoc(CartesianCS* csys, CartesianLoc& l) :
    c_x(l.c_x),
    c_y(l.c_y)
{
    Location::cs = csys;
}

void CartesianLoc::CalcWorldCoordinates()
{
    if (cs == nullptr) // World CS is used
    {
        world_x = c_x;
        world_y = c_y;
    }
    else
    {
        CartesianCS* cs = static_cast<CartesianCS*>(Location::cs);

        world_x = cs->center_x + cs->a[0] * c_x + cs->b[0] * c_y;
        world_y = cs->center_y + cs->a[1] * c_x + cs->b[1] * c_y;
    }
}

void CartesianLoc::CalcOwnCoordinates()
{
    if (cs == nullptr) // World CS is used
    {
        c_x = world_x;
        c_y = world_y;
    }
    else
    {
        CartesianCS* cs = static_cast<CartesianCS*>(Location::cs);

        fp_t p[2] =
        {
            world_x - cs->center_x,
            world_y - cs->center_y
        };

        c_x = (cs->a[0] * p[0] + cs->a[1] * p[1]) / cs->a_len / cs->a_len;
        c_y = (cs->b[0] * p[0] + cs->b[1] * p[1]) / cs->b_len / cs->a_len;
    }
}

void CartesianLoc::TransformTo(CoordinateSystem* csys)
{
    CalcWorldCoordinates();
    this->cs = csys;
    CalcOwnCoordinates();
}

fp_t CartesianLoc::x()
{
    return c_x;
}

fp_t CartesianLoc::y()
{
    return c_y;
}

void CartesianLoc::SetX(fp_t x)
{
    this->c_x = x;
}

void CartesianLoc::SetY(fp_t y)
{
    this->c_y = y;
}


fp_t Position::getWorldX()
{
    CalcWorldCoordinates();
    return world_x;
}

fp_t Position::getWorldY()
{
    CalcWorldCoordinates();
    return world_y;
}

fp_t Position::getWorldPhi()
{
    CalcWorldCoordinates();
    return world_phi;
}

CartesianPos::CartesianPos(CartesianCS* csys, fp_t x, fp_t y, fp_t phi) :
    c_x(x),
    c_y(y),
    c_phi(phi)
{
    Position::cs = csys;
}

CartesianPos::CartesianPos(Position& p) :
    Position(p)
{
    CalcOwnCoordinates();
}

CartesianPos::CartesianPos(CartesianCS* csys, CartesianPos& p) :
    c_x(p.c_x),
    c_y(p.c_y),
    c_phi(p.c_phi)
{
    Position::cs = csys;
}

void CartesianPos::CalcWorldCoordinates()
{
    if (cs == nullptr) // World CS is used
    {
        world_x = c_x;
        world_y = c_y;
        world_phi = c_phi;
    }
    else
    {
        CartesianCS* cs = reinterpret_cast<CartesianCS*>(Position::cs);

        world_x = cs->center_x + cs->a[0] * c_x + cs->b[0] * c_y;
        world_y = cs->center_y + cs->a[1] * c_x + cs->b[1] * c_y;
        world_phi = cs->alpha + c_phi;
    }
}

void CartesianPos::CalcOwnCoordinates()
{
    if (cs == nullptr) // World CS is used
    {
        c_x = world_x;
        c_y = world_y;
        c_phi = world_phi;
    }
    else
    {
        CartesianCS* cs = reinterpret_cast<CartesianCS*>(Position::cs);

        fp_t p[2] =
        {
            p[0] = world_x - cs->center_x,
            p[1] = world_y - cs->center_y
        };

        c_x = (cs->a[0] * p[0] + cs->a[1] * p[1]) / cs->a_len / cs->a_len;
        c_y = (cs->b[0] * p[0] + cs->b[1] * p[1]) / cs->b_len / cs->a_len;
        c_phi = world_phi - cs->alpha;
    }
}

void CartesianPos::TransformTo(CoordinateSystem* csys)
{
    CalcWorldCoordinates();
    this->cs = csys;
    CalcOwnCoordinates();
}

fp_t CartesianPos::x()
{
    return c_x;
}

fp_t CartesianPos::y()
{
    return c_y;
}

fp_t CartesianPos::phi()
{
    return c_phi;
}

void CartesianPos::SetX(fp_t x)
{
    this->c_x = x;
}

void CartesianPos::SetY(fp_t y)
{
    this->c_y = y;
}

void CartesianPos::SetPhi(fp_t phi)
{
    this->c_phi = phi;
}
