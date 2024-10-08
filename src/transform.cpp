#include "m_pd.h"
#include "Quaternion.h"
#include <math.h>

// const double angleMax = 30.0; // degrees
// const double stringLength = 3000.0; // mm
// const double distance2strings = 100.0; // mm
// const double stringOffset = 35.0; // mm - measured

const double angleMax = 34.7; // degrees - measured (default value was 30.0)
const double stringLength = 3065.0; // mm - measured (default value was 3000.0)
const double distance2strings = 130.0; // mm - measured (default value was 100.0)
const double stringOffset = 35.0; // mm - measured

static t_class *transform_class;

typedef struct _gametrak {
    t_object  x_obj;
    t_float   minIn, maxIn;
    t_float   lTheta, lPhi, lRho;
    t_float   rTheta, rPhi, rRho;
    t_inlet   *in_lx, *in_ly, *in_lz;
    t_inlet   *in_rx, *in_ry, *in_rz;
    t_outlet  *out_lx, *out_ly, *out_lz;
    t_outlet  *out_rx, *out_ry, *out_rz;
} t_gametrak;

void transform_setGametrak_thetaPhiRho(t_gametrak *gt, t_floatarg lTheta, t_floatarg lPhi, t_floatarg lRho, t_floatarg rTheta, t_floatarg rPhi, t_floatarg Rho){
    gt->lTheta = lTheta;
    gt->lPhi = lPhi;
    gt->lRho = lRho;
    gt->rTheta = rTheta;
    gt->rPhi = rPhi;
    gt->rRho = Rho;

    // post("lTheta: %.2f, lPhi: %.2f, lRho: %.2f, rTheta: %.2f, rPhi: %.2f, rRho: %.2f", gt->lTheta, gt->lPhi, gt->lRho, gt->rTheta, gt->rPhi, gt->rRho);
}

Vecteur3D spherical_to_cartesian(double theta, double phi, double rho)
{
    Quaternion Qtheta, Qphi, Qres;
    Qtheta.AxisToQuaternion(Vecteur3D(0,1,0),theta * M_PI / 180.0);
    Qphi.AxisToQuaternion(Vecteur3D(1,0,0),phi * M_PI / 180.0);
    Qres = Qtheta * Qphi;
    return Qres.RotateVector(Vecteur3D(0,0,rho));
}

// // x=ρ⋅sin(ϕ)⋅cos(θ)
// // y=ρ⋅sin(ϕ)⋅sin(θ)
// // z=ρ⋅cos(ϕ)
// Vecteur3D spherical_to_cartesian(double theta, double phi, double rho)
// {
//     theta = theta * M_PI / 180.0;
//     phi = phi * M_PI / 180.0;
    
//     double x = rho * sin(phi) * cos(theta);
//     double y = rho * sin(phi) * sin(theta);
//     double z = rho * cos(phi);
    
//     return Vecteur3D(x, y, z);
// }

inline float midpoint(t_gametrak *gt) {
     return (gt->maxIn - gt->minIn)/2.0;
}

void transform_onSet_LX(t_gametrak *gt, t_floatarg in_lx){
    float lTheta = (in_lx - midpoint(gt)) * angleMax / midpoint(gt);
    // transform_setGametrak_thetaPhiRho(gt, 
    //                                   gt->lTheta, gt->lPhi, gt->lRho,
    //                                   gt->rTheta, gt->rPhi, gt->rRho);
    transform_setGametrak_thetaPhiRho(gt, 
                                      lTheta,     gt->lPhi, gt->lRho,
                                      gt->rTheta, gt->rPhi, gt->rRho);

    Vecteur3D cartesian = spherical_to_cartesian(static_cast<double>(gt->lTheta),
                                                 static_cast<double>(gt->lPhi),
                                                 static_cast<double>(gt->lRho));

    outlet_float(gt->out_lx, static_cast<float>(cartesian.x - distance2strings / 2.0));
    outlet_float(gt->out_ly, static_cast<float>(cartesian.y));
    outlet_float(gt->out_lz, static_cast<float>(cartesian.z));
}

