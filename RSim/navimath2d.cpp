////////////////////////////////////////////////////////////////////////////////
//
//  NaviMath2D v1.0.0
//  by Balazs Megyeri
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
    this->CalcWorldCoordinates();

    return world_x;
}

fp_t Location::getWorldY()
{
    this->CalcWorldCoordinates();

    return world_y;
}

CartesianLoc::CartesianLoc(CartesianCS* csys, fp_t x, fp_t y) :
    x(x),
    y(y)
{
    Location::cs = csys;
}

CartesianLoc::CartesianLoc(Location& l) :
    Location(l)
{
    CalcOwnCoordinates();
}

CartesianLoc::CartesianLoc(CartesianCS* csys, CartesianLoc& l) :
    x(l.x),
    y(l.y)
{
    Location::cs = csys;
}

void CartesianLoc::CalcWorldCoordinates()
{
    if (cs == nullptr) // World CS is used
    {
        world_x = x;
        world_y = y;
    }
    else
    {
        CartesianCS* cs = reinterpret_cast<CartesianCS*>(Location::cs);

        world_x = cs->center_x + cs->a[0] * x + cs->b[0] * x;
        world_y = cs->center_y + cs->a[1] * y + cs->b[1] * y;
    }
}

void CartesianLoc::CalcOwnCoordinates()
{
    if (cs == nullptr) // World CS is used
    {
        x = world_x;
        y = world_y;
    }
    else
    {
        CartesianCS* cs = reinterpret_cast<CartesianCS*>(Location::cs);

        fp_t p[2] =
        {
            p[0] = world_x - cs->center_x,
            p[1] = world_y - cs->center_y
        };

        x = (cs->a[0] * p[0] + cs->a[1] * p[1]) / cs->a_len / cs->a_len;
        y = (cs->b[0] * p[0] + cs->b[1] * p[1]) / cs->b_len / cs->a_len;
    }
}

CartesianLoc::~CartesianLoc()
{

}

void CartesianLoc::TransformTo(CoordinateSystem* csys)
{
    CalcWorldCoordinates();
    this->cs = csys;
    CalcOwnCoordinates();
}

fp_t CartesianLoc::GetX()
{
    return x;
}

fp_t CartesianLoc::GetY()
{
    return y;
}

void CartesianLoc::SetX(fp_t x)
{
    this->x = x;
}

void CartesianLoc::SetY(fp_t y)
{
    this->y = y;
}
