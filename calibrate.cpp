#include "m_pd.h"
#include <math.h>

static t_class *calibrate_class;

typedef struct _gametrak {
    t_object  x_obj;
    t_float   state;
    t_float   minLX, maxLX;
    t_float   minLY, maxLY;
    t_float   minLZ, maxLZ;
    t_float   minRX, maxRX;
    t_float   minRY, maxRY;
    t_float   minRZ, maxRZ;
    t_inlet   *in_state;
    t_inlet   *in_lx, *in_ly, *in_lz;
    t_inlet   *in_rx, *in_ry, *in_rz;
    t_outlet  *out_state;
    t_outlet  *out_lx, *out_ly, *out_lz;
    t_outlet  *out_rx, *out_ry, *out_rz;
} t_gametrak;

void calibrate_onSet_State(t_gametrak *gt, t_floatarg in_state){
    gt->state = in_state;
}

void calibrate_onSet_LX(t_gametrak *gt, t_floatarg in_lx){
    gt->maxLX = in_lx > gt->maxLX ? in_lx : gt->maxLX;
    gt->minLX = in_lx < gt->minLX ? in_lx : gt->minLX;
}

void calibrate_onSet_LY(t_gametrak *gt, t_floatarg in_ly){
    gt->maxLY = in_ly > gt->maxLY ? in_ly : gt->maxLY;
    gt->minLY = in_ly < gt->minLY ? in_ly : gt->minLY;
}

void calibrate_onSet_LZ(t_gametrak *gt, t_floatarg in_lz){
    gt->maxLZ = in_lz > gt->maxLZ ? in_lz : gt->maxLZ;
    gt->minLZ = in_lz < gt->minLZ ? in_lz : gt->minLZ;
}

void calibrate_onSet_RX(t_gametrak *gt, t_floatarg in_rx){
    gt->maxRX = in_rx > gt->maxRX ? in_rx : gt->maxRX;
    gt->minRX = in_rx < gt->minRX ? in_rx : gt->minRX;
}

void calibrate_onSet_RY(t_gametrak *gt, t_floatarg in_ry){
    gt->maxRY = in_ry > gt->maxRY ? in_ry : gt->maxRY;
    gt->minRY = in_ry < gt->minRY ? in_ry : gt->minRY;
}

void calibrate_onSet_RZ(t_gametrak *gt, t_floatarg in_rz){
    gt->maxRZ = in_rz > gt->maxRZ ? in_rz : gt->maxRZ;
    gt->minRZ = in_rz < gt->minRZ ? in_rz : gt->minRZ;
}

void *calibrate_new()
{
    t_gametrak *gt = (t_gametrak *)pd_new(calibrate_class);

    gt->in_state = inlet_new(&gt->x_obj, &gt->x_obj.ob_pd, &s_float, gensym("state"));
    gt->in_lx = inlet_new(&gt->x_obj, &gt->x_obj.ob_pd, &s_float, gensym("lx"));
    gt->in_ly = inlet_new(&gt->x_obj, &gt->x_obj.ob_pd, &s_float, gensym("ly"));
    gt->in_lz = inlet_new(&gt->x_obj, &gt->x_obj.ob_pd, &s_float, gensym("lz"));
    gt->in_rx = inlet_new(&gt->x_obj, &gt->x_obj.ob_pd, &s_float, gensym("rx"));
    gt->in_ry = inlet_new(&gt->x_obj, &gt->x_obj.ob_pd, &s_float, gensym("ry"));
    gt->in_rz = inlet_new(&gt->x_obj, &gt->x_obj.ob_pd, &s_float, gensym("rz"));

    gt->out_state = outlet_new(&gt->x_obj, &s_float);
    gt->out_lx = outlet_new(&gt->x_obj, &s_float);
    gt->out_ly = outlet_new(&gt->x_obj, &s_float);
    gt->out_lz = outlet_new(&gt->x_obj, &s_float);
    gt->out_rx = outlet_new(&gt->x_obj, &s_float);
    gt->out_ry = outlet_new(&gt->x_obj, &s_float);
    gt->out_rz = outlet_new(&gt->x_obj, &s_float);

    return (void *)gt;
}

void calibrate_free(t_gametrak *gt) {
    inlet_free(gt->in_state);
    inlet_free(gt->in_lx);
    inlet_free(gt->in_ly);
    inlet_free(gt->in_lz);
    inlet_free(gt->in_rx);
    inlet_free(gt->in_ry);
    inlet_free(gt->in_rz);

    outlet_free(gt->out_state);
    outlet_free(gt->out_lx);
    outlet_free(gt->out_ly);
    outlet_free(gt->out_lz);
    outlet_free(gt->out_rx);
    outlet_free(gt->out_ry);
    outlet_free(gt->out_rz);
}

extern "C" {

void calibrate_setup(void) {
    calibrate_class = class_new(gensym("calibrate"),
        (t_newmethod)calibrate_new,
        (t_method)calibrate_free,
        sizeof(t_gametrak),
        CLASS_NOINLET,
        A_NULL,
        0);

    class_addmethod(calibrate_class,
        (t_method)calibrate_onSet_State,
        gensym("state"),
        A_DEFFLOAT,
        0);
    class_addmethod(calibrate_class,
        (t_method)calibrate_onSet_LX,
        gensym("lx"),
        A_DEFFLOAT,
        0);
    class_addmethod(calibrate_class,
        (t_method)calibrate_onSet_LY,
        gensym("ly"),
        A_DEFFLOAT,
        0);
    class_addmethod(calibrate_class,
        (t_method)calibrate_onSet_LZ,
        gensym("lz"),
        A_DEFFLOAT,
        0);
    class_addmethod(calibrate_class,
        (t_method)calibrate_onSet_RX,
        gensym("rx"),
        A_DEFFLOAT,
        0);
    class_addmethod(calibrate_class,
        (t_method)calibrate_onSet_RY,
        gensym("ry"),
        A_DEFFLOAT,
        0);
    class_addmethod(calibrate_class,
        (t_method)calibrate_onSet_RZ,
        gensym("rz"),
        A_DEFFLOAT,
        0);
}

}