void transform_onSet_RX(t_gametrak *gt, t_floatarg in_rx){
    float rTheta = (in_rx - midpoint(gt)) * angleMax / midpoint(gt);
    transform_setGametrak_thetaPhiRho(gt, 
                                      gt->lTheta, gt->lPhi, gt->lRho,
                                      rTheta,     gt->rPhi, gt->rRho);

    Vecteur3D cartesian = spherical_to_cartesian(static_cast<double>(gt->rTheta),
                                                 static_cast<double>(gt->rPhi),
                                                 static_cast<double>(gt->rRho));

    outlet_float(gt->out_rx, static_cast<float>(cartesian.x + distance2strings / 2.0));
    outlet_float(gt->out_ry, static_cast<float>(cartesian.y));
    outlet_float(gt->out_rz, static_cast<float>(cartesian.z));
}

void transform_onSet_LY(t_gametrak *gt, t_floatarg in_ly){
    float lPhi = -(in_ly - midpoint(gt)) * angleMax / midpoint(gt);
    transform_setGametrak_thetaPhiRho(gt, 
                                      gt->lTheta, lPhi,     gt->lRho,
                                      gt->rTheta, gt->rPhi, gt->rRho);

    Vecteur3D cartesian = spherical_to_cartesian(static_cast<double>(gt->lTheta),
                                                 static_cast<double>(gt->lPhi),
                                                 static_cast<double>(gt->lRho));

    outlet_float(gt->out_lx, static_cast<float>(cartesian.x - distance2strings / 2.0));
    outlet_float(gt->out_ly, static_cast<float>(cartesian.y));
    outlet_float(gt->out_lz, static_cast<float>(cartesian.z));
}

void transform_onSet_RY(t_gametrak *gt, t_floatarg in_ry){
    float rPhi = -(in_ry - midpoint(gt)) * angleMax / midpoint(gt);
    transform_setGametrak_thetaPhiRho(gt, 
                                      gt->lTheta, gt->lPhi, gt->lRho,
                                      gt->rTheta, rPhi,     gt->rRho);

    Vecteur3D cartesian = spherical_to_cartesian(static_cast<double>(gt->rTheta),
                                                 static_cast<double>(gt->rPhi),
                                                 static_cast<double>(gt->rRho));

    outlet_float(gt->out_rx, static_cast<float>(cartesian.x + distance2strings / 2.0));
    outlet_float(gt->out_ry, static_cast<float>(cartesian.y));
    outlet_float(gt->out_rz, static_cast<float>(cartesian.z));
}

