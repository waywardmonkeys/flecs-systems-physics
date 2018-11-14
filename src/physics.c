#include <include/physics.h>
#include <string.h>
#include <math.h>

void EcsMove2D_w_Rotation(EcsRows *rows) {
    void *row;
    for (row = rows->first; row < rows->last; row = ecs_next(rows, row)) {
        EcsPosition2D *p = ecs_column(rows, row, 0);
        EcsSpeed *speed = ecs_column(rows, row, 1);
        EcsRotation2D *r = ecs_column(rows, row, 2);
        float x_speed = 1;
        float y_speed = 0;

        x_speed = cos(r->angle) * speed->value;
        y_speed = sin(r->angle) * speed->value;

        p->x += x_speed;
        p->y += y_speed;
    }
}

void EcsMove2D_w_Velocity(EcsRows *rows) {
    void *row;
    for (row = rows->first; row < rows->last; row = ecs_next(rows, row)) {
        EcsPosition2D *p = ecs_column(rows, row, 0);
        EcsSpeed *speed = ecs_column(rows, row, 1);
        EcsVelocity2D *v = ecs_column(rows, row, 2);
        float x_speed = v->x;
        float y_speed = v->y;

        if (speed) {
            x_speed *= speed->value;
            y_speed *= speed->value;
        }

        p->x += x_speed;
        p->y += y_speed;
    }
}

void Rotate2D(EcsRows *rows) {
    void *row;
    for (row = rows->first; row < rows->last; row = ecs_next(rows, row)) {
        EcsRotation2D *r = ecs_column(rows, row, 0);
        EcsAngularSpeed *s = ecs_column(rows, row, 1);
        r->angle += s->value;
    }
}

void EcsMove3D_w_Rotation(EcsRows *rows) {
    void *row;
    for (row = rows->first; row < rows->last; row = ecs_next(rows, row)) {
        EcsPosition3D *p = ecs_column(rows, row, 0);
        EcsSpeed *speed = ecs_column(rows, row, 1);
        EcsRotation3D *r = ecs_column(rows, row, 2);
        float x_speed = cos(r->z) * sin(r->y) * speed->value;
        float y_speed = cos(r->x) * sin(r->z) * speed->value;
        float z_speed = cos(r->y) * sin(r->x) * speed->value;
        p->x += x_speed;
        p->y += y_speed;
        p->z += z_speed;
    }
}

void EcsMove3D_w_Velocity(EcsRows *rows) {
    void *row;
    for (row = rows->first; row < rows->last; row = ecs_next(rows, row)) {
        EcsPosition3D *p = ecs_column(rows, row, 0);
        EcsSpeed *speed = ecs_column(rows, row, 1);
        EcsVelocity3D *v = ecs_column(rows, row, 2);
        float x_speed = v->x;
        float y_speed = v->y;
        float z_speed = v->z;

        if (speed) {
            x_speed *= speed->value;
            y_speed *= speed->value;
            z_speed *= speed->value;
        }

        p->x += x_speed;
        p->y += y_speed;
    }
}

void EcsRotate3D(EcsRows *rows) {
    void *row;
    for (row = rows->first; row < rows->last; row = ecs_next(rows, row)) {
        EcsRotation3D *r = ecs_column(rows, row, 0);
        EcsAngularSpeed *s = ecs_column(rows, row, 1);
        EcsAngularVelocity *v = ecs_column(rows, row, 2);
        float speed = 1;

        if (s) {
            speed = s->value;
        }

        r->x += v->x * speed;
        r->y += v->y * speed;
        r->z += v->z * speed;
    }
}

void EcsSystemsPhysics(
    EcsWorld *world,
    int flags,
    void *handles_out)
{
    EcsSystemsPhysicsHandles *handles = handles_out;
    bool do_2d = !flags || flags & ECS_2D;
    bool do_3d = !flags || flags & ECS_3D;

    memset(handles, 0, sizeof(EcsSystemsPhysicsHandles));

    if (do_2d) {
        ECS_SYSTEM(world, EcsMove2D_w_Rotation, EcsPeriodic,
            EcsPosition2D, EcsRotation2D, EcsSpeed, !EcsVelocity2D);

        ECS_SYSTEM(world, EcsMove2D_w_Velocity, EcsPeriodic,
            EcsPosition2D, ?EcsSpeed, EcsVelocity2D);

        ECS_SYSTEM(world, Rotate2D, EcsPeriodic,
            EcsRotation2D, EcsAngularSpeed);

        ECS_FAMILY(world, EcsMove2D, EcsMove2D_w_Rotation, EcsMove2D_w_Velocity);

        handles->Move2D_w_Rotation = EcsMove2D_w_Rotation_h;
        handles->Move2D_w_Velocity = EcsMove2D_w_Velocity_h;
        handles->Move2D = EcsMove2D_h;
        handles->Rotate2D = Rotate2D_h;
    }

    if (do_3d) {
        ECS_SYSTEM(world, EcsMove3D_w_Rotation, EcsPeriodic,
            EcsPosition3D, ?EcsRotation3D, EcsSpeed, !EcsVelocity3D);

        ECS_SYSTEM(world, EcsMove3D_w_Velocity, EcsPeriodic,
            EcsPosition3D, ?EcsSpeed, EcsVelocity3D);

        ECS_SYSTEM(world, EcsRotate3D, EcsPeriodic,
            EcsRotation3D, ?EcsAngularSpeed, EcsAngularVelocity);

        ECS_FAMILY(world, EcsMove3D, EcsMove3D_w_Rotation, EcsMove3D_w_Velocity);

        handles->Move3D_w_Rotation = EcsMove3D_w_Rotation_h;
        handles->Move3D_w_Velocity = EcsMove3D_w_Velocity_h;
        handles->Move3D = EcsMove3D_h;
        handles->Rotate3D = EcsRotate3D_h;
    }

    if (do_2d && do_3d) {
        ECS_FAMILY(world, Move, EcsMove2D, EcsMove3D);
        ECS_FAMILY(world, Rotate, Rotate2D, EcsRotate3D);

        handles->Rotate = Rotate_h;
        handles->Move = Move_h;
    } else if (!do_2d) {
        ECS_FAMILY(world, Move, EcsMove3D);
        ECS_FAMILY(world, Rotate, EcsRotate3D);

        handles->Move = Move_h;
        handles->Rotate = Rotate_h;
    } else if (!do_3d) {
        ECS_FAMILY(world, Move, EcsMove2D);
        ECS_FAMILY(world, Rotate, Rotate2D);

        handles->Move = Move_h;
        handles->Rotate = Rotate_h;
    }

    ecs_enable(world, handles->Move, false);
    ecs_enable(world, handles->Rotate, false);
}
