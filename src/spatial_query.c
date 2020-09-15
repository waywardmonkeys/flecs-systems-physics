#include <flecs_systems_physics.h>

struct ecs_squery_t {
    ecs_query_t *q;
    ecs_octree_t *ot;
};

#define EXPR_PREFIX "flecs.components.transform.Position3,"

ecs_squery_t* ecs_squery_new(
    ecs_world_t *world,
    const char *expr,
    vec3 center,
    float size)
{
    ecs_assert(world != NULL, ECS_INVALID_PARAMETER, NULL);
    ecs_assert(expr != NULL, ECS_INVALID_PARAMETER, NULL);
    ecs_assert(size > 0, ECS_INVALID_PARAMETER, NULL);

    ecs_squery_t *result = ecs_os_calloc(sizeof(ecs_squery_t));

    /* Prepend position to query so we have fast access to entity position when
     * updating the octree */
    char *full_expr = ecs_os_malloc(strlen(expr) + strlen(EXPR_PREFIX) + 1);
    sprintf(full_expr, EXPR_PREFIX "%s", expr);

    result->q = ecs_query_new(world, full_expr);
    result->ot = ecs_octree_new(center, size);

    return result;
}

void ecs_squery_free(
    ecs_squery_t *sq)
{
    ecs_query_free(sq->q);
    ecs_octree_free(sq->ot);
    ecs_os_free(sq);
}

void ecs_squery_update(
    ecs_squery_t *sq)
{
    if (ecs_query_changed(sq->q)) {
        ecs_octree_clear(sq->ot);

        ecs_iter_t it = ecs_query_iter(sq->q);
        while (ecs_query_next(&it)) {
            EcsPosition3 *p = ecs_column(&it, EcsPosition3, 1);

            int i;
            for (i = 0; i < it.count; i ++) {
                vec3 vp, vs;
                vp[0] = p[i].x;
                vp[1] = p[i].y;
                vp[2] = p[i].z;

                vs[0] = 1;
                vs[1] = 1;
                vs[2] = 1;

                ecs_octree_insert(sq->ot, it.entities[i], vp, vs);
            }
        }
    }
}

void ecs_squery_findn(
    ecs_squery_t *sq,
    vec3 position,
    float range,
    ecs_vector_t **result)
{
    ecs_squery_update(sq);
    ecs_octree_findn(sq->ot, position, range, result);
}