float map(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void transform_onSet_LZ(t_gametrak *gt, t_floatarg in_lz){
    // float lRho = -(stringLength/4096.0) * in_z + stringLength;
    float lRho = map(in_lz, gt->minIn, gt->maxIn, 0, stringLength);
    transform_setGametrak_thetaPhiRho(gt, 
                                      gt->lTheta, gt->lPhi, lRho,
                                      gt->rTheta, gt->rPhi, gt->rRho);

    Vecteur3D cartesian = spherical_to_cartesian(static_cast<double>(gt->lTheta),
                                                 static_cast<double>(gt->lPhi),
                                                 static_cast<double>(gt->lRho));

    outlet_float(gt->out_lx, static_cast<float>(cartesian.x - distance2strings / 2.0));
    outlet_float(gt->out_ly, static_cast<float>(cartesian.y));
    outlet_float(gt->out_lz, static_cast<float>(cartesian.z));
}

void transform_onSet_RZ(t_gametrak *gt, t_floatarg in_rz){
    // float rRho = -(stringLength/4096.0) * in_z + stringLength;
    float rRho = map(in_rz, gt->minIn, gt->maxIn, 0, stringLength);
    transform_setGametrak_thetaPhiRho(gt, 
                                      gt->lTheta, gt->lPhi, gt->lRho,
                                      gt->rTheta, gt->rPhi, rRho);

    Vecteur3D cartesian = spherical_to_cartesian(static_cast<double>(gt->rTheta),
                                                 static_cast<double>(gt->rPhi),
                                                 static_cast<double>(gt->rRho));

    outlet_float(gt->out_rx, static_cast<float>(cartesian.x + distance2strings / 2.0));
    outlet_float(gt->out_ry, static_cast<float>(cartesian.y));
    outlet_float(gt->out_rz, static_cast<float>(cartesian.z));
}

void *transform_new(t_floatarg minIn = 0, t_floatarg maxIn = 4095)
{
    t_gametrak *gt = (t_gametrak *)pd_new(transform_class);

    gt->minIn = minIn;
    gt->maxIn = maxIn;

    transform_setGametrak_thetaPhiRho(gt, midpoint(gt), midpoint(gt), 0, midpoint(gt), midpoint(gt), 0);

    gt->in_lx = inlet_new(&gt->x_obj, &gt->x_obj.ob_pd, &s_float, gensym("lx"));
    gt->in_ly = inlet_new(&gt->x_obj, &gt->x_obj.ob_pd, &s_float, gensym("ly"));
    gt->in_lz = inlet_new(&gt->x_obj, &gt->x_obj.ob_pd, &s_float, gensym("lz"));
    gt->in_rx = inlet_new(&gt->x_obj, &gt->x_obj.ob_pd, &s_float, gensym("rx"));
    gt->in_ry = inlet_new(&gt->x_obj, &gt->x_obj.ob_pd, &s_float, gensym("ry"));
    gt->in_rz = inlet_new(&gt->x_obj, &gt->x_obj.ob_pd, &s_float, gensym("rz"));

    gt->out_lx = outlet_new(&gt->x_obj, &s_float);
    gt->out_ly = outlet_new(&gt->x_obj, &s_float);
    gt->out_lz = outlet_new(&gt->x_obj, &s_float);
    gt->out_rx = outlet_new(&gt->x_obj, &s_float);
    gt->out_ry = outlet_new(&gt->x_obj, &s_float);
    gt->out_rz = outlet_new(&gt->x_obj, &s_float);

    return (void *)gt;
}

void transform_free(t_gametrak *gt) {
    inlet_free(gt->in_lx);
    inlet_free(gt->in_ly);
    inlet_free(gt->in_lz);
    inlet_free(gt->in_rx);
    inlet_free(gt->in_ry);
    inlet_free(gt->in_rz);
    outlet_free(gt->out_lx);
    outlet_free(gt->out_ly);
    outlet_free(gt->out_lz);
    outlet_free(gt->out_rx);
    outlet_free(gt->out_ry);
    outlet_free(gt->out_rz);
}

extern "C" {

void transform_setup(void) {
    transform_class = class_new(gensym("transform"),
        (t_newmethod)transform_new,
        (t_method)transform_free,
        sizeof(t_gametrak),
        CLASS_NOINLET,
        A_DEFFLOAT,
        A_DEFFLOAT,
        0);

    class_addmethod(transform_class,
        (t_method)transform_onSet_LX,
        gensym("lx"),
        A_DEFFLOAT,
        0);
    class_addmethod(transform_class,
        (t_method)transform_onSet_LY,
        gensym("ly"),
        A_DEFFLOAT,
        0);
    class_addmethod(transform_class,
        (t_method)transform_onSet_LZ,
        gensym("lz"),
        A_DEFFLOAT,
        0);
    class_addmethod(transform_class,
        (t_method)transform_onSet_RX,
        gensym("rx"),
        A_DEFFLOAT,
        0);
    class_addmethod(transform_class,
        (t_method)transform_onSet_RY,
        gensym("ry"),
        A_DEFFLOAT,
        0);
    class_addmethod(transform_class,
        (t_method)transform_onSet_RZ,
        gensym("rz"),
        A_DEFFLOAT,
        0);
}